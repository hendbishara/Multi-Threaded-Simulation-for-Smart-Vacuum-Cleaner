#include "mapnode.h"
#include <iostream>
/**
 * @brief constructor for the MapNode
 * 
 * @param k the coords of the node baised of the vaccum map
 * @param dist the min dist of the node from the docking station
 * @return MapNode 
 */
MapNode :: MapNode(int k[2], int dist):key({k[0], k[1]}),min_dist_from_docker(dist){}

int MapNode::getDist() const{
    return min_dist_from_docker;
}
int MapNode::setDist(int dist){
    if(dist>=0){
        min_dist_from_docker = dist;
        return 1;
    }

    return 0;
    
}
/**
 * @brief sets parent for the node as the point to return if we want to return to the docking station
 * 
 * @param node 
 * @return int returns 1 if we succed else return 0
 */

int MapNode::setPi(std::shared_ptr<MapNode> node)
{
    if(node != nullptr)
    {
        pi=node;
        return 1;
    }
    return 0;
}
/**
 * @brief returns the parent of the Node
 * 
 * @return MapNode* 
 */
std::shared_ptr<MapNode> MapNode::getPi() const
{
    if (!pi) {
        std::cout << "Error: pi is null!" << std::endl;
    }
    return pi;
    
}

/**
 * @brief returns the coords of the node based on the vaccum map
 * 
 * @return std::array<int, 2> 
 */
std::array<int, 2> MapNode::getCoord() const{
    return key;
}

bool MapNode::operator==(const MapNode& other) const {
        
        bool is_equal=true;
        if(key[0] != other.getCoord()[0])
            is_equal=false;

        if(key[1] != other.getCoord()[1])
            is_equal=false;

        return is_equal;
    }


/**
 * @brief returns boolean indicating if node was visited during dfs
 * 
 * @return bool
 */
bool MapNode::isVisited() const{
    return visited;
}

/**
 * @brief sets node as visisted 
 * 
 * @return vpid
 */

void MapNode::setVisited(){
    visited = true;
}


