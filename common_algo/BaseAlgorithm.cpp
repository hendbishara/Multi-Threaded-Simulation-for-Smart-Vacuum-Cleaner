#include "BaseAlgorithm.h"
#include "../common/enums.h"
#include "iostream" 


/**
 * @brief sets the field max steps
 * @param std::size_t maxsteps
 * @return void
 */
void BaseAlgorithm::setMaxSteps(std::size_t maxSteps){
    total_steps = maxSteps;
}

/**
 * @brief sets wall sensor
 * @param const WallSensor& ws
 * @return void 
 */
void BaseAlgorithm::setWallsSensor(const WallsSensor& ws){
    walls_sensor = &ws;
}

/**
 * @brief sets dirt sensor
 * @param const DirtSensor& ws
 * @return void 
 */
void BaseAlgorithm::setDirtSensor(const DirtSensor& ds){ 
    dirt_sensor = &ds;
}

/**
 * @brief sets battery sensor
 * @param const BatteryMeter& bm
 * @return void 
 */
void BaseAlgorithm::setBatteryMeter(const BatteryMeter& bm){
    battery_meter = &bm;
    max_battery_steps = battery_meter->getBatteryState();
}

/**
 * @brief pops last element from stack and return step to go in according to the stack's state
 * 
 * @return Step
 */

Step BaseAlgorithm::pop_stack(){
    //check if in docking and stack is empty
    if(dfs.empty() && (x_coord == 0) && (y_coord == 0)){return Step::Finish;}
    //if only one item left in stack, use parent pointers to go back do cking
    if(dfs.size() == 1){
        //go to docking with parent pointer
        Step direction;
        const std::array<int, 2> parent = vmap->getPiCoord(x_coord,y_coord);
        if(parent[0]-x_coord == -1){direction=Step::South;}
        else if (parent[0]-x_coord == 1){direction = Step::North;}
        else if(parent[1]-y_coord == 1){direction=Step::East;}
        else{direction=Step::West;}
        x_coord = parent[0];
        y_coord = parent[1];
        total_steps-=1;
        dfs.pop();
        return direction;
    }
    dfs.pop();
    int x = dfs.top().first;
    int y = dfs.top().second;

    if(x-x_coord == -1){total_steps-=1; x_coord -=1;  return Step::South;}
    if(x-x_coord == 1){total_steps-=1; x_coord +=1; return Step::North;}
    if(y-y_coord == -1){total_steps-=1; y_coord -=1; return Step::West;}
    else{total_steps-=1; y_coord +=1; return Step::East;}
}

/**
 * @brief checks if sensors were properly initialized
 *
 * @return bool 
 */
bool BaseAlgorithm::check_sensors_init() const{
    if(battery_meter == nullptr){
        std::cout << "ERROR: Battery Meter Sensor not initialized!" << std::endl;
        return false;}
    if(walls_sensor == nullptr){
        return false;
         std::cout << "ERROR: Wall Sensor not initialized!" << std::endl;
         }
    if(dirt_sensor == nullptr){
        return false;
         std::cout << "ERROR: Dirt Sensor not initialized!" << std::endl;
        }
    return true;
}
