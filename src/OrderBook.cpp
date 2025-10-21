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

  Level price_level;
  if (!level_map.contains(order.price)) {
    level_map.emplace(order.price, Level{order.price, node, node});
    price_level = level_map[order.price];
  }
  // there will always be a head in this else branch
  else {
    price_level = level_map.at(order.price);
    if (auto tail_ptr = price_level.tail.lock()) {
      tail_ptr->right = node;
      node->left = tail_ptr;
      price_level.tail = node; 
    }
    
  }

  OrderMap& order_map = (order.direction == Sell) ? ask_order_map : bid_order_map;
  // add order to the order_map
  order_map.emplace(order.id, OrderMetadata{node, price_level});

  // fix invariants
  switch (order.direction) {
    case Sell:
      ask_price = order.price < ask_price ? order.price : ask_price;
      break;
    case Buy:
      bid_price = order.price > bid_price ? order.price : bid_price;
      break;
  }
};

// void OrderBook::cancel_order(OrderID order_id) {
//   OrderMap order_map;
//   Side direction;
//   // find order in either bid_price or ask_price maps
//   if (bid_order_map.contains(order_id)) {
//     order_map = bid_order_map;
//     direction = Buy;
//   } else if (ask_order_map.contains(order_id)) {
//     order_map = ask_order_map;
//     direction = Sell;
//   } else {
//     // error
//   }
//   auto node = order_map.extract(order_id);
//   OrderMetadata& order_metadata = node.mapped();
//   std::weak_ptr<OrderNode>& order_node = order_metadata.order_node;

//   // only order in level
//   if (order_node.left == nullptr && order_node.right == nullptr) {
//     order_metadata->level->head = nullptr;
//     order_metadata->level->tail = nullptr;
//     // TODO: handle best bid ask price

//     if (direction == Buy && order_node->order.price == bid_price) {
//       // serach container for next best price
//       // unofruntately since levels are in an unordered map instead of
//       // something like a heap or binary tree, this will be O(n) to find the
//       // next best price
//       Price max_price{std::numeric_limits<Price>::min()};
//       for (auto it = order_map->begin(); it != order_map->end(); ++it) {
//         // iterators for map have an overload -> operator which returns a
//         // std::pair of the key and value
//         Price curr_price = it->second->level->price;

//         max_price = max_price < curr_price ? curr_price : max_price;
//       }
//       bid_price = max_price;
//     } else if (direction == Sell && order_node->order.price == ask_price) {
//       Price min_price{std::numeric_limits<Price>::max()};
//       for (auto it = order_map->begin(); it != order_map->end(); ++it) {
//         // iterators for map have an overload -> operator which returns a
//         // std::pair of the key and value
//         Price curr_price = it->second->level->price;

//         min_price = min_price < curr_price ? min_price : curr_price;
//       }
//       ask_price = min_price;
//     }
//   }
//   // order was the tail of the level
//   else if (order_node->right == nullptr) {
//     order_metadata->level->tail = order_node->left;
//   }
//   // order was the head of the level
//   else if (order_node->left == nullptr) {
//     order_metadata->level->head = order_node->right;
//   }
//   // order was in the middle of the level, no need to adjust level pointers
//   else {
//     order_node->left->right = order_node->right;
//     order_node->right->left = order_node->left;
//   }
//   // TODO: fix up the memory management, make sure that order metadata and
//   // order nodes are deallocated after this scope hopefully through automatic
//   // memory management with the smart pointers
// }

Price OrderBook::get_bid_price() const { return bid_price; }
Price OrderBook::get_ask_price() const { return ask_price; }