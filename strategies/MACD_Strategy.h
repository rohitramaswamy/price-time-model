#ifndef MACDSTRATEGY_H
#define MACDSTRATEGY_H

#include "TradingStrategy.h"

class MACDStrategy : public TradingStrategy
{
public:
    std::vector<std::pair<int, double>> generateSignals(const std::vector<double> &data) override;

private:
    std::vector<double> calculateEMA(const std::vector<double> &data, int period);
};

#endif // MACDSTRATEGY_H
