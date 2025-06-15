#pragma once

#include <string>
#include <vector>

#include "OrderBook.hpp"
#include "RingBuffer.hpp"

/**
 *
 */
OrderBook& build_orderbook();

/*
 * Returns the orders in the CSV at 'path'
 */
std::vector<Order> readCSV(std::string path);
