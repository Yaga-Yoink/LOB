#include "OrderBook.hpp"

class OrderBook {
 public:
  // place order always goes through, it is the job of the matching engine to
  // figure out whether it is an aggressive or passive order
  void place_order(Order& order) {
    std::unordered_map<Price, std::unique_ptr<Level>>& level_map =
        (order.direction == -1) ? ask_level_map : bid_level_map;
    std::unique_ptr<OrderNode> node =
        std::make_unique<OrderNode>(order, nullptr, nullptr);
    if (!level_map.contains(order.price)) {
      std::unique_ptr<Level> level =
          std::make_unique<Level>(order.price, std::move(node), nullptr);
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
    order_map.insert(
        {order.id, std::make_unique<OrderMetadata>(node, &price_level)});

    // fix invariants
    switch (order.direction) {
      case -1:
        ask_price = order.price < ask_price ? order.price : ask_price;
      case 1:
        bid_price = order.price > bid_price ? order.price : bid_price;
    }
  };
  void cancel_order(OrderID order_id);

 private:
  Price bid_price;
  Price ask_price;
  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>> bid_order_map;
  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>> ask_order_map;
  std::unordered_map<Price, std::unique_ptr<Level>> bid_level_map;
  std::unordered_map<Price, std::unique_ptr<Level>> ask_level_map;
};