#ifndef CAMERA_H
#define CAMERA_H

#include <gl/glu.h>

#include <gl/glut.h>
#include "../include/Point3D.h"
class Camera
{
    public:
        Camera();
        virtual ~Camera();
        Camera(const Camera& other);
        Camera(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ, GLfloat upX, GLfloat upY, GLfloat upZ);
        void set_eye(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ);
        void set_eye(Point3D eye);
        void set_center(GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ);
        void set_center(Point3D center);
        void set_upvector(GLfloat upX, GLfloat upY, GLfloat upZ);
        void set_upvector(Point3D upvector);
        void callGluLookAt();
        Point3D get_eye() const;
        Point3D get_center() const;
        Point3D get_upvector() const;
    protected:
        Point3D eye;
        Point3D center;
        Point3D up;
    private:
};

#endif // CAMERA_H
