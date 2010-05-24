//
// Filename:        "Formation.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a formation.
//

// preprocessor directives
#ifndef FORMATION_H
#define FORMATION_H
#include <vector>
#include "Relationship.h"
using namespace std;



// mathematical functional type redefinition
typedef GLfloat (*Function)(const GLfloat);



// global constants
static const Function DEFAULT_FORMATION_FUNCTION = NULL;
static const GLfloat  DEFAULT_FORMATION_RADIUS   = 1.0f;
static const GLdouble X_ROOT_THRESHOLD           = 5E-7;
static const GLint    X_N_ITERATIONS             = 100;



// describes a formation
class Formation: protected vector<Function>
{

    public:

        // <constructors>
        Formation(Function      f     = DEFAULT_FORMATION_FUNCTION,
                  const GLfloat r     = DEFAULT_FORMATION_RADIUS,
                  const Vector  sGrad = Vector(),
                  const GLint   sID   = ID_BROADCAST,
                  const GLint   fID   = -1,
                  const GLfloat theta = 0.0f);
        Formation(vector<Function> f,
                  const GLfloat    r     = DEFAULT_FORMATION_RADIUS,
                  const Vector     sGrad = Vector(),
                  const GLint      sID   = ID_BROADCAST,
                  const GLint      fID   = -1,
                  const GLfloat    theta = 0.0f);
        Formation(const Formation &f);

        // <public mutator functions>
        bool setFunction(const Function f = DEFAULT_FORMATION_FUNCTION);
        bool setFunctions(const vector<Function> &f);
        bool addFunction(const Function f = DEFAULT_FORMATION_FUNCTION);
        bool addFunctions(const vector<Function> &f);
        bool removeFunction(const GLint pos = 0);
        bool removeFunctions();
        bool setRadius(const GLfloat r = DEFAULT_FORMATION_RADIUS);
        bool setSeedGradient(const Vector sGrad = Vector());
        bool setSeedID(const GLint sID = ID_BROADCAST);
        bool setFormationID(const GLint fID = -1);
        bool setHeading(const GLfloat theta = 0.0f);

        // <public accessor functions>
        Function         getFunction(const GLint pos = 0) const;
        vector<Function> getFunctions()                   const;
        GLfloat          getRadius()                      const;
        Vector           getSeedGradient()                const;
        GLint            getSeedID()                      const;
        GLint            getFormationID()                 const;
        GLfloat          getHeading()                     const;

        // <public utility functions>
        vector<Vector> getRelationships(const Vector c = Vector());
        Vector getRelationship(const Function f = DEFAULT_FORMATION_FUNCTION,
                               const GLfloat  r = DEFAULT_FORMATION_RADIUS,
                               const Vector   c = Vector(),
                               const GLfloat  theta = 0.0f);
        Vector getRelationship(const GLint   pos   = 0,
                               const GLfloat r     = DEFAULT_FORMATION_RADIUS,
                               const Vector  c     = Vector(),
                               const GLfloat theta = 0.0f);

        // <virtual overloaded operators>
        virtual Formation& operator =(const Formation &f);


        // <protected data members>
        GLfloat radius, heading;
    protected:
        Vector  seedGradient;
        GLint   seedID, formationID;

        // <protected utility functions>
        GLfloat fIntersect(const Function f = DEFAULT_FORMATION_FUNCTION,
                           const GLfloat  r = DEFAULT_FORMATION_RADIUS,
                           const Vector   c = Vector(),
                           const GLfloat  x = 0.0f);
};  // Formation

#endif
