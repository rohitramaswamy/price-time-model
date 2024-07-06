#include "market.h"
#include "marketMakers.h"
#include "bot.h"
#include <limits>

// if the id is zero, it's a market maker
Market::Market(std::vector<MarketMakers> MM) : market_makers(std::move(MM))
{
    for (size_t i = 0; i < market_makers.size(); ++i)
    {
        buy_limit_orders.push({OrderType::BUY, OrderMode::Limit, market_makers[i].getBid(), std::numeric_limits<double>::infinity(), 0});
        sell_limit_orders.push({OrderType::SELL, OrderMode::Limit, market_makers[i].getAsk(), std::numeric_limits<double>::infinity(), 0});
    }

    highest_bid = buy_limit_orders.top().getPrice();
    lowest_ask = sell_limit_orders.top().getPrice();
}

bool Market::getRandomBoolean(double number)
{
    std::random_device rd;
    std::default_random_engine engine(rd());

    // Assume number ranges from -40 to 40
    // Map the range -40 to 40 to [0, 1]
    double probability = (number + 40) / 80.0;
    probability = std::clamp(probability, 0.0, 1.0);

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine) < probability;
}

double Market::getRandomDouble(double min, double max)
{
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine);
}

int Market::getRandomInteger(int min, int max)
{
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

void Market::execute_trade(double price, int trade_quantity)
{

    order_book.emplace_back(price, trade_quantity);
}

double Market::getHighestBid()
{
    return highest_bid;
}

// Getter for lowest_ask

double Market::getLowestAsk()
{
    return lowest_ask;
}

void Market::check_orders_and_spread(int seconds, std::atomic<bool> &running)
{
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < seconds)
    {
        // Your code here
        check_limit_orders();
        regenerate_spread();
        std::cout << highest_bid << " " << lowest_ask << std::endl;

        // Sleep for a short duration to avoid excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    running = false;
}

void Market::simulate_market(int seconds, int num_bots, int trades_per_bot)
{
    std::mutex vec_mutex;
    std::atomic<bool> running(true);

    // check if any new limit orders have been created
    std::thread check_orders_and_spread(&Market::check_orders_and_spread, this, seconds, std::ref(running));

    std::vector<std::thread> bot_threads;
    std::vector<std::unique_ptr<Bots>> bots_list;

    for (int i = 0; i < num_bots; ++i)
    {
        bots_list.emplace_back(std::make_unique<Bots>(10000.0, 500, i + 1));
        bot_threads.emplace_back(&Bots::bot_trade, bots_list.back().get(), std::ref(*this), std::ref(vec_mutex), trades_per_bot, seconds, std::ref(running));
        bots.insert({i + 1, *bots_list.back()});
    }

    for (auto &thread : bot_threads)
    {
        thread.join();
    }

    check_orders_and_spread.join();

    std::ofstream outFile("numbers.txt");
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    for (const auto &trade : order_book)
    {
        outFile << trade.price << std::endl;
    }

    outFile.close();
    std::cout << "Simulation finished" << std::endl;
}

double Market::calculate_buy_rate(std::chrono::seconds interval)
{
    auto now = std::chrono::high_resolution_clock::now();
    while (!buy_timestamps.empty() && (now - buy_timestamps.front()) > interval)
    {
        buy_timestamps.pop();
    }
    double rate = static_cast<double>(buy_timestamps.size()) / interval.count();
    std::cout << "buy " << rate << std::endl;

    return rate;
}

double Market::calculate_sell_rate(std::chrono::seconds interval)
{
    auto now = std::chrono::high_resolution_clock::now();
    while (!sell_timestamps.empty() && (now - sell_timestamps.front()) > interval)
    {
        sell_timestamps.pop();
    }
    double rate = static_cast<double>(sell_timestamps.size()) / interval.count();
    std::cout << "sell " << rate << std::endl;
    return rate;
}

// double Market::calculate_highest_bid()
// {
//     double max_bid = market_makers[0].getBid();
//     for (size_t i = 1; i < market_makers.size(); ++i)
//     {

//         if (market_makers[i].getBid() > max_bid)
//         {
//             max_bid = market_makers[i].getBid();
//         }
//     }
//     return max_bid;
// }

// double Market::calculate_lowest_ask()
// {
//     double min_ask = market_makers[0].getAsk();
//     for (size_t i = 1; i < market_makers.size(); ++i)
//     {

//         if (market_makers[i].getBid() < min_ask)
//         {
//             min_ask = market_makers[i].getAsk();
//         }
//     }
//     return min_ask;
// }

// void Market::update_market_maker()
// {
//     double new_sell_rate = calculate_sell_rate(std::chrono::seconds(1));
//     double new_buy_rate = calculate_buy_rate(std::chrono::seconds(1));

//     for (size_t i = 0; i < market_makers.size(); ++i)
//     {
//         market_makers[i].setBid(market_makers[i].getBid() + getRandomDouble(1, 1.5) * (new_buy_rate - current_buy_rate));
//         market_makers[i].setAsk(market_makers[i].getAsk() + getRandomDouble(1, 1.5) * (new_sell_rate - current_sell_rate));
//     }
//     current_buy_rate = new_buy_rate;
//     current_sell_rate = new_sell_rate;

//     highest_bid = buy_limit_orders.top().getPrice();
//     lowest_ask = sell_limit_orders.top().getPrice();
// }

double Market::change_in_buy_rate()
{
    double old_buy_rate = current_buy_rate;
    current_buy_rate = calculate_buy_rate(std::chrono::seconds(1));
    return current_buy_rate - old_buy_rate;
}

double Market::change_in_sell_rate()
{
    double old_sell_rate = current_sell_rate;
    current_sell_rate = calculate_buy_rate(std::chrono::seconds(1));
    return current_buy_rate - old_sell_rate;
}

double Market::change_in_price(std::chrono::seconds interval)
{
    auto now = std::chrono::high_resolution_clock::now();
    while (!price_timestamps_queue.empty() && (now - price_timestamps_queue.front().second) > interval)
    {
        price_timestamps_queue.pop();
    }
    double rate = static_cast<double>(price_timestamps_queue.back().first - price_timestamps_queue.front().first);
    return rate;
}