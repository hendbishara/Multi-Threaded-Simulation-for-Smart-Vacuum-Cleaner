#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

#include "house.h"
#include <cstddef>
#include "./common/enums.h"
#include "memory"
#include <vector>

class RobotState{
    std::unique_ptr<House> house;
    int total_steps;
    std::size_t battery_left;
    int curr_coord_x;
    int curr_coord_y;

    public:
        RobotState(std::vector<std::vector<int>> &&house_map, int total_steps, std::size_t battery_left, int x, int y, int house_rows, int house_cols);
        std::size_t getBatteryLeft() const;
        int getDirt() const;
        int wallSensor(Direction d) const;
        int getTotalSteps() const;
        int getXcoord() const;
        int getYcoord() const;
        void setTotalSteps(int steps);
        void setCurrCoord(int x,int y);
        void reduceBattery();
        void cleanDirtInPoint();
        RobotState(const RobotState& robot) = delete;
        RobotState& operator=(const RobotState& robot) = delete;
        void setBattery(std::size_t battery);


};

#endif //ROBOT_STATE_H_