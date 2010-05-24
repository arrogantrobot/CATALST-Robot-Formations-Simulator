//
// Filename:        "Robot.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a 2-dimensional robot.
//

// preprocessor directives
#ifndef ROBOT_H
#define ROBOT_H
#include <queue>
#include "Behavior.h"
#include "Circle.h"
#include "Packet.h"
using namespace std;



//
#define SENSOR_RANGE (2.0f)



// global constants
static const Color    DEFAULT_ROBOT_COLOR        = DEFAULT_CIRCLE_COLOR;
static const GLfloat  DEFAULT_ROBOT_RADIUS       = 0.03f;
static const Behavior DEFAULT_ROBOT_BEHAVIOR     = Behavior();
static const bool     DEFAULT_ROBOT_SHOW_HEADING = true;
static const bool     DEFAULT_ROBOT_SHOW_LINE    = false;
static const bool     DEFAULT_ROBOT_SHOW_HEAD    = true;
static const bool     DEFAULT_ROBOT_SHOW_FILLED  = false;
static const GLfloat  FACTOR_MAX_SPEED           = 0.3f;
static const GLfloat  FACTOR_THRESHOLD           = 1.0f;
static const GLfloat  FACTOR_COLLISION_RADIUS    = 3.0f;



// forward declaration of a robot cell environment
class Environment;



// describes a 2-dimensional robot
class Robot: public Circle
{

    public:

        // <public data members>
        Vector   heading;          // 3D vector heading of robot
        Behavior behavior;         // behavior of robot
        bool     showHeading;      // shows the vector heading of the robot
        queue<Packet> msgQueue;    // message packet queue for communication

        // <constructors>
        Robot(const GLfloat dx         = 0.0f,
              const GLfloat dy         = 0.0f,
              const GLfloat dz         = 0.0f,
              const GLfloat theta      = 0.0f,
              const Color   colorIndex = DEFAULT_ROBOT_COLOR);
        Robot(const Robot &r);

        // <destructors>
        virtual ~Robot();

        // <virtual public mutator functions>
        virtual bool setRadius(const GLfloat r = DEFAULT_ROBOT_RADIUS);
        virtual bool setHeading(const GLfloat theta);
        virtual bool setEnvironment(Environment *e);
        virtual void translateRelative(Vector v);
        virtual void translateRelative(const GLfloat dx = 0.0f,
                                       const GLfloat dy = 0.0f);
        virtual void rotateRelative(GLfloat theta);

        // <virtual public accessor functions>
        virtual Environment* getEnvironment() const;

        // <public accessor functions>
        GLint   getID()        const;
        GLfloat getHeading()   const;
        GLfloat getTransVel()  const;
        GLfloat getRotVel()    const;
        GLfloat getAngVel()    const;
        GLfloat getVelocity()  const;
        GLfloat getArcRadius() const;

        // <virtual public utility functions>
        virtual void draw();
        virtual void step();
        Robot*  auctioningStep();

        // <public utility functions>
        Vector  getRelationship(Vector &target) const;
        GLfloat getDistanceTo(Vector &target)   const;
        GLfloat getAngleTo(Vector &target)      const;
        GLfloat maxSpeed()                      const;
        GLfloat maxAngSpeed()                   const;
        GLfloat threshold()                     const;
        GLfloat angThreshold()                  const;
        GLfloat collisionRadius()               const;

        // <public environment functions>
        Vector  getRelationship(const GLint toID) const;
        GLfloat getDistanceTo(const GLint toID)   const;
        GLfloat getAngleTo(const GLint toID)      const;

        // <virtual public environment functions>
        virtual bool sendMsg(const Message &msg  = NULL,
                             const GLint    toID = ID_BROADCAST,
                             const GLint    type = 0);
        virtual bool sendPacket(const Packet &p = Packet());

        // <public primitive behaviors>
        Behavior moveArc(const Vector &target);
        Behavior moveArcBehavior(const Vector &target);
        Behavior moveArc(const GLfloat t = 0.0f,
                         const GLfloat r = 0.0f,
                         const Status  s = ACTIVE);
        Behavior moveArcBehavior(const GLfloat t = 0.0f,
                                 const GLfloat r = 0.0f,
                                 const Status  s = ACTIVE);
        Behavior moveForward(const GLfloat speed);
        Behavior moveForwardBehavior(const GLfloat speed);
        Behavior moveBackward(const GLfloat speed);
        Behavior moveBackwardBehavior(const GLfloat speed);
        Behavior moveStop();
        Behavior moveStopBehavior();

        // <public pair behaviors>
        Behavior orientTo(const Vector &target, const GLfloat theta = 0.0f);
        Behavior orientToBehavior(const Vector &target,
                                  const GLfloat theta = 0.0f);
        Behavior follow(const Vector &target, const GLfloat dist);
        Behavior followBehavior(const Vector &target, const GLfloat dist);
        Behavior avoid(const Vector &target, const GLfloat dist);
        Behavior avoidBehavior(const Vector &target, const GLfloat dist);
        Behavior orientForOrbit(const Vector &target, const GLfloat dist);
        Behavior orientForOrbitBehavior(const Vector &target,
                                        const GLfloat dist);
        Behavior orbit(const Vector &target, const GLfloat dist);
        Behavior orbitBehavior(const Vector &target, const GLfloat dist);
        bool     processPacket(Packet &p);
        bool     processPackets();
        GLint   getNBids() const;
        int     getAuctionStepCount() const;
        float   getDistanceTraveled() const;
        void    settleAuction();
        bool    setAuctionStepCount(const int& asc);
        void updateDistanceTraveled();
        bool isNumber(const GLfloat& n);
        bool isInfNum(const GLfloat& n);




    protected:

        // <protected data members>
        GLint         ID;     // identification number of robot
        Environment  *env;    // the environment of the robot
        vector<Bid *> bids;
        GLint         numBids;
        GLfloat       prevX, prevY;
        int           auctionStepCount;
        GLfloat       distanceTraveled;

        // <protected static data members>
        static GLint  nRobots;    // number of total robots

        // <protected utility functions>
        GLfloat rangeSensor(Packet &p);
        GLfloat bearingSensor(GLint &cellID);

        // <virtual protected utility functions>
        virtual bool init(const GLfloat dx         = 0.0f,
                          const GLfloat dy         = 0.0f,
                          const GLfloat dz         = 0.0f,
                          const GLfloat theta      = 0.0f,
                          const Color   colorIndex = DEFAULT_ROBOT_COLOR);
};  // Robot

#endif
