#include "VaccumMap.h"
#include <iostream>
#include <memory>

/**
 * @brief Construct a new Vaccum Map:: Vaccum Map object
 * 
 */

VaccumMap::VaccumMap(){
    int root_key[2]={0,0}; 
    std::shared_ptr<MapNode> root = std::make_shared<MapNode>(root_key,0);
    std::vector<int> r = {0,0};
    existing_nodes[r] = root;
    root->setVisited();
}


/**
 * @brief inserts a node to the vaccum map ,the function gets coords of the point that we want to insert and coords of the point that we were on before
 * and checks if we visited the point before and if we did we check if the minimal dist to the docking station is now less than the previous , if it is we 
 * update the min_dist and the pi (the parent of the node or in other words the node we came from ). if the node doesnt exist we create a new one and the min_dist
 * is the parents min_dist+1 and the parent is the node we came from .
 * 
 * @param x the x coord for the point that we want to insert
 * @param y the y coord for the point that we want to insert
 * @param neighbor_x the x coord for the point that we came from to this point (the parent coord)
 * @param neighbor_y the y coord for the point that we came from to this point (the parent coord)
 * @param dist the min_dist from the docking station
 * @return int returns 1 if succeed else returns -1
 */
int VaccumMap::insertNode(int x, int y, int neighbor_x, int neighbor_y, int dist){
    std::shared_ptr<MapNode> neighbor;
    int key[2] = {x,y};
    std::vector<int> v_key= {x,y};
    std::vector<int> n_key= {neighbor_x,neighbor_y};
    auto it_2 = existing_nodes.find(n_key);
    if (it_2 != existing_nodes.end()) {
            neighbor = it_2->second;
    } 
    else {
        return -1;
    }

    auto it_1 = existing_nodes.find(v_key);

    
    //if node does not exist
    if(it_1 == existing_nodes.end()){
        std::shared_ptr<MapNode> node = std::make_shared<MapNode>(key,dist);
        if((node->setPi(neighbor))==0){return -1;};
        existing_nodes[v_key]=node;
    }

    //node already exists in map
    else{
        std::shared_ptr<MapNode> node = it_1->second;
        if(dist<node->getDist()){
            if((node->setDist(dist)) == 0){return -1;} ;
            if((node->setPi(neighbor))==0){return -1;};
        }
    }
    return 1;

}

/**
 * @brief the function takes coords of a given point in the vaccum map and returns the min_dist from the docking station
 * 
 * @param x 
 * @param y 
 * @return int 
 */

int VaccumMap::getMinDist(int x, int y) const{

    std::vector<int> key= {x,y};
    std::shared_ptr<MapNode> point;
    auto it = existing_nodes.find(key);
    if (it != existing_nodes.end()) {
            point = it->second;
            return point->getDist();
    } 
    else {
        return -1;
    }

}

/**
 * @brief the function takes coords of a point and returns the parent of the node(to were we have to return so we take the minimal route)
 * 
 * @param x 
 * @param y 
 * @return std::array<int, 2> 
 */

std::array<int, 2> VaccumMap::getPiCoord(int x,int y) const{

    
    std::vector<int> key = {x,y};
    std::shared_ptr<MapNode> node = existing_nodes.at(key);
    std::shared_ptr<MapNode> parent = node->getPi();
    std::array<int, 2> coords = parent->getCoord();
    return coords;


}

/**
 * @brief function checks if node [x,y] was visited
 * 
 * @param x 
 * @param y 
 * @return bool
 */
bool VaccumMap::isVisitedCoord(int x, int y) const{
    std::vector<int> key= {x,y};
    std::shared_ptr<MapNode> point;
    auto it = existing_nodes.find(key);
    if (it != existing_nodes.end()) {
            point = it->second;
            return point->isVisited();
    } 
    
    return false;
}


/**
 * @brief sets the node [x,y] as visited
 * 
 * @return void
 */

void VaccumMap::setVisited(int x, int y){
    std::vector<int> key = {x,y};
    std::shared_ptr<MapNode> node = existing_nodes.at(key);
    node->setVisited();
}






