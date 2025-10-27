#include <gtest/gtest.h>
#include "OrderBook.hpp"

TEST(BASIC, BasicAssertions) {
    OrderBook ob;
    
    Order order1{1, Side::Buy, 10, 5};
    Order order2{1, Side::Sell, 15, 5};

    ob.place_order(order1);
    ob.place_order(order2);
    EXPECT_EQ(ob.get_bid_price(), 10);
    EXPECT_EQ(ob.get_ask_price(), 15);
}