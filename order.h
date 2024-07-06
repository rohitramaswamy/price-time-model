#ifndef ORDER_H
#define ORDER_H

#include <chrono>

enum OrderType
{
    BUY,
    SELL
};

enum class OrderMode
{
    Limit,
    Market
};

#include <chrono>

class Order
{
private:
    OrderType order;
    OrderMode mode;
    double price;
    int quantity;
    int id;
    int order_id;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    static int next_order_id; // Static member to keep track of the next order ID

public:
    // Constructor
    Order(const OrderType &ord, const OrderMode &ordM, double prc, int qty, int bot_id)
        : order(ord), mode(ordM), price(prc), quantity(qty), id(bot_id), start_time(std::chrono::high_resolution_clock::now())
    {
        order_id = next_order_id++; // Assign current order ID and increment for the next order
    }

    // Getter for order_id
    int getOrderID() const
    {
        return order_id;
    }

    // Method to get the price
    double getPrice() const { return price; }

    // Method to set the price
    void setPrice(double prc) { price = prc; }

    // Method to get the quantity
    int getQuantity() const { return quantity; }

    // Method to set the quantity
    void setQuantity(int qty) { quantity = qty; }

    // Method to get the ID
    int getId() const { return id; }

    // Method to set the ID
    void setId(int newId) { id = newId; }

    // Method to get the order type
    OrderType getOrderType() const { return order; }

    // Method to set the order type
    void setOrderType(OrderType ord) { order = ord; }

    // Method to get the order mode
    OrderMode getOrderMode() const { return mode; }

    // Method to set the order mode
    void setOrderMode(OrderMode ordM) { mode = ordM; }

    // Method to get the start time
    std::chrono::time_point<std::chrono::high_resolution_clock> getTime() const { return start_time; }

    // Method to set the start time
    void setTime(std::chrono::time_point<std::chrono::high_resolution_clock> time) { start_time = time; }
};

// Definition of the static member variable
int Order::next_order_id = 1; // Initialize to 1 or any other starting value

;

#endif // ORDER_H
