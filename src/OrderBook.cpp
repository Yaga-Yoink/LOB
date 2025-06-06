#include "OrderBook.hpp"

#include <iostream>

// place order always goes through, it is the job of the matching engine to
// figure out whether it is an aggressive or passive order
void OrderBook::place_order(const Order& order) {
  std::unordered_map<Price, std::unique_ptr<Level>>& level_map =
      (order.direction == -1) ? ask_level_map : bid_level_map;
  std::unique_ptr<OrderNode> node =
      std::make_unique<OrderNode>(OrderNode{order, nullptr, nullptr});
  OrderNode* node_ptr = node.get();
  Level* price_level;
  if (!level_map.contains(order.price)) {
    std::unique_ptr<Level> level =
        std::make_unique<Level>(Level{order.price, node_ptr, nullptr});
    price_level = level.get();
    level->tail = level->head;
    level_map.insert({order.price, std::move(level)});

  }

  // there will always be a head in this else branch
  else {
    price_level = level_map[order.price].get();
    // std::cout << (price_level->tail == nullptr) << std::endl;

    price_level->tail->right = node_ptr;
    node->left = price_level->tail;
    price_level->tail = node_ptr;
  }
  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>>& order_map =
      (order.direction == Sell) ? ask_order_map : bid_order_map;
  // add order to the order_map
  order_map.insert({order.id, std::make_unique<OrderMetadata>(OrderMetadata{
                                  std::move(node), price_level})});

  // // fix invariants
  switch (order.direction) {
    case Sell:
      ask_price = order.price < ask_price ? order.price : ask_price;
      break;
    case Buy:
      bid_price = order.price > bid_price ? order.price : bid_price;
      break;
  }
};
void OrderBook::cancel_order(OrderID order_id) {}

Price OrderBook::get_bid_price() const { return bid_price; }
Price OrderBook::get_ask_price() const { return ask_price; }