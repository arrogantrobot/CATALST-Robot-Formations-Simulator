//
// Filename:        "Circle.cpp"
//
// Programmer:      Ross Mead
// Last modified:   06Nov2009
//
// Description:     This class implements a 2-dimensional circle.
//

// preprocessor directives
#include "Circle.h"



// <constructors>

//
// Circle(dx, dy, dz, r, colorIndex)
// Last modified: 04Sep2006
//
// Default constructor that initializes
// this circle to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      dx          in      the initial x-coordinate of the circle (default 0)
//      dy          in      the initial y-coordinate of the circle (default 0)
//      dz          in      the initial z-coordinate of the circle (default 0)
//      r           in      the initial radius of the circle (default 0)
//      colorIndex  in      the initial array index of the color of the circle
//
Circle::Circle(const GLfloat dx, const GLfloat dy, const GLfloat dz,
               const GLfloat r,  const Color   colorIndex)
{
    init(dx, dy, dz, r, colorIndex);
}   // Circle(const GLfloat..<4>, const Color)



//
// Circle(v, r)
// Last modified: 27Aug2006
//
// Default constructor that initializes
// the circle to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      v           in      the initial vector coordinates of the circle
//      r           in      the initial radius of the circle (default 0)
//
Circle::Circle(const Vector &v, const GLfloat r)
{
    init(v.x, v.y, v.z, r);
    setColor(v.color);
}   // Circle(const Vector, const GLfloat)



//
// Circle(c)
// Last modified: 27Aug2006
//
// Copy constructor that copies the contents of
// the parameterized circle into this circle.
//
// Returns:     <none>
// Parameters:
//      c       in/out      the circle being copied
//
Circle::Circle(const Circle &c)
{
    init(c.x, c.y, c.z, c.radius, DEFAULT_CIRCLE_COLOR);
    setColor(c.color);
    for (GLint i = 0; i < 3; ++i)
    {
        translate[i] = c.translate[i];
        rotate[i]    = c.rotate[i];
        scale[i]     = c.scale[i];
    }
    showLine         = c.showLine;
    showHead         = c.showHead;
    showPos          = c.showPos;
}   // Circle(const Circle &)



// <destructors>

//
// ~Circle()
// Last modified: 27Aug2006
//
// Destructor that clears this circle.
//
// Returns:     <none>
// Parameters:  <none>
//
Circle::~Circle()
{
}   // ~Circle()



// <virtual public mutator functions>

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
bool Circle::setRadius(const GLfloat r)
{
    if (r <= 0.0f) return false;
    radius = r;
    return true;
}   // setRadius(const GLfloat)



//
// bool setDiameter(d)
// Last modified: 28Aug2006
//
// Attempts to set the radius based upon the parameterized diameter,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      d       in      the diameter to determine the radius be set to
//
bool Circle::setDiameter(const GLfloat d)
{
    return setRadius(0.5f * d);
}   // setDiameter(const GLfloat)



//
// bool setCircumference(c)
// Last modified: 28Aug2006
//
// Attempts to set the radius based upon the parameterized circumference,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      c       in      the circumference to determine the radius be set to
//
bool Circle::setCircumference(const GLfloat c)
{
    return setRadius(0.5f * c / PI);
}   // setCircumference(const GLfloat)



//
// bool setArea(a)
// Last modified: 28Aug2006
//
// Attempts to set the radius based upon the parameterized area,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      a       in      the area to determine the radius be set to
//
bool Circle::setArea(const GLfloat a)
{
    return setRadius(sqrt(a / PI));
}   // setArea(const GLfloat)



// <public accessor functions>

//
// GLfloat getRadius() const
// Last modified: 27Aug2006
//
// Returns the radius of this circle.
//
// Returns:     the radius of this circle
// Parameters:  <none>
//
GLfloat Circle::getRadius() const
{
    return radius;
}   // getRadius() const



//
// GLfloat getDiameter() const
// Last modified: 27Aug2006
//
// Returns the diameter of this circle.
//
// Returns:     the diameter of this circle
// Parameters:  <none>
//
GLfloat Circle::getDiameter() const
{
    return 2.0f * radius;
}   // getDiameter() const



//
// GLfloat getCircumference() const
// Last modified: 27Aug2006
//
// Returns the circumference of this circle.
//
// Returns:     the circumference of this circle
// Parameters:  <none>
//
GLfloat Circle::getCircumference() const
{
    return getDiameter() * PI;
}   // getCircumference() const



//
// GLfloat getArea() const
// Last modified: 27Aug2006
//
// Returns the area of this circle.
//
// Returns:     the area of this circle
// Parameters:  <none>
//
GLfloat Circle::getArea() const
{
	return PI * radius * radius;
    //return PI * pow(radius, 2.0f);
}   // getArea() const



// <public utility functions>

//
// void draw()
// Last modified: 06Nov2009
//
// Renders the circle with current vector position and radius.
//
// Returns:     <none>
// Parameters:  <none>
//
void Circle::draw()
{
    if ((color[GLUT_RED]   == COLOR[INVISIBLE][GLUT_RED])   &&
        (color[GLUT_GREEN] == COLOR[INVISIBLE][GLUT_GREEN]) &&
        (color[GLUT_BLUE]  == COLOR[INVISIBLE][GLUT_BLUE])) return;
    glColor3fv(color);
    glLineWidth(VECTOR_LINE_WIDTH);

	  // draw vector position
    if (showPos) Vector::draw();

    // draw a polygonal approximation to the circle
    glPushMatrix();
        glRotated(rotate[0], 1, 0, 0);
        glRotated(rotate[1], 0, 1, 0);
        glRotated(rotate[2], 0, 0, 1);
        glTranslated(x + translate[0], y + translate[1], z + translate[2]);
        glRotated(angle(), 0, 0, 1);
        glScaled(scale[0], scale[1], scale[2]);

        // draw each edge of the polygonal approximation to the circle
        GLfloat dx = radius, dy = 0.0f, tx = 0.0f, ty = 0.0f;
        if (showFilled)
        {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);    // origin
        }
        else glBegin(GL_LINE_LOOP);
        for (GLint i = 0; i <= CIRCLE_N_LINKS; ++i)
	      {
            glVertex2f(dx, dy);    // output vertex

            // calculate the tangential vector;
            // remember, the radial vector is (x, y);
            // to get the tangential vector, we flip those coordinates
            // and negate one of them
            tx  = -dy;
            ty  =  dx;

            // add the tangential vector (tan-theta)
            dx += tx * CIRCLE_TAN_THETA;
            dy += ty * CIRCLE_TAN_THETA;

            // correct using the radial factor (cos-theta)
            dx *= CIRCLE_COS_THETA;
            dy *= CIRCLE_COS_THETA;
	      }
        glEnd();
    glPopMatrix();
}   // draw()



// <virtual overloaded operators>

//
// Circle& =(c)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized circle into this circle.
//
// Returns:     this circle
// Parameters:
//      c       in/out      the circle being copied
//
Circle Circle::operator = (const Circle &c)
{
    set(c);
    setRadius(c.radius);
    return *this;
}   // =(const Circle &)



// <virtual protected utility functions>

//
// bool init(dx, dy, dz, r, colorIndex)
// Last modified: 06Nov2009
//
// Initializes the circle to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx          in      the initial x-coordinate of the circle (default 0)
//      dy          in      the initial y-coordinate of the circle (default 0)
//      dz          in      the initial z-coordinate of the circle (default 0)
//      r           in      the initial radius of the circle (default 0)
//      colorIndex  in      the initial array index of the color of the circle
//
bool Circle::init(const GLfloat dx, const GLfloat dy, const GLfloat dz,
                  const GLfloat r,  const Color   colorIndex)
{
    Vector::init(dx, dy, dz, colorIndex);
    if (!setRadius(r)) return false;
    showPos    = DEFAULT_CIRCLE_SHOW_POS;
    showFilled = DEFAULT_CIRCLE_SHOW_FILLED;
    return true;
}   // init(const GLfloat..<4>, const Color)
