#pragma once

#include <memory>
#include <unordered_map>

using OrderID = uint64_t;
using Price = double;
using OrderVolume = uint64_t;

enum Side { Buy = 1, Sell = -1 };

/**
 * Represents a single limit order inside of a price level.
 */
struct Order {
  OrderID id;
  Side direction;
  Price price;
  OrderVolume volume;
};

/**
 * Represents a node in a doubly linked list
 */
struct OrderNode {
  Order order;
  // left refers to older order with higher priority
  OrderNode* left;
  OrderNode* right;
};

/**
 * Represents a price level in the order book with a doubly linked list of
 * orders.
 */
struct Level {
  Price price;
  OrderNode* head;
  OrderNode* tail;
};

/**
 * Intermediary datastructure for referencing the order and its price level.
 */
struct OrderMetadata {
  std::unique_ptr<OrderNode> order_node;
  Level* level;
};

/**
 * Representation for the order book
 */
class OrderBook {
 public:
  /**
   * Add an 'order' to the order book.
   */
  void place_order(const Order& order);
  /**
   * Cancel the order with ID 'order_id' from the order book.
   */
  void cancel_order(OrderID order_id);

  /**
   * Get the best bid price
   */
  Price get_bid_price() const;

  /**
   * Get the best ask price
   */
  Price get_ask_price() const;

 private:
  // best bid price on order book
  Price bid_price = std::numeric_limits<Price>::min();
  // best ask price on order book
  Price ask_price = std::numeric_limits<Price>::max();
  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>> bid_order_map;
  std::unordered_map<OrderID, std::unique_ptr<OrderMetadata>> ask_order_map;
  std::unordered_map<Price, std::unique_ptr<Level>> bid_level_map;
  std::unordered_map<Price, std::unique_ptr<Level>> ask_level_map;
};