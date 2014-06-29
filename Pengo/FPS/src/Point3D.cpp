#include "../include/Point3D.h"
#include <cmath>
Point3D::Point3D()
    : x(0.0), y(0.0), z(0.0)
{
    this->values = new GLfloat[4];
    this->values[0] = this->values[1] = this->values[2] = 0.0;
    this->values[3] = 1.0;
}

Point3D::~Point3D()
{
    delete [] (this->values);
}

Point3D::Point3D(GLfloat x, GLfloat y, GLfloat z)
    : Point3D()
{
    this->setX(x);
    this->setY(y);
    this->setZ(z);
}


Point3D::Point3D(const Point3D& other)
    : Point3D()
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


int Point3D::getMatrixX() const
{
    return 24/2 + (int) floor(this->x); // 24 é o planesize
}

int Point3D::getMatrixZ() const
{
    return 24/2 + (int) floor(this->z); //24 é o planesize e estou com preguiça defazer isso direto
}


void Point3D::setX(GLfloat x)
{
    this->values[0] = this->x = x;
}

void Point3D::setY(GLfloat y)
{
    this->values[1] = this->y = y;
}

void Point3D::setZ(GLfloat z)
{
    this->values[2] = this->z = z;
}

void Point3D::set_coords(GLfloat x, GLfloat y, GLfloat z)
{
    this->setX(x); this->setY(y); this->setZ(z);
}

Point3D Point3D::operator-(const Point3D& other) const
{
    return Point3D(
           this->getX() - other.getX(),
           this->getY() - other.getY(),
           this->getZ() - other.getZ());
}

Point3D Point3D::operator+(const Point3D& other) const
{
    return Point3D(
            this->getX() + other.getX(),
            this->getY() + other.getY(),
            this->getZ() + other.getZ());
}

Point3D operator*(const double& left, const Point3D& right)
{
    return right * left;
}

Point3D Point3D::operator*(double scalar) const
{
    return Point3D(
       scalar*this->getX(),
       scalar*this->getY(),
       scalar*this->getZ()
    );
}

double Point3D::distance_to(const Point3D& other) const
{
    return std::sqrt(std::pow(other.getX() - this->getX(), 2) +
    std::pow(other.getY() - this->getY(), 2) +
    std::pow(other.getZ() - this->getZ(), 2));
}

Point3D::operator const GLfloat*() const
{
    return this->values;
}

const GLfloat* Point3D::as_opengl_vector() const
{
    return this->values;
}
