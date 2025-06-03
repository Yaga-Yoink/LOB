#include "OrderBook.hpp"

// place order always goes through, it is the job of the matching engine to
// figure out whether it is an aggressive or passive order
void OrderBook::place_order(Order& order) {
  std::unordered_map<Price, std::unique_ptr<Level>>& level_map =
      (order.direction == -1) ? ask_level_map : bid_level_map;
  std::unique_ptr<OrderNode> node =
      std::make_unique<OrderNode>(OrderNode{order, nullptr, nullptr});
  if (!level_map.contains(order.price)) {
    std::unique_ptr<Level> level =
        std::make_unique<Level>(Level{order.price, std::move(node), nullptr});
    level_map.insert({order.price, std::move(level)});
  }
  // add order to the price_level
  Level* price_level = level_map[order.price].get();
  node->left = price_level->tail;
  price_level->tail->right = node.get();
  price_level->tail = node.get();

  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>>& order_map =
      (order.direction == -1) ? ask_order_map : bid_order_map;
  // add order to the order_map
  order_map.insert({order.id, std::make_unique<OrderMetadata>(
                                  OrderMetadata{node.get(), price_level})});

  // fix invariants
  switch (order.direction) {
    case -1:
      ask_price = order.price < ask_price ? order.price : ask_price;
    case 1:
      bid_price = order.price > bid_price ? order.price : bid_price;
  }
};
void OrderBook::cancel_order(OrderID order_id) {}