#ifndef POINT3D_H
#define POINT3D_H

#include <gl/gl.h>

class Point3D
{
    public:
        Point3D();
        virtual ~Point3D();
        Point3D(GLfloat x, GLfloat y, GLfloat z);
        Point3D(const Point3D& other);
        GLfloat getX() const;
        GLfloat getY() const;
        GLfloat getZ() const;
        void setX(GLfloat x);
        void setY(GLfloat y);
        void setZ(GLfloat z);
        void set_coords(GLfloat x, GLfloat y, GLfloat z);

        Point3D operator- (const Point3D& other);
        Point3D operator+ (const Point3D& other);

        double distance_to(const Point3D& other);
    protected:
        GLfloat x, y, z;
    private:
};

#endif // POINT3D_H
