#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

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

using Row = std::tuple<Time, EventType, OrderID, OrderVolume, Price, Side>;

class CSVParser {
 public:
  CSVParser(std::string fp) : stream{fp} {};

  /**
   * Returns the next row of the CSV containing a 'message' that updated
   * the limit order book.
   * Throws runtime_exception if there are no more rows to read.
   */
  Row next();

 private:
  std::ifstream stream;
};