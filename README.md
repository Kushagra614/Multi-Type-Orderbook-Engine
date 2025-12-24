# 📊 Multi-Type Orderbook Engine

A high-performance C++ implementation of an order matching engine that supports multiple order types with a command-line interface. This implementation provides efficient price-time priority matching and real-time trade execution.

## 🏷️ Supported Order Types

- **GTC (Good Till Cancel) ⏳**
  - Remains active in the order book until explicitly cancelled or filled
  - Example: Buy 100 shares at $50 GTC

- **FAK (Fill and Kill) ⚡**
  - Fills as much as possible at the specified price
  - Cancels any unfilled portion immediately
  - Example: Sell 50 shares at $100 FAK

- **FOK (Fill or Kill) 💥**
  - Must be filled completely at the specified price or better
  - If not filled immediately, the entire order is cancelled
  - No partial fills

- **GFD (Good For Day) 🌞**
  - Active until the end of the trading day
  - Automatically cancelled if not filled by market close

- **Market Order 🏪**
  - Executes immediately at the best available price
  - No price guarantee - fills at current market prices
  - Highest execution priority

## 🚀 Features

### Interactive Command-Line Interface
- **User-friendly Menu** - Simple text-based interface for order management
- **Real-time Order Book** - View current market depth with bids and asks
- **Order Status** - Immediate feedback on order execution and trades

### Order Book Display
- **Price Levels** - Clear view of bid and ask prices with quantities
- **Order Matching** - Visual feedback on executed trades
- **Active Orders** - Track all open orders in the system

### Order Management
- **Add Orders** - Place new buy/sell orders with various order types
- **Cancel Orders** - Remove existing orders by ID
- **Trade Execution** - Automatic matching of compatible orders

## 🛠️ Building and Running

```bash
# Clone the repository
git clone https://github.com/yourusername/multi-type-orderbook.git
cd multi-type-orderbook

# Build the project
g++ -std=c++20 -Iinclude src/Orderbook.cpp main.cpp -o orderbook_run

# Run the order book
./orderbook_run
```

## 📋 Usage

1. **Add Buy/Sell Order**
   - Select order type (GTC, FAK, FOK, GFD, Market)
   - Enter quantity and price (except for market orders)
   - Order is processed and matched immediately

2. **Cancel Order**
   - Enter the order ID to cancel
   - Immediate confirmation of cancellation

3. **View Order Book**
   - See current market depth
   - View all active bids and asks
   - Check total number of active orders

## 🛠️ Building from Source

### Prerequisites
- C++20 compatible compiler (GCC 10+, Clang 12+, or MSVC 2019+)
- CMake 3.15+ (recommended)
- Google Test (for running tests)

### Build Instructions

```bash
# Clone the repository
git clone [your-repo-url]
cd OrderBook

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build the project
make -j$(nproc)

# Run the demo
./orderbook_demo

## 📈 Next Steps
- Explore the codebase and experiment with different order types
- Run the test suite to verify functionality
- Contribute by reporting issues or submitting pull requests
- Check out the examples in the `examples/` directory

---

> "The stock market is a device for transferring money from the impatient to the patient." — Warren Buffett

Happy trading! 🚀
