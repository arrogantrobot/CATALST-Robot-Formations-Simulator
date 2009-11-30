//
// Filename:        "State.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This structure defines a robot cell state.
//

// preprocessor directives
#ifndef STATE_H
#define STATE_H
#include <vector>
#include "Formation.h"
#include "Vector.h"
using namespace std;



// defines a robot cell state
struct State
{

    // <data members>
    Formation            formation;     // the current formation
    Vector               gradient;      // the formation gradient
    vector<Relationship> rels;          // the formation relationships
    Vector               transError;    // the summed translational error
    GLfloat              rotError;      // the summed rotational error
    GLint                tStep;         // the time step in the formation



    // <constructors>

    //
    // State(f, grad, r, e, d, s)
    // Last modified: 29Nov2009
    //
    // Default constructor that initializes
    // this state to the parameterized values.
    //
    // Returns:     <none>
    // Parameters:
    //      f       in      the default formation
    //      grad    in      the default gradient
    //      r       in      the default list of relationships
    //      tError  in      the default translational error
    //      rError  in      the default rotational error
    //      ts      in      the default time step
    //
    State(const Formation            f      = Formation(),
          const Vector               grad   = Vector(),
          const vector<Relationship> r      = vector<Relationship>(),
          const Vector               tError = Vector(),
          const GLfloat              rError = 0.0f,
          const GLint                ts     = 0)
          : formation(f),       gradient(grad),   rels(r),
            transError(tError), rotError(rError), tStep(ts)
    {
    }   // State(const..{Formation, Vector, LL<Relationship>, Vector, GLint})
};  // State

#endif
