#include "../include/Enemy.h"
#include "../include/collision.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
    //dtor
}

Enemy::Enemy(int x, int y, float sX, float sY)
{
    this->matrixPosition = make_pair(x,y);
    this->screenPosition = make_pair(sX,sY);
}

Enemy::Enemy(pair<int, int> matrixPosition, pair<float,float> screenPosition)
{
    this->matrixPosition = matrixPosition;
    this->screenPosition = screenPosition;
}

void Enemy::move_me(OBJ_ENUM *matrix, int height, int width)
{
    if (this->moving) { this->keep_moving(matrix); return; };
    vector<pair<int,int>> neighbours;


    const int myRow = matrixPosition.first;
    const int myCol = matrixPosition.second;
    pair<int,int> move_to;




    for (int i = -1; i <= 1; ++i)
    {
        for (int j = ((i != 0) ? 0 : -1) ; j <= ((i != 0) ? 0 : 1); ++j)
        {

            int k = myRow + i;
            if (k < 0 || k >= height) continue;
            int l = myCol + j;
            if (l < 0 || l >= width) continue;
            if (k == myRow && l == myCol)
                continue;

            if (matrix[k*24+l] == PENGO)
            {
                extern bool pengoDead;
                move_to = make_pair(k,l);
                pengoDead = true;
                this->targetPosition = make_pair((float) move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
                this->moving = true;
                return;
            }
            if (matrix[k*24+l] == NOTHING)
                neighbours.push_back(make_pair(k,l));
        }
    }
    if (neighbours.empty()) return;
    std::shuffle(neighbours.begin(), neighbours.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    move_to = neighbours[0];
    this->targetPosition = make_pair((float)move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
    this->moving = true;
    /*
    matrix[this->matrixPosition.first*24 + this->matrixPosition.second] = NOTHING;
    if (this->valid){
        matrix[move_to.first*24 + move_to.second] = ENEMY;
        this->matrixPosition = move_to;
        this->targetPosition = make_pair((float)move_to.first - 12.0 + 0.5, (float)move_to.second - 12.0 + 0.5);
        this->moving = true;
    }
    */
}

bool Enemy::isValid() {
    return valid;
}



void Enemy::keep_moving(OBJ_ENUM *matrix)
{
    /*
    const int myRow = matrixPosition.first;
    const int myCol = matrixPosition.second;
                if ((matrix[myRow*24+myCol] == ICECUBE || matrix[(myRow+1)*24+myCol] == ICECUBE || matrix[(myRow-1)*24+myCol] == ICECUBE || matrix[myRow*24+myCol+1] == ICECUBE || matrix[myRow*24+myCol-1] == ICECUBE)){ //COLLISION
                this->moving = false;
                this->valid = false;
            }
    */
    if (std::fabs(this->screenPosition.first - this->targetPosition.first) <= 0.1)
    {
        this->screenPosition.first = this->targetPosition.first;
    }
    if (std::fabs(this->screenPosition.second - this->targetPosition.second) <= 0.1)
    {
        this->screenPosition.second = this->targetPosition.second;
    }
    if (this->screenPosition.first != this->targetPosition.first)
    {
        this->screenPosition.first += (this->screenPosition.first < this->targetPosition.first ? 0.05 : -0.05);
        return;
    }
    if (this->screenPosition.second != this->targetPosition.second)
    {
        this->screenPosition.second += (this->screenPosition.second < this->targetPosition.second ? 0.05 : -0.05);
    }

    if (this->screenPosition == this->targetPosition)
    {
        this->moving = false;
    }
    this->matrixPosition =
            make_pair((int)std::round(this->screenPosition.first - 0.5) + 12, (int)std::round(this->screenPosition.first - 0.5) + 12);
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = (i != 0 ? 0 : -1); j <= ( i != 0 ? 0 : 1); j += (i == 0 ? 2 : 1))
        {
            if (matrix[(this->matrixPosition.first+i)*24 + this->matrixPosition.second + j] == PENGO)
            {
                extern bool pengoDead;
                pengoDead = true;
                this->moving = false;
                return;
            }
        }
    }
    if (matrix[this->matrixPosition.first*24 + this->matrixPosition.second] != NOTHING)
    {
        this->moving = false;
    }
}

pair<int,int> Enemy::get_matrix_pos()
{
    return this->matrixPosition;
}

pair<float,float> Enemy::get_screen_pos()
{
    return this->screenPosition;
}

void Enemy::set_valid(bool b)
{
    this->valid = b;
}

bool Enemy::is_moving()
{
    return this->moving;
}

void Enemy::set_moving(bool b)
{
    this->moving = b;
}
