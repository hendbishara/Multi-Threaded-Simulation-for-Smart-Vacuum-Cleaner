#ifndef ALGO_209120054_207759085_RANDOM_H
#define ALGO_209120054_207759085_RANDOM_H



#include "../common/WallSensor.h"
#include "../common/WallSensor.h"
#include "../common/BatteryMeter.h"
#include "../common/DirtSensor.h"
#include <stack>
#include "../common_algo/VaccumMap.h"
#include <utility>
#include "../algorithm/AlgorithmRegistration.h"
#include "../common_algo/BaseAlgorithm.h"


class Algo_209120054_207759085_Random : public BaseAlgorithm{
    
    std::stack<Step> return_path;
    bool charging = false;
    bool returntotop = false;



    public:
        Algo_209120054_207759085_Random() =default;
        Step nextStep() override;
        Algo_209120054_207759085_Random(const Algo_209120054_207759085_Random& alg) = delete;
        Algo_209120054_207759085_Random& operator=(const Algo_209120054_207759085_Random& alg) = delete;
        std::pair<int,int> nextCoord (int x, int y, Direction d);


};

#endif //Algo_209120054_207759085_Random_H