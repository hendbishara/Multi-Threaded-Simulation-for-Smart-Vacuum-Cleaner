#ifndef MYWALLSENSOR_H_
#define MYWALLSENSOR_H_

#include "./common/WallSensor.h"
#include "robot_state.h"
#include <memory>

class MyWallSensor: public WallsSensor{
    std::shared_ptr<RobotState> robot = nullptr;

    public:
        MyWallSensor() = default;
        MyWallSensor(std::shared_ptr<RobotState> myrobot);
        virtual bool isWall(Direction d) const override;
        MyWallSensor(const MyWallSensor& wallsensor) = delete;
        MyWallSensor& operator =(const MyWallSensor& WallsSensor) = delete;





};

#endif //MYWALLSENSOR_H_