#pragma once

#include <memory>
#include <unordered_map>
#include <limits>
#include <algorithm>

struct Order;
struct OrderNode;
struct Level;
struct OrderMetadata;

using OrderID = uint64_t;
using Price = double;
using OrderVolume = uint64_t;

using LevelMap = std::unordered_map<Price, std::shared_ptr<Level>>;
using OrderMap = std::unordered_map<OrderID, OrderMetadata>;

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
  std::weak_ptr<OrderNode> left;
  std::shared_ptr<OrderNode> right;
};

/**
 * Represents a price level in the order book with a doubly linked list of
 * orders.
 */
struct Level {
  Price price;
  std::shared_ptr<OrderNode> head;
  std::weak_ptr<OrderNode> tail;
};

/**
 * Intermediary datastructure for referencing the order and its price level.
 */
struct OrderMetadata {
  std::shared_ptr<OrderNode> order_node;
  std::shared_ptr<Level> level;
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
  OrderMap bid_order_map;
  OrderMap ask_order_map;
  LevelMap bid_level_map;
  LevelMap ask_level_map;
};
