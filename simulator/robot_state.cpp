#include "robot_state.h"
#include <memory>
#include <iostream>



/**
 * @brief constructor for RobotState class
 * @param std::vector<std::vector<int>> &&house_map
 * @param int total_steps
 * @param std::size_t battery_left
 * @param int x
 * @param int y
 * @param int house_rows
 * @param int house_cols
 * @return RobotState
 */

RobotState::RobotState(std::vector<std::vector<int>> &&house_map, int total_steps, std::size_t battery_left, int x, int y, int house_rows, int house_cols): total_steps(total_steps), battery_left(battery_left), curr_coord_x(x), curr_coord_y(y){
    house = std::make_unique<House>(std::move(house_map),house_rows,house_cols);

}


/**
 * @brief returns battery left in robot
 * 
 * @return std::size_t
 */

std::size_t RobotState::getBatteryLeft() const{
    return battery_left;
}


/**
 * @brief returns dirt in current point that the robot is on
 * 
 * @return int 
 */

int RobotState::getDirt() const{
    return  house->getDirt(curr_coord_x,curr_coord_y);
}


/**
 * @brief returns if there is a wall in direction d from current point, if wall returns 1 else 0
 * 
 * @return int 
 */

int RobotState::wallSensor(Direction d) const{
    if(d == Direction::North){
        return house->getWall(curr_coord_x-1,curr_coord_y);
    }
    if(d == Direction::South){
         return house->getWall(curr_coord_x + 1,curr_coord_y);
    }
    if(d==Direction::East){
         return house->getWall(curr_coord_x,curr_coord_y+1);
    }
    return house->getWall(curr_coord_x,curr_coord_y-1);
}


/**
 * @brief returns total steps left to perform 
 * 
 * @return int 
 */

int RobotState::getTotalSteps() const{
    return total_steps;
}

/**
 * @brief returns current x coordinate
 * @return int 
 */
int RobotState::getXcoord() const{
    return curr_coord_x;
}

/**
 * @brief returns current y coordinate
 * @return int 
 */
int RobotState::getYcoord() const{
    return curr_coord_y;
}

/**
 * @brief updates total steps left
 * @param int steps
 * @return int 
 */
void RobotState::setTotalSteps(int steps){
    total_steps = steps;
}


/**
 * @brief updates current coordinates of the robot
 * @param int x
 * @param int y
 * @return void
 */
void RobotState::setCurrCoord(int x,int y){
   curr_coord_x = x;
   curr_coord_y = y; 
}

/**
 * @brief reduces battery left -1
 * @return void
 */
void RobotState::reduceBattery(){
    battery_left-=1;
}

/**
 * @brief reduces dirt in current point robot is in with 1
 * @return void
 */

void RobotState::cleanDirtInPoint(){
    house->setDirt(curr_coord_x,curr_coord_y,house->getDirt(curr_coord_x,curr_coord_y)-1);
}

/**
 * @brief updates battery left in robot
 * @param std::size_t battery
 * @return void
 */

void RobotState::setBattery(std::size_t battery){
    battery_left = battery;
}