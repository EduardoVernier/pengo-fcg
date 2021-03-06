#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include <vector>
#include "../include/ObjEnum.h"
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;
class Enemy
{
    public:
        Enemy();
        Enemy(pair<int,int> matrixPosition, pair<float, float> screenPosition);
        Enemy(int x, int y, float sX, float sY);
        virtual ~Enemy();
        void move_me(OBJ_ENUM *, int, int);
        void keep_moving(OBJ_ENUM *matrix);
        pair<int,int> get_matrix_pos();
        pair<float,float> get_screen_pos();
        bool isValid();
        void set_valid(bool b);
        bool is_moving();
        void set_moving(bool b);


    protected:
        bool moving = false;
        bool valid = true;
        pair<float,float> targetPosition;
        pair<float,float> screenPosition;
        pair<int,int> matrixPosition;
    private:
};

#endif // ENEMY_H
