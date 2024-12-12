#ifndef VACCUMMAP_H_
#define VACCUMMAP_H_

#include "mapnode.h"
#include <unordered_map>
#include <vector>
#include <list>

namespace std{
    template <>
    struct hash<std::vector<int>> {
        size_t operator()(const std::vector<int>& vec) const {
            size_t hash_value = 0;
            for(int elem:vec){
                hash_value ^= std::hash<int>()(elem) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
            }
            return hash_value;
        }
    };
}

class VaccumMap{
    std::unordered_map<std::vector<int>,std::shared_ptr<MapNode>> existing_nodes;

    public:
    VaccumMap();
    VaccumMap(const VaccumMap& vacum) = delete;
    VaccumMap& operator=(const VaccumMap& vacum) = delete;
    int insertNode(int x, int y, int neighbor_x, int neighbor_y, int dist);
    int getMinDist(int x ,int y) const;
    std::array<int, 2> getPiCoord(int x,int y) const;
    bool isVisitedCoord(int x, int y) const;
    void setVisited(int x, int y);
    


};

#endif //VACCUMMAP_H_