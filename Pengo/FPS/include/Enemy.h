#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include <vector>
#include "../include/ObjEnum.h"
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;
class Enemy
{
    public:
        Enemy();
        Enemy(pair<int,int> matrixPosition, pair<float, float> screenPosition);
        Enemy(int x, int y, float sX, float sY);
        virtual ~Enemy();
        void move_me(vector<vector<OBJ_ENUM>>& matrix);
    protected:
        pair<float,float> screenPosition;
        pair<int,int> matrixPosition;
    private:
};

#endif // ENEMY_H
