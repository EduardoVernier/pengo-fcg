#include "../include/Camera.h"

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

Camera::Camera(const Camera& other)
{
    other.get_eye(&this->eyeX, &this->eyeY, &this->eyeZ);
    other.get_center(&this->cntrX, &this->cntrY, &this->cntrZ);
    other.get_upvector(&this->upX, &this->upY, &this->upZ);
}

Camera::Camera(GLfloat _eyeX, GLfloat _eyeY, GLfloat _eyeZ, GLfloat _cntrX, GLfloat _cntrY, GLfloat _cntrZ, GLfloat _upX, GLfloat _upY, GLfloat _upZ)
    : eyeX(_eyeX), eyeY(_eyeY), eyeZ(_eyeZ), cntrX(_cntrX), cntrY(_cntrY), cntrZ(_cntrZ), upX(_upX), upY(_upY), upZ(_upZ)
{

}

void Camera::get_center(GLfloat *cntrX, GLfloat *cntrY, GLfloat *cntrZ) const
{
    if (!cntrX || !cntrY || !cntrZ) return;
    *cntrX = this->cntrX;
    *cntrY = this->cntrY;
    *cntrZ = this->cntrZ;
}

void Camera::get_eye(GLfloat *eyeX, GLfloat *eyeY, GLfloat *eyeZ) const
{
    if (!eyeX || !eyeY || !eyeZ) return;
    *eyeX = this->eyeX;
    *eyeY = this->eyeY;
    *eyeZ = this->eyeZ;
}

void Camera::get_upvector(GLfloat *upX, GLfloat *upY, GLfloat *upZ) const
{
    if (!upX || !upY || !upZ) return;
    *upX = this->upX;
    *upY = this->upY;
    *upZ = this->upZ;
}

void Camera::set_eye(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ)
{
    this->eyeX = eyeX;
    this->eyeY = eyeY;
    this->eyeZ = eyeZ;
}
void Camera::set_center(GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ)
{
    this->cntrX = cntrX;
    this->cntrY = cntrY;
    this->cntrZ = cntrZ;
}
void Camera::set_upvector(GLfloat upX, GLfloat upY, GLfloat upZ)
{
    this->upX = upX;
    this->upY = upY;
    this->upZ = upZ;
}
void Camera::callGluLookAt()
{
    gluLookAt(this->eyeX, this->eyeY, this->eyeZ,
              this->cntrX, this->cntrY, this->cntrZ,
              this->upX, this->upY, this->upZ);
}
