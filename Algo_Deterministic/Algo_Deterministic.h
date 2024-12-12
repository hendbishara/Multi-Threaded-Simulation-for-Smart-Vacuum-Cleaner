#ifndef ALGO_209120054_207759085_Deterministic_H
#define ALGO_209120054_207759085_Deterministic_H



#include "../common/WallSensor.h"
#include "../common/WallSensor.h"
#include "../common/BatteryMeter.h"
#include "../common/DirtSensor.h"
#include <stack>
#include "../common_algo/VaccumMap.h"
#include <utility>
#include "../algorithm/AlgorithmRegistration.h"
#include "../common_algo/BaseAlgorithm.h"


class Algo_209120054_207759085_Deterministic : public BaseAlgorithm{
    
    std::stack<Step> return_path;
    bool charging = false;
    bool returntotop = false;



    public:
        Algo_209120054_207759085_Deterministic() =default;
        Step nextStep() override;
        Algo_209120054_207759085_Deterministic(const Algo_209120054_207759085_Deterministic& alg) = delete;
        Algo_209120054_207759085_Deterministic& operator=(const Algo_209120054_207759085_Deterministic& alg) = delete;


};

#endif //Algo_209120054_207759085_DETERMINISTIC_H