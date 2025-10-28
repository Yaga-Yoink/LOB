#include <gtest/gtest.h>
#include "OrderBook.hpp"

TEST(BASIC, BasicAssertions) {
    OrderBook ob;
    
    Order order1{1, Side::Buy, 10, 5};
    Order order2{2, Side::Sell, 15, 5};

    ob.place_order(order1);
    ob.place_order(order2);
    EXPECT_EQ(ob.get_bid_price(), 10);
    EXPECT_EQ(ob.get_ask_price(), 15);

    ob.cancel_order(1);
    EXPECT_EQ(ob.get_bid_price(), std::numeric_limits<Price>::min());
    
    ob.cancel_order(2);
    EXPECT_EQ(ob.get_ask_price(), std::numeric_limits<Price>::max());
}

TEST(MULT_BIDS, BasicAssertions) {
    OrderBook ob;
    
    Order order1{1, Side::Buy, 10, 5};
    Order order2{2, Side::Buy, 16, 5};
    Order order3{3, Side::Buy, 10, 5};
    ob.place_order(order1);
    ob.place_order(order2);
    ob.place_order(order3);

    EXPECT_EQ(ob.get_bid_price(), 16);
    ob.cancel_order(2);
    EXPECT_EQ(ob.get_bid_price(), 10);
    ob.cancel_order(1);
    EXPECT_EQ(ob.get_bid_price(), 10);
}