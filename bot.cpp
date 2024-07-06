#include "order.h"        // Make sure to include the Order header file
#include "marketMakers.h" // Include MarketMakers header file if exists
#include "bot.h"          // Include MarketMakers header file if exists
#include <thread>
#include "market.h"

Bots::Bots(double money, int stuff, int id) : cash(money), assets(stuff), bot_id(id){};

void Bots::bot_trade(Market &m, std::mutex &vec_mutex, int num_trades, double market_seconds, std::atomic<bool> &running, TradingStrategy &s)
{
    double num_seconds = (market_seconds / num_trades) * Market::getRandomDouble(1, 1.5);
    std::chrono::duration<double> duration_in_seconds(num_seconds);

    while (running)
    {

        std::this_thread::sleep_for(duration_in_seconds);

        std::lock_guard<std::mutex> guard(vec_mutex);

        double price;
        int quantity = Market::getRandomInteger(1, 10);

        bool patient = Market::getRandomBoolean(1.1);

        if (!patient)
        {
            if (Market::getRandomBoolean)
            {
                Bots::BuyMarketValue();
            }
            else
            {
                Bots::SellMarketValue();
            }
        }
        else
        {
            std::vector<std::pair<int, double>> signal = s.generateSignals(m.market_data);
            Bots::PlaceLimitOrders(signal);
        }

        // if market order (the bot is impatient)
    }
}
}

void Bots::BuyMarketValue()
{

    price = m.getLowestAsk();

    if (quantity * price - cash > 0)
    {
        // if keep buying until the complete quantity desired is bought

        // quantity to buy
        int quantity_available = m.order_quantity[m.buy_limit_orders.top().getOrderID()];
        int current_quantity = quantity;

        while (quantity_available - current_quantity > 0 && current_quantity * price > 0)
        {

            current_quantity = current_quantity - quantity_available;
            m.order_quantity.erase(m.sell_limit_orders.top().getOrderID());
            m.sell_limit_orders.pop();
            quantity_available = m.order_quantity[m.sell_limit_orders.top().getOrderID()];

            m.execute_trade(price, quantity);
            cash -= price * current_quantity;

            // adding to price timestamps queue to find the price rate
            m.price_timestamps_queue.push({price, std::chrono::high_resolution_clock::now()});

            price = m.getLowestAsk();

            assets += current_quantity;
        }

        if (price * current_quantity - cash >= 0)
        {
            cash -= price * current_quantity;
            // adding to price timestamps queue to find the price rate

            m.price_timestamps_queue.push({price, std::chrono::high_resolution_clock::now()});
            m.execute_trade(price, quantity);

            assets += current_quantity;
            if (m.order_quantity[m.buy_limit_orders.top().getOrderID()] -= current_quantity)
            {
                m.order_quantity.erase(m.buy_limit_orders.top().getOrderID());
            }
            assets += current_quantity;
        }
        //

        std::cout << "Bot " << bot_id << " added buy market order at " << price << " dollars." << std::endl;
    }

    else
    {
        std::cout << "Bot " << bot_id << " does not have sufficient assets to buy: " << std::endl;
    }
}

void Bots::SellMarketValue()
{

    // if bot is selling two but has one, the trade falls through, cant ask for more than what you have

    if (assets - quantity > 0)
    {
        // if keep selling until the complete quantity desired is sold
        // quantity to buy
        int quantity_available = m.order_quantity[m.buy_limit_orders.top().getOrderID()];
        int current_quantity = quantity;

        while (quantity_available - current_quantity > 0)
        {

            current_quantity = current_quantity - quantity_available;
            m.order_quantity.erase(m.buy_limit_orders.top().getOrderID());
            m.buy_limit_orders.pop();
            quantity_available = m.order_quantity[m.buy_limit_orders.top().getOrderID()];

            m.execute_trade(price, quantity);
            cash += price * current_quantity;

            m.price_timestamps_queue.push({price, std::chrono::high_resolution_clock::now()});

            price = m.getHighestBid();
        }

        cash += price * current_quantity;

        //
        if (m.order_quantity[m.buy_limit_orders.top().getOrderID()] -= current_quantity)
        {
            m.order_quantity.erase(m.buy_limit_orders.top().getOrderID());
        }

        m.order_quantity.erase(m.buy_limit_orders.top().getOrderID());

        assets -= quantity;

        std::cout << "Bot " << bot_id << " added sell market order at " << price << " dollars." << std::endl;
        m.price_timestamps_queue.push({price, std::chrono::high_resolution_clock::now()});

        m.order_quantity[m.buy_limit_orders.top().getOrderID()];
    }

    else
    {
        std::cout << "Bot " << bot_id << " does not have sufficient assets to sell: " << std::endl;
    }
}

void Bots::PlaceLimitOrders(std::vector<std::pair<int, double>> signal)
{

    for (size_t i = 0; i < signal.size(); ++i)
    {
        if (signal.back().first == 1)
        {
            // if price is decreasing, more people will buy, if increasing, less will buy

            // buying between the lowest price you can buy current and 5
            price = Market::getRandomDouble(m.getLowestAsk() - 5, m.getLowestAsk());
            Order o = Order(OrderType::BUY, OrderMode::Limit, price, quantity, bot_id);
            m.add_to_buy_limit_queue(o);
            m.order_quantity[o.getOrderID()] = quantity;

            std::cout << "Bot " << bot_id << " added buy limit order at " << price << " dollars." << std::endl;

            m.buy_timestamps.push(std::chrono::high_resolution_clock::now());
        }
        // if sell

        else if (signal.back().first == -1)
        {
            // selling between the highest price you can sell current and 5

            price = Market::getRandomDouble(m.getHighestBid(), m.getHighestBid() + 5);

            Order o = Order(OrderType::SELL, OrderMode::Limit, price, quantity, bot_id);
            m.add_to_sell_limit_queue(o);
            m.order_quantity[o.getOrderID()] = quantity;

            std::cout << "Bot " << bot_id << " added sell limit order at " << price << " dollars." << std::endl;
        }

        m.sell_timestamps.push(std::chrono::high_resolution_clock::now());
    }
};

double Bots::getCash() const
{
    return cash;
}

// Setter for cash
void Bots::setCash(double c)
{
    cash = c;
}

// Getter for assets
int Bots::getAssets() const
{
    return assets;
}

// Setter for assets
void Bots::setAssets(int a)
{
    assets = a;
}