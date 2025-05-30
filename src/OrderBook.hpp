#include <array>
#include <memory>
#include <unordered_map>

using OrderID = uint64_t;
using Price = double;
using OrderVolume = uint64_t;

/**
 * Represents a single limit order inside of a price level.
 */
struct Order {
  OrderID id;
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
  OrderNode* order_node;
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
  void place_order(Order order);
  /**
   * Cancel the order with ID 'order_id' from the order book.
   */
  void cancel_order(OrderID order_id);

 private:
  std::unordered_map<OrderID, OrderMetadata> order_map;
  std::unordered_map<Price, Level> level_map;
};