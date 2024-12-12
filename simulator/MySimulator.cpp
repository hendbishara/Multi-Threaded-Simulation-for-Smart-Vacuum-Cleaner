#include "MySimulator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include "mywallsensor.h"
#include "mydirtsensor.h"
#include "mybatterymeter.h"



MySimulator::MySimulator(): myalgo(){
}


/**
 * @brief reads house file, returns 1 if reading the file was unsuccessful 
 * @param std:string filename
 * @return int 
 */
int MySimulator::readHouseFile(const std::string &filename){
    //read file:
    //open file
    std::ifstream file;
    file.open(filename,std::ios::in);
    const std::string extension = ".house";
    size_t extlen = extension.length();
    house_name = filename.substr(0,filename.length()-extlen);
    if(!file.is_open()){
        //error handeling
        std::cout << "Error: Could not open the file " << filename << " for reading." << std::endl;
        return 1;
    }

    //reading house parameters
    if(!readParameters(file)){
        return 2;
    }
    

    //read house map
    if(!readHouseMap(file)){
        return 2;
    }


    //initialize robot
    robot = std::make_shared<RobotState>(std::move(house_map),max_steps,static_cast<size_t>(max_battery_steps),docking_x,docking_y,house_rows,house_cols);
    
    //initialize sensors
    wall_sensor = std::make_unique<MyWallSensor>(robot);
    dirt_sensor = std::make_unique<MyDirtSensor>(robot);
    battery_meter = std::make_unique<MyBatteryMeter>(robot);
    
    
    file.close();

    return 0;


}

/**
 * @brief set's the name of the algorithm that is currently running the simulator 
 * @param std:string name
 * @return void
 */
void MySimulator::setAlgName(const std::string& name){
    alg_name = name;
}

/**
 * @brief returns the score  
 * 
 * @return int
 */
int const MySimulator::getScore(){
    return score;
}

/**
 * @brief returns the maximum steps 
 * 
 * @return int
 */
int const MySimulator::getMaxSteps(){
    return max_steps;
}

/**
 * @brief returns the initial dirt in the house
 * 
 * @return int
 */
int const MySimulator::getInitDirt(){
    return total_dirt;
}


/**
 * @brief set's the bool create file indicating if to craete an output file in this run 
 * @param bool
 * @return void
 */
void MySimulator::create_out_file(bool b){
    create_file = b;
}


/**
 * @brief returns the direction the step is going in
 * @param Step
 * @return Direction
 */
Direction MySimulator::getDirection(Step s){
    if(s == Step::East){
        return Direction::East;
    }
    else if (s==Step::North){
        return Direction::North;
    }

    else if (s==Step::South){
        return Direction::South;
    }

    else{
        return Direction::West;
    }
}

/**
 * @brief reads parametes of the house, returns false if an error accured
 * @param std::ifstream file
 * @return bool 
 */
bool MySimulator::readParameters(std::ifstream& file){
    std::string line;
    // Line 1: house name / description
    std::getline(file, line);
    house_description = line;

    // Line 2: MaxSteps for the simulation
    max_steps = readPLine(file,"MaxSteps=");
    if(max_steps == -1){return false;}

    // Line 3: MaxBattery for the simulation
    int battery = readPLine(file,"MaxBattery=");
    max_battery_steps = battery;
    if(battery == -1){return false;}

    // Line 4: Number of Rows in house
    house_rows = readPLine(file, "Rows=");
    if(house_rows == -1){return false;}
    
    // Line 5: Number of Cols in house
    house_cols = readPLine(file, "Cols=");
    if(house_cols == -1){return false;}

    return true;
}

/**
 * @brief reads the line containing the required parameter, returns the value of the requested parameter
 * @param std::ifstream file
 * @param std::string param
 * @return int
 */
int MySimulator::readPLine(std::ifstream& file, const std::string param){
    std::string line;
    std::getline(file, line);
    //erae spaces from line
    line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
    //check for param
    if (line.find(param) != std::string::npos) {
       try {
                int p = std::stoi(line.substr(line.find("=") + 1));
                return p;
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Invalid argument: " << ia.what() << std::endl;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Out of range error: " << oor.what() << std::endl;
            } catch (...) {
                std::cerr << "An unexpected error occurred during conversion." << std::endl;
            }
       
    } else {
        //return -1 on failure
        std::cerr << "Invalid format for " << param.substr(0, param.size() - 1) << std::endl;
        return -1;
    }

    return -1;
    
}

/**
 * @brief reads the house map, returns false if an error occured
 * @param std::ifstream file
 * @return bool
 */

bool MySimulator::readHouseMap(std::ifstream& file){
    std::string line;
    int current_row = 0;
    int current_col = 0;
    int docking_counter = 0;
    try{
        //read lines untill end of file
        while(std::getline(file, line)){
            if (current_row >= house_rows) {
                break;  // Already read required number of rows
            }
            // Initialize row with 0
            house_map.emplace_back(std::vector<int>(house_cols, 0));  
            if(line.empty()){current_row++ ;continue;}
            
            // Process each character in the line
            for (char ch : line){
                if (current_col >= house_cols) {
                    break;  // Ignore additional columns beyond house_cols
                }

                //if wall insert 11
                if (ch == 'W'){ 
                    house_map[current_row][current_col] = 11;
                }
                // if docking station, insert zero dirt and update docking station coordinates
                else if(ch == 'D'){
                    house_map[current_row][current_col] = 0;
                    docking_counter++;
                    //Save docking station coordinates
                    docking_x = current_row;
                    docking_y = current_col;
                }

                //if dirt, insert dirt amount
                else if((ch>='0') && (ch<='9')){
                    house_map[current_row][current_col] = ch - '0';
                    total_dirt += (house_map)[current_row][current_col];
                }
                //space found - fill with zero
                else if(isspace(ch) || ch == '\n'){
                    house_map[current_row][current_col] = 0;
                }
                //in valid character
                else{
                    std::cout << house_name << " :ERROR: invalid input!" << std::endl;
                    return false;
                }

                current_col++;

            }

            current_row++;
            current_col = 0;

        }

        // Check if the last read operation failed due to an error other than EOF
        if (file.bad()) {
            throw std::runtime_error("Error: Failed to read the file.");
        }
    } catch(const std::exception& e){
        std::cerr << house_name << e.what() << std::endl;
        return false;
    }


    // Fill remaining rows with zeros if fewer rows were provided in the file
    while (current_row < house_rows) {
        house_map.emplace_back(std::vector<int>(house_cols, 0));
        current_row++;
    }

    // Check if exactly one docking station was found
    if (docking_counter!=1) {
        std::cerr << house_name << ": Error: No docking station found or more than one found in the house map." << std::endl;
        return false;
    }


    return true;
}

/**
 * @brief sets sensors off the algorithm
 * @param AbstractAlgorithm algo
 * @return void 
 */
void MySimulator::setAlgorithm(AbstractAlgorithm& algo){
    
    myalgo = &algo; 
    myalgo->setMaxSteps(max_steps);
    myalgo->setWallsSensor(std::move(*wall_sensor));
    myalgo->setDirtSensor(std::move(*dirt_sensor));
    myalgo->setBatteryMeter(std::move(*battery_meter));

}

/**
 * @brief properly opens the output file with the required name, returns 1 if an error occures
 * @param std::ofstream file
 * @param std::string file_name
 * @return int
 */
int MySimulator::openFileForWrite(std::ofstream& file, const std::string& house_name,const std::string& alg_name) {
    std::string output_file_name = house_name + "-" + alg_name + ".txt";
    file.open(output_file_name, std::ios::out);
    
    if (!file.is_open()) {
        // Error handling
        std::cout << "Error: Could not open the file " << output_file_name << " for writing." << std::endl;
        return 1;
    }

    return 0;
}

/**
 * @brief this function charges the battery based on the formula.
 * 
 */
void MySimulator::chargeBattery(){
    if (robot->getBatteryLeft()<max_battery_steps){
        if(max_battery_steps <20){
            robot->setBattery(robot->getBatteryLeft() + 1);
        }
        else{robot->setBattery(robot->getBatteryLeft() + (max_battery_steps/20.0));}
    }
    if(robot->getBatteryLeft() > max_battery_steps)
    {
        robot->setBattery(max_battery_steps);
    }
}

/**
 * @brief calculates the score of the ru
 * @param std:string status, int number of steps
 * @return void
 */
void MySimulator::calcScore(std::string status,int NumSteps){
    if(status == "DEAD"){
        score = max_steps+(total_dirt*300) + 2000;
    }

    if(status == "FINISHED" && !((robot->getXcoord() == docking_x) && (robot->getYcoord() == docking_y))){
        score = max_steps + (total_dirt*300) + 3000;
    }

    else{
        if((robot->getXcoord() == docking_x) && (robot->getYcoord() == docking_y)){
            score = NumSteps + total_dirt*300;
        }
        else{
            score = NumSteps + total_dirt*300+1000;
        }
        
    }


}





/**
 * @brief runs the robot and cleans the house, as it gets each step from the algorithm 
 * @param const WallSensor& ws
 * @return void 
 */

void MySimulator::run(){
    Step next_step;
    bool hit_a_wall = false;
    int cnt = 0; //steps counter
    std::ofstream out_file;
    if(!create_file){
        openFileForWrite(out_file,house_name,alg_name);
    }
    

    while ((robot->getTotalSteps()>0) && !((total_dirt==0) && (docking_x==robot->getXcoord()) &&  (docking_y==robot->getYcoord())))
    {

        if(myalgo == nullptr){
            std::cout << alg_name << ": ERROR: Algorithm is not initialized properly" << std::endl;
            break;
        }
        next_step = myalgo->nextStep();

        if(next_step == Step::Finish){break;}

        //check if algorithm returns a step that goes into a wall
        if(next_step != Step::Stay){
            Direction d = getDirection(next_step);
            if(robot->wallSensor(d) == 1){
                std::cout << alg_name << " :The algoriyhm's next move is illegal, Robot hit a wall!" << std::endl;
                hit_a_wall = true; 
                break;
            }
        }

        
        if(next_step==Step::West){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            robot->reduceBattery();
            robot->setCurrCoord(robot->getXcoord(),robot->getYcoord()-1);
            steps_performed.push_back('W');
        }
        else if(next_step==Step::East){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            robot->reduceBattery();
            robot->setCurrCoord(robot->getXcoord(),robot->getYcoord()+1);
            steps_performed.push_back('E');
        }
        else if(next_step==Step::North){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            robot->reduceBattery();
            robot->setCurrCoord(robot->getXcoord()-1,robot->getYcoord());
            steps_performed.push_back('N');
        }
        else if(next_step==Step::South){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            robot->reduceBattery();
            robot->setCurrCoord(robot->getXcoord()+1,robot->getYcoord());
            steps_performed.push_back('S');
        }
        //clean
        else if(next_step==Step::Stay && (robot->getXcoord()!=docking_x || robot->getYcoord()!=docking_y) ){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            robot->reduceBattery();
            total_dirt-=1;
            robot -> cleanDirtInPoint();
            steps_performed.push_back('s');
        }
        //charge
        else if(next_step == Step::Stay && (robot->getXcoord()==docking_x && robot->getYcoord()==docking_y)){
            robot->setTotalSteps(robot->getTotalSteps() -1);
            chargeBattery();
            steps_performed.push_back('s');

        }
        else{
            break;
        }

        cnt++;
    }
    
    std::string status;
    bool indock = ((robot->getXcoord() == docking_x) && (robot->getYcoord() == docking_y));

    //check status:
    if(indock) {
        status = "FINISHED";
    }

    else if((robot->getBatteryLeft() == 0) || hit_a_wall){
        status = "DEAD";
    }

    else {
        status = "WORKING";
    }

    //calculate score
    calcScore(status,cnt);

    if(!create_file){
        //print to output file
        out_file << "NumSteps = "<< cnt << std::endl;
        out_file << "DirtLeft = "<< total_dirt << std::endl;
        out_file << "Status = "<< status << std::endl;
        out_file << "InDock = " << (indock? "True":"False") << std::endl;
        out_file << "Score = " << score << std::endl;
        out_file << "Steps: "<< std::endl;
        for (char ch : steps_performed) {
            out_file << ch;
        }

        //add F if finished!!
        if (status ==  "FINISHED"){
            out_file << 'F' ;
        }

    }


}

