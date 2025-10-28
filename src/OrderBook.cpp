#include "OrderBook.hpp"

#include <iostream>

/**
 * Places the 'order' into the limit order book
 */
// place order always goes through, it is the job of the matching engine to
// figure out whether it is an aggressive or passive order
void OrderBook::place_order(const Order& order) {
  LevelMap& level_map =
      (order.direction == Side::Sell) ? ask_level_map : bid_level_map;
  std::shared_ptr<OrderNode> node = std::make_shared<OrderNode>(OrderNode{order, std::weak_ptr<OrderNode>{}, nullptr});

  std::shared_ptr<Level> price_level;
  if (!level_map.contains(order.price)) {
    price_level = std::make_shared<Level>(order.price, node, node);
    level_map.emplace(order.price, price_level);
  }
  // there will always be a head in this else branch
  else {
    price_level = level_map.at(order.price);
    if (auto tail_ptr = price_level->tail.lock()) {
      tail_ptr->right = node;
      node->left = tail_ptr;
      price_level->tail = node; 
    }
  }

  OrderMap& order_map = (order.direction == Side::Sell) ? ask_order_map : bid_order_map;
  // add order to the order_map
  order_map.emplace(order.id, OrderMetadata{node, std::move(price_level)});

  // fix invariants
  switch (order.direction) {
    case Side::Sell:
      ask_price = std::min({ask_price, order.price});
      break;
    case Side::Buy:
      std::cout << "Bid Price: " << bid_price << ", Order Price: " << order.price << std::endl;
      bid_price = std::max({bid_price, order.price});
      break;
  }
};

void OrderBook::cancel_order(OrderID order_id) {
  Side direction;
  if (bid_order_map.contains(order_id)) {
    direction = Side::Buy;
  }
  else if (ask_order_map.contains(order_id)) {
    direction = Side::Sell;
  }
  else {
    return;
  }

  OrderMap& order_map = (direction == Side::Buy) ? bid_order_map : ask_order_map;
  LevelMap& level_map = (direction == Side::Buy) ? bid_level_map : ask_level_map; 
  // find order in either bid_price or ask_price maps
  OrderMetadata& order_metadata = order_map[order_id];
  std::shared_ptr<OrderNode>& order_node = order_metadata.order_node;
  // order_map.erase(order_id);

  // only order in level
  // std::cout << "order_node is nullptr: " << !order_node << std::endl;
  
  if (order_node->left.expired() && order_node->right == nullptr) {
    std::cout << "Detected Deleting Level" << std::endl;
    // TODO: handle best bid ask price
    order_metadata.level->head = nullptr;
    order_metadata.level->tail.reset();
    order_node->left.reset();
    order_node->right = nullptr;
    Price order_price = order_node->order.price;  // Save price before erasing
    order_map.erase(order_id);
    if (direction == Side::Buy && order_price == bid_price) {
      std::cout << "Canceling Curr Price Order Here: " << std::endl;
      Price max_price{std::numeric_limits<Price>::min()};
      for (auto it = order_map.begin(); it != order_map.end(); ++it) {
        std::cout << "Key : " << it->first << ", Value Price: " << it->second.level->price << std::endl;
        max_price = std::max({max_price, it->second.level->price});
      }
      bid_price = max_price;
    } else if (direction == Side::Sell && order_price == ask_price) {
      Price min_price{std::numeric_limits<Price>::max()};
      for (auto it = order_map.begin(); it != order_map.end(); ++it) {
        min_price = std::min({min_price, it->second.level->price});
      }
      ask_price = min_price;
    }
    level_map.erase(order_price);
  }
  // order was the tail of the level
  else if (order_node->right == nullptr) {
    order_metadata.level->tail = order_node->left;
    order_map.erase(order_id);
  }
  // order was the head of the level
  else if (order_node->left.expired()) {
    order_metadata.level->head = order_node->right;
    order_map.erase(order_id);
  }
  // order was in the middle of the level, no need to adjust level pointers
  else {
    if (auto tmp = order_node->left.lock()) {
      tmp->right = order_node->right;
    }
    order_node->right->left = order_node->left;
    order_map.erase(order_id);
  }
}

Price OrderBook::get_bid_price() const { return bid_price; }
Price OrderBook::get_ask_price() const { return ask_price; }