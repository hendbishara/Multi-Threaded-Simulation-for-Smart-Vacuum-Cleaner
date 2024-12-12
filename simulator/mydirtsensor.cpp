#include "mydirtsensor.h"


/**
 * @brief constructor for MyDirtSenssor class
 * @param std::shared_ptr<RobotState>  myrobot
 * @return MyDirtSensor
 */
MyDirtSensor::MyDirtSensor(std::shared_ptr<RobotState>  myrobot): robot(myrobot){}


/**
 * @brief gest dirt level in current point the robot is in
 * @return int
 */

int MyDirtSensor::dirtLevel() const {
    return robot->getDirt();
}

