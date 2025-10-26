Rapidcsv for handling the LOBSTER data
https://github.com/d99kris/rapidcsv

To run the project

mkdir -p build
cd build
cmake ..
make
./main


More features I want to implement in the future
  - networked component
  - thread for handling updates to order book
  - threads for getting atomic snapshots of the order book
  - use the ring buffer for updating order book
  - more handling of different kinds of orders such as updating orders
  - more analytics on the order book, things like implied volatility
    calculations and greeks perhaps
  - more performance analysis and finetuning of it
  - perhaps make a fast parser of some standard protocol like itch
