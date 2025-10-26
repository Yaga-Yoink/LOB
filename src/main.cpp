#include <iostream>

#include "CSVParser.hpp"
#include "OrderBook.hpp"
#include "RingBuffer.hpp"
#include "rapidcsv.h"

int main() {
  CSVParser parser("../data/GOOG_2012-06-21_34200000_57600000_message_1.csv");

  OrderBook book{};
  while (true) {
    try {
      Row row = parser.next();
      Order order{row.id, row.side, row.price, row.volume};
      book.place_order(order);
      printf("Placed Order: %lld\n", row.id);
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      break;
    }
  }
  Order order{1, Buy, 1, 1};
  book.place_order(order);
  Order order2{1, Buy, 1, 1};
  book.place_order(order2);
  std::cout << "here" << std::endl;
  std::cout << book.get_bid_price() << std::endl;

  std::cout << sizeof(Side) << std::endl;

  Order_QUEUE<100> queue{};
  Order order3{1, Buy, 1, 1};
  queue.write(order3);

  Order order4{};
  bool flag = queue.poll(&order4);

  // rapidcsv::Document doc(
  //     "data/GOOG_2012-06-21_34200000_57600000_message_1.csv");

  return 0;
}