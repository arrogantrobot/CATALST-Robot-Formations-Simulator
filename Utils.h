//
// Filename:        "Utils.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This file contains various utility functions.
//

// preprocessor directives
#ifndef UTILS_H
#define UTILS_H
#include <cstdlib>
#include <iostream>
#include "GLIncludes.h"
using namespace std;



// debug definitions
//#define DEBUG (1)



// math pi definition
#ifndef PI
#define PI (M_PI)
#endif



// global constants
static const GLfloat TWO_PI      = 2.0f * PI;
static const GLfloat PI_OVER_180 = PI / 180.0f;



//
// GLfloat scaleDegrees(theta)
// Last modified: 26Aug2006
//
// Scales the parameterized angle (in degrees) to an angle [-180, 180].
//
// Returns:     the scaled angle (in degrees)
// Parameters:
//      theta   in      the angle (in degrees) to be scaled
//
inline GLfloat scaleDegrees(GLfloat theta)
{
    if         (theta >   0.0f)
        while ((theta >=  360.0f) || (theta >  180.0f)) theta -= 360.0f;
    else if    (theta <   0.0f)
        while ((theta <= -360.0f) || (theta < -180.0f)) theta += 360.0f;
    return theta;
}   // scaleDegrees(GLfloat)



//
// GLfloat scaleRadians(theta)
// Last modified: 26Aug2006
//
// Scales the parameterized angle (in radians) to an angle [-2 * PI, 2 * PI].
//
// Returns:     the scaled angle (in radians)
// Parameters:
//      theta   in      the angle (in radians) to be scaled
//
inline GLfloat scaleRadians(GLfloat theta)
{
    if         (theta >   0.0f)
        while ((theta >=  TWO_PI) || (theta >  PI)) theta -= TWO_PI;
    else if    (theta <   0.0f)
        while ((theta <= -TWO_PI) || (theta < -PI)) theta += TWO_PI;
    return theta;
}   // scaleRadians(GLfloat)



//
// GLfloat degreesToRadians(theta)
// Last modified: 26Aug2006
//
// Converts the parameterized angle (in degrees) to an angle in radians.
//
// Returns:     the converted angle (in radians)
// Parameters:
//      theta   in      the angle (in degrees) to converted to radians
//
inline GLfloat degreesToRadians(GLfloat theta)
{
    return scaleDegrees(theta) * PI_OVER_180;
}   // degreesToRadians(GLfloat)



//
// GLfloat radiansToDegrees(theta)
// Last modified: 26Aug2006
//
// Converts the parameterized angle (in radians) to an angle in degrees.
//
// Returns:     the converted angle (in degrees)
// Parameters:
//      theta   in      the angle (in radians) to converted to degrees
//
inline GLfloat radiansToDegrees(GLfloat theta)
{
    return scaleRadians(theta) / PI_OVER_180;
}   // radiansToDegrees(GLfloat)



//
// GLfloat frand(min, max)
// Last modified: 08Nov2009
//
// Returns a floating-point number [min, max].
//
// Returns:     a floating-point number [min, max]
// Parameters:
//      min     in      the minimum of the number being returned
//      max     in      the maximum of the number being returned
//
inline GLfloat frand(const GLfloat min = 0.0f, const GLfloat max = 1.0f)
{
    return min + (max - min) * GLfloat(rand()) / GLfloat(RAND_MAX);
}   // frand()



//
// GLint irand(min, max)
// Last modified: 08Nov2009
//
// Returns an integer number [min, max].
//
// Returns:     an integer number [min, max]
// Parameters:
//      min     in      the minimum of the number being returned
//      max     in      the maximum of the number being returned
//
inline GLint irand(const GLint min = 0, const GLint max = 1)
{
    return min + rand() % (max + 1);
}   // irand()



//
// GLfloat randSign()
// Last modified: 26Aug2006
//
// Returns -1 or 1.
//
// Returns:     -1 or 1
// Parameters:  <none>
//
inline GLfloat randSign()
{
    return (rand() % 2) ? -1.0f : 1.0f;
}   // randSign()



//
// GLfloat sign()
// Last modified: 26Aug2006
//
// Returns the sign of the parameterized number.
//
// Returns:     the sign of the parameterized number
// Parameters:
//      f       in      the number to determine the sign of
//
inline GLfloat sign(const GLfloat f)
{
    return (f < 0.0f) ? -1.0f : 1.0f;
}   // sign(const GLfloat)

#endif
