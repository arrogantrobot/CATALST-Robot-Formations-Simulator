//
// Filename:        "Environment.cpp"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class implements a robot cell environment.
//

// preprocessor directives
#include "Environment.h"



// <constructors>

//
// Environment(n, f, colorIndex)
// Last modified: 08Nov2009
//
// Default constructor that initializes
// this environment to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      n           in      the initial number of cells
//      f           in      the initial formation
//      colorIndex  in      the initial array index of the color of the cells
//
Environment::Environment(const GLint     n,
                         const Formation f,
                         const Color     colorIndex,
                         const int       insert)
{
    if (!init(n, f, colorIndex, insert)) clear();
}   // Environment(const GLint, const Formation, const Color)



//
// Environment(e)
// Last modified: 27Aug2006
//
// Copy constructor that copies the contents of
// the parameterized environment into this environment.
//
// Returns:     <none>
// Parameters:
//      e       in/out      the environment being copied
//
Environment::Environment(const Environment &e)
    : cells(e.cells), msgQueue(e.msgQueue)
{
}   // Environment(const Environment &)



// <destructors>

//
// ~Environment()
// Last modified: 27Aug2006
//
// Destructor that clears this environment.
//
// Returns:     <none>
// Parameters:  <none>
//
Environment::~Environment()
{
    clear();
}   // ~Environment()



// <virtual public mutator functions>

//
// bool setColor(r, g, b)
// Last modified: 03Sep2006
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
bool Environment::setColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
    color[GLUT_RED]   = r;
    color[GLUT_GREEN] = g;
    color[GLUT_BLUE]  = b;
    return true;
}   // setColor(const GLfloat, const GLfloat, const GLfloat)



//
// bool setColor(clr)
// Last modified: 03Sep2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      clr     in/out  the color to be set to
//
bool Environment::setColor(const GLfloat clr[3])
{
    return setColor(clr[GLUT_RED], clr[GLUT_GREEN], clr[GLUT_BLUE]);
}   // setColor(const GLfloat [])



//
// bool setColor(colorIndex)
// Last modified: 27Aug2006
//
// Attempts to set the color to the parameterized RGB color values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      colorIndex  in/out  the index of the color to be set to
//
bool Environment::setColor(const Color colorIndex)
{
    return setColor(COLOR[(GLint)colorIndex]);
}   // setColor(const Color)



// <public mutator functions>

//
// bool addCell(c)
// Last modified: 27Aug2006
//
// Attempts to add a cell to the environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      c       in      the cell being added
//
bool Environment::addCell(Cell *c)
{
	if(c==NULL)
	{
		if ((c == NULL) && ((c = new Cell()) == NULL)) return false;
	}

    bool done = true;
    c->setEnvironment(this);

    // attempt to add this cell to the cell list
	cells.push_back(c);
    return true;
}   // addCell(Cell *)



//
// bool removeCell()
// Last modified: 27Aug2006
//
// Attempts to remove a cell from the environment,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Environment::removeCell()
{
    Cell *c = cells[cells.size()-1];
    if (!removeCell(c)) return false;
    delete c;
    return true;
}   // removeCell()



//
// bool removeCell(c)
// Last modified: 27Aug2006
//
// Attempts to remove a cell from the environment,
// storing the address of the removed cell and
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      c       in/out  set to the cell being removed
//
bool Environment::removeCell(Cell* c)
{
	if(c==NULL)
	{
		return false;
	}
	bool answer = false;
	for(GLint i=0;i< cells.size();i++)
	{
		if(c==cells[i])
		{
			cells.erase(cells.begin()+i);
			answer = true;
			break;
		}
	}
	return answer;
}   // removeCell(Cell* &)



// <public accessor functions>

//
// Cell* getCell() const
// Last modified: 27Aug2006
//
// Returns the cell at the parameterized position.
//
// Returns:     the cell at the parameterized position
// Parameters:  <none>
//
Cell* Environment::getCell(GLint pos) const
{
	Cell *c = NULL;
	for(GLint i=0;i< cells.size();i++)
	{
		if(pos==cells[i]->getID())
		{
			c = cells[i];
			break;
		}
	}
	return c;
}   // getCell(GLint) const



//
// vector<Cell *> getCells()
// Last modified: 27Aug2006
//
// Returns all of the cells in the environment.
//
// Returns:     all of the cells in the environment
// Parameters:  <none>
//
vector<Cell *> Environment::getCells()
{
    return cells;
}   // getCells()



//
// vector<Robot *> getRobots()
// Last modified: 08Nov2009
//
// Returns all of the [free] robots in the environment.
//
// Returns:     all of the [free] robots in the environment
// Parameters:  <none>
//
vector<Robot *> Environment::getRobots()
{
    return robots;
}   // getRobots()



//
// GLint getNCells() const
// Last modified: 27Aug2006
//
// Returns the number of cells in the environment.
//
// Returns:     the number of cells in the environment
// Parameters:  <none>
//
GLint Environment::getNCells() const
{
    return cells.size();
}   // getNCells() const



//
// GLint getNFreeRobots() const
// Last modified: 08Nov2009
//
// Returns the number of [free] robots in the environment.
//
// Returns:     the number of [free] robots in the environment
// Parameters:  <none>
//
GLint Environment::getNFreeRobots() const
{
    return robots.size();
}   // getNFreeRobots() const



// <virtual public utility functions>

//
// void draw()
// Last modified: 08Nov2009
//
// Renders the environment.
//
// Returns:     <none>
// Parameters:  <none>
//
void Environment::draw()
{

    // draw cells
    if (cells.size() > 0)
    {
    	getCell(formation.getSeedID())->setColor(GREEN);
    }
    for (GLint i = 0; i < getNCells(); ++i)
    {
    	cells[i]->draw();
    }

    // draw [free] robots
    Robot *currRobot = NULL;
    for (GLint i = 0; i < robots.size(); ++i)
    {
		robots[i]->draw();
    }
}   // draw()



//
// void step()
// Last modified: 27Aug2006
//
// Executes the next step in each cell in the environment
// and forwards all sent packets to their destinations.
//
// Returns:     <none>
// Parameters:  <none>
//
bool Environment::step()
{
    Cell *currCell = NULL;

	Robot *r = NULL;
	if(insertion) {
	    vector<Robot*> auctionCalls;
	    Robot *auctionCall = NULL;
        for (GLint i = 0; i < robots.size(); ++i)
        {
            auctionCall = robots[i]->auctioningStep();
            if((auctionCall != NULL)&&(startFormation))
            {
                auctionCalls.push_back(auctionCall);
            }
        }
        if(startFormation)
        {
            for(int i=0; i<auctionCalls.size();i++)
            {
                Robot* a = auctionCalls[i];
                Formation f = formation;
                bool dir;
                //if (a->rightNbr == NULL)
                //{
                //    dir = true;
                //}else
                //{
                //    dir = false;
                //}
                Insertion_Auction_Announcement* aa = new Insertion_Auction_Announcement(a->getID());
                sendMsg((Message)aa, ID_BROADCAST, a->getID(), INSERTION_AUCTION_ANNOUNCEMENT);
                a->setAuctionStepCount(1);
            }

            for(int i=0;i<cells.size();i++)
            {
                cells[i]->processPackets();
                cells[i]->cStep();
            }
            forwardPackets();
            auctionCalls.clear();
            for(int i=0; i<robots.size(); i++)
            {
                if(robots[i]->getAuctionStepCount() >= AUCTION_STEP_COUNT)
                {
                    robots[i]->settleAuction();
                }
            }
        }
	} else {
        vector<Cell*> auctionCalls;
        Cell *auctionCall = NULL;
	    for (GLint i = 0; i < getNCells(); ++i)
        {
            auctionCall = cells[i]->cStep();
            if((auctionCall != NULL)&&(startFormation))
            {
                auctionCalls.push_back(auctionCall);
            }
        }
        if(startFormation)
        {
            for(int i=0; i<auctionCalls.size();i++)
            {
                Cell* a = auctionCalls[i];
                State s = a->getState() ;
                Formation f = formation;
                bool dir;
                if (a->rightNbr == NULL)
                {
                    dir = true;
                }else
                {
                    dir = false;
                }
                Push_Auction_Announcement* aa = new Push_Auction_Announcement(
                                         a->getState().gradient, s, dir);
                sendMsg((Message)aa, ID_BROADCAST, a->getID(), PUSH_AUCTION_ANNOUNCEMENT);
                a->setAuctionStepCount(1);
            }

            for(int i=0;i<robots.size();i++)
            {
                robots[i]->processPackets();
                robots[i]->step();
            }
            forwardPackets();
            auctionCalls.clear();
            for(int i=0; i<getNCells(); i++)
            {
                if(cells[i]->getAuctionStepCount() >= AUCTION_STEP_COUNT)
                {
                    cells[i]->settleAuction();
                }
            }
        }
	}
    return true;
}   // step()



//
// void clear()
// Last modified: 27Aug2006
//
// Clears this environment.
//
// Returns:     <none>
// Parameters:  <none>
//
void Environment::clear()
{
    while (removeCell());
}   // clear()



// <public utility functions>

//
// Vector getRelationship(toID, fromID)
// Last modified: 27Aug2006
//
// Returns the relationship between the two cells
// with the parameterized ID's.
//
// Returns:     the relationship between two cells
// Parameters:
//      toID    in      the ID of the cell being related to
//      fromID  in      the ID of the cell being related from
//
Vector Environment::getRelationship(const GLint toID, const GLint fromID)
{
    Cell  *toCell = getCell(toID), *fromCell = getCell(fromID);
    if ((toCell == NULL) || (fromCell == NULL)) return Vector();
    Vector temp   = *toCell - *fromCell;
    temp.rotateRelative(-fromCell->getHeading());
    return temp;
}   // getRelationship(const GLint, const GLint)



//
// bool sendMsg(msg, toID, fromID, type)
// Last modified: 07Sep2006
//
// Attempts to send a packet to its destination
// based upon the given parameters, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      msg     in/out  the message being sent
//      toID    in      the ID of the cell receiving the packet
//      fromID  in      the ID of the cell sending the packet
//      type    in      the type of message being sent
//
//removed const from parameters
bool Environment::sendMsg(const Message &msg,
                          const GLint    toID,
                          const GLint    fromID,
                          const GLint    type)
{
	Message msg_c = msg;
	bool answer = sendPacket(Packet(msg_c, toID, fromID, type));
	//if(VERBOSE)printf("Received sendPacket answer\n");
	return answer;
}   // sendMsg(const Message &, const GLint, const GLint, const GLint)



//
// bool sendPacket(p)
// Last modified: 07Sep2006
//
// Attempts to send a packet to its destination,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out  the packet being sent
//

//removed const
bool Environment::sendPacket(const Packet &p)
{

    // discrete message passing
    //if (msgQueue.enqueue(p)) return true;

    // continuous message passing
	//if(p.type==AUCTION_ANNOUNCEMENT)printf("auctionannouncement in sendPacket()\n");
    if (forwardPacket(p)) return true;
	//if(VERBOSE)printf("just before delete p.msg;\n");
	//(cast as message type -- delete (state *)
	if(p.msg != NULL)
	{
		if(p.type==STATE)
		{
			//if(VERBOSE) printf("attempting to delete STATE message\n");
			delete (State *)p.msg;
			//if(VERBOSE) printf("successfully deleted STATE message\n");
		}else if(p.type==CHANGE_FORMATION)
		{
			//if(VERBOSE) printf("attempting to delete CHANGE_FORMATION message\n");
			delete (Formation *)p.msg;
		}
	}
	//p.msg = NULL;
	if(VERBOSE) printf("finished sendPacket()\n");
    return false;
}   // sendPacket(const Packet &)

//
// bool forwardPacket(p)
// Last modified: 29Nov2009
//
// Attempts to forward a packet to its destination,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out  the packet being forwarded
//

//remove const
bool Environment::forwardPacket(const Packet &p)
{
	Cell *c;
	if(!p.fromBroadcast())
	{
		c = getCell(p.toID);
	}
	int to = p.toID;
    if (c != NULL)
    {
        c->msgQueue.push(p);
        return true;
    }
	//if (c == NULL) printf("CELL is NULL!\n");
	//if p.msg != NULL
	//if(p.msg != NULL) delete p.msg;
	if(p.msg != NULL)
	{
		if(p.type==STATE)
		{
			delete (State *)p.msg;
		}else if(p.type==CHANGE_FORMATION)
		{
			delete (Formation *)p.msg;
		}else if(p.type == PUSH_AUCTION_ANNOUNCEMENT)
		{
			Robot* r;
			for(int i=0;i<robots.size();i++)
			{
				robots[i]->msgQueue.push(p);
			}
		}
	}
	if(VERBOSE) printf("finished forwarding Packet to %d\n",to);
    return false;
}   // forwardPacket(const Packet &)



//
// bool forwardPackets()
// Last modified: 29Nov2009
//
// Attempts to forward all packets to their destinations,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Environment::forwardPackets()
{
    Packet p;
    while (!msgQueue.empty())
    {
        p = msgQueue.front();
        msgQueue.pop();
        if (!forwardPacket(p)) return false;
    }
    return true;
}   // forwardPackets()



// <public utility cell functions>

//
// bool showLine(show)
// Last modified: 28Aug2006
//
// Attempts to display the line of the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the line
//
bool Environment::showLine(const bool show)
{
    for (GLint i = 0; i < getNCells(); ++i)
    {
    	cells[i]->heading.showLine = show;
    }
    return true;
}   // showPos(const bool)



//
// bool showHead(show)
// Last modified: 28Aug2006
//
// Attempts to display the head of the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the head
//
bool Environment::showHead(const bool show)
{
    for (GLint i = 0; i < getNCells(); ++i)
    {
    	cells[i]->heading.showHead = show;
    }
    return true;
}   // showHeading(const bool)



//
// bool showPos(show)
// Last modified: 28Aug2006
//
// Attempts to display the position vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the position vector
//
bool Environment::showPos(const bool show)
{
    for (GLint i = 0; i < getNCells(); ++i)
    {
    	cells[i]->showPos = show;
    }
    return true;
}   // showPos(const bool)



//
// bool showHeading(show)
// Last modified: 28Aug2006
//
// Attempts to display the heading vector of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      show    in      display flag for showing the heading vector
//
bool Environment::showHeading(const bool show)
{
    for (GLint i = 0; i < getNCells(); ++i)
    {
    	cells[i]->showHeading = show;
    }
    return true;
}   // showHeading(const bool)



// <virtual protected utility functions>

//
// bool init(n, f, colorIndex)
// Last modified: 07Nov2009
//
// Initializes the environment to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n           in      the initial number of robots
//      f           in      the initial formation
//      colorIndex  in      the initial array index of the color of the cells
//
bool Environment::init(const GLint     n,
                       const Formation f,
                       const Color     colorIndex,
                       const int       insert)
{
    srand(time(NULL));

    nRobots      = n;
    formation    = f;
    formation.setFormationID(0);
    formationID  = 0;
    defaultColor = colorIndex;
    insertion = insert;

    bool result = true;
    startFormation = false;
    //initCells(n, f);
    //system("PAUSE");
    initRobots();

    if (VERBOSE) printf("finished initCells()\n");
    return result && setColor(colorIndex);
}   // init(const GLint, const Formation, const Color)

bool Environment::initRobots()
{
    for (GLint i = 0; i < nRobots; ++i)
        addRobot(randSign() * frand(),
                 randSign() * frand(),
                 0.0f,
                 randSign() * frand(0.0f, 180.0f));
    return true;
}

bool Environment::addRobot(GLfloat x, GLfloat y, GLfloat z, GLfloat theta)
{
    if (VERBOSE)
        printf("new Robot(x = %.2f, y = %.2f, z = %.2f, theta = %.2f)\n",
               x, y, z, theta);
    Robot *r = new Robot(x, y, z, theta, DEFAULT_ROBOT_COLOR);
    r->setEnvironment(this);
    robots.push_back(r);
    return true;
}

//
// bool initCells(n, f)
// Last modified: 28Aug2006
//
// Initializes each cell to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      n           in      the initial number of cells
//      f           in      the initial formation
//
/*bool Environment::initCells(const GLint n, const Formation f)
{
    srand(time(NULL));
    for (GLint i = 0; i < n; ++i) if (!addCell()) return false;

    // initialize each robot's neighborhood
    if (!initNbrs()) return false;

    // organizes the cells into an initial formation (default line)
    Cell *c  = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
    {
        if (!cells.getHead(c)) return false;
        c->x = formation.getRadius() *
               ((GLfloat)i - (GLfloat)(getNCells() - 1) / 2.0f);
        c->y = 0.0f;
        c->setColor(DEFAULT_ROBOT_COLOR);
        c->setHeading(formation.getHeading());
        ++cells;
		if(VERBOSE) printf("iterating through cells...\n");
    }
	newestCell = c;
	//printf("newestCell == %d\n",c->getID());
    return (getNCells() == n) &&
           sendMsg(new Formation(formation), formation.getSeedID(),
                   ID_OPERATOR,      CHANGE_FORMATION);
}*/   // initCells(const GLint, const Formation f)



//
// bool initNbrs(nNbrs)
// Last modified: 03Sep2006
//
// Initializes the neighborhood of each cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nNbrs       in      the initial number of neighbors
//
/*bool Environment::initNbrs(const GLint nNbrs)
{
    Cell *c = NULL;
    for (GLint i = 0; i < getNCells(); ++i)
    {
        if (!cells.getHead(c))                            return false;
        c->clearNbrs();
        if ((i > 0)               && (!c->addNbr(i - 1))) return false;
        else c->leftNbr  = c->nbrWithID(i - 1);
        if ((i < getNCells() - 1) && (!c->addNbr(i + 1))) return false;
        else c->rightNbr = c->nbrWithID(i + 1);
        ++cells;
    }
	if(VERBOSE) printf("finished initNbrs()\n");
    return true;
} */  // initNbrs(const GLint)

Robot * Environment::getNearestRobot(Cell *c)
{
	Robot *r,*rr;
	GLfloat minDistance = 999999.9, distance;
	distance = minDistance;
	for (GLint i = 0; i < getNFreeRobots(); ++i)
    {
		distance = distanceToRobot(c,robots[i]);
        if(minDistance> distance)
		{
			r = robots[i];
			minDistance = distance;
		}
    }
	//printf("minDistance = %f\n",minDistance);
	return r;
}

Robot * Environment::getNearestRobot(GLfloat x, GLfloat y)
{
	Robot *r;
	GLfloat minDistance = 999999.9, distance;
	distance = minDistance;
	for (GLint i = 0; i < getNFreeRobots(); ++i)
    {
		distance = distanceToRobot(x,y,robots[i]);
        if(minDistance> distance)
		{
			r = robots[i];
			minDistance = distance;
		}
    }
	//printf("minDistance = %f\n",minDistance);
	return r;
}


GLfloat Environment::distanceToRobot(Cell *c,Robot *r)
{
	GLfloat x=0,y=0;
	x = fabs(r->x - c->x);
	y = fabs(r->y - c->y);
	return sqrt(pow(x,2)+pow(y,2));
}

GLfloat Environment::distanceToRobot(GLfloat xx,GLfloat yy, Robot* r)
{
	GLfloat x=0,y=0;
	x = fabs(r->x - xx);
	y = fabs(r->y - yy);
	return sqrt(pow(x,2)+pow(y,2));
}

/*bool Environment::auctionPosition(Cell * a)
{
	if(robots.getSize()>0)
	{
		Cell* c;
		c = new Cell();
		addCell(c);
		Robot *r = getNearestRobot(a);
		Robot *rr;
		c->x = r->x;
		c->y = r->y;
		for(GLint ii=0;ii< robots.getSize();ii++)
		{
			robots.getHead(rr);
			if(r==rr)
			{
				robots.removeHead();
				break;
			}else{
				++robots;
			}
		}
		c->setColor(255,0,0);
		c->clearNbrs();
		c->addNbr(a->getID());
		a->addNbr(c->getID());

		if(a->rightNbr == NULL)
    {
      c->leftNbr  = c->nbrWithID(a->getID());
      a->rightNbr = a->nbrWithID(c->getID());
      newestCell  = c;
    }
    else if(a->leftNbr == NULL)
    {
      c->rightNbr = c->nbrWithID(a->getID());
      a->leftNbr  = a->nbrWithID(c->getID());
      newestCell  = c;
    }
		formation.setFormationID(++formationID);
		sendMsg(new Formation(formation),
            formation.getSeedID(),
            ID_OPERATOR,
            CHANGE_FORMATION);
	}
	return true;
}*/

void Environment::formUp()
{

}

//get the nearest robot, turn it into a cell, and start the formation
void Environment::formFromClick(GLfloat x, GLfloat y)
{
	addCell();
    Cell *c  = cells[0];

	Robot* r = getNearestRobot(x,y);
	Robot* rr;

	c->x = r->x;
	c->y = r->y;
	for(GLint ii=0;ii< robots.size();ii++)
	{
		if(r==robots[ii])
		{
			robots.erase(robots.begin()+ii);
			break;
		}
	}
	//c->setColor(MAGENTA);
	c->setHeading(formation.getHeading());
	newestCell = c;
    sendMsg(new Formation(formation), formation.getSeedID(),
                 ID_OPERATOR,      CHANGE_FORMATION);
}

//used by Simulator object to pass user formation change commands to the formation
bool Environment::changeFormation(Formation &f)
{
	formation = f;
	return sendMsg(&formation, formation.getSeedID(), ID_OPERATOR, CHANGE_FORMATION);
}

//return a pointer to the robot with the matching id
Robot* Environment::getRobot(GLint id)
{
	Robot* r,*rr;
	GLint bID=id;
	for(int i=0;i<robots.size();i++)
	{
		if(robots[i]->getID()== bID)
		{
			rr = robots[i];
		}
	}
	return rr;
}

void Environment::settlePushAuction(Cell* a,GLint bID)
{
	//cout << "entering env->settleAuction()" << endl;

	if(robots.size()>0)
	{
		Cell* c;
		c = new Cell();
		if(!addCell(c))
		{
			cout << "addCell() failed!" << endl;
			system("PAUSE");
		}
		Robot *r = getRobot(bID);
    if (r == NULL)
    {
      cout << ">> ERROR: Robot[" << bID << "] not found!\n\n";
      return;
    }
		//cout <<"Robot should be "<< bID << " but env->settleAuction() is # "<< r->getID()<<endl;
		//Robot *rr;
		//a->setColor(255,0,255);
		//r->setColor(255,0,255);
		//a->draw();
		//r->draw();
		//system("PAUSE");
		c->x = r->x;
		c->y = r->y;
		for(GLint ii=0;ii< robots.size();ii++)
		{
			if(r->getID()==robots[ii]->getID())
			{
				robots.erase(robots.begin()+ii);
				break;
			}
		}
		//c->setColor(MAGENTA);
		c->clearNbrs();
    c->leftNbr = c->rightNbr = NULL;
		c->addNbr(a->getID());
		a->addNbr(c->getID());

    if(a->rightNbr == NULL)
    {
      c->leftNbr  = c->nbrWithID(a->getID());
      a->rightNbr = a->nbrWithID(c->getID());
      //cout << "a->rightNbr = " << a->rightNbr->ID << endl;
      newestCell  = c;
    }
    else if(a->leftNbr == NULL)
    {
      c->rightNbr = c->nbrWithID(a->getID());
      a->leftNbr  = a->nbrWithID(c->getID());
      //cout << "a->leftNbr = " << a->leftNbr->ID << endl;
      newestCell  = c;
    }
		formation.setFormationID(++formationID);
		sendMsg(new Formation(formation),
            formation.getSeedID(),
            ID_OPERATOR,
            CHANGE_FORMATION);
		//getCell(formation.getSeedID())->sendStateToNbrs();
		//system("PAUSE");
	}
}

void Environment::settleInsertionAuction(Robot* a,GLint bID)
{
	cout << "entering env->settleInsertionAuction()" << endl;
    /*
	if(robots.size()>0)
	{
		Cell* c;
		c = new Cell();
		if(!addCell(c))
		{
			cout << "addCell() failed!" << endl;
			system("PAUSE");
		}
		Cell *n = getCell(bID);
        if (n == NULL)
        {
          cout << ">> ERROR: Robot[" << bID << "] not found!\n\n";
          return;
        }
		//cout <<"Robot should be "<< bID << " but env->settleAuction() is # "<< r->getID()<<endl;
		//Robot *rr;
		//a->setColor(255,0,255);
		//r->setColor(255,0,255);
		//a->draw();
		//r->draw();
		//system("PAUSE");
		c->x = r->x;
		c->y = r->y;
		for(GLint ii=0;ii< robots.size();ii++)
		{
			if(r->getID()==robots[ii]->getID())
			{
				robots.erase(robots.begin()+ii);
				break;
			}
		}
		//c->setColor(MAGENTA);
		c->clearNbrs();
        c->leftNbr = c->rightNbr = NULL;
		c->addNbr(a->getID());
		a->addNbr(c->getID());

    if(a->rightNbr == NULL)
    {
      c->leftNbr  = c->nbrWithID(a->getID());
      a->rightNbr = a->nbrWithID(c->getID());
      //cout << "a->rightNbr = " << a->rightNbr->ID << endl;
      newestCell  = c;
    }
    else if(a->leftNbr == NULL)
    {
      c->rightNbr = c->nbrWithID(a->getID());
      a->leftNbr  = a->nbrWithID(c->getID());
      //cout << "a->leftNbr = " << a->leftNbr->ID << endl;
      newestCell  = c;
    }
		formation.setFormationID(++formationID);
		sendMsg(new Formation(formation),
            formation.getSeedID(),
            ID_OPERATOR,
            CHANGE_FORMATION);
		//getCell(formation.getSeedID())->sendStateToNbrs();
		//system("PAUSE");
	}*/
}



// remove a robot form the environment
bool Environment::removeRobot(Robot *r)
{
	for(GLint i=0;i< robots.size();i++)
	{
		if(r==robots[i])
		{
			robots.erase(robots.begin()+i);
			break;
		}
	}
    return true;
}   // removeRobot(Robot *)



//
vector<Cell *> Environment::getCellVector()
{
    return cells;
}   // getCellVector()



//
vector<Robot *> Environment::getRobotVector()
{
    return robots;
}   // getRobotVector()

void Environment::writeDistanceData(char * filename)
{
    ofstream distanceOut;
    distanceOut.open(filename);

    for(int i=0;i<cells.size();i++)
    {
        distanceOut << cells[i]->getID() <<", "<<cells[i]->getDistanceTraveled() << endl;
        cout << cells[i]->getID() <<", "<<cells[i]->getDistanceTraveled() << endl;
    }
    distanceOut.close();
}

bool Environment::useInsertion()
{
    if(insertion) return 1;
    else return 0;
}
