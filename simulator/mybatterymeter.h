#ifndef MYBATTERYMETER_H
#define MYBATTERYMETER_H

#include "./common/BatteryMeter.h"
#include "robot_state.h"
#include "memory"

class MyBatteryMeter :public BatteryMeter{
    std::shared_ptr<RobotState> robot;
    public:
        MyBatteryMeter() = default;
        MyBatteryMeter(std::shared_ptr<RobotState>  myrobot);
        std::size_t getBatteryState() const override;
        MyBatteryMeter(const MyBatteryMeter& batterysensor) = delete;
        MyBatteryMeter& operator =(const MyBatteryMeter& batterySensor) = delete;
};

#endif  // MYBATTERYMETER_H
