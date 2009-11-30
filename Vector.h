//
// Filename:        "Vector.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a 3-dimensional vector.
//

// preprocessor directives
#ifndef VECTOR_H
#define VECTOR_H
#include <cmath>
#include <iostream>
#include "Color.h"
#include "Utils.h"
using namespace std;



// global constants
static const Color   DEFAULT_VECTOR_COLOR        = WHITE;
static const GLfloat DEFAULT_VECTOR_TRANSLATE[3] = {0.0f, 0.0f, 0.0f};
static const GLfloat DEFAULT_VECTOR_ROTATE[3]    = {0.0f, 0.0f, 0.0f};
static const GLfloat DEFAULT_VECTOR_SCALE[3]     = {1.0f, 1.0f, 1.0f};
static const bool    DEFAULT_VECTOR_SHOW_LINE    = true;
static const bool    DEFAULT_VECTOR_SHOW_HEAD    = true;
static const GLfloat VECTOR_LINE_WIDTH           = 1.0f;
static const GLfloat VECTOR_HEAD_HEIGHT          = 0.015f;
static const GLfloat VECTOR_HEAD_WIDTH           = 0.5f * VECTOR_HEAD_HEIGHT;



// describes a 3-dimensional vector
class Vector
{
    public:

        // <public data members>
        GLfloat x,        y,            z;
        GLfloat color[3], translate[3], rotate[3], scale[3];
        bool    showLine, showHead;
        
        // <constructors>
        Vector(const GLfloat dx         = 0.0f,
               const GLfloat dy         = 0.0f,
               const GLfloat dz         = 0.0f,
               const Color   colorIndex = DEFAULT_VECTOR_COLOR);
        Vector(const Vector &v);

        // <destructors>
        virtual ~Vector();

        // <virtual public mutator functions>
        virtual bool set(const GLfloat dx = 0.0f,
                         const GLfloat dy = 0.0f,
                         const GLfloat dz = 0.0f);
        virtual bool set(const Vector &v);
        virtual bool setColor(const GLfloat r,
                              const GLfloat g,
                              const GLfloat b);
        virtual bool setColor(const GLfloat clr[3]);
        virtual bool setColor(const Color colorIndex = DEFAULT_VECTOR_COLOR);
        virtual void translated(const GLfloat dx,
                                const GLfloat dy,
                                const GLfloat dz);
        virtual void translated(const Vector &v);
        virtual void rotated(GLfloat theta);
        virtual void rotateRelative(GLfloat theta);
        virtual void scaled(GLfloat s);

        // <public mutator functions>
        bool setPolar(GLfloat magnitude = 1.0f,
                      GLfloat theta     = 0.0f,
                      GLfloat dz        = 0.0f);
        bool setDiff(const Vector &dest, const Vector &source = Vector());
        bool setAngle(const GLfloat theta = 0.0f);
        bool setMagnitude(const GLfloat mag = 1.0f);
        //bool setNorm(const GLfloat mag = 1.0f);
        bool setPerp();
        bool setAvg(const Vector v[], const GLint n = 1);
        bool normalize();

        // <virtual public utility functions>
        virtual void draw();

        // <public utility functions>
        GLfloat angle() const;
        GLfloat magnitude() const;
        //GLfloat norm()  const;
        Vector  perp();
        GLfloat perpDot(const Vector &v) const;

        // <virtual overloaded operators>
        virtual Vector& operator  =(const Vector &v);
        virtual Vector  operator  +(const Vector &v);
        virtual Vector  operator  -(const Vector &v);
        virtual Vector& operator +=(const Vector &v);
        virtual Vector& operator -=(const Vector &v);
        virtual Vector& operator *=(const GLfloat scalar);
        virtual bool    operator ==(const Vector &v);
        virtual bool    operator !=(const Vector &v);

        // <friend functions>
        friend ostream& operator << (ostream &out, const Vector &v);
        friend Vector   operator -(const Vector &v);
        friend Vector   operator *(const GLfloat scalar, const Vector &v);
        friend Vector   operator *(const Vector &v, const GLfloat scalar);
        friend Vector   unit(const Vector &v);
        friend Vector   crossProduct(const Vector &v1, const Vector &v2);
        friend GLfloat  dotProduct(const Vector &v1,
                                   const Vector &v2);
        friend GLfloat  angle(const Vector &v);
        friend GLfloat  angle(const Vector &v1, const Vector &v2);

    protected:

        // <virtual protected utility functions>
        virtual bool init(const GLfloat dx         = 0.0f,
                          const GLfloat dy         = 0.0f,
                          const GLfloat dz         = 0.0f,
                          const Color   colorIndex = DEFAULT_VECTOR_COLOR);
};  // Vector

#endif
