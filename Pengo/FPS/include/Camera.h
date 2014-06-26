#ifndef CAMERA_H
#define CAMERA_H

#include <gl/glu.h>

#include <gl/glut.h>

class Camera
{
    public:
        Camera();
        virtual ~Camera();
        Camera(const Camera& other);
        Camera(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ, GLfloat upX, GLfloat upY, GLfloat upZ);
        void set_eye(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ);
        void set_center(GLfloat cntrX, GLfloat cntrY, GLfloat cntrZ);
        void set_upvector(GLfloat upX, GLfloat upY, GLfloat upZ);
        void callGluLookAt();
        void get_eye(GLfloat *eyeX, GLfloat *eyeY, GLfloat *eyeZ) const;
        void get_center(GLfloat *cntrX, GLfloat *cntrY, GLfloat *cntrZ) const ;
        void get_upvector(GLfloat *upX, GLfloat *upY, GLfloat *upZ) const;
    protected:
        GLfloat eyeX, eyeY, eyeZ, cntrX, cntrY, cntrZ, upX, upY, upZ;
    private:
};

#endif // CAMERA_H
