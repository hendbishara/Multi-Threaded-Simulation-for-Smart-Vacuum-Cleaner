#include "mywallsensor.h"
#include <memory>

/**
 * @brief constructor for MyWallSenssor class
 * @param std::shared_ptr<RobotState>  myrobot
 * @return MyWallSensor
 */
MyWallSensor::MyWallSensor(std::shared_ptr<RobotState> myrobot):robot(myrobot){}


/**
 * @brief checks if there is a wall in direction d, if wall is peresent returns true
 * @param Direction d
 * @return bool
 */
bool MyWallSensor::isWall(Direction d) const{
    if(robot->wallSensor(d) == 1){
        return true;
    }

    return false;
}


