#ifndef MOVINGAVERAGESTRATEGY_H
#define MOVINGAVERAGESTRATEGY_H

#include "TradingStrategy.h"

class MovingAverageStrategy : public TradingStrategy {
public:
    std::vector<std::pair<int, double>> generateSignals(const std::vector<double>& data) override;
private:
    std::vector<double> calculateSMA(const std::vector<double>& data, int period);
};

#endif // MOVINGAVERAGESTRATEGY_H
