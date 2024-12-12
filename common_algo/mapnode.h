#ifndef MAPNODE_H_
#define MAPNODE_H_

#include <array>
#include <memory>

class MapNode{
    std::array<int, 2> key;
    int min_dist_from_docker;
    std::shared_ptr<MapNode> pi;
    bool visited = false;

    public:
        MapNode(int k[2], int dist);
        MapNode(const MapNode& node) = delete;
        MapNode& operator=(const MapNode& node) = delete;
        int getDist() const;
        int setDist(int dist);
        int setPi(std::shared_ptr<MapNode> node);
        std::shared_ptr<MapNode> getPi() const;
        std::array<int, 2> getCoord() const;
        bool operator==(const MapNode& other) const;
        bool isVisited() const;
        void setVisited();

};

#endif //MAPNODE_H_