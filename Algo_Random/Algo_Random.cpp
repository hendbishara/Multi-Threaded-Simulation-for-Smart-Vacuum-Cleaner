
#include "Algo_209120054_207759085_Random.h"
#include "../common/enums.h"
#include "iostream" 

std::pair<int,int> Algo_209120054_207759085_Random::nextCoord(int x, int y, Direction d){
    std::pair<int,int> coords;
    if(d == Direction::West){
        coords.first = x;
        coords.second = y-1;
    }

    else if (d == Direction::East){
        coords.first = x;
        coords.second = y+1;
    }

    else if (d == Direction::North){
        coords.first = x+1;
        coords.second = y;
    }

    else{
        coords.first = x-1;
        coords.second = y;
    }

    return coords;
}

Step Algo_209120054_207759085_Random::nextStep(){
    
    //check proper initialization of sensors:
    if(!check_sensors_init()) {return Step::Finish;}

    if(charging)
    {   
        //if we are charging and the battery is not full yet we continue to charge
        if(battery_meter->getBatteryState() <max_battery_steps)
        {
            total_steps-=1;
            return Step::Stay;
        }
        else{
            charging=false;
            returntotop = true;
        }
    }



    std::size_t min_dist = vmap->getMinDist(x_coord,y_coord);




    //return to top of stack
    if(returntotop && (battery_meter->getBatteryState()>min_dist+1) && (total_steps>min_dist)){

        if(!(return_path.empty())){
            total_steps-=1;
            Step s = return_path.top();
            //upadte coordinates:
            if(s == Step::East){
                y_coord +=1;
            }
            else if(s==Step::West){y_coord-=1;}
            else if(s==Step::North){x_coord+=1;}
            else{x_coord-=1;}
            
            return_path.pop();
            return s;


        }

        else{
            returntotop = false;
        }
    }
    

    // checking if we are in the docking and one total step is left
    if(total_steps == 1 && x_coord==0 && y_coord == 0){
        return Step::Finish;
    }


    if((min_dist*2 + 2 == max_battery_steps) && (dirt_sensor->dirtLevel()==0)){
        return pop_stack();
    }



     //checking if we can continue cleaning or we should go back to the docking station
    if(((battery_meter->getBatteryState()>min_dist+1) ||(((battery_meter->getBatteryState()>min_dist)) && (dirt_sensor->dirtLevel()>0))) && (total_steps>0) && (total_steps>min_dist))
    {
        
        // staying on the same point
        if(dirt_sensor->dirtLevel()>0)
        {
            total_steps-=1;
            return Step::Stay;
        }


        //choose the next direction to move in
        else{

            int non_wall_counter = 0;
            int dist = min_dist + 1;
            //add all non wall point to the vaccum map
            if(!(walls_sensor->isWall(Direction::West))){if((vmap->insertNode(x_coord,y_coord-1,x_coord,y_coord,dist))==-1){return Step::Finish;}; non_wall_counter++;} //left point 
            if(!(walls_sensor->isWall(Direction::East))){if((vmap->insertNode(x_coord,y_coord+1,x_coord,y_coord,dist))==-1){return Step::Finish;};non_wall_counter++;} //right point
            if(!(walls_sensor->isWall(Direction::North))){if((vmap->insertNode(x_coord+1,y_coord,x_coord,y_coord,dist))==-1){return Step::Finish;};non_wall_counter++;} //up point
            if(!(walls_sensor->isWall(Direction::South))){if((vmap->insertNode(x_coord-1,y_coord,x_coord,y_coord,dist))==-1){return Step::Finish;};non_wall_counter++;} //down point
           

            if(non_wall_counter ==0){
                return Step::Finish;
            }
            
            std::vector<std::pair<Step,Direction>> possible_dir = {{Step::North,Direction::North}, {Step::East,Direction::East}, {Step::South,Direction::South}, {Step::West,Direction::West}};

            while(possible_dir.size() !=0){
                //randomly select an index from possible_dit
                int random_idx = std::rand() % possible_dir.size();
                std::pair<Step,Direction>& p = possible_dir[random_idx];
                std::pair<int,int> next_coords = nextCoord(x_coord,y_coord,p.second);
                
                //choose this step if it is not a wall and not already visited
                if(!(walls_sensor->isWall(p.second)) && !(vmap->isVisitedCoord(next_coords.first,next_coords.second))){
                    dfs.push(std::make_pair(next_coords.first,next_coords.second));
                    vmap->setVisited(next_coords.first,next_coords.second);
                    x_coord = next_coords.first;
                    y_coord = next_coords.second;
                    total_steps-=1;
                    return p.first;
                }

                //if can't choose this step, delete it from possible_dir
                possible_dir.erase(possible_dir.begin() + random_idx);
            }


            //if no step is possible, pop the top of the stack
            return pop_stack();

        }

    }
    //go back to docking station
    else{
        // if we are in the docking start charging
        if(x_coord == 0 && y_coord == 0){
            charging=true;
            total_steps-=1;
            return Step::Stay;
        }
        // else returns to the docking station based on the shortest route ,that we get from the pi of each node
        else{
            Step direction;
            const std::array<int, 2> parent = vmap->getPiCoord(x_coord,y_coord);
            if(parent[0]-x_coord == -1){direction=Step::South; return_path.push(Step::North);}
            else if (parent[0]-x_coord == 1){direction = Step::North; return_path.push(Step::South);}
            else if(parent[1]-y_coord == 1){direction=Step::East; return_path.push(Step::West);}
            else{direction=Step::West; return_path.push(Step::East);}
            x_coord = parent[0];
            y_coord = parent[1];
            total_steps-=1;
            return direction;

        }
    }

    return Step::Finish;

}

extern "C"
{
REGISTER_ALGORITHM(Algo_209120054_207759085_Random);
}

