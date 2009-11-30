//
// Filename:        "Vector.cpp"
//
// Programmer:      Ross Mead
// Last modified:   07Nov2009
//
// Description:     This class implements a 3-dimensional vector.
//

// preprocessor directives
#include "Vector.h"



// <constructors>

//
// Vector(dx, dy, dz, colorIndex)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// the vector to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      dx          in      the initial x-coordinate of the vector (default 0)
//      dy          in      the initial y-coordinate of the vector (default 0)
//      dz          in      the initial z-coordinate of the vector (default 0)
//      colorIndex  in      the initial array index of the color of the vector
//
Vector::Vector(const GLfloat dx, const GLfloat dy, const GLfloat dz,
               const Color   colorIndex)
{
    init(dx, dy, dz, colorIndex);
}   // Vector(const GLfloat, const GLfloat, const GLfloat, const Color)



//
// Vector(v)
// Last modified: 04Sep2006
//
// Copy constructor that copies the contents of
// the parameterized vector into this vector.
//
// Returns:     <none>
// Parameters:
//      c       in/out      the circle being copied
//
Vector::Vector(const Vector &v)
{
    init(v.x, v.y, v.z, DEFAULT_VECTOR_COLOR);
    setColor(v.color);
    for (GLint i = 0; i < 3; ++i)
    {
        translate[i] = v.translate[i];
        rotate[i]    = v.rotate[i];
        scale[i]     = v.scale[i];
    }
    showLine         = v.showLine;
    showHead         = v.showHead;
}   // Vector(const Vector &)



// <destructors>

//
// ~Vector()
// Last modified: 04Sep2006
//
// Destructor that clears this vector.
//
// Returns:     <none>
// Parameters:  <none>
//
Vector::~Vector()
{
}   // ~Vector()



// <virtual public mutator functions>

//
// bool set(dx, dy, dz)
// Last modified: 04Sep2006
//
// Attempts to set the xyz-coordinates to the corresponding
// parameterized values, returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx      in      the x-coordinate to be set to (default 0)
//      dy      in      the y-coordinate to be set to (default 0)
//      dz      in      the z-coordinate to be set to (default 0)
//
bool Vector::set(const GLfloat dx, const GLfloat dy, const GLfloat dz)
{
    x = dx;
    y = dy;
    z = dz;
    return true;
}   // set(const GLfloat, const GLfloat, const GLfloat)



//
// bool set(v)
// Last modified: 04Sep2006
//
// Attempts to set the xyz-coordinates to based upon the
// parameterized vector, returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      v       in      the vector to be set to
//
bool Vector::set(const Vector &v)
{
    return set(v.x, v.y, v.z);
}   // set(const Vector &v)



//
// bool setColor(r, g, b)
// Last modified: 04Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in/out  the red in the color to be set to
//      g       in/out  the green in the color to be set to
//      b       in/out  the blue in the color to be set to
//
bool Vector::setColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
    color[GLUT_RED]   = r;
    color[GLUT_GREEN] = g;
    color[GLUT_BLUE]  = b;
    return true;
}   // setColor(const GLfloat, const GLfloat, const GLfloat)



//
// bool setColor(clr)
// Last modified: 04Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      clr     in/out  the color to be set to
//
bool Vector::setColor(const GLfloat clr[3])
{
    return setColor(clr[GLUT_RED], clr[GLUT_GREEN], clr[GLUT_BLUE]);
}   // setColor(const GLfloat [])



//
// bool setColor(colorIndex)
// Last modified: 04Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      colorIndex  in/out  the index of the color to be set to
//
bool Vector::setColor(const Color colorIndex)
{
    return setColor(COLOR[(GLint)colorIndex]);
}   // setColor(const Color)



//
// void translated(dx, dy, dz)
// Last modified: 04Sep2006
//
// Translates the vector display based on the
// parameterized xyz-coordinate translations.
//
// Returns:     <none>
// Parameters:
//      dx      in      the x-coordinate translation transformation
//      dy      in      the y-coordinate translation transformation
//      dz      in      the z-coordinate translation transformation
//
void Vector::translated(const GLfloat dx, const GLfloat dy, const GLfloat dz)
{
    translate[0] = dx;
    translate[1] = dy;
    translate[2] = dz;
}   // translated(const GLfloat, const GLfloat, const GLfloat)



//
// void translated(v)
// Last modified: 04Sep2006
//
// Translates the vector display based on the
// parameterized translation vector.
//
// Returns:     <none>
// Parameters:
//      v       in      the translation transformation vector
//
void Vector::translated(const Vector &v)
{
    translated(v.x, v.y, v.z);
}   // translated(const Vector &)



//
// void rotated(theta)
// Last modified: 04Sep2006
//
// Rotates the vector display about itself (in 2-dimensions)
// based on the parameterized rotation angle.
//
// Returns:     <none>
// Parameters:
//      theta   in      the rotation transformation angle
//
void Vector::rotated(GLfloat theta)
{
    rotate[2] = theta;
}   // rotated(GLfloat)



//
// void rotateRelative(theta)
// Last modified: 22Dec2006
//
// Rotates the vector about itself (in 2-dimensions)
// based on the parameterized rotation angle.
//
// Returns:     <none>
// Parameters:
//      theta   in      the rotation angle
//
void Vector::rotateRelative(GLfloat theta)
{
    theta = degreesToRadians(theta);
    set(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta), z);
}   // rotateRelative(GLfloat)



//
// void scaled(s)
// Last modified: 04Sep2006
//
// Scales the vector display based on the parameterized scalar.
//
// Returns:     <none>
// Parameters:
//      s       in      the transformation scalar
//
void Vector::scaled(const GLfloat s)
{
    scale[0] = scale[1] = scale[2] = s;
}   // scaled(const GLfloat)



// <public mutator functions>

//
// bool setPolar(mag, theta, dz)
// Last modified: 07Nov2009
//
// Attempts to set the vector based on the parameterized polar coordinates,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      mag     in      the magnitude to be set to (default 1)
//      theta   in      the angle to be set to (default 0)
//      dz      in      the z-coordinate to be set to (default 0)
//
bool Vector::setPolar(GLfloat mag, GLfloat theta, GLfloat dz)
{
    theta = degreesToRadians(theta);
    return set(mag * cos(theta), mag * sin(theta), dz);
}   // setPolar(GLfloat, GLfloat, GLfloat)



//
// bool setDiff(dest, source)
// Last modified: 04Sep2006
//
// Attempts to set the vector to the difference from the
// parameterized source to the parameterized destination,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dest    in      the destination vector
//      source  in      the source vector
//
bool Vector::setDiff(const Vector &dest, const Vector &source)
{
    return set(source.x - dest.x, source.y - dest.y, source.z - dest.z);
}   // setDiff(const Vector &, const Vector &)



//
// bool setAngle(theta)
// Last modified: 07Nov2009
//
// Attempts to set the angle to the parameterized angle,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      theta   in      the angle to be set to (default 0)
//
bool Vector::setAngle(const GLfloat theta)
{
	return setPolar(magnitude(), theta);
}   // setAngleconst GLfloat)



//
// bool setMagnitude(mag)
// Last modified: 07Nov2009
//
// Attempts to set the magnitude (normal) to the parameterized magnitude,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      mag     in      the magnitude to be set to (default 1)
//
bool Vector::setMagnitude(const GLfloat mag)
{
    if (!normalize()) return false;
    return set(x * mag, y * mag, z * mag);
}   // setMagnitude(const GLfloat)



//
// bool setNorm(mag)
// Last modified: 07Nov2009
//
// Attempts to set the normal (magnitude) to the parameterized magnitude,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      mag     in      the magnitude to be set to (default 1)
//
/*bool Vector::setNorm(const GLfloat mag)
{
    if (!normalize()) return false;
    return set(x * mag, y * mag, z * mag);
}   // setNorm(const GLfloat)*/



//
// bool setPerp()
// Last modified: 04Sep2006
//
// Attempts to set this vector to its perpendicular vector,
// returning true if successful, false otherwise.
//
// Returns:     <none>
// Parameters:  <none>
//
bool Vector::setPerp()
{
    GLfloat temp = x;
    x            = -y;
    y            = temp;
    return true;
}   // setPerp()



//
// bool setAvg(v, n)
// Last modified: 04Sep2006
//
// Attempts to set the vector based on the average
// of the parameterized vectors, returning true
// if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      v       in/out  the vectors to be averaged
//      n       in      the number of vectors to be averaged
//
bool Vector::setAvg(const Vector v[], const GLint n)
{
    if (n <= 0) return false;
    for (GLint i = 0; i < n; i++) *this += v[i];
    x /= (GLfloat)n;
    y /= (GLfloat)n;
    z /= (GLfloat)n;
    return true;
}   // avg(const Vector [], const GLint)



//
// bool normalize()
// Last modified: 07Nov2009
//
// Attempts to adjust the vector to unit length,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Vector::normalize()
{
    GLfloat mag = magnitude();

    // does nothing to zero vectors
    if (mag == 0.0f) return false;
    return set(x / mag, y / mag, z / mag);
}   // normalize()



// <virtual public utility functions>

//
// void draw()
// Last modified: 07Nov2009
//
// Renders the vector as a line segment with a triangle head.
//
// Returns:     <none>
// Parameters:  <none>
//
void Vector::draw()
{
    if ((color[GLUT_RED]   == COLOR[INVISIBLE][GLUT_RED])   &&
        (color[GLUT_GREEN] == COLOR[INVISIBLE][GLUT_GREEN]) &&
        (color[GLUT_BLUE]  == COLOR[INVISIBLE][GLUT_BLUE])) return;
    glColor3fv(color);
    glLineWidth(VECTOR_LINE_WIDTH);

    glPushMatrix();
        glRotated(rotate[0], 1, 0, 0);
        glRotated(rotate[1], 0, 1, 0);
        glRotated(rotate[2], 0, 0, 1);
        glTranslated(translate[0], translate[1], translate[2]);
        glRotated(angle(),   0, 0, 1);
        glScaled(scale[0], scale[1], scale[2]);

        // draw vector head
        GLfloat mag = magnitude();
        if ((showHead) && (mag >= VECTOR_HEAD_HEIGHT))
        {
            glColor3fv(color);
            glBegin(GL_TRIANGLES);
                glVertex3f(mag, 0.0f, 0.0f);
                glVertex3f(mag - VECTOR_HEAD_HEIGHT,
                           VECTOR_HEAD_WIDTH,
                           0.0f);
                glVertex3f(mag - VECTOR_HEAD_HEIGHT,
                           -VECTOR_HEAD_WIDTH,
                           0.0f);
            glEnd();
        }

        // draw vector line
        if (showLine)
        {
            glBegin(GL_LINES);
                glVertex3f(0.0f,      0.0f, 0.0f);
                glVertex3f(mag, 0.0f, 0.0f);
            glEnd();
        }
    glPopMatrix();
}   // draw()



// <public utility functions>

//
// GLfloat angle() const
// Last modified: 04Sep2006
//
// Returns the angle of this vector.
//
// Returns:     the angle of this vector
// Parameters:  <none>
//
GLfloat Vector::angle() const
{
    if ((x == 0.0f) && (y == 0.0f) && (z == 0.0f)) return 0.0f;
    return sign(y) *
           radiansToDegrees(acos(dotProduct(unit(*this), unit(Vector(1.0f)))));
}   // angle()



//
// GLfloat magnitude() const
// Last modified: 07Nov2009
//
// Returns the magnitude (normal) of this vector.
//
// Returns:     the magnitude (normal) of this vector
// Parameters:  <none>
//
GLfloat Vector::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}   // magnitude()



//
// GLfloat norm() const
// Last modified: 07Nov2009
//
// Returns the normal (magnitude) of this vector.
//
// Returns:     the normal (magnitude) of this vector
// Parameters:  <none>
//
/*GLfloat Vector::norm() const
{
    return sqrt(x * x + y * y + z * z);
}   // norm()*/



//
// void perp() const
// Last modified: 04Sep2006
//
// Returns the perpendicular vector of this vector.
//
// Returns:     the perpendicular vector of this vector
// Parameters:  <none>
//
Vector Vector::perp()
{
    Vector v = *this;
    v.setPerp();
    return v;
}   // perp()

//
// GLfloat perpDot(v) const
// Last modified: 04Sep2006
//
// Returns the dot product of the perpendicular vector
// of this vector and the parameterized vector.
//
// Returns:     the dot product of the perpendicular vector
//              of this vector and the parameterized vector
// Parameters:
//      v       in/out  the vector to be dotted with
//
GLfloat Vector::perpDot(const Vector &v) const
{
    return x * v.x - y * v.y;
}   // perpDot(const Vector &)


// <overloaded operators>

//
// Vector& =(v)
// Last modified: 05Sep2006
//
// Copies the contents of the parameterized vector into this vector.
//
// Returns:     this vector
// Parameters:
//      v       in/out  the vector being copied
//
Vector& Vector::operator =(const Vector &v)
{
    set(v.x, v.y, v.z);
    return *this;
}   // =(const Vector &)



//
// Vector +(v)
// Last modified: 05Sep2006
//
// Calculates the sum of the contents of
// the parameterized vector and this vector.
//
// Returns:     the vector sum
// Parameters:
//      v       in/out  the vector being added
//
Vector Vector::operator +(const Vector &v)
{
    return Vector(this->x + v.x, this->y + v.y, this->z + v.z);
}   // +(const Vector &)



//
// Vector -(v)
// Last modified: 05Sep2006
//
// Calculates the difference of the contents of
// the parameterized vector and this vector.
//
// Returns:     this vector difference
// Parameters:
//      v       in/out  the vector being subtracted
//
Vector Vector::operator -(const Vector &v)
{
    return *this + -v;
}   // -(const Vector &)



//
// Vector& +=(v)
// Last modified: 05Sep2006
//
// Adds the contents of the parameterized vector to this vector.
//
// Returns:     this vector
// Parameters:
//      v       in/out  the vector being added
//
Vector& Vector::operator +=(const Vector &v)
{
    return *this = *this + v;
}   // +=(const Vector &)



//
// Vector& -=(v)
// Last modified: 05Sep2006
//
// Subtracts the contents of the parameterized vector from this vector.
//
// Returns:     this vector
// Parameters:
//      v       in/out  the vector being subtracted
//
Vector& Vector::operator -=(const Vector &v)
{
    return *this = *this - v;
}   // +=(const Vector &)



//
// Vector& *=(v)
// Last modified: 05Sep2006
//
// Multiplies this vector by the parameterized scalar.
//
// Returns:     this vector
// Parameters:
//      scalar  in      the vector multiplier
//
Vector& Vector::operator *=(const GLfloat scalar)
{
    return *this = *this * scalar;
}   // *=(const Vector &)



//
// bool ==(v)
// Last modified: 05Sep2006
//
// Compares this vector to the parameterized vector,
// returning true if they are the same, false otherwise.
//
// Returns:     true if the vectors are the same, false otherwise
// Parameters:
//      v       in/out  the vector to compare to
//
bool Vector::operator ==(const Vector &v)
{
    return (x == v.x) && (y == v.y) && (z == v.z);
}   // ==(const Vector &)



//
// bool !=(v)
// Last modified: 05Sep2006
//
// Compares this vector to the parameterized vector,
// returning true if they are not the same, false otherwise.
//
// Returns:     true if the vectors are not the same, false otherwise
// Parameters:
//      v       in/out  the vector to compare to
//
bool Vector::operator !=(const Vector &v)
{
    return !(*this == v);
}   // ==(const Vector &)



// <friend functions>

//
// ostream& <<(out, v)
// Last modified: 05Sep2006
//
// Outputs the parameterized vector to the parameterized output file stream.
//
// Returns:     the output file stream
// Parameters:
//      out     in/out  the output files stream
//      v       in/out  the vector to output
//
ostream& operator << (ostream &out, const Vector &v)
{
    out << "<" << v.x << ", " << v.y << ", " << v.z << ">";
    return out;
}   // <<(ostream &, const Vector &)



//
// Vector -(v)
// Last modified: 05Sep2006
//
// Returns the negation of the parameterized vector.
//
// Returns:     the negation of the parameterized vector
// Parameters:
//      v       in/out  the vector to be negated
//
Vector operator -(const Vector &v)
{
    return -1.0f * v;
}   // -(const Vector &)



//
// Vector *(scalar, v)
// Last modified: 05Sep2006
//
// Returns the parameterized vector multiplied by the parameterized scalar.
//
// Returns:     the negation of the parameterized vector
// Parameters:
//      scalar  in      the vector multiplier
//      v       in/out  the vector to be negated
//
Vector operator *(const GLfloat scalar, const Vector &v)
{
    return Vector(scalar * v.x, scalar * v.y, scalar * v.z);
}   // *(const GLfloat, const Vector &)



//
// Vector *(v, scalar)
// Last modified: 05Sep2006
//
// Returns the parameterized vector multiplied by the parameterized scalar.
//
// Returns:     the negation of the parameterized vector
// Parameters:
//      v       in/out  the vector to be negated
//      scalar  in      the vector multiplier
//
Vector operator *(const Vector &v, const GLfloat scalar)
{
    return scalar * v;
}   // *(const Vector &, const GLfloat)



//
// Vector unit(v)
// Last modified: 05Sep2006
//
// Returns the unit vector of the parameterized vector.
//
// Returns:     the unit vector of the parameterized vector
// Parameters:
//      v       in/out  the vector to be unitized
//
Vector unit(const Vector &v)
{
    Vector temp = v;
    temp.normalize();
    return temp;
}   // unit(const Vector &)



//
// Vector crossProduct(v1, v2)
// Last modified: 05Sep2006
//
// Returns the cross product of the parameterized vectors.
//
// Returns:     the cross product of the parameterized vectors
// Parameters:
//      v1      in/out  the first vector
//      v2      in/out  the second vector
//
Vector crossProduct(const Vector &v1, const Vector &v2)
{
    return Vector(v1.y * v2.z - v2.y * v1.z,
                  v1.z * v2.x - v2.z * v1.x,
                  v1.x * v2.y - v2.x * v1.y);
}   // crossProduct(const Vector &, const Vector &)

//
// Vector dotProduct(v1, v2)
// Last modified: 05Sep2006
//
// Returns the dot product of the parameterized vectors.
//
// Returns:     the dot product of the parameterized vectors
// Parameters:
//      v1      in/out  the first vector
//      v2      in/out  the second vector
//
GLfloat dotProduct(const Vector &v1, const Vector &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}   // dotProduct(const Vector &, const Vector &)



//
// GLfloat angle(v)
// Last modified: 05Sep2006
//
// Returns the angle of the parameterized vector.
//
// Returns:     the angle of the parameterized vector
// Parameters:
//      v       in/out  the vector
//
GLfloat angle(const Vector &v)
{
    return v.angle();
}   // angle(const Vector &)



//
// GLfloat angle(v1, v2)
// Last modified: 05Sep2006
//
// Returns the angle between the parameterized vectors.
//
// Returns:     the angle between the parameterized vectors
// Parameters:
//      v1      in/out  the first vector
//      v2      in/out  the second vector
//
GLfloat angle(const Vector &v1, const Vector &v2)
{
    return scaleDegrees(v1.angle() - v2.angle());
}   // angle(const Vector &, const Vector &)



// <virtual protected utility functions>

//
// bool init(dx, dy, dz, colorIndex)
// Last modified: 05Sep2006
//
// Initializes the vector to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx          in      the initial x-coordinate of the vector (default 0)
//      dy          in      the initial y-coordinate of the vector (default 0)
//      dz          in      the initial z-coordinate of the vector (default 0)
//      colorIndex  in      the initial array index of the color of the vector
//
bool Vector::init(const GLfloat dx, const GLfloat dy, const GLfloat dz,
                  const Color   colorIndex)
{
    set(dx, dy, dz);
    setColor(colorIndex);
    for (GLint i = 0; i < 3; ++i)
    {
        translate[i] = DEFAULT_VECTOR_TRANSLATE[i];
        rotate[i]    = DEFAULT_VECTOR_ROTATE[i];
        scale[i]     = DEFAULT_VECTOR_SCALE[i];
    }
    showLine         = DEFAULT_VECTOR_SHOW_LINE;
    showHead         = DEFAULT_VECTOR_SHOW_HEAD;
    return true;
}   // init(const GLfloat, const GLfloat, const GLfloat, const Color)
