
#include "OrderBookBuilder.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "OrderBook.hpp"
#include "RingBuffer.hpp"

// Order& parseLine(std::string str) {
//   Order order{};
//   std::string delimiter = ",";
//   std::size_t pos = str.find(delimiter);
//   str.substr(0, pos);

//   str.erase(0, pos);
// }

// std::vector<Order> readCSV(std::string path) {
//   std::ifstream file(path);
//   std::string str;
//   std::vector<Order&> vec;
//   while (std::getline(file, str)) {
//     vec.push_back(parseLine(str));
//   }
// }