#include "../include/MovableBlock.h"

MovableBlock::MovableBlock()
{
    //ctor
}

MovableBlock::MovableBlock(pair<int,int> matrixPosition, pair<float,float> screenPosition)
{
    this->matrixPosition = matrixPosition;
    this->screenPosition = screenPosition;
}

MovableBlock::~MovableBlock()
{
    //dtor
}

void MovableBlock::move(OBJ_ENUM* sceneMatrix)
{
    this->move(sceneMatrix, this->speedX, this->speedZ);
}

void MovableBlock::move(OBJ_ENUM *sceneMatrix, int speedX , int speedZ)
{
    this->moving = true;
    this->speedX = speedX;
    this->speedZ = speedZ;
    int myX = this->matrixPosition.first;
    int myZ = this->matrixPosition.second;
    int nextX = myX + speedX;
    int nextZ = myZ + speedZ;

    if (nextX < 0 || nextX >= 24 || nextZ < 0 || nextZ >= 24 || (sceneMatrix[nextX*24 + nextZ] != NOTHING && sceneMatrix[nextX*24+nextZ] != ENEMY))
    {
        if (breakWhenStop){
            // CAN'T MOVE, SHOULD BREAK!
            this->isValid = false;

        }
        this->moving = false;
        this->screenPosition = make_pair((float)this->matrixPosition.first - 12.0 + 0.5, (float)this->matrixPosition.second - 12.0 + 0.5);
        return;
    }
    if (sceneMatrix[nextX*24+nextZ] == ENEMY)
    {
        this->moving = false;
        this->isValid = false;
        return;
    }
    this->screenPosition = make_pair(this->screenPosition.first + speedX * 0.05, this->screenPosition.second + speedZ*0.05);
    this->matrixPosition = make_pair((int)std::round(this->screenPosition.first - 0.5) + 12,
                                     (int)std::round(this->screenPosition.second- 0.5) + 12);
    this->breakWhenStop = false;
}

bool MovableBlock::is_moving()
{
    return this->moving;
}

pair<int,int> MovableBlock::get_matrix_pos()
{
    return this->matrixPosition;
}

pair<float,float> MovableBlock::get_screen_pos()
{
    return this->screenPosition;
}

bool MovableBlock::is_valid()
{
    return this->isValid;
}

void MovableBlock::start_moving(int speedX, int speedZ)
{
    this->breakWhenStop = true;
    this->isValid = true;
    this->moving = true;
    this->speedX = speedX;
    this->speedZ = speedZ;
}

void MovableBlock::insert_item(ITEMS which)
{
    this->item = which;
}
