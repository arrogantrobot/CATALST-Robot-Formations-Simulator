//
// Filename:        "Simulator.cpp"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This program tests the robot cell simulator.
//

// preprocessor directives
#include <signal.h>
#include <string.h>
#include "Environment.h"
using namespace std;



// define SIGPIPE if not defined (compatibility for win32)
#ifndef SIGPIPE
#define SIGPIPE 13
#endif



// simulation environment function prototypes
void printUsage(GLint argc, char **argv);
bool parseArguments(GLint   argc,
                    char  **argv,
                    GLint   &nRobots,
                    GLint   &fIndex,
                    GLfloat &fRadius,
                    GLfloat &fHeading,
                    GLint   &dt,
                    GLint   &ins,
                    GLfloat &trans,
                    string  &randInput);
bool validateParameters(const GLint   nRobots,
                        const GLint   fIndex,
                        const GLfloat fRadius,
                        const GLfloat fHeading,
                        const GLint   dt);
void terminate(int retVal);
void displayMenu();
bool initEnv(const GLint nRobots, const GLint fIndex);
bool deinitEnv();
bool changeFormation(const GLint index);



// OpenGL function prototypes
void initWindow();
void display();
void keyboardPress(unsigned char keyPressed, GLint mouseX, GLint mouseY);
void keyboardPressSpecial(GLint keyPressed, GLint mouseX, GLint mouseY);
void keyboardReleaseSpecial(GLint keyReleased, GLint mouseX, GLint mouseY);
void mouseClick(GLint mouseButton, GLint mouseState,
                GLint mouseX,      GLint mouseY);
void mouseDrag(GLint mouseX, GLint mouseY);
void resizeWindow(GLsizei w, GLsizei h);
void timerFunction(GLint value);



// formation-testing function prototypes
GLfloat  line(const GLfloat x);
GLfloat  x(const GLfloat x);
GLfloat  absX(const GLfloat x);
GLfloat  negHalfX(const GLfloat x);
GLfloat  negAbsHalfX(const GLfloat x);
GLfloat  negAbsX(const GLfloat x);
GLfloat  parabola(const GLfloat x);
GLfloat  cubic(const GLfloat x);
GLfloat  condSqrt(const GLfloat x);
GLfloat  sine(const GLfloat x);
GLfloat  xRoot3(const GLfloat x);
GLfloat  negXRoot3(const GLfloat x);
Function formations[] = {line,        x,       absX,     negHalfX,
                         negAbsHalfX, negAbsX, parabola, cubic,
                         condSqrt,    sine,    xRoot3,   negXRoot3};



// OpenGL global constants
const GLint INIT_WINDOW_POSITION[2] = {0, 0};      // window offset
const char  CHAR_ESCAPE             = char(27);    // 'ESCAPE' character key



// OpenGL global variables
GLint   g_windowSize[2] = {800, 800};   // window size in pixels
GLfloat g_windowWidth   = 2.0f;         // resized window width
GLfloat g_windowHeight  = 2.0f;         // resized window height



// simulation global constants
const GLfloat   SELECT_RADIUS     = 1.5f * DEFAULT_ROBOT_RADIUS;
const GLint     N_CELLS           = 0;
const GLint     MIDDLE_CELL       = 0;//(N_CELLS - 1) / 2;
const Formation DEFAULT_FORMATION = Formation(formations[0],
                                              DEFAULT_ROBOT_RADIUS *
                                              FACTOR_COLLISION_RADIUS,
                                              Vector(), MIDDLE_CELL, 0,
                                              90.0f);



// simulation global variables
Environment *g_env           = NULL;
GLint        g_nRobots       = 25;
GLfloat      g_fRadius       = DEFAULT_FORMATION.getRadius();
GLint        g_sID           = DEFAULT_FORMATION.getSeedID();
GLint        g_fID           = DEFAULT_FORMATION.getFormationID();
GLfloat      g_fHeading      = DEFAULT_FORMATION.getHeading();
GLint        g_fIndex        = 0;
GLint        g_selectedIndex = g_sID;
GLint        g_dt            = 50;    // time interval (in milliseconds)
GLint        g_ins           = 0;
GLfloat      g_trans         = 0.02;
string       randInput       = "randOut.txt";



//
// GLint main(argc, argv)
// Last modified:   04Sep2006
//
// Uses the OpenGL Utility Toolkit to set the
// window up to display the window and its contents.
//
// Returns:     the result of the OpenGL main loop
// Parameters:
//      argc    in      an argument counter
//      argv    in      initialization arguments
//
int main(GLint argc, char **argv)
{

    // parse command line arguments
    if (!parseArguments(argc, argv,
                        g_nRobots, g_fIndex, g_fRadius, g_fHeading, g_dt,g_ins,g_trans,randInput))
    {
        cerr << ">> ERROR: Unable to parse arguments...\n\n";
        return 1;
    }

    // validate parameters
    if (!validateParameters(g_nRobots, g_fIndex, g_fRadius, g_fHeading, g_dt))
    {
        cerr << ">> ERROR: Unable to validate parameters...\n\n";
        return 1;
    }

    // create handler for interrupts (i.e., ^C)
    if (signal(SIGINT, SIG_IGN) != SIG_IGN) signal(SIGINT, terminate);
    signal(SIGPIPE, SIG_IGN);

    // use the GLUT utility to initialize the window, to handle
    // the input and to interact with the windows system
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(g_windowSize[0], g_windowSize[1]);
    glutInitWindowPosition(INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1]);
    glutCreateWindow("Simulator");

    // specify the resizing, refreshing, and interactive routines
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboardPress);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseDrag );
    glutReshapeFunc(resizeWindow);
    glutSpecialFunc(keyboardPressSpecial);
    glutSpecialUpFunc(keyboardReleaseSpecial);
    glutTimerFunc(50, timerFunction, 1);

    // initialize and execute the robot cell environment
    if (!initEnv(g_nRobots, g_fIndex))
    {
        cerr << ">> ERROR: Unable to initialize simulation environment...\n\n";
        return 1;
    }
    initWindow();
    displayMenu();
    glutMainLoop();

    deinitEnv();

    return 0;
}   // main(GLint, char **)



//
// void printUsage(argc, argv)
// Last modified: 08Nov2009
//
// Prints the program usage message.
//
// Returns:     <none>
// Parameters:
//      argc    in      an argument counter
//      argv    in      initialization arguments
//
void printUsage(GLint argc, char **argv)
{
    cout << "USAGE: " << argv[0]
         << " [-n <nRobots>]"
         << " [-f <fIndex>]"
         << " [-r <fRadius>]"
         << " [-h <fHeading>]"
         << " [-t <dT>]"
         << endl
         << "      -n <nRobots>: number of robots"
         << " [0, MAX_N_ROBOTS] (DEFAULT=19)"        << endl
         << "      -f <fIndex>: formation function index"
         << " [0, 9] (default=0):"                   << endl
         << "             0) f(x) = 0;"              << endl
         << "             1) f(x) = x;"              << endl
         << "             2) f(x) = |x|;"            << endl
         << "             3) f(x) = -0.5 x;"         << endl
         << "             4) f(x) = -|0.5 x|;"       << endl
         << "             5) f(x) = -|x|;"           << endl
         << "             6) f(x) = x^2;"            << endl
         << "             7) f(x) = x^3;"            << endl
         << "             8) f(x) = {sqrt(x),  x >= 0 | -sqrt|x|, x < 0};"
         << endl
         << "             9) f(x) = 0.05 sin(10 x);" << endl
         << "      -r <fRadius>: formation radius"
         << " [0.0, 1.0] (default=?.?)"              << endl
         << "      -h <fHeading>: formation heading"
         << " (in degrees; default=90.0)"            << endl
         << "      -t <dt>: update time interval"
         << " [1, ??] (in milliseconds; default=50)"
         << endl;
}   // printUsage(GLint, char **)



//
// bool parseArguments(argc, argv, nRobots, fIndex, fRadius, fHeading, dt)
// Last modified: 08Nov2009
//
// Parses the parameterized program arguments,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      argc          in      an argument counter
//      argv          in      initialization arguments
//      nRobots       in/out  the number of robots
//      fIndex        in/out  the initial function index of the formation
//      fRadius       in/out  the initial radius between robots in the formation
//      fHeading      in/out  the initial heading of robots in the formation
//      dt            in/out  the time interval (in ms) between OpenGL updates
//
bool parseArguments(GLint    argc,
                    char   **argv,
                    GLint   &nRobots,
                    GLint   &fIndex,
                    GLfloat &fRadius,
                    GLfloat &fHeading,
                    GLint   &dt,
                    GLint   &ins,
                    GLfloat &trans,
                    string  &randInput)
{
    int i = 0;
    while (++i < argc)
    {
        if      (!strncmp(argv[i], "--help", 6))
        {
            printUsage(argc, argv);
            exit(0);
        }
        else if (!strncmp(argv[i], "-i", 2))
        {
            ins = 1;
        }
        else if (!strncmp(argv[i], "-n", 2))
        {
            if (++i < argc) nRobots = atoi(argv[i]);
            else
            {
                printUsage(argc, argv);
                return false;
            }
        }
        else if (!strncmp(argv[i], "-f", 2))
        {
            if (++i < argc) fIndex = atoi(argv[i]);
            else
            {
                printUsage(argc, argv);
                return false;
            }
        }
        else if (!strncmp(argv[i], "-r", 2))
        {
            if (++i < argc) fRadius = atof(argv[i]);
            else
            {
                printUsage(argc, argv);
                return false;
            }
        }
        else if (!strncmp(argv[i], "-h", 2))
        {
            if (++i < argc) fHeading = scaleDegrees(atof(argv[i]));
            else
            {
                printUsage(argc, argv);
                return false;
            }
        }
        else if (!strncmp(argv[i], "-t", 2))
        {
            if (++i < argc) dt = atoi(argv[i]);
            else
            {
                printUsage(argc, argv);
                return false;
            }
        }
        else if(!strncmp(argv[i], "-e", 2))
        {
            if(++i < argc) trans = atoi(argv[i]);
            else
            {
                cout << "failed to parse translational error argument." << endl;
                return false;
            }
        }
        else if(!strncmp(argv[i], "-s", 2))
        {
            if(++i < argc)
            {
                randInput = argv[i];
            }
            else
            {
                cout << "Failed to parse randInput" << endl;
                return false;
            }
        }
    }

    return true;
}   // parseArguments(GLint, char **, GLint &, GLint &, GLfloat &, GLfloat &)



//
// bool validateParameters(nRobots, fIndex, fRadius, fHeading, dt)
// Last modified: 08Nov2009
//
// Tests the validate if the specified parameters,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nRobots       in/out  the number of robots
//      fIndex        in/out  the initial function index of the formation
//      fRadius       in/out  the initial radius between robots in the formation
//      fHeading      in/out  the initial heading of robots in the formation
//      dt            in/out  the time interval (in ms) between OpenGL updates
//
bool validateParameters(const GLint   nRobots,
                        const GLint   fIndex,
                        const GLfloat fRadius,
                        const GLfloat fHeading,
                        const GLint   dt)
{
    bool valid = true;

    // validate the number of robots
    if (nRobots < 0)
    {
        cout << "Parameter: "
             << "'nRobots' must be non-negative." << endl;
        valid = false;
    }

    // validate the formation function index
    if ((fIndex < 0) || (fIndex > 9))
    {
        cout << "Parameter: "
             << "'fIndex' must be in the range [0, 9]." << endl;
        valid = false;
    }

    // validate the formation radius
    if (fRadius < 0.0f)
    {
        cout << "Parameter: "
             << "'fRadius' must be non-negative." << endl;
        valid = false;
    }

    // validate the time interval
    if (dt < 1)
    {
        cout << "Parameter: "
             << "'dt' must be greater than 1." << endl;
        valid = false;
    }

    return valid;
}   // validateParameters(GLint, GLint, GLfloat, GLfloat)



//
// void terminate(retVal)
// Last modified: 08Nov2009
//
// Terminates the program on interrupt (i.e., ^C).
//
// Returns:     <none>
// Parameters:
//      retVal  in      the exit return code
//
void terminate(int retVal)
{
    signal(SIGINT, SIG_IGN);
    deinitEnv();
    signal(SIGINT, SIG_DFL);
    exit(retVal);
}   // terminate(int)



//
// void displayMenu()
// Last modified: 08Nov2009
//
// Displays the following menu (program description) to the console:
//
// ******************************************
// *                                        *
// * ALGORITHMS FOR CONTROL AND INTERACTION *
// *     OF LARGE FORMATIONS OF ROBOTS      *
// *                                        *
// *   Ross Mead & Dr. Jerry B. Weinberg    *
// *  Southern Illinois Univ. Edwardsville  *
// *                                        *
// ******************************************
//
// Use the 'ARROW' keys to translate and rotate the seed robot.
//
// Use the '+/-' keys to increase/decrease the separation between robots.
//
// Use the '</>' keys to rotate the robot headings relative to the formation.
//
// Use the '0-9' keys to change to a formation seeded at the selected robot.
//
// PRESET FORMATIONS
// -----------------
// 0) f(x) = 0
// 1) f(x) = x
// 2) f(x) = |x|
// 3) f(x) = -0.5 x
// 4) f(x) = -|0.5 x|
// 5) f(x) = -|x|
// 6) f(x) = x^2
// 7) f(x) = x^3
// 8) f(x) = {sqrt(x),  x >= 0 | -sqrt|x|, x < 0}
// 9) f(x) = 0.05 sin(10 x)
//
// Use the mouse to select a robot.
//
// Use 'h|l|p|t' to toggle robot display settings.
//
// Returns:     <none>
// Parameters:  <none>
//
void displayMenu()
{
    cout << "******************************************"      << endl
         << "*                                        *"      << endl
         << "* ALGORITHMS FOR CONTROL AND INTERACTION *"      << endl
         << "*     OF LARGE FORMATIONS OF ROBOTS      *"      << endl
         << "*                                        *"      << endl
         << "*   Ross Mead & Dr. Jerry B. Weinberg    *"      << endl
         << "*  Southern Illinois Univ. Edwardsville  *"      << endl
         << "*                                        *"      << endl
         << "******************************************"      << endl << endl
         << "Use the 'ARROW' keys to "
         << "translate and rotate the seed robot."
         << endl << endl
         << "Use the '+/-' keys to "
         << "increase/decrease the separation between robots."
         << endl << endl
         << "Use the '</>' keys to "
         << "rotate the robot headings relative to the formation."
         << endl << endl
         << "Use the '0-9' keys to "
         << "change to a formation seeded at the selected robot."
         << endl << endl
         << "PRESET FORMATIONS\n-----------------"            << endl
         << "0) f(x) = 0"                                     << endl
         << "1) f(x) = x"                                     << endl
         << "2) f(x) = |x|"                                   << endl
         << "3) f(x) = -0.5 x"                                << endl
         << "4) f(x) = -|0.5 x|"                              << endl
         << "5) f(x) = -|x|"                                  << endl
         << "6) f(x) = x^2"                                   << endl
         << "7) f(x) = x^3"                                   << endl
         << "8) f(x) = {sqrt(x),  x >= 0 | -sqrt|x|, x < 0}"  << endl
         << "9) f(x) = 0.05 sin(10 x)"                        << endl << endl
         << "Use the mouse to select a robot."                << endl << endl
         << "Use 'h|l|p|t' to toggle robot display settings." << endl << endl
         << "Use ESC to exit."                                << endl << endl;
}   // displayMenu()



//
// bool initEnv(const GLint, const GLint)
// Last modified: 08Nov2009
//
// Attempts to initialize the environment based on
// the parameterized values, returning true if successful,
// false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nRobots       in      the number of robots
//      fIndex        in      the index of the initial formation
//
bool initEnv(const GLint nRobots, const GLint fIndex)
{
    if (g_env != NULL)
    {
        delete g_env;
        g_env = NULL;
    }

    Formation f(formations[fIndex], g_fRadius, Vector(),
                g_sID,            ++g_fID,     g_fHeading);
    return (g_env = new Environment(nRobots, f,DEFAULT_ENV_COLOR,g_ins,g_trans,randInput)) != NULL;
}   // initEnv(const GLint, const GLint)



//
// bool deinitEnv()
// Last modified: 08Nov2009
//
// Attempts to deinitialize the environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool deinitEnv()
{
    if (g_env != NULL)
    {
        delete g_env;
        g_env = NULL;
    }
    return g_env == NULL;
}   // deinitEnv()



//
// bool changeFormation(index)
// Last modified: 08Nov2009
//
// Attempts to change the current formation,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      index   in      the index of the formation to change to
//
bool changeFormation(const GLint index)
{
    g_fIndex = index;
    if (!g_env->startFormation)
    {
        g_env->startFormation = true;
    }
    // determine if a new seed has been selected
    if (g_selectedIndex != -1)
    {
        g_env->getCell(g_sID)->setColor(DEFAULT_CELL_COLOR);
        g_sID = g_selectedIndex;
    }
    Formation f(formations[index], g_fRadius,          Vector(),
                g_sID,           ++g_env->formationID, g_fHeading);

    return g_env->changeFormation(f);
}   // changeFormation(const GLint)



//
// void initWindow()
// Last modified:   08Nov2009
//
// Initializes the simulator window.
//
// Returns:     <none>
// Parameters:  <none>
//
void initWindow()
{

    // clear background color
    glClearColor(g_env->color[0], g_env->color[1], g_env->color[2], 0.0f);

    // viewport transformation
    glViewport(0, 0, g_windowSize[0], g_windowSize[1]);

    glMatrixMode(GL_PROJECTION);    // projection transformation
    glLoadIdentity();               // initialize projection identity matrix
}   // initWindow()



//
// void display()
// Last modified:   08Nov2009
//
// Clears the frame buffer and draws the simulated cells within the window.
//
// Returns:     <none>
// Parameters:  <none>
//
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);   // clear background color
    glMatrixMode(GL_MODELVIEW);     // modeling transformation

    // draws environment robot cells
    if (g_env->getCells().size() > 0)
    {
        g_env->getCell(g_sID)->setColor(GREEN);
    }
    g_env->draw();

    glFlush();                      // force the execution of OpenGL commands
    glutSwapBuffers();              // swap visible buffer and writing buffer
}   // display()



//
// void keyboardPress(keyPressed, mouseX, mouseY)
// Last modified:   08Nov2009
//
// Handles the keyboard input (ASCII Characters).
//
// Returns:     <none>
// Parameters:
//      keyPressed  in      the ASCII key that was pressed
//      mouseX      in      the x-coordinate of the mouse position
//      mouseY      in      the y-coordinate of the mouse position
//
void keyboardPress(unsigned char keyPressed, GLint mouseX, GLint mouseY)
{
    if ((keyPressed >= '0') && (keyPressed <= '9'))
    {
        if (g_env->getNCells() > 0)
        {
            char cIndex = keyPressed;
            changeFormation(atoi(&cIndex));
        }
    }
    else switch (keyPressed)
    {

        // change formation heading
        case '<': case ',':
            if (g_env->getNCells() > 0)
            {
                g_fHeading += g_env->getCell(g_sID)->maxAngSpeed();
                changeFormation(g_fIndex);
                g_env->getCell(g_sID)->rotateRelative(
                    g_env->getCell(g_sID)->maxAngSpeed());
                    //min(1.0f, g_env->getCell(g_sID)->maxAngSpeed()));
            }
            break;
        case '>': case '.':
            if (g_env->getNCells() > 0)
            {
                g_fHeading -= g_env->getCell(g_sID)->maxAngSpeed();
                changeFormation(g_fIndex);
                g_env->getCell(g_sID)->rotateRelative(
                    -g_env->getCell(g_sID)->maxAngSpeed());
                    //-min(1.0f, g_env->getCell(g_sID)->maxAngSpeed()));
            }
            break;

        // change formation scale
        case '+': case '=':
            if (g_env->getNCells() > 0)
            {
                g_fRadius += 0.01f;
                changeFormation(g_fIndex);
            }
            break;
        case '-': case '_':
            if (g_env->getNCells() > 0)
            {
                g_fRadius -= 0.01f;
                g_fRadius  = max(g_fRadius,
                                 g_env->getCell(g_sID)->collisionRadius());
                changeFormation(g_fIndex);
            }
            break;

        // show position
        case 'h': case 'H':
            if (g_env->getNCells() > 0)
            {
                g_env->showHeading(!g_env->getCell(g_sID)->showHeading);
            }
            break;
        case 'l': case 'L':
            if (g_env->getNCells() > 0)
            {
                g_env->showLine(!g_env->getCell(g_sID)->heading.showLine);
            }
            break;
        case 'p': case 'P':
            if (g_env->getNCells() > 0)
            {
                g_env->showPos(!g_env->getCell(g_sID)->showPos);
            }
            break;
        case 't': case 'T':
            if (g_env->getNCells() > 0)
            {
                g_env->showHead(!g_env->getCell(g_sID)->heading.showHead);
            }
            break;
        case CHAR_ESCAPE:
            {
                g_env->writeDistanceData("out.out","distances.out");
                g_env->writeHeader();

                g_env->dumpMessagesToFile("messages.out");
                g_env->dumpErrorToFile("errors.out");

                deinitEnv();
                exit(0);
            }
    }
}   // keyboardPress(unsigned char, GLint, GLint)



//
// void keyboardPressSpecial(keyPressed, mouseX, mouseY)
// Last modified:   08Nov2009
//
// Handles the keyboard input (non-ASCII Characters).
//
// Returns:     <none>
// Parameters:
//      keyPressed  in      the non-ASCII key that was pressed
//      mouseX      in      the x-coordinate of the mouse position
//      mouseY      in      the y-coordinate of the mouse position
//
void keyboardPressSpecial(GLint keyPressed, GLint mouseX, GLint mouseY)
{
    switch (keyPressed)
    {
        case GLUT_KEY_LEFT:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->rotError =
                    -(1.0001f * g_env->getCell(g_sID)->angThreshold());
                //g_env->getCell(g_sID)->rotateRelative(
                //    min(1.0f, g_env->getCell(g_sID)->maxAngSpeed()));
            }
            break;
        case GLUT_KEY_UP:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->transError.x =
                    1.0001f * g_env->getCell(g_sID)->threshold();
                //g_env->getCell(g_sID)->translateRelative(
                //    min(0.001f, g_env->getCell(g_sID)->maxSpeed()));
            }
            break;
        case GLUT_KEY_RIGHT:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->rotError =
                    1.0001f * g_env->getCell(g_sID)->angThreshold();
                //g_env->getCell(g_sID)->rotateRelative(
                //    -min(1.0f, g_env->getCell(g_sID)->maxAngSpeed()));
            }
            break;
        case GLUT_KEY_DOWN:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->transError.x =
                    -(1.0001f * g_env->getCell(g_sID)->threshold());
                //g_env->getCell(g_sID)->translateRelative(
                //    -min(0.001f, g_env->getCell(g_sID)->maxSpeed()));
            }
            break;
        default: break;
    }
}   // keyboardPressSpecial(GLint, GLint, GLint)



//
// void keyboardReleaseSpecial(keyPressed, mouseX, mouseY)
// Last modified:   06Nov2009
//
// Handles the keyboard input (non-ASCII Characters).
//
// Returns:     <none>
// Parameters:
//      keyReleased  in      the non-ASCII key that was released
//      mouseX       in      the x-coordinate of the mouse position
//      mouseY       in      the y-coordinate of the mouse position
//
void keyboardReleaseSpecial(GLint keyReleased, GLint mouseX, GLint mouseY)
{
    switch (keyReleased)
    {
        case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->rotError = 0.0f;
            }
            break;
        case GLUT_KEY_UP: case GLUT_KEY_DOWN:
            if (g_env->getNCells() > 0)
            {
                g_env->getCell(g_sID)->transError.x = 0.0f;
            }
            break;
        default: break;
    }
}   // keyboardReleaseSpecial(GLint, GLint, GLint)



//
// void mouseClick(mouseButton, mouseState, mouseX, mouseY)
// Last modified:   08Nov2009
//
// Reacts to mouse clicks.
//
// Returns:     <none>
// Parameters:
//      mouseButton     in      the mouse button that was pressed
//      mouseState      in      the state of the mouse
//      mouseX          in      the x-coordinate of the mouse position
//      mouseY          in      the y-coordinate of the mouse position
//
void mouseClick(GLint mouseButton,    GLint mouseState,
                GLint mouseX, GLint mouseY)
{
	GLint mod = glutGetModifiers();
    if (mouseState == GLUT_DOWN)
    {
		if(mod == GLUT_ACTIVE_CTRL)
		{
			if(g_env->getCells().size() > 0)
			{
				for (GLint i = 0; i < g_env->getNCells(); ++i)
				{
					GLfloat x     = g_windowWidth * mouseX / g_windowSize[0] -
							0.5 * g_windowWidth;
					GLfloat y     = 0.5 * g_windowHeight -
							(g_windowHeight * mouseY / g_windowSize[1]);
					g_selectedIndex = g_sID;
					GLfloat dx = g_env->getCell(i)->x - x,
							dy = g_env->getCell(i)->y - y;
					if ((g_selectedIndex == g_sID) &&
						(sqrt(dx * dx  + dy * dy) < SELECT_RADIUS))
					{
						g_env->removeCell(g_env->getCell(g_selectedIndex = i));

					}else if (i != g_sID){
						g_env->getCell(i)->setColor(DEFAULT_CELL_COLOR);
					}
				}
			}
			if(g_env->getRobots().size()>0)
			{
			    GLint ii=-1;
				for (GLint i = 0; i < g_env->getNFreeRobots(); ++i)
				{
					GLfloat x     = g_windowWidth * mouseX / g_windowSize[0] -
							0.5 * g_windowWidth;
					GLfloat y     = 0.5 * g_windowHeight -
							(g_windowHeight * mouseY / g_windowSize[1]);
					g_selectedIndex = -1;
					GLfloat dx = g_env->getRobot(ii)->x - x,
							dy = g_env->getRobot(ii)->y - y;
                    GLfloat distance = sqrt(dx * dx  + dy * dy);
                    cout << "Distance between click and robotID " << ii << " is " << distance << endl;
					if (distance < SELECT_RADIUS)
					{
                        printf("IS WITHIN RADIUS");
                        cout << "Remove robotID = " << ii << endl;
						g_env->removeRobot(g_env->getRobot(g_selectedIndex = ii));
						//g_env->getCell(i)->setColor(DEFAULT_CELL_COLOR);
						//cout << "Remove robotID = " << ii << endl;
						//g_env->removeRobot(g_env->getRobot(g_selectedIndex = ii));
					}
					ii--;
				}
			}
		}
		if(g_env->getCells().size()>0)
		{
			GLfloat x     = g_windowWidth * mouseX / g_windowSize[0] -
							0.5 * g_windowWidth;
			GLfloat y     = 0.5 * g_windowHeight -
							(g_windowHeight * mouseY / g_windowSize[1]);
			g_selectedIndex = g_sID;
			for (GLint i = 0; i < g_env->getNCells(); ++i)
			{
				GLfloat dx = g_env->getCell(i)->x - x,
						dy = g_env->getCell(i)->y - y;
				if ((g_selectedIndex == g_sID) &&
					(sqrt(dx * dx  + dy * dy) < SELECT_RADIUS))
						g_env->getCell(g_selectedIndex = i)->setColor(RED);
					else if (i != g_sID)
						g_env->getCell(i)->setColor(DEFAULT_CELL_COLOR);
			}
		}else{
			GLfloat x     = g_windowWidth * mouseX / g_windowSize[0] -
							0.5 * g_windowWidth;
			GLfloat y     = 0.5 * g_windowHeight -
							(g_windowHeight * mouseY / g_windowSize[1]);
			g_env->formFromClick(x,y);
		}
    }
    glutPostRedisplay();            // redraw the scene
}   // mouseClick(GLint, GLint, GLint, GLint)

//
// void mouseDrag(mouseX, mouseY)
// Last modified:   08Nov2009
//
// Moves the position of a nearby cell to the
// current mouse pointer position if the mouse
// button is pressed and the pointer is in motion.
//
// Returns:     <none>
// Parameters:
//      mouseX  in      the x-coordinate of the mouse position
//      mouseY  in      the y-coordinate of the mouse position
//
void mouseDrag(GLint mouseX, GLint mouseY)
{
    if (g_env->getNCells() > 0)
    {
        if (g_selectedIndex != ID_NO_NBR)
        {
            g_env->getCell(g_selectedIndex)->
            set(g_windowWidth * mouseX / g_windowSize[0] - 0.5 *
                g_windowWidth, 0.5 * g_windowHeight - (g_windowHeight *
                mouseY / g_windowSize[1]));
        }
    }
    glutPostRedisplay();    // redraw the scene
}   // mouseDrag(GLint, GLint)



//
// void resizeWindow(w, h)
// Last modified:   08Nov2009
//
// Scales the rendered scene according to the window dimensions,
// setting the global variables so the mouse operations will
// correspond to mouse pointer positions.
//
// Returns:     <none>
// Parameters:
//      w       in      the new screen width
//      h       in      the new screen height
//
void resizeWindow(GLsizei w, GLsizei h)
{
    g_windowSize[0] = w;            // obtain new screen width
    g_windowSize[1] = h;            // obtain new screen height

    glClear(GL_COLOR_BUFFER_BIT);   // clear color buffer to draw next frame
    glViewport(0, 0, w, h);         // viewport transformation

    glMatrixMode(GL_PROJECTION);    // projection transformation
    glLoadIdentity();               // initialize projection identity matrix

    if (w <= h)
    {
        g_windowWidth  = 2.0f;
        g_windowHeight = 2.0f * (GLfloat)h / (GLfloat)w;
        glOrtho(-1.0f, 1.0f, -1.0f * (GLfloat)h / (GLfloat)w,
                (GLfloat)h / (GLfloat)w, -10.0f, 10.0f);
    }
    else
    {
        g_windowWidth  = 2.0f * (GLfloat)w / (GLfloat)h;
        g_windowHeight = 2.0f;
        glOrtho(-1.0f * (GLfloat)w / (GLfloat)h,
                (GLfloat)w / (GLfloat)h,
                -1.0f, 1.0f, -10.0f, 10.0f);
    }
    glutPostRedisplay();            // redraw the scene
}   // resizeWindow(GLsizei, GLsizei)



//
// void timerFunction(value)
// Last modified:   08Nov2009
//
// Updates the environment and redraws.
//
// Returns:     <none>
// Parameters:
//      value   in      the value of the timer
//
void timerFunction(GLint value)
{
    if(!g_env->step())
    {
        g_env->writeFinalPositions();
        g_env->writeDistanceData("out.out","distances.out");
        g_env->writeHeader();

        g_env->dumpMessagesToFile("messages.out");
        g_env->dumpErrorToFile("errors.out");
        deinitEnv();
        exit(0);
    }          // update the robot cell environment

    // force a redraw after a number of milliseconds
    glutPostRedisplay();    // redraw the scene
    glutTimerFunc(g_dt, timerFunction, 1);
}   // timerFunction(GLint)



// <test formation functions>

//
// GLfloat line(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = 0.
//
// Returns:     f(x) = 0
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat line(const GLfloat x)
{
    return 0.0f;
}   // line(const GLfloat)



//
// GLfloat x(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x.
//
// Returns:     f(x) = x
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat x(const GLfloat x)
{
    return x;
}   // x(const GLfloat)



//
// GLfloat absX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = |x|.
//
// Returns:     f(x) = |x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat absX(const GLfloat x)
{
    return abs(x);
}   // absX(const GLfloat)



//
// GLfloat negHalfX(x)
// Last modified:   07Jan2007
//
// Returns formation function definition f(x) = -0.5 x.
//
// Returns:     f(x) = -0.5 x
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negHalfX(const GLfloat x)
{
    return -0.5f * x;
}   // negHalfX(const GLfloat)



//
// GLfloat negAbsHalfX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -|0.5 x|.
//
// Returns:     f(x) = -|0.5 x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negAbsHalfX(const GLfloat x)
{
    return -abs(0.5f * x);
}   // negAbsHalfX(const GLfloat)



//
// GLfloat negAbsX(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -|x|.
//
// Returns:     f(x) = -|x|
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negAbsX(const GLfloat x)
{
    return -abs(x);
}   // negAbsX(const GLfloat)



//
// GLfloat parabola(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x^2.
//
// Returns:     f(x) = x^2
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat parabola(const GLfloat x)
{
	return x * x;
    //return pow(x, 2.0f);
}   // parabola(const GLfloat)



//
// GLfloat cubic(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x^3.
//
// Returns:     f(x) = x^3
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat cubic(const GLfloat x)
{
	return x * x * x;
    //return pow(x, 3.0f);
}   // cubic(const GLfloat)



//
// GLfloat condSqrt(x)
// Last modified:   04Sep2006
//
// Returns formation function definition
// f(x) = {sqrt(x),  x = 0 | -sqrt|x|, x < 0}.
//
// Returns:     f(x) = {sqrt(x),  x = 0 | -sqrt|x|, x < 0}
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat condSqrt(const GLfloat x)
{
    return sqrt(abs(0.5f * x)) * ((x >= 0) ? 1.0f : -1.0f);
}   // condSqrt(const GLfloat)



//
// GLfloat sine(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = 0.05 sin(10 x).
//
// Returns:     f(x) = 0.05 sin(10 x)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat sine(const GLfloat x)
{
    //0.1 is max amplitude
    return 0.1f * sin(10.0f * x);
}   // sine(const GLfloat)



//
// GLfloat xRoot3(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = x sqrt(3).
//
// Returns:     f(x) = x sqrt(3)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat xRoot3(const GLfloat x)
{
    return x * sqrt(3.0f);
}   // xRoot3(const GLfloat)



//
// GLfloat negXRoot3(x)
// Last modified:   04Sep2006
//
// Returns formation function definition f(x) = -x sqrt(3).
//
// Returns:     f(x) = -x sqrt(3)
// Parameters:
//      x       in      used to evaluate the function
//
GLfloat negXRoot3(const GLfloat x)
{
    return -x * sqrt(3.0f);
}   // negXRoot3(const GLfloat)
