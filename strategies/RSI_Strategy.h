#ifndef RSISTRATEGY_H
#define RSISTRATEGY_H

#include "TradingStrategy.h"

class RSIStrategy : public TradingStrategy {
public:
    std::vector<std::pair<int, double>> generateSignals(const std::vector<double>& data) override;
private:
    double calculateRSI(const std::vector<double>& data, int period);
};

#endif // RSISTRATEGY_H
