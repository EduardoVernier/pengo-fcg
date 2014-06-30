#include "../include/Camera.h"

Camera::Camera()
    : eye(0.0,0.0,0.0), center(0.0,0.0,0.0), up(0.0,1.0,0.0)
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

Camera::Camera(const Camera& other)
{
    this->eye = other.get_eye();
    this->center = other.get_center();
    this->up = other.get_upvector();
}

Camera::Camera(GLfloat _eyeX, GLfloat _eyeY, GLfloat _eyeZ, GLfloat _cntrX, GLfloat _cntrY, GLfloat _cntrZ, GLfloat _upX, GLfloat _upY, GLfloat _upZ)
    : eye(_eyeX, _eyeY, _eyeZ), center(_cntrX, _cntrY, _cntrZ), up(_upX, _upY, _upZ)
{

}

Point3D Camera::get_center() const
{
    return this->center;
}

Point3D Camera::get_eye() const
{
    return this->eye;
}

Point3D Camera::get_upvector() const
{
    return this->up;
}

void Camera::set_eye(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ)
{
    this->eye.set_coords(eyeX, eyeY, eyeZ);
}

void Camera::set_eye(Point3D eye)
{
    this->eye = eye;
}

void Camera::set_center(GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ)
{
    this->center.set_coords(cntrX, cntrY, cntrZ);
}

void Camera::set_center(Point3D center)
{
    this->center = center;
}

void Camera::set_upvector(GLfloat upX, GLfloat upY, GLfloat upZ)
{
    this->up.set_coords(upX, upY, upZ);
}

void Camera::set_upvector(Point3D upvector)
{
    this->up = upvector;
}

void Camera::callGluLookAt()
{
    gluLookAt(this->eye.getX(), this->eye.getY(), this->eye.getZ(),
              this->center.getX(), this->center.getY(), this->center.getZ(),
              this->up.getX(), this->up.getY(), this->up.getZ());
}
