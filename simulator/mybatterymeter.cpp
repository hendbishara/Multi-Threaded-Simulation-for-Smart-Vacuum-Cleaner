#include "mybatterymeter.h"



/**
 * @brief constructor for MyBatteryMeter class
 * 
 * @param std::shared_ptr<RobotState>  myrobot
 * @return MyBatteryMeter
 */
MyBatteryMeter::MyBatteryMeter(std::shared_ptr<RobotState>  myrobot): robot(myrobot){
}


/**
 * @brief gets the battery left currently in the robot
 * @return std::size_t
 */
std::size_t MyBatteryMeter::getBatteryState() const{
    return robot->getBatteryLeft();
}


