#include "CSVParser.hpp"

Row CSVParser::next() {
  std::string line;
  if (!std::getline(stream, line)) {
    throw std::runtime_error("No more lines to read.");
  }

  Time time;
  EventType type;
  OrderID id;
  OrderVolume volume;
  Price price;
  Side side;

  std::vector<std::string> res;
  res.reserve(6);
  std::stringstream ss{line};

  std::string entry;
  while (std::getline(ss, entry, ',')) {
    res.push_back(entry);
  }
  time = std::stod(res[0]);
  type = static_cast<EventType>(std::stoi(res[1]) - 1);
  id = static_cast<long long>(std::stoll(res[2]));
  volume = static_cast<long long>(std::stoll(res[3]));
  price = std::stod(res[4]);
  side = static_cast<Side>(std::stoi(res[5]));

  return {time, type, id, volume, price, side};
}
