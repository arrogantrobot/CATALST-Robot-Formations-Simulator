//
// Filename:        "Behavior.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a robot behavior.
//

// preprocessor directives#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include <cmath>
#include <iostream>
#include "GLIncludes.h"
using namespace std;



// enumerated status of a behavior
enum Status
{
    INACTIVE = 0,
    DONE,
    ACTIVE
};  // Status



// describes a robot behavior
class Behavior
{

    public:

        // <constructors>
        Behavior(const Status  s     = INACTIVE,
                 const GLfloat t     = 0.0f,
                 const GLfloat r     = 0.0f,
                 const GLfloat speed = 1.0f);
        Behavior(const Behavior &beh);

        // <public mutator functions>
        bool setStatus(const Status s);
        bool setTransVel(const GLfloat t);
        bool setRotVel(const GLfloat r);
        bool setDiffVel(const GLfloat right, const GLfloat left);
        bool setVelocity(const GLfloat t, const GLfloat r);
        bool setMaxSpeed(const GLfloat speed);

        // <public accessor functions>
        Status  getStatus()    const;
        bool    isActive()     const;
        bool    isDone()       const;
        bool    isInactive()   const;
        GLfloat getTransVel()  const;
        GLfloat getRotVel()    const;
        GLfloat getVelocity()  const;
        GLfloat getSpeed()     const;
        GLfloat getMaxSpeed()  const;

        // <overloaded operators>
        Behavior& operator =(const Behavior &beh);

        // <friend functions>
        friend Behavior subsumeBehaviors(const Behavior behLow,
                                         const Behavior behHigh);
        friend Behavior sumBehaviors(const Behavior beh1, const Behavior beh2);

    protected:

        // <protected data members>
        Status  status;
        GLfloat transVel, rotVel, maxSpeed;

        // <protected utility functions>
        GLfloat scaleVel();
};  // Behavior

#endif
