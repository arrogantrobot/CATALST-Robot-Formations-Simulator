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
                         const int       insert,
                         const float     max_t_error,
                         string          randInput)
{
    //string s = randInput;
    inputFile = randInput;
    if (!init(n, f, colorIndex, insert,max_t_error)) clear();
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
    c->getState().formation.setFormationID(formation.getFormationID());
    cout << " new cell " << c->getID() << " has a formation ID = " << c->getState().formation.getFormationID() << endl;
    c->insertion = insertion;
    c->max_trans_error = max_trans_error;

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
    //cout << "entering env->step()" << endl;
    if(stepCount==0)
    {
        formFromClick(0.0001,0.0001);
    }

    Cell *currCell = NULL;

	Robot *r = NULL;
    //displayStateOfEnv();
    //cout << "finished calling displayStateOfEnv()"<<endl;
	if(robots.size()==0)
	{
	    //exit(0);
	}

	if(insertion) {
	    //cout << "entering insertion auction section of env->step()" << endl;

        //cout << "done wif ma robots" << endl;
        if((startFormation))
        {
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
            //cout << "done stepping cells" << endl;
            //forwardPackets();
            for(int i=0; i<auctionCalls.size();i++)
            {
                Robot* a = auctionCalls[i];
                Formation f = formation;
                bool dir;
                Insertion_Auction_Announcement* aa = new Insertion_Auction_Announcement(a->getID());
                sendMsg((Message)aa, ID_BROADCAST, a->getID(), INSERTION_AUCTION_ANNOUNCEMENT);
                a->setAuctionStepCount(1);
            }

            //forwardPackets();
            for(int i=0;i<cells.size();i++)
            {
                //cells[i]->processPackets();
                //cout << " Between processPackets and cStep ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cells[i]->cStep();
                if(isnan(cells[i]->getDistanceTraveled()))
                {
                    cout << "Cell["<<cells[i]->getID()<<"] has total distance of Nan." << endl;
                    cout << "current  x="<<cells[i]->getX()<<"   and y="<<cells[i]->getY()<<endl;
                    exit(1);
                } else if(cells[i]->getDistanceTraveled()>5.0)
                {
                    cout << "Cell["<<cells[i]->getID()<<"] has total distance > 5.0." << endl;
                    cout << "current  x="<<cells[i]->getX()<<"   and y="<<cells[i]->getY()<<endl;
                    //exit(1);
                }
                //cout << "  After cStep " << endl;
            }

            //forwardPackets();
            //cout << "after forwardPackets " << endl;
            auctionCalls.clear();
            //cout << " after auctionCalls.clear() " << endl;
            bool auctionSettled = false;
            for(int i=0; i<robots.size(); i++)
            {
                //cout << "hitting robot["<<robots[i]->getID()<<"]"<<endl;
                //if(robots[i]->getAuctionStepCount() >= AUCTION_STEP_COUNT)
                //{
                robots[i]->processPackets();
                //cout << "robot processSaucePacketsess" << endl;
                if(!auctionSettled)
                {
                    auctionSettled = robots[i]->settleAuction();
                    if(auctionSettled) cout << "settled an auction..." << endl;
                }
                robots[i]->bids.clear();
                //}
            }
        }
        //cout << "done with env->step()" << endl;
        for(int i=0;i<cells.size();i++)
        {
            cells[i]->outstandingBid = 0;
        }
	} else {
	    //cout << "considering if we should hold a push auction." << endl;
        vector<Cell*> auctionCalls;
        Cell *auctionCall = NULL;
	    for (GLint i = 0; i < getNCells(); ++i)
        {
            auctionCall = cells[i]->cStep();
            if((auctionCall != NULL)&&(startFormation))
            {
                //cout << "We have located a cell which wishes to hold an auction. " << endl;
                auctionCalls.push_back(auctionCall);
            }
        }
        if(startFormation)
        {
            for(int i=0; i<auctionCalls.size();i++)
            {
                //cout << "starting an auction!" << endl;
                Cell* a = auctionCalls[i];
                State s = a->getState() ;
                Formation f = formation;
                bool dir;
                if (a->rightNbr == NULL)
                {
                    dir = true;
                } else {
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

    gatherMessages();
    gatherDistances();
    gatherError();
    stepCount++;

    if(quiescence())
    {
        return false;
    }
    //cout << "end of turn - press key to continue: ";
    //string thing;
    //getline(cin, thing);
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
    totalMessages++;
    //messagesPerStep.push_back(p);
    if(p.toID < ID_BROADCAST)
    {
        //cout << "Forwarding a packet to robot["<<p.toID<<"] from "<< p.fromID << endl;
        Robot *r;
        if(!p.fromBroadcast())
        {
            r = getRobot(p.toID);
        }
        if(r !=NULL)
        {
            //cout << "not nullsy for robot id = "<<r->getID() << endl;
            r->msgQueue.push(p);
            //cout << "finished sending packet to robot" << endl;
            return true;
        }else{
            //cout << "could not locate robot["<<p.toID<<"]"<< endl;
            return false;
        }

    }
    Cell *c;
	if((!p.fromBroadcast()) &&(p.toID!=ID_OPERATOR))
	{
	    //cout << "sending a message directly to cell cell["<< p.toID << "]." << endl;
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
			//Robot* r;
			//cout << "Sending PUSH_AUCTION_ANNOUNCEMENT" << endl;
			for(int i=0;i<robots.size();i++)
			{
				robots[i]->msgQueue.push(p);
			}
		}else if(p.type == INSERTION_AUCTION_ANNOUNCEMENT)
		{
            for(int i=0;i<cells.size();i++)
            {
                cells[i]->msgQueue.push(p);
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
                       const int       insert,
                       const float     max_t_error)
{
    srand(time(NULL));

    nRobots      = n;
    formation    = f;
    formation.setFormationID(0);
    formationID  = 0;
    stepCount = 0;
    qCount = 0;
    defaultColor = colorIndex;
    insertion = insert;
    max_trans_error = max_t_error;

    //char * name = "input.txt";
    //strncpy(name,inputFile,9);
    //inputFile[9]='\0';
    ifstream inp;
    inp.open(inputFile.c_str());
    for(int i=0;i<(nRobots*2);i++)
    {
        float loc;
        inp >> loc;
        initLocs.push(loc);
    }
    cout << "initLocs.size() = " << initLocs.size() << endl;
    inp.close();

    if(insertion){
        printf("\n\nUsing Insertion Auction Algorithm\n\n");
    }
    totalMessages = 0;
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
    if(initLocs.size()==nRobots*2)
    {
        for(int i=0;i<nRobots;i++)
        {
            float x,y;
            x = initLocs.front();
            initLocs.pop();
            y = initLocs.front();
            initLocs.pop();
            addRobot(x,y,0.0,randSign() * frand(0.0f, 180.0f));
        }
    } else {
        for (GLint i = 0; i < nRobots; ++i)
            addRobot(randSign() * frand(),
                     randSign() * frand(),
                     0.0f,
                     randSign() * frand(0.0f, 180.0f));
    }
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
    startFormation = true;
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
	Robot* r=NULL,*rr = NULL;
	GLint bID=id;
	//cout << "looking for robot " << id << endl;
	for(int i=0;i<robots.size();i++)
	{
		if(robots[i]->getID()== bID)
		{
			rr = robots[i];
			//cout << "found robot "<< rr->getID() << endl;
			break;
		}
	}
	if(!rr)
	{
	    //cout << "rr = null" << endl;
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
        c->x = r->x;
        c->y = r->y;
        c->startX = c->x;
        c->startY = c->y;
        for(GLint ii=0;ii< robots.size();ii++)
        {
            if(r->getID()==robots[ii]->getID())
            {
                robots.erase(robots.begin()+ii);
                break;
            }
        }
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
	}
}

void Environment::settleInsertionAuction(Robot* r,GLint bID)
{
	cout << "entering env->settleInsertionAuction()" << endl;
	//exit(1);

	if(robots.size()<1)
	{
        cout << "settle insertion auction called when robots.size() is < 1 " << endl;
        exit(1);
	}

    Cell* b;
    b = new Cell();
    if(!addCell(b))
    {
        cout << "addCell() failed!" << endl;
        exit(1);
    }
    cout << "just after cell creation" << endl;
    Cell *a = getCell(bID);
    a->timeOfLastAuction = stepCount;
    if (a == NULL)
    {
        cout << ">> ERROR: Robot[" << bID << "] not found!\n\n";
        return;
    }

    b->x = r->x;
    b->y = r->y;
    b->startX = b->x;
    b->startY = b->y;
    for(GLint ii=0;ii< robots.size();ii++)
    {
        if(r->getID()==robots[ii]->getID())
        {
            robots.erase(robots.begin()+ii);
            break;
        }
    }

    b->clearNbrs();
    b->leftNbr = b->rightNbr = NULL;
    b->lftNbrID = b->rghtNbrID = DEFAULT_NEIGHBOR_ID;


    cout << "About to set neighbor relations" << endl;
    cout << "if a.id = "<< a->getID() << " and seed id = " << formation.getSeedID() << " then we're solid." << endl;
    Cell* c;
    if(a->getID() == formation.getSeedID())
    {
        if(a->rghtNbrID == DEFAULT_NEIGHBOR_ID)
        {
            b->addNbr(a->getID());
            a->addNbr(b->getID());
            b->leftNbr  = b->nbrWithID(a->getID());
            b->lftNbrID = a->getID();
            a->rightNbr = a->nbrWithID(b->getID());
            a->rghtNbrID = b->getID();

        } else if(a->lftNbrID == DEFAULT_NEIGHBOR_ID) {

            b->addNbr(a->getID());
            a->addNbr(b->getID());
            b->rightNbr = b->nbrWithID(a->getID());
            b->rghtNbrID = a->getID();
            a->leftNbr  = a->nbrWithID(b->getID());
            a->lftNbrID = b->getID();
            cout << "n->leftNbr = " << a->leftNbr->ID << endl;
            //newestCell  = c;
        }else{
            cout << "Winner of auction was the seed, seed has two neighbors." << endl;
            if(getHopCount(a,LEFT) <= getHopCount(a,RIGHT))
            {
                c = getCell(a->lftNbrID);
                cout << "Calling insertCell("<<c->getID()<<", "<<b->getID()<<", "<<a->getID()<<") " << endl;
                insertCell(c,b,a);
            } else {
                c = getCell(a->rghtNbrID);
                cout << "Calling insertCell("<<a->getID()<<", "<<b->getID()<<", "<<c->getID()<<") " << endl;
                insertCell(a,b,c);
            }
        }
    } else {

        cout << "wwwwwwwwwwwwwwwwwwww00000000000000000000000000000000000000000000000000000000000000ttttttttttttttttttttttttttttttttttttttttttttttt" << endl;
        c = getCell(a->nbrWithMinGradient()->ID);
        if(c->rghtNbrID == a->getID())   //   seed<----(b)<==>(c)<==>(n)
        {
            insertCell(c,b,a);
        }else if (c->lftNbrID == a->getID())  //   (n)<==>(c)<==>(b)----->seed
        {
            insertCell(a,b,c);
        }else{
            cout << "no matching neighbors..." << endl;
            dieDisplayCells();
        }
        //newestCell = c;

    }

            //if(a) a->showNeighbors();
            //if(b) b->showNeighbors();
            //if(c) c->showNeighbors();


    formation.setFormationID(++formationID);
    //testCellNaN(c);
    sendMsg(new Formation(formation),
            formation.getSeedID(),
            ID_OPERATOR,
            CHANGE_FORMATION);
    //testCellNaN(c);
    cout << "sent formation change message" << endl;
    //b->processPackets();
    //testCellNaN(c);
    //b->updateState();
    //getCell(formation.getSeedID())->sendStateToNbrs();
    //system("PAUSE");

    cout << endl << endl;
    cout << "exiting settleInsertionAuction()" << endl;



    //displayStateOfEnv();
    //testCellNaN(c);
}

void Environment::testCellNaN(Cell * c)
{
    if((isnan(c->x) || isnan(c->y)) || isnan(c->getDistanceTraveled()))
    {
        cout << " cell "<<c->getID()<<" has Nan values" << endl;
        cout << " totalDistance = " << c->getDistanceTraveled() << endl;
        cout << "c->x = "<<c->x<<" c->y = "<<c->y<< endl;
        //exit(1);
    }
    cout << "cell  "<<c->getID()<<" did not have NaN values"<<endl;
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



bool Environment::useInsertion()
{
    if(insertion) return 1;
    else return 0;
}

void Environment::displayNeighborhood(Cell *c)
{

    cout << "cell["<<c->getID() << "]";
    cout << "   leftNbr = ";
    if( c->leftNbr != NULL)
    {
        cout << c->leftNbr->ID ;
    }
    cout << "   rightNbr = ";
    if( c->rightNbr != NULL)
    {
        cout << c->rightNbr->ID ;
    }
    cout << endl;
}

void Environment::displayStateOfEnv()
{/*
    //show Cells
    if(cells.size() >0)
    {
        for(int i=0;i<cells.size();i++)
        {
            cout << "cells["<<cells[i]->getID() << "]";
            cout << "   leftNbr = ";
            if( cells[i]->leftNbr != NULL)
            {
                cout << cells[i]->leftNbr->ID ;
            }
            cout << "   rightNbr = ";
            if( cells[i]->rightNbr != NULL)
            {
                cout << cells[i]->rightNbr->ID ;
            }
            cout << endl;
        }

        stack<int> s;
        int count=0;
        cout << "displayStateOfEnv() thinks getSeedID() = " << formation.getSeedID() << endl;
        s.push(formation.getSeedID());
        while(getCell(s.top())->leftNbr != NULL)
        {
            //cout << "getCell(s.top())->leftNbr->ID = " << getCell(s.top())->leftNbr->ID << endl;
            //cout << "leftNbr = " << getCell(s.top())->leftNbr->ID << "    ( " << s.top() << " )    rightNbr = " << getCell(s.top())->rightNbr->ID << endl;
            s.push(getCell(s.top())->leftNbr->ID);
            count++;


            if(count > cells.size()*10)
            {
                cout << "There is an anomaly in Neighborland." << endl;
                dieDisplayCells();
            }
        }
        cout << "finished traversing leftNbr in displayStateOfEnv() " << endl;
        cout << endl;
        while(! s.empty())
        {
            int c = s.top();
            s.pop();
            if(s.empty())
            {
                cout << " SEED=>";
            }
            cout << "( " << c << " ) <---> ";
        }
        Cell * c;
        c = getCell(formation.getSeedID());
        while(c->rightNbr != NULL)
        {
            c = getCell(c->rightNbr->ID);
            cout << "( " << c->getID() << " )";
            if(c->rightNbr != NULL)
            {
                cout << " <---> ";
            }
        }
        cout << endl;
    }*/

}

/*void Environment::displayStateOfEnv()
{
    //show Cells
    if(cells.size() >0)
    {
        for(int i=0;i<cells.size();i++)
        {
            cout << "cells["<<cells[i]->getID() << "]";
            cout << "   leftNbr = ";
            if( cells[i]->lftNbrID > DEFAULT_NEIGHBOR_ID)
            {
                cout << cells[i]->lftNbrID ;
            }
            cout << "   rightNbr = ";
            if( cells[i]->rghtNbrID > DEFAULT_NEIGHBOR_ID)
            {
                cout << cells[i]->rghtNbrID ;
            }
            cout << endl;
        }

        stack<int> s;
        int count=0;
        //cout << "displayStateOfEnv() thinks getSeedID() = " << formation.getSeedID() << endl;
        s.push(formation.getSeedID());
        while(getCell(s.top())->lftNbrID > DEFAULT_NEIGHBOR_ID)
        {
            //cout << "getCell(s.top())->leftNbr->ID = " << getCell(s.top())->leftNbr->ID << endl;
            //cout << "leftNbr = " << getCell(s.top())->leftNbr->ID << "    ( " << s.top() << " )    rightNbr = " << getCell(s.top())->rightNbr->ID << endl;
            s.push(getCell(s.top())->lftNbrID);
            count++;


            if(count > cells.size()*10)
            {
                cout << "There is an anomaly in Neighborland." << endl;
                dieDisplayCells();
            }
        }
        cout << "finished traversing leftNbr in displayStateOfEnv() " << endl;
        cout << endl;
        while(! s.empty())
        {
            int c = s.top();
            s.pop();
            if(s.empty())
            {
                cout << " SEED=>";
            }
            cout << "( " << c << " ) <---> ";
        }
        Cell * c;
        c = getCell(formation.getSeedID());
        while(c->rghtNbrID > DEFAULT_NEIGHBOR_ID)
        {
            c = getCell(c->rghtNbrID);
            cout << "( " << c->getID() << " )";
            if(c->rghtNbrID > DEFAULT_NEIGHBOR_ID)
            {
                cout << " <---> ";
            }
        }
        cout << endl;
    }

}*/

/*int Environment::getHopCount(Cell * c, Direction d)
{
    int answer=0;
    Cell * current = c;

    if(d == RIGHT)
    {
        while(current->rightNbr != NULL)
        {
            answer++;
            if(answer > cells.size())
            {
                cout << "There is an anomaly in Neighborland. More cells down one branch than there are total...." << endl;
                //exit(1);
                dieDisplayCells();
            }
            current = getCell(current->rightNbr->ID);
        }
    } else {
        while(current->leftNbr != NULL)
        {
            answer++;
            if(answer >= cells.size())
            {
                cout << "There is an anomaly in Neighborland. More cells down one branch than there are total...." << endl;
                //exit(1);
                dieDisplayCells();
            }
            current = getCell(current->leftNbr->ID);
        }
    }

    return answer;
}*/

int Environment::getHopCount(Cell * c, Direction d)
{
    int answer=0;
    Cell * current = c;

    if(d == RIGHT)
    {
        while(current->rghtNbrID > DEFAULT_NEIGHBOR_ID)
        {
            answer++;
            if(answer > cells.size())
            {
                cout << "There is an anomaly in Neighborland. More cells down one branch than there are total...." << endl;
                //exit(1);
                dieDisplayCells();
            }
            current = getCell(current->rghtNbrID);
        }
        cout << "RIGHT hopCount = " << answer << endl;
    } else {
        while(current->lftNbrID > DEFAULT_NEIGHBOR_ID)
        {
            answer++;
            if(answer >= cells.size())
            {
                cout << "There is an anomaly in Neighborland. More cells down one branch than there are total...." << endl;
                //exit(1);
                dieDisplayCells();
            }
            current = getCell(current->lftNbrID);
        }
        cout << "LEFT hopCount = " << answer << endl;
    }

    return answer;
}

void Environment::dieDisplayCells()
{
    for(int i=0;i<cells.size();i++)
    {
        displayNeighborhood(cells[i]);
    }
    cout << endl << endl << "Shutting Down" << endl;
    exit(1);
}

void Environment::insertCell(Cell* a, Cell *b, Cell* c)
{
    //Cells a and c are currently neighbors, and b should be placed between them
    int ac = a->getID(), bc = b->getID(), cc = c->getID();
    /*
    cout <<"\n\n\n\n\n\n";
    cout << "Entering insertCell()"<< endl;
    cout << "( "<<ac<<" )<====>( "<<bc<<" )<====>( "<<cc<<" )"<<endl;
    cout <<"\n\n\n\n\n\n";*/
    //    a<===>c    b


    b->addNbr(ac);
    b->addNbr(cc);
    a->addNbr(bc);
    c->addNbr(bc);


    //b->updateState();


    a->rightNbr = a->nbrWithID(bc);
    a->rghtNbrID = bc;

    //cout << "a->rghtNbrID now == " << a->rghtNbrID << endl;
    c->leftNbr = c->nbrWithID(bc);
    c->lftNbrID = bc;

    b->leftNbr = b->nbrWithID(ac);
    b->lftNbrID = ac;
    b->rightNbr = b->nbrWithID(cc);
    b->rghtNbrID = cc;

    c->removeNbr(ac);
    a->removeNbr(cc);



    //a->cStep();
    //b->cStep();
    //c->cStep();

    //a->updateState();
    //c->updateState();
    displayStateOfEnv();

    //    /--b--\
    //   a<=====>c
}

void Environment::gatherMessages()
{
    Message_Log m(messagesPerStep,stepCount);
    allMessages.push_back(m);
    messagesPerStep.clear();
}

void Environment::gatherError()
{
    float totalTrans=0,totalRot=0;
    for(int i=0;i<cells.size();i++)
    {
        totalTrans+=cells[i]->getState().transError.magnitude();
        totalRot+=abs(cells[i]->getState().rotError);
    }
    Error_Log e(totalTrans,totalRot,stepCount);
    errorSum.push_back(e);
}

void Environment::gatherDistances()
{
    vector<float> distances;
    for(int i=0;i<cells.size();i++)
    {
        distances.push_back(cells[i]->getDistanceTraveled());
    }
    Distances_Log d(distances,stepCount);
    totalDistances.push_back(d);
}

void Environment::gatherConvergence()
{



}


/*void Environment::writeHeader(ostream &st)
{


}*/

void Environment::writeDistanceData(char * filename,char * filename2)
{
    ofstream distanceOut,totalDistance;
    distanceOut.open(filename);
    //writeHeader(distanceOut);
    overallDistance = 0.0;

    for(int i=0;i<cells.size();i++)
    {
        distanceOut << cells[i]->getID() <<", "<<cells[i]->getDistanceTraveled() << endl;

        if(!isnan(cells[i]->getDistanceTraveled()))
        {
            overallDistance += cells[i]->getDistanceTraveled();
        }

        cout << cells[i]->getID() <<", "<< cells[i]->getDistanceTraveled() << endl;

        /*if(1)// cells[i]->getDistanceTraveled() > 1.0 )
        {
            cout << "starting position:  x = " << cells[i]->startX << "   y = " << cells[i]->startY << endl;
            cout << "final position:  x = " << cells[i]->x << "   y = " << cells[i]->y << endl;
            float x, y, xx, yy;
            x = cells[i]->startX;
            y = cells[i]->startY;
            xx = cells[i]->x;
            yy = cells[i]->y;
            float dist = sqrt(((x-xx)*(x-xx))+(y-yy)*(y-yy));
            cout << "distance between start and final, straight line = "<< dist << endl;
        }*/
    }
    distanceOut << overallDistance << endl;
    distanceOut << (float)overallDistance/(float)cells.size() << endl;
    cout << "Overall Distance traveled = " << overallDistance << endl;
    cout << "Average Distance = " << (float)overallDistance/(float)cells.size() << endl;

    distanceOut.close();

    totalDistance.open(filename2);
    //writeHeader(totalDistance);


    for(int i=0;i<totalDistances.size();i++)
    {
        Distances_Log td = totalDistances[i];
        totalDistance << td.s <<", ";
        for(int j=0;j<td.d.size();j++)
        {
            totalDistance << td.d[j] << ", ";
        }
        if(td.d.size()==0)
        {
            totalDistance << 0;
        }
        totalDistance << endl;
    }
    totalDistance.close();
}

void Environment::dumpMessagesToFile(char * filename)
{
/*    cout << "total messages passed = " << totalMessages << endl;

    ofstream messagesOut;
    messagesOut.open(filename);

    //writeHeader(messagesOut);

    for(int i=0;i<allMessages.size();i++)
    {
        Message_Log mLog = allMessages[i];
        vector<Packet> p = mLog.m;
        for(int j=0;j<p.size();j++)
        {
            //messagesOut << mLog.s <<", "<<p[j].fromID << ", "<<p[j].toID<<", "<<p[j].type<<endl;
            messagesOut << mLog.s <<", "<<p.size()<<endl;
        }
        if(p.size()==0)
        {
            messagesOut << mLog.s <<", "<<0<<endl;
        }
        //cout << allMessages[i].fromID << ", " << allMessages[i].toID << ", " << allMessages[i].type << endl;
    }
    messagesOut.close();
*/
}


void Environment::writeFinalPositions()
{
    ofstream os;
    os.open("positions.out");
    for(int i=0;i<cells.size();i++)
    {
        os << cells[i]->x << endl;
        os << cells[i]->y << endl;
    }
    os.close();
}

void Environment::writeHeader()
{
    ofstream os;
    os.open("summary.out");
    os << "Random Seed File:  " << inputFile << endl;
    os << "Total Initial Robots: " << nRobots << endl;
    os << "Formation Type: " << formation.getFunction() << endl;
    if(INSERTION) os << "Insertion Auction" << endl;
    if(!INSERTION) os << "Push Auction" << endl;
    int convergence_total=0;
    int num_cells_converged=0;
    for(int i=0;i<cells.size();i++)
    {
        if(cells[i]->convergedAt()>=0)
        {
            num_cells_converged++;
            convergence_total+=cells[i]->convergedAt();
        }
    }
    os << endl << endl;

    os << "TOTAL_TIME_TO_CONVERGE:  " << stepCount << endl;
    //os << "Total Converged Cells: " << num_cells_converged << endl;
    os << "AVERAGE_TIME_TO_CONVERGE:  " << (float)(convergence_total/num_cells_converged) << endl << endl;
    os << "TOTAL_DISTANCE_TRAVELED:  " << overallDistance << endl;
    os << "AVERAGE_DISTANCE_TRAVELED:  " << (float)overallDistance/(float)cells.size() << endl << endl;

    os << "TOTAL_MESSAGES_SENT:  " << totalMessages << endl;
    os << "AVERAGE_MESSAGES_SENT:  " << (float)totalMessages/(float)cells.size() << endl;


    cout << "Total Converged Cells:  " << num_cells_converged << endl;
    cout << "Sum of convergence steps:  " << convergence_total << endl;
    cout << "Average Steps to convergence:  " << (float)convergence_total/(float)num_cells_converged << endl;
    cout << "Total Messages sent :  " << totalMessages << endl;
    cout << "Average Messages :   " << (float)totalMessages/(float)cells.size() << endl;
    //os << endl << endl;
    //os << "[DATA]"<<endl;
    os.close();
}

void Environment::dumpErrorToFile( char * filename)
{
    cout << "total steps = " << stepCount << endl;

    ofstream errorOut;
    errorOut.open(filename);

    //writeHeader(errorOut);

    for(int i=0;i<errorSum.size();i++)
    {
        Error_Log e = errorSum[i];
        errorOut << e.s << ", "<<e.trans<<", "<<e.rot<<endl;
        //cout << allMessages[i].fromID << ", " << allMessages[i].toID << ", " << allMessages[i].type << endl;
    }
    errorOut.close();




}

bool Environment::quiescence()
{
    bool answer=true;
    if(cells.size()==nRobots)
    {
        for(int i=0;i<cells.size();i++)
        {
            if(cells[i]->getState().transError.magnitude() > MAX_TRANSLATIONAL_ERROR)
            {
                answer = false;
                //cout << endl << endl << endl << "cell["<<cells[i]->getID()<<"] is further than MAX_TRANSLATIONAL_ERROR away form desired location" << endl << endl << endl;
                break;
            }
        }

    }else{
        answer = false;
    }
    if(answer)
    {
        qCount++;
        //cout << endl << endl << endl << "incrementing qCount" << endl << endl << endl;
    }
    if(qCount>QUIESCENCE_COUNT)
    {
        return true;
    }
    return false;
}
