#include <iostream>

#include "OrderBook.hpp"

int main() {
  OrderBook book{};
  Order order{1, 1, 1, 1};
  book.place_order(order);
  std::cout << "here" << std::endl;
  return 0;
}