# 📊 Multi-Type-Orderbook-Engine


A high-performance, thread-safe C++ implementation of a limit order book (LOB) designed for electronic trading systems. This implementation provides efficient order matching, comprehensive order types, and real-time trade execution.

## 🏷️ Order Types

### Core Order Types
- **GTC (Good Till Cancel) ⏳**
  - Remains active in the order book until explicitly cancelled
  - Default order type for most trading activities
  - Example: Buy 100 shares at $50 GTC

- **IOC (Immediate or Cancel) ⚡**
  - Executes immediately at the best available price
  - Any unfilled portion is automatically cancelled
  - Ideal for time-sensitive trades

- **FOK (Fill or Kill) 💥**
  - Must be filled in its entirety or cancelled
  - No partial fills allowed
  - Ensures complete execution or none at all

- **Market Order 🏪**
  - Executes immediately at the best available price
  - No price guarantee, fills at current market prices
  - Highest priority in the matching engine

- **Limit Order 🎯**
  - Executes only at the specified price or better
  - Can be partially filled across multiple price levels
  - Provides price protection

## 🚀 Features

### Matching Engine
- **Price-Time Priority** - Orders are matched based on price first, then time of entry
- **Efficient Data Structures** - Uses optimized containers for fast order operations
- **Real-time Execution** - Immediate trade generation and execution reports

### Order Management
- **Order Lifecycle** - Full support for order states (New, Partially Filled, Filled, Cancelled)
- **Bulk Operations** - Batch order operations for improved performance
- **Order Modification** - In-place order updates without losing priority

### Performance
- **Lock-free Design** - Minimized locking for high throughput
- **Memory Efficient** - Low-latency memory management
- **Scalable** - Handles high-frequency trading volumes

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
