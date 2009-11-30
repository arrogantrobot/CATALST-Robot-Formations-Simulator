//
// Filename:        "GLIncludes.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This file includes OpenGL libraries (platform independent).
//

// preprocessor directives
#ifndef GL_INCLUDE_H
#define GL_INCLUDE_H

#ifdef __WIN32__
#include "glut.h"
#endif

#ifdef _WIN32
#include "glut.h"
#endif

#ifdef WIN32
#include "glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/glut.h>
#endif

#endif

