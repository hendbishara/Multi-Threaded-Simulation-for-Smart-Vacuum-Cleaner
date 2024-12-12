#include "house.h"


/**
 * @brief constructor for house class
 * 
 * @param std::vector<std::vector<int>> &&house_map
 * @param int rows
 * @param int cols
 * @return House
 */

House::House(std::vector<std::vector<int>> &&house_map, int rows, int cols): house(std::move(house_map)),map_rows(rows),map_cols(cols){}

/**
 * @brief retrieves dirt in point [x,y]
 * 
 * @param int x
 * @param int y
 * @return int 
 */

int House::getDirt(int x, int y) const{
    return house[x][y];
}


/**
 * @brief checks if point in house is wall, if wall returns 1 else returns 0
 * 
 * @param int i
 * @param int j
 * @return int
 */
int House::getWall(int i, int j) const{
    if(i>=0 && i<map_rows && j>=0 && j<map_cols)
    {
        if(house[i][j]<10){return 0;}
        else{
            return 1;}
    }
    //outside the house vector stucture is considered surrounded by walls
    return 1;
}

/**
 * @brief sets dirt in point [x,y], returns 1 on sucess, 0 on failure
 * 
 * @param int x
 * @param int y
 * @param int dirt
 * @return int
 */
int House::setDirt(int x, int y, int dirt){
    if(x>=0 && x<map_rows && y>=0 && y<map_cols){
        house[x][y] = dirt;
        return 1;
    }

    return 0;
}