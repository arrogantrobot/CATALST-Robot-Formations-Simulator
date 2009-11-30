//
// Filename:        "Neighbor.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This structure defines a robot cell neighbor.
//

// preprocessor directives
#ifndef NEIGHBOR_H
#define NEIGHBOR_H
#include "State.h"
using namespace std;



// defines a robot cell neighbor
struct Neighbor: Relationship, State
{

    // <constructors>

    //
    // Neighbor(r, s)
    // Last modified: 04Sep2006
    //
    // Default constructor that initializes
    // this neighbor to the parameterized values.
    //
    // Returns:     <none>
    // Parameters:
    //      r       in      the default relationship
    //      s       in      the default state
    //
    Neighbor(const Relationship r = Relationship(),
             const State        s = State()): Relationship(r), State(s)
    {
    }   // Neighbor(const Relationship, const State)



    //
    // Neighbor(id, s, desired, actual)
    // Last modified: 12Aug2006
    //
    // Default constructor that sets the specified
    // data members to the appropriate values.
    //
    // Returns:     <none>
    // Parameters:
    //      id      in      the default ID
    //      s       in      the default state
    //      desired in      the default desired relationship
    //      actual  in      the default actual relationship
    //
    Neighbor(const GLint  id,
             const State  s       = State(),
             const Vector desired = Vector(),
             const Vector actual  = Vector())
        : Relationship(desired, actual, id), State(s)
    {
    }   // Neighbor(const GLint, const State, const Vector, const Vector)



    //
    // Neighbor(n)
    // Last modified: 12Aug2006
    //
    // Copy constructor that copies the contents of
    // the parameterized neighbor into this neighbor.
    //
    // Returns:     <none>
    // Parameters:
    //      n       in/out      the neighbor being copied
    //
    Neighbor(const Neighbor &n): Relationship(n), State(n)
    {
    }   // Neighbor(const Neighbor &)



    // <accessor functions>

    //
    // Relationship getRelationship() const
    // Last modified: 12Aug2006
    //
    // Returns the relationship to this neighbor.
    //
    // Returns:     the relationship to this neighbor
    // Parameters:  <none>
    //
    Relationship getRelationship() const
    {
        return (Relationship)(*this);
    }   // getRelationship() const



    //
    // State getState() const
    // Last modified: 12Aug2006
    //
    // Returns the state of this neighbor.
    //
    // Returns:     the state of this neighbor
    // Parameters:  <none>
    //
    State getState() const
    {
        return (State)(*this);
    }   // getState() const



    // <virtual overloaded operators>

    //
    // Neighbor& =(r)
    // Last modified: 12Aug2006
    //
    // Copies the contents of the parameterized
    // relationship into this neighbor.
    //
    // Returns:     this neighbor
    // Parameters:
    //      r       in/out      the relationship being copied
    //
    virtual Neighbor& operator =(const Relationship &r)
    {
        relDesired = r.relDesired;
        relActual  = r.relActual;
        ID         = r.ID;
        return *this;
    }   // =(const Relationship &)
    


    //
    // Neighbor& =(s)
    // Last modified: 06Nov2009
    //
    // Copies the contents of the parameterized
    // state into this neighbor.
    //
    // Returns:     this neighbor
    // Parameters:
    //      s       in/out      the state being copied
    //
    virtual Neighbor& operator =(const State &s)
    {
        formation  = s.formation;
        gradient   = s.gradient;
        rels       = s.rels;
        transError = s.transError;
        rotError   = s.rotError;
        tStep      = s.tStep;
        return *this;
    }   // =(const State &)
};  // Neighbor

#endif
