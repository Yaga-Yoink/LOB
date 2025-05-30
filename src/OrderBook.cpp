#include "OrderBook.hpp"

class OrderBook {
 public:
  void place_order(Order& order) {
    if (!level_map.contains(order.price)) {
      OrderNode node{order, nullptr, nullptr};
      Level level{order.price, &node, nullptr};
      level_map.insert({order.price, level});
    }
    // add order to the price_level
    Level price_level = level_map[order.price];
    OrderNode node{order, price_level.tail, nullptr};
    price_level.tail->right = &node;
    price_level.tail = price_level.tail->right;

    // add order to the order_map
    order_map.insert({order.id, {&node, &price_level}});
  };
  void cancel_order(OrderID order_id);

 private:
  std::unordered_map<OrderID, OrderMetadata> order_map;
  std::unordered_map<Price, Level> level_map;
};