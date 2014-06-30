#include "../include/Enemy.h"

Enemy::Enemy()
{
    //ctor
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

void Enemy::move_me(vector<vector<OBJ_ENUM>>& matrix)
{
    vector<pair<int,int>> neighbours;

    const int height = matrix.size();
    const int width = matrix[0].size();
    const int myRow = matrixPosition.first;
    const int myCol = matrixPosition.second;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == myRow && j == myCol)
                continue;
            int k = myRow + i;
            if (k < 0 || k >= height) continue;
            int l = myCol + j;
            if (l < 0 || l >= width) continue;
            if (matrix[k][j] == ENEMY)
                matrix[k][j] = NOTHING;
            if (matrix[k][j] == PENGO)
            {
                // OMG MUST CATCH PENGO ASAP
            }
            else if (matrix[k][j] == NOTHING)
            {
                neighbours.push_back(make_pair(k,j));
            }
        }
    }
    default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, neighbours.size() - 1);
    pair<int,int> move_to = neighbours[dist(generator)];
}
