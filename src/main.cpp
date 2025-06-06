#include <iostream>

#include "OrderBook.hpp"

int main() {
  OrderBook book{};
  Order order{1, 1, 1, 1};
  book.place_order(order);
  Order order2{1, 1, 1, 1};
  book.place_order(order2);
  std::cout << "here" << std::endl;
  std::cout << book.get_bid_price() << std::endl;
  return 0;
}