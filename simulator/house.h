#ifndef HOUSE_H_
#define HOUSE_H_

#include <vector>

class House{
    std::vector<std::vector<int>> house;
    int map_rows;
    int map_cols;
    public:
        House()=default;
        House(std::vector<std::vector<int>> &&house_map, int rows, int cols);
        House(const House& h) = delete;
        House& operator=(const House& h) = delete;
        int getDirt(int x, int y) const;
        int getWall(int i, int j) const;
        int setDirt(int x, int y, int dirt);


};

#endif //HOUSE_H_