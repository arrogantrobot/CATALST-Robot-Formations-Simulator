//
// Filename:        "Circle.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a 2-dimensional circle.
//

// preprocessor directives
#ifndef CIRCLE_H
#define CIRCLE_H
#include "Vector.h"
using namespace std;



// global constants
static const Color   DEFAULT_CIRCLE_COLOR       = DEFAULT_VECTOR_COLOR;
static const GLfloat DEFAULT_CIRCLE_RADIUS      = 1.0f;
static const bool    DEFAULT_CIRCLE_SHOW_POS    = false;
static const bool    DEFAULT_CIRCLE_SHOW_FILLED = false;
static const GLint   CIRCLE_N_LINKS             = 24;  // polygonal circle
static const GLfloat CIRCLE_THETA               = 360.0f / CIRCLE_N_LINKS;
static const GLfloat CIRCLE_COS_THETA           = cos(degreesToRadians(
                                                           CIRCLE_THETA));
static const GLfloat CIRCLE_SIN_THETA           = sin(degreesToRadians(
                                                           CIRCLE_THETA));
static const GLfloat CIRCLE_TAN_THETA           = tan(degreesToRadians(
                                                           CIRCLE_THETA));



// describes a 2-dimensional circle
class Circle: public Vector
{
	public:

        // <public data members>
        bool    showPos;       // shows the vector position of the circle
        bool    showFilled;    // shows the circle filled

        // <constructors>
        Circle(const GLfloat dx         = 0.0f,
               const GLfloat dy         = 0.0f,
               const GLfloat dz         = 0.0f,
               const GLfloat r          = DEFAULT_CIRCLE_RADIUS,
               const Color   colorIndex = DEFAULT_CIRCLE_COLOR);
        Circle(const Vector &c, const GLfloat r = DEFAULT_CIRCLE_RADIUS);
        Circle(const Circle &c);

        // <destructors>
        virtual ~Circle();

        // <public mutator functions>
        bool setRadius(const GLfloat r = DEFAULT_CIRCLE_RADIUS);
        bool setDiameter(const GLfloat d);
        bool setCircumference(const GLfloat c);
        bool setArea(const GLfloat a);

        // <public accessor functions>
        GLfloat getRadius()        const;
        GLfloat getDiameter()      const;
        GLfloat getCircumference() const;
        GLfloat getArea()          const;

        // <virtual public utility functions>
        virtual void draw();

        // <virtual overloaded operators>
        virtual Circle operator = (const Circle &c);

    protected:

        // <protected data members>
        GLfloat radius;     // radius of circle

        // <virtual protected utility functions>
        virtual bool init(const GLfloat dx         = 0.0f,
                          const GLfloat dy         = 0.0f,
                          const GLfloat dz         = 0.0f,
                          const GLfloat r          = DEFAULT_CIRCLE_RADIUS,
                          const Color   colorIndex = DEFAULT_CIRCLE_COLOR);
};  // Circle
#endif
