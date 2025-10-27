#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <istream>
#include <vector>

#include "OrderBook.hpp"

using Time = double;  // seconds

enum EventType {
  submission,
  cancellation,
  deletion,
  execution_vis,
  execution_hid,
  cross_trade,
  trade_halt
};

struct Row {
    Time time;
    EventType type;
    OrderID id;
    OrderVolume volume;
    Price price;
    Side side;
};


class CSVParser {
 public:
  CSVParser(std::string fp) : stream{fp}{
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  };

  /**
   * Returns the next row of the CSV containing a 'message' that updated
   * the limit order book.
   * Throws runtime_exception if there are no more rows to read.
   */
  Row next();

 private:
  std::ifstream stream;
};
