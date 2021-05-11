#include "utils.h"

Point::Point(int x, int y)
{
    x_ = x;
    y_ = y;
}
Point::Point() : Point(-1, -1)
{
}

bool Point::operator==(const Point &p)
{
    return this->x_ == p.x_ && this->y_ == p.y_;
}

int Sign(int a)
{
    return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
}
