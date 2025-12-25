# 📊 Multi-Type Orderbook Engine

A high-performance C++ implementation of an order matching engine that supports multiple order types with a command-line interface. This implementation provides efficient price-time priority matching and real-time trade execution.

## 🌟 Features

### Supported Order Types
- **GTC (Good Till Cancel)**: Remains active until cancelled or filled
- **FAK (Fill and Kill)**: Fills partially or not at all
- **FOK (Fill or Kill)**: Must fill completely or not at all
- **GFD (Good For Day)**: Active until market close
- **Market Order**: Executes immediately at best available price

### Key Functionality
- Real-time order matching
- Order book visualization
- Trade execution reports
- Order modification and cancellation

## 🖥️ Menu Interface

### Main Menu
```
=== Multi-Type Orderbook ===
1. Add New Order
2. Cancel Order
3. View Order Book
4. View Trades
5. View Active Orders
6. Exit

Enter your choice (1-6): 
```

### Add Order Screen
```
=== Add New Order ===
Order Type:
1. GTC (Good Till Cancel)
2. FAK (Fill and Kill)
3. FOK (Fill or Kill)
4. GFD (Good For Day)
5. Market Order

Select order type (1-5): 
```

## 🛠️ Building and Running

### Prerequisites
- CMake 3.12 or higher
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)

### Build Instructions
```bash
# Clone the repository
git clone https://github.com/Kushagra614/Multi-Type-Orderbook.git
cd Multi-Type-Orderbook

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make
```

### Running the Application
```bash
./OrderBook
```

## 📊 Order Book Display Example
```
=== Order Book ===
Bids (Buy)            Asks (Sell)
-------------------   -------------------
Price    Qty          Price    Qty
------   ------       ------   ------
100.50   500          101.00   300
100.25   300          101.25   200
100.00   200          101.50   100
```


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
```

> "The stock market is a device for transferring money from the impatient to the patient." — Warren Buffett

Happy trading! 🚀
