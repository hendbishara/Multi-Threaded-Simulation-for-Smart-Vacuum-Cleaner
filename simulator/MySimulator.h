#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include "house.h"
#include "../common/AbstractAlgorithm.h"
#include "robot_state.h"
#include <memory>
#include <string>




class MySimulator{
    //std::unique_ptr<House> house;
    std::shared_ptr<RobotState> robot = nullptr;
    AbstractAlgorithm* myalgo;
    int max_steps;
    std::size_t max_battery_steps;
    int house_rows;
    int house_cols;
    int docking_x;
    int docking_y;
    int total_dirt =0;
    std::string house_name;
    std::string alg_name;
    std::vector<char> steps_performed;
    std::string house_description;
    std::vector<std::vector<int>> house_map;
    std::unique_ptr<WallsSensor> wall_sensor;
    std::unique_ptr<DirtSensor> dirt_sensor;
    std::unique_ptr<BatteryMeter> battery_meter;
    bool create_file = true;
    int score;
    Direction getDirection(Step s);


    public:
    MySimulator();
    int readHouseFile(const std::string &filename);
    void setAlgorithm(AbstractAlgorithm& algo);
    void run();
    void chargeBattery();

    bool readParameters(std::ifstream& file);
    bool readHouseMap(std::ifstream& file);
    int readPLine(std::ifstream& file, const std::string param);
    int openFileForWrite(std::ofstream& file, const std::string& house_name,const std::string& alg_name);

    int const getScore();
    void create_out_file(bool b);
    void calcScore(std::string status,int NumSteps);
    void setAlgName(const std::string& name);
    int const getMaxSteps ();
    int const getInitDirt();
    

    MySimulator(const MySimulator& s) = delete;
    MySimulator& operator=(const MySimulator& s) = delete;
};


#endif  // MYSIMULATOR_H