#ifndef BASEALGORITHM_H
#define BASEALGORITHM_H



#include "../common/WallSensor.h"
#include "../common/WallSensor.h"
#include "../common/BatteryMeter.h"
#include "../common/DirtSensor.h"
#include "../common/AbstractAlgorithm.h"
#include <stack>
#include "../common_algo/VaccumMap.h"
#include <utility>
#include "../algorithm/AlgorithmRegistration.h"


class BaseAlgorithm : public AbstractAlgorithm{
    protected:
        std::size_t total_steps;
        std::size_t max_battery_steps;
        const WallsSensor* walls_sensor;
        const DirtSensor* dirt_sensor;
        const BatteryMeter* battery_meter;
        int x_coord = 0;
        int y_coord = 0;
        std::stack<std::pair<int,int>> dfs;
        
        std::unique_ptr<VaccumMap> vmap = std::make_unique<VaccumMap>(); 
    





    

    public:
        void setMaxSteps(std::size_t maxSteps) override;
        void setWallsSensor(const WallsSensor&) override;
        void setDirtSensor(const DirtSensor&) override;
        void setBatteryMeter(const BatteryMeter&) override;
        Step pop_stack();
        bool check_sensors_init() const;
    
};

#endif //BASEALGORITHM_H