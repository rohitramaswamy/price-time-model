#include <iostream>
#include <fstream>

#include <string>

#include <queue>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <atomic>
#include <mutex>

#include <cstdlib> // For the system() function

#include "order.h"
#include "market.h"

using namespace std;

int main()
{
    // Creating an object of Market class
    MarketMakers one(21.0, 22.0);
    MarketMakers two(21.1, 22.2);
    MarketMakers three(21.3, 22.4);

    Market m({one, two, three});

    // Run the market simulation for 10 seconds with 5 bots and 10 trades per bot
    m.simulate_market(100, 5, 1000);

    // std::string command = "python3 stockmarketplot.py"; // Use "python" instead of "python3" if Python 3 is default
    // int result = std::system(command.c_str());

    // if (result == 0)
    // {
    //     std::cout << "Python script executed successfully." << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Error executing Python script." << std::endl;
    // }

    return 0;
}
