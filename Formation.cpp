//
// Filename:        "Formation.cpp"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class implements a formation.
//

// preprocessor directives
#include "Formation.h"



// <constructors>

//
// Formation(f, r, sGrad, sID, fID, theta)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this formation to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      f           in      the initial function of the formation
//      r           in      the initial radius of the formation
//      sGrad       in      the initial seed gradient of the formation
//      sID         in      the initial seed ID of the formation
//      fID         in      the initial ID of the formation
//      theta       in      the initial heading of the formation
//
Formation::Formation(const Function f,
                     const GLfloat  r,
                     const Vector   sGrad,
                     const GLint    sID,
                     const GLint    fID,
                     const GLfloat  theta)
{
    setFunction(f);
    setRadius(r);
    setSeedGradient(sGrad);
    setSeedID(sID);
    setFormationID(fID);
    setHeading(theta);
}   // Formation(const..{Function, GLfloat, Vector, GLint, GLint, GLfloat})



//
// Formation(f, r, sGrad, sID, fID, theta)
// Last modified: 29Nov2009
//
// Default constructor that initializes
// the formation to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      f           in      the initial set of functions of the formation
//      r           in      the initial radius of the formation
//      sGrad       in      the initial seed gradient of the formation
//      sID         in      the initial seed ID of the formation
//      fID         in      the initial ID of the formation
//      theta       in      the initial heading of the formation
//
Formation::Formation(vector<Function> f,
                     const GLfloat    r,
                     const Vector     sGrad,
                     const GLint      sID,
                     const GLint      fID,
                     const GLfloat    theta)
{
    setFunctions(f);
    setRadius(r);
    setSeedGradient(sGrad);
    setSeedID(sID);
    setFormationID(fID);
    setHeading(theta);
}   // Formation(const..{LL<Function>, GLfloat, Vector, GLint..<2>, GLfloat})



//
// Formation(f)
// Last modified: 28Aug2006
//
// Copy constructor that copies the contents of
// the parameterized formation into this formation.
//
// Returns:     <none>
// Parameters:
//      f       in/out      the formation being copied
//
Formation::Formation(const Formation &f)
{
    *this = f;
}   // Formation(const Formation &)



// <public mutator functions>

//
// bool setFunction(f)
// Last modified: 28Aug2006
//
// Attempts to set the function to the parameterized function,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in/out  the function to be set to
//
bool Formation::setFunction(const Function f)
{
    clear();
    return addFunction(f);
}   // setFunction(const Function)



//
// bool setFunctions(f)
// Last modified: 29Nov2009
//
// Attempts to set the set of functions
// to the parameterized set of functions,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in/out  the set of functions to be set to
//
bool Formation::setFunctions(const vector<Function> &f)
{
    clear();
    return addFunctions(f);
}   // setFunctions(const vector<Function> &)



//
// bool addFunction(f)
// Last modified: 29Nov2009
//
// Attempts to add the parameterized function to the formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in      the function being added
//
bool Formation::addFunction(const Function f)
{
    if (f == NULL) return false;
    push_back(f);
    return true;
}   // addFunction(const Function)



//
// bool addFunctions(f)
// Last modified: 29Nov2009
//
// Attempts to add the parameterized set of functions to the formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in/out  the set of functions being added
//
bool Formation::addFunctions(const vector<Function> &f)
{
    for (GLint i = 0; i < f.size(); ++i)
        if (!addFunction(f[i])) return false;
    return true;
}   // addFunctions(const vector<Function> &)



//
// bool removeFunction(pos)
// Last modified: 29Nov2009
//
// Attempts to remove the function at the
// parameterized position from the formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      pos     in      the position to remove (default 0)
//
bool Formation::removeFunction(const GLint pos)
{
    if ((pos < 0) || (pos >= size())) return false;
    erase(begin() + pos);
    return true;
}   // removeFunction(const GLint)



//
// bool removeFunctions()
// Last modified: 28Aug2006
//
// Attempts to remove all of the functions from the formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      pos     in      the position to remove (default 0)
//
bool Formation::removeFunctions()
{
    clear();
    return true;
}   // removeFunctions()



//
// bool setRadius(r)
// Last modified: 28Aug2006
//
// Attempts to set the radius to the parameterized radius,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in      the radius to be set to
//
bool Formation::setRadius(const GLfloat r)
{
    if (r <= 0.0f) return false;
    radius = r;
    return true;
}   // setRadius(const GLfloat)



//
// bool setSeedGradient(sGrad)
// Last modified: 28Aug2006
//
// Attempts to set the seed gradient
// to the parameterized seed gradient,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      sGrad   in/out  the seed gradient to be set to
//
bool Formation::setSeedGradient(const Vector sGrad)
{
    seedGradient = sGrad;
    return true;
}   // setSeedGradient(const Vector)



//
// bool setSeedID(sID)
// Last modified: 28Aug2006
//
// Attempts to set the seed ID to the parameterized seed ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      sID     in/out  the seed ID to be set to
//
bool Formation::setSeedID(const GLint sID)
{
    if ((sID < 0) && (sID != ID_OPERATOR) && (sID != ID_BROADCAST))
        return false;
    seedID = sID;
    return true;
}   // setSeedID(const GLint)



//
// bool setFormationID(fID)
// Last modified: 28Aug2006
//
// Attempts to set the formation ID to the parameterized formation ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      fID     in/out  the formation ID to be set to
//
bool Formation::setFormationID(const GLint fID)
{
    formationID = fID;
    return true;
}   // setFormationID(const GLint)



//
// bool setHeading(theta)
// Last modified: 28Aug2006
//
// Attempts to set the heading to the parameterized heading,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      theta   in/out  the heading to be set to
//
bool Formation::setHeading(const GLfloat theta)
{
    heading = scaleDegrees(theta);
    return true;
}   // setHeading(const GLint)



// <public accessor functions>

//
// Function getFunction() const
// Last modified: 30Nov2009
//
// Returns the function at the parameterized position in this formation.
//
// Returns:     the function at the parameterized position in this formation
// Parameters:
//      pos     in      the position of the desired function (default 0)
//
Function Formation::getFunction(const int pos) const
{
    if ((pos < 0) || (pos > this->size())) return NULL;
    return at(pos);
}   // getFunction(const int) const



//
// vector<Function> getFunctions() const
// Last modified: 29Nov2009
//
// Returns the set of functions of this formation.
//
// Returns:     the set of functions of this formation
// Parameters:  <none>
//
vector<Function> Formation::getFunctions() const
{
    return *this;
}   // getFunctions() const



//
// GLfloat getRadius() const
// Last modified: 28Aug2006
//
// Returns the radius of this formation.
//
// Returns:     the radius of this formation
// Parameters:  <none>
//
GLfloat Formation::getRadius() const
{
    return radius;
}   // getRadius() const



//
// Vector getSeedGradient() const
// Last modified: 28Aug2006
//
// Returns the seed gradient of this formation.
//
// Returns:     the seed gradient of this formation
// Parameters:  <none>
//
Vector Formation::getSeedGradient() const
{
    return seedGradient;
}   // getSeedGradient() const



//
// GLint getSeedID() const
// Last modified: 28Aug2006
//
// Returns the seed ID of this formation.
//
// Returns:     the seed ID of this formation
// Parameters:  <none>
//
GLint Formation::getSeedID() const
{
    return seedID;
}   // getSeedID() const



//
// GLint getFormationID() const
// Last modified: 28Aug2006
//
// Returns the formation ID of this formation.
//
// Returns:     the formation ID of this formation
// Parameters:  <none>
//
GLint Formation::getFormationID() const
{
    return formationID;
}   // getFormationID() const



//
// GLfloat getHeading() const
// Last modified: 28Aug2006
//
// Returns the heading of this formation.
//
// Returns:     the heading of this formation
// Parameters:  <none>
//
GLfloat Formation::getHeading() const
{
    return heading;
}   // getHeading() const



// <public utility functions>

//
// vector<Vector> getRelationships(c)
// Last modified: 29Nov2009
//
// Calculates the intersections of the set of functions of this formation
// and a circle centered at the parameterized vector position c with
// the appropriate radius, returning a list of these vectors.
//
// Returns:     list of the desired relationship vectors
// Parameters:
//      c       in      the position to be centered at
//
vector<Vector> Formation::getRelationships(const Vector c)
{
    if (empty()) return vector<Vector>();
    vector<Vector> rels;
    Function       curr = NULL;
    for (GLint i = 0; i < size(); ++i)
    {
        curr = at(i);
        rels.push_back(getRelationship(curr, -radius, c, heading));
        rels.push_back(getRelationship(curr,  radius, c, heading));
    }
    return rels;
}   // getRelationships(const Vector)

//
// Vector getRelationship(f, r, c, theta)
// Last modified: 28Aug2006
//
// Uses the secant method to calculate the intersection of the function
// and a circle centered at the parameterized vector position c with
// the appropriate radius, returning a vector from c to this intersection.
//
// The secant method is defined by the following recurrence relation:
//
//      x_(n + 1) = x_n - f(x_n) * (x_n - x_(n - 1)) / (f(x_n) - f(x_(n - 1))).
//
// Returns:     vector from the parameterized vector position c
//              to the intersection of the function and appropriate circle
// Parameters:
//      f           in      the intersecting function
//      r           in      the radius of the intersecting circle
//      c           in      the position to be centered at
//      theta       in      the rotation of the relationship (default 0)
//
Vector Formation::getRelationship(const Function f,
                                  const GLfloat  r,
                                  const Vector   c,
                                  const GLfloat  theta)
{
    if (f == NULL) return Vector();
    GLfloat xn        = c.x + r + X_ROOT_THRESHOLD,
            xn_1      = c.x + r - X_ROOT_THRESHOLD,
            intersect = 0.0f, error = 0.0f;
    for (int i = 0; i < X_N_ITERATIONS; ++i)
    {
        intersect     = fIntersect(f, r, c, xn);
        error         = intersect * (xn - xn_1) /
                       (intersect - fIntersect(f, r, c, xn_1));
        if (abs(error) <= X_ROOT_THRESHOLD) break;
        xn_1          = xn;
        xn           -= error;
    }
    Vector rel        = Vector(xn, f(xn)) - c;
    rel.rotateRelative(-theta);
    return rel;
}   // getRelationship(const..{Function, GLfloat, Vector, GLfloat})



//
// Vector getRelationship(pos, r, c, theta)
// Last modified: 28Aug2006
//
// Calculates the intersection of the function at the parameterized position
// and a circle centered at the parameterized vector position c with
// the appropriate radius, returning a vector from c to this intersection.
//
// Returns:     vector from the parameterized vector position c
//              to the intersection of the function and appropriate circle
// Parameters:
//      pos     in      the position of the desired function (default 0)
//      r       in      the radius of the intersecting circle
//      c       in      the position to be centered at
//      theta   in      the rotation of the relationship (default 0)
//
Vector Formation::getRelationship(const GLint   pos,
                                  const GLfloat r,
                                  const Vector  c,
                                  const GLfloat theta)
{
    return getRelationship(getFunction(pos), r, c, theta);
}   // getRelationship(const GLint, const GLfloat, const Vector, const GLfloat)



// <virtual overloaded operators>

//
// Formation& =(f)
// Last modified: 28Aug2006
//
// Copies the contents of the parameterized formation into this formation.
//
// Returns:     this formation
// Parameters:
//      f       in/out      the formation being copied
//
Formation& Formation::operator =(const Formation &f)
{
    setFunctions(f);
    setRadius(f.radius);
    setSeedGradient(f.seedGradient);
    setSeedID(f.seedID);
    setFormationID(f.formationID);
    setHeading(f.heading);
    return *this;
}   // =(const Formation &)



// <protected utility functions>

//
// GLfloat fIntersect(f, r, c, x)
// Last modified: 28Aug2006
//
// Used to determine the intersection of the parameterized function
// and a circle centered at the parameterized vector position c and
// offset by x with the appropriate radius.
//
// Returns:     this formation
// Parameters:
//      f       in/out      the formation being copied
//
GLfloat Formation::fIntersect(const Function f, const GLfloat r,
                              const Vector   c, const GLfloat x)
{
	GLfloat dx = x - c.x, dy = f(x) - c.y;
	return  dx * dx + dy * dy - r * r;
    //return pow(x - c.x, 2.0f) + pow(f(x) - c.y, 2.0f) - pow(r, 2.0f);
}   // fIntersect(const Function, const GLfloat, const Vector, const GLfloat)
