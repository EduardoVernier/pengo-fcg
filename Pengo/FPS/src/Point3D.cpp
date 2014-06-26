#include "../include/Point3D.h"

Point3D::Point3D()
    : x(0.0), y(0.0), z(0.0)
{
    //ctor
}

Point3D::~Point3D()
{
    //dtor
}

Point3D::Point3D(GLfloat x, GLfloat y, GLfloat z)
    : x(x), y(y), z(z)
{

}

Point3D::Point3D(const Point3D& other)
{
    this->setX(other.getX());
    this->setY(other.getY());
    this->setZ(other.getZ());
}

GLfloat Point3D::getX() const
{
    return this->x;
}
GLfloat Point3D::getY() const
{
    return this->y;
}
GLfloat Point3D::getZ() const
{
    return this->z;
}

void Point3D::setX(GLfloat x)
{
    this->x = x;
}

void Point3D::setY(GLfloat y)
{
    this->y = y;
}

void Point3D::setZ(GLfloat z)
{
    this->z = z;
}

void Point3D::set_coords(GLfloat x, GLfloat y, GLfloat z)
{
    this->setX(x); this->setY(y); this->setZ(z);
}
