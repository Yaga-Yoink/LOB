#pragma once

#include <array>
#include <atomic>

#include "OrderBook.hpp"

/*
 * This is a single-producer single-consumer queue.
 * Requires: only a single producer thread and a single-consumer thread
 */
template <unsigned int N>
class Order_QUEUE {
  std::array<Order, N> queue;
  unsigned int head;
  std::atomic<unsigned int> tail;

 public:
  Order_QUEUE() : queue{}, head{0} {
    tail.store(0, std::memory_order_seq_cst);
  };

  /*
   *  'write' appends 'order' to the queue.
   */
  void write(const Order& order);

  /*
   *  Returns true if the queue had an element to read, false otherwise.
   * Side-effects: 'order_ptr' points to the order at the tail.
   */
  bool poll(Order* order_ptr);
};

// TODO: handle overflow, prevent tail from overwriting the value at head
template <unsigned int N>
void Order_QUEUE<N>::write(const Order& order) {
  unsigned tl = tail.load(std::memory_order_relaxed);
  unsigned i = tl % N;
  queue[i] = order;
  tail.store(tl + 1, std::memory_order_release);
};

template <unsigned int N>
bool Order_QUEUE<N>::poll(Order* order) {
  unsigned tl = tail.load(std::memory_order_acquire);
  if (tl > head) {
    unsigned i = head % N;
    *order = queue[i];
    head++;
    return true;
  } else {
    return false;
  }
};