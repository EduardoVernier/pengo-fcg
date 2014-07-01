#ifndef MOVABLEBLOCK_H
#define MOVABLEBLOCK_H
#include <iostream>
#include <cmath>
#include "ObjEnum.h"
#include <map>
#include "Enemy.h"
using namespace std;

class MovableBlock
{
    public:
        MovableBlock();
        MovableBlock(pair<int,int> matrixPosition, pair<float,float> screenPosition);
        virtual ~MovableBlock();

        void move(OBJ_ENUM *sceneMatrix);
        void move(OBJ_ENUM *sceneMatrix, int speedX, int speedZ);

        void start_moving(int speedX, int speedZ);

        pair<int,int> get_matrix_pos();
        pair<float,float> get_screen_pos();

        void insert_item(ITEMS which);

        bool is_moving();
        bool is_valid();


        //friend void give_powerup(ITEMS which);
    protected:
        float speedX, speedZ;
        pair<int,int> matrixPosition;
        pair<float,float> screenPosition;
        bool moving = false;
        bool breakWhenStop = true;
        bool isValid = true;
        ITEMS item = NO_ITEM;
        bool hitFlag = false;
    private:
};

#endif // MOVABLEBLOCK_H
