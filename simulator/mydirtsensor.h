#ifndef MYDIRTSENSOR_H_
#define MYDIRTSENSOR_H_

#include "./common/DirtSensor.h"
#include "robot_state.h"
#include <memory>

class MyDirtSensor: public DirtSensor{
    std::shared_ptr<RobotState> robot;
    public:
        MyDirtSensor() = default;
        MyDirtSensor(std::shared_ptr<RobotState>  myrobot);
        int dirtLevel() const override;      
        MyDirtSensor(const MyDirtSensor& dirtsensor) = delete;
        MyDirtSensor& operator =(const MyDirtSensor& dirtsensor) = delete;
};


#endif //MYDIRTSENSOR_H_