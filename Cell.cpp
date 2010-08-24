//
// Filename:        "Cell.cpp"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class implements a robot cell.
//

// preprocessor directives
#include "Cell.h"
#include "Environment.h"

#define FLT_MAX 1E+37

/*bool isNumber(const GLfloat& n) {
    return (n==n);
}

bool isInfNum(const GLfloat& n) {
    return (n <= FLT_MAX && n >= -FLT_MAX);
}*/

// <protected static data members>
GLint Cell::nCells = 0;



// <constructors>

//
// Cell(dx, dy, dz, theta, colorIndex)
// Last modified: 06Nov2009
//
// Default constructor that initializes
// this cell to the parameterized values.
//
// Returns:     <none>
// Parameters:
//      dx          in      the initial x-coordinate of the cell (default 0)
//      dy          in      the initial y-coordinate of the cell (default 0)
//      dz          in      the initial z-coordinate of the cell (default 0)
//      theta       in      the initial heading of the cell (default 0)
//      colorIndex  in      the initial array index of the color of the cell
//
Cell::Cell(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
           const GLfloat theta, const Color   colorIndex,bool ins)
    : State(), Neighborhood(), Robot(dx, dy, dz, theta, colorIndex)
{
    init(dx, dy, dz, theta, colorIndex,ins);
    ID      = nCells++;
    numBids = 0;
    //insertion = ins;
}   // Cell(const GLfloat..<4>, const Color)



//
// Cell(c)
// Last modified: 27Aug2006
//
// Copy constructor that copies the contents of
// the parameterized cell into this cell.
//
// Returns:     <none>
// Parameters:
//      c       in/out      the cell being copied
//LgTxpqYm
Cell::Cell(const Cell &c): State(c), Neighborhood(c), Robot(c)
{
    leftNbr  = c.leftNbr;
    rightNbr = c.rightNbr;
    lftNbrID = c.lftNbrID;
    rghtNbrID = c.rghtNbrID;
}   // Cell(const Cell &)



// <destructors>

//
// ~Cell()
// Last modified: 27Aug2006
//
// Destructor that clears this cell.
//
// Returns:     <none>
// Parameters:  <none>
//
Cell::~Cell()
{
}   // ~Cell()



// <public mutator functions>

//
// bool setState(s)
// Last modified: 27Aug2006
//
// Attempts to set the state to the parameterized state,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      s       in/out  the state to be set to
//
bool Cell::setState(const State &s)
{
    *this = s;
    return true;
}   // setState(const State &)



//
// bool setNbrs(nh)
// Last modified: 27Aug2006
//
// Attempts to set the neighborhood to the parameterized neighborhood,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      nh      in/out  the neighborhood to be set to
//
bool Cell::setNbrs(Neighborhood &nh)
{
    *this = nh;
    return true;
}   // setNbrs(Neighborhood &)



//
// bool setRobot(r)
// Last modified: 14May2007
//
// Attempts to set the robot to the parameterized robot,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      r       in/out  the robot to be set to
//
bool Cell::setRobot(const Robot &r)
{
	//if(VERBOSE) printf("in setRobot() ============\n");

	//changed to  cast *this as a Robot variable
    (Robot)*this = r;
	//1if(VERBOSE) printf("robot set============\n");
    return true;
}   // setRobot(const Robot &)

bool Cell::setRobotP(Robot *r)
{
	//(Robot *)this = r;
	return true;
}


// <public accessor functions>

//
// State getState() const
// Last modified: 27Aug2006
//
// Returns the state of this cell.
//
// Returns:     the state of this cell
// Parameters:  <none>
//
State Cell::getState() const
{
    return (State)*this;
}   // getState() const



//
// Neighborhood getNbrs() const
// Last modified: 27Aug2006
//
// Returns the neighborhood of this cell.
//
// Returns:     the neighborhood of this cell
// Parameters:  <none>
//
Neighborhood Cell::getNbrs() const
{
    return (Neighborhood)*this;
}   // getNbrs() const



//
// Robot getRobot() const
// Last modified: 27Aug2006
//
// Returns the robot of this cell.
//
// Returns:     the robot of this cell
// Parameters:  <none>
//
Robot Cell::getRobot() const
{
    return (Robot)*this;
}   // getRobot() const



// <virtual public utility functions>

//
// void draw()
// Last modified: 27Aug2006
//
// Renders the cell as a robot.
//
// Returns:     <none>
// Parameters:  <none>
//
void Cell::draw()
{
    Robot::draw();
}   // draw()



//
// void step()
// Last modified: 30Nov2009
//
// Processes packets received and updates the state of the cell,
// which is then broadcast within the neighborhood of the cell.
//
// Returns:     <none>
// Parameters:  <none>
//
Cell* Cell::cStep()
{
    //cout << "################################## ----     CALLING CSTEP     ----- ###########################################################"<<endl;
    Cell* answer = NULL;


    if(rghtNbrID!=-1)
    {
        rightNbr = nbrWithID(rghtNbrID);
    }
    if(lftNbrID!=-1)
    {
        leftNbr = nbrWithID(lftNbrID);
    }

    if((getState().transError.magnitude()<CONVERGENCE_ERROR_MAX)&&(converged<0))
    {
        converged = env->stepCount;
    }
    processPackets();
	//if (processPackets())
	//{
	    //cout << "done here 1" << endl;
    if (getNNbrs() > 0)
    {
        //cout << "done here 2" << endl;
        updateState();
        //cout << "done here 3" << endl;
        sendStateToNbrs();
        //cout << "done here 4" << endl;
    }
    moveError();
	//  }

    //if((gradient.magnitude() < 0.05f)&&(ID!=formation.getSeedID()))
    //{
        //cout << "Cell " << ID << " thinks its gradient magnitude is " << gradient.magnitude() << endl;
        //cout << "    x=" << x << "    y=" << y << endl;
    //}

	if(auctionStepCount>0)
	{
		auctionStepCount++;
	}

	//cout << " Done with stepwise accounting in cStep    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo insertion = " << insertion <<  endl;

	if(!insertion){
        if((AUTONOMOUS_INIT)&&(env->getRobots().size()>0))
        {
            if((getNNbrs() < NEIGHBORHOOD_SIZE)&&(bids.size()==0))
            {
                if(((getState().transError.magnitude()>0)||
                    (getID()==formation.getSeedID()))&&
                    (getState().transError.magnitude() < max_trans_error))//MAX_TRANSLATIONAL_ERROR))
                {
                    if(auctionStepCount==0)
                    {
                        //cout << "Cell["<<this->ID<<"]->gradient = " << this->gradient << endl;
                        //if(getID()==formation.getSeedID())
                        //{
                        answer = this;
                        //}
                    }
                }
            }
        }
	}else{
	    bidOnInsertionAuction();
	    //cout << "finished call to bidOnInsertionAuction()" << endl;
	}
	//insertion_auctions.clear();
	if(CELL_INFO_VIEW)
	{
		cout << "============================="<<endl;
		cout << "cell.getID() = " << getID() << endl;
		cout << "cell.getNNbrs() = " << getNNbrs() << endl;
		cout << "cell.rightNbr->getID() = ";
    	if(rightNbr != NULL)
		{
			cout << rightNbr->ID << endl;
		}else
		{
			cout << " NULL " << endl;
		}
		cout << "cell.leftNbr->getID() = ";
		if(leftNbr != NULL)
		{
			cout << leftNbr->ID << endl;
		}else
		{
			cout << " NULL " << endl;
		}
		cout << "rotError = " << rotError << endl;
		cout << "transError = " << transError.magnitude() << endl;
		cout << "behavior.getStatus() = " << behavior.getStatus() << endl;
		cout << "seedID = " << formation.getSeedID() << endl;
		cout << "gradient = " << gradient << endl;
		cout << "formationID = " << formation.getFormationID() << endl;
		cout << "x,y location = " << x << ","<<y<<endl;
		cout <<"================================" << endl << endl;
	}
    Robot::step();
    updateDistanceTraveled();
    return answer;
}


//
// void updateState()
// Last modified: 30Nov2009
//
// Updates the state of the cell based upon the
// current states of the neighbors of the cell.
//
// Returns:     <none>
// Parameters:  <none>
//

void Cell::updateState()
{
    /*if ((getNNbrs()               == 0)     ||
        (nbrWithMinStep()->tStep  <  tStep) ||
        ((formation.getSeedID()   != ID)    &&
         (nbrWithMaxStep()->tStep == tStep))) return;*/

    // update actual relationships to neighbors
    Neighbor *currNbr = NULL;
    for (GLint i = 0; i < size(); ++i)
    {
        currNbr = getNbr(i);
        if (currNbr == NULL) break;

        // change formation if a neighbor has changed formation
        if ((currNbr->formation.getFormationID() > formation.getFormationID()))//&&(currNbr->ID == nbrWithMinGradient()->ID))
        {
            /*if((ID ==3)||(ID==2))
            {
                cout << "cell "<<ID<<" called changeFormation( " << currNbr->ID << " )    with formationID = " << formation.getFormationID() << endl;
            }*/
            changeFormation(currNbr->formation, *currNbr);

        }
        currNbr->relActual = getRelationship(currNbr->ID);
    }
    rels = getRelationships();

    // reference the neighbor with the minimum gradient
    // to establish the correct position in formation
    if (getNNbrs() > 0)
    {
        Neighbor     *refNbr     = nbrWithMinGradient(
                                       formation.getSeedGradient());
        if(ID==3)
        {
            //cout << "Cell 3 thinks refNbr is " << refNbr->ID << endl;
        }
        Relationship *nbrRelToMe = relWithID(refNbr->rels, ID);
        if ((formation.getSeedID() != ID)   &&
            (refNbr                != NULL) &&
            (nbrRelToMe            != NULL))
        {

            // error (state) is based upon the
            // accumulated error in the formation
            Vector  nbrRelToMeDesired = nbrRelToMe->relDesired;
            nbrRelToMeDesired.rotateRelative(-refNbr->rotError);
            GLfloat theta = scaleDegrees(nbrRelToMe->relActual.angle() - (-refNbr->relActual).angle());
            rotError      = scaleDegrees(theta + refNbr->rotError);
            transError    = nbrRelToMeDesired - nbrRelToMe->relActual + refNbr->transError;
            transError.rotateRelative(-theta);
        }
    }

    tStep = max(tStep + 1, nbrWithMaxStep()->tStep);
}   // updateState()


/*void Cell::updateState()
{
    Neighbor currNbr;
    for (GLint i = 0; i < getNNbrs(); ++i)
    {
        if (!getHead(currNbr)) break;

        // change formation if a neighbor has changed formation
        if (getNbr(0)->formation.getFormationID() > formation.getFormationID())
            changeFormation(getNbr(0)->formation, *getNbr(0));
        getNbr(0)->relActual = getRelationship(currNbr.ID);
        ++(*this);
    }
    rels = getRelationships();

    // reference the neighbor with the smallest gradient
    // to establish correct position in formation
    Neighbor     *refNbr = nbrWithMinGradient();
    Relationship *nbrRel = relWithID(refNbr->rels, ID);
    if ((formation.getSeedID() != ID) && (refNbr != NULL) && (nbrRel != NULL))
    {

        // error (state) is based upon the accumulated error in the formation
        nbrRel->relDesired.rotateRelative(-refNbr->rotError);
        GLfloat theta = scaleDegrees(nbrRel->relActual.angle() -
                                     (-refNbr->relActual).angle());
        rotError      = scaleDegrees(theta + refNbr->rotError);
        transError    = nbrRel->relDesired - nbrRel->relActual +
                        refNbr->transError;
        transError.rotateRelative(-theta);
        if (transError.norm() > threshold()) moveArc(transError);
        else
			if (abs(rotError) > angThreshold())
				moveArc(0.0, degreesToRadians(-rotError));
            //if (abs(scaleDegrees(refNbr->relActual.angle() -
             //                    refNbr->relDesired.angle())) > angThreshold())
                //orientTo(refNbr->relActual, refNbr->relDesired.angle());
            else moveStop();
    }
    else moveStop();
}   // updateState()*/

/*void Cell::updateState()
{
	if(VERBOSE) printf("entering updateState()\n");
    Neighbor currNbr;
    for (GLint i = 0; i < getNNbrs(); ++i)
    {
        if (!getHead(currNbr)) break;

        // change formation if a neighbor has changed formation
        if (getNbr(0)->formation.getFormationID() > formation.getFormationID())
            changeFormation(getNbr(0)->formation, *getNbr(0));
        getNbr(0)->relActual = getRelationship(currNbr.ID);
        ++(*this);
    }
    rels = getRelationships();

    // reference the neighbor with the smallest gradient
    // to establish correct position in formation
	if(VERBOSE) printf("updateState() -- BEFORE nbrWithMinGradient()\n");
    Neighbor     *refNbr = nbrWithMinGradient(formation.getSeedGradient());
	if(VERBOSE) printf("updateState() -- AFTER nbrWithMinGradient()\n");
	Relationship *nbrRel;
	if(refNbr == NULL){
		//printf("refNbr == NULL\n");
	}else{
		nbrRel = relWithID(refNbr->rels, ID);


		if(VERBOSE) printf("updateState() -- AFTER relWithID()\n");
	}
    if ((formation.getSeedID() != ID) && (refNbr != NULL) && (nbrRel != NULL))
    {

        // error (state) is based upon the accumulated error in the formation
        nbrRel->relDesired.rotateRelative(-refNbr->rotError);
        GLfloat theta = scaleDegrees(nbrRel->relActual.angle() -
                                     (-refNbr->relActual).angle());
        rotError      = scaleDegrees(theta + refNbr->rotError);
        transError    = nbrRel->relDesired - nbrRel->relActual +
                        refNbr->transError;
        transError.rotateRelative(-theta);
        if (transError.norm() > threshold()) moveArc(transError);
        else
			if (abs(rotError) > angThreshold())
				moveArc(0.0, degreesToRadians(-rotError));
            //if (abs(scaleDegrees(refNbr->relActual.angle() -
              //                   refNbr->relDesired.angle())) > angThreshold())
                //orientTo(refNbr->relActual, refNbr->relDesired.angle());
            else moveStop();
    }
    else moveStop();
	if(VERBOSE) printf("leaving updateState()\n");
}   // updateState()*/



// <virtual public neighborhood functions>

//
// bool changeFormation()
// Last modified: 30Nov2009
//
// Attempts to change the formation of the cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      f       in/out      the formation to change to
//      n       in          the neighbor instigating the formation change
//
bool Cell::changeFormation(const Formation &f, Neighbor n)
{
    //cout << endl;
    //cout << "Cell["<<ID<<"] has received a changeFormation command from "<<n.ID << endl;
    formation = f;
    if (formation.getSeedID() == ID)
    {
        gradient   = formation.getSeedGradient();
        transError = Vector();
        rotError   = 0.0f;
    }
    else
    {
        Relationship *nbrRelToMe = relWithID(n.rels, ID);
        //cout << "\n\n\n"<<"Cell " << ID << " thinks that it's neighbor to link with is " << n.ID << "\n\n\n";
        //cout << " cell " << ID << " is using ID " << nbrRelToMe->ID << " to calc gradient......................................................... from neighbnor ID   "  << n.ID <<  endl;
        //cout << " formationID = " << formation.getFormationID() << endl;
        if (nbrRelToMe == NULL) return false;
        nbrRelToMe->relDesired.rotateRelative(n.formation.getHeading());

        //cout << " relDesired = " << nbrRelToMe->relDesired << "   and n.gradient = " << n.gradient;
        gradient                 = n.gradient + nbrRelToMe->relDesired;
        //cout << " which makes my grad = " << gradient << endl;
        transError               = Vector();
        rotError                 = 0.0f;
    }
    vector<Vector> r = formation.getRelationships(gradient);
    {

    /*cout << "For Cell["<<ID<<"]"<<endl;
    for(int i=0;i<r.size(),i++)
    {
        cout << "ID at i="<<i<<" is  " << r[i].ID << endl;
    }*/

    /*--ROSS--
    GLfloat currDist        = 0.0f, closestDist = GLfloat(RAND_MAX);
    GLint   closestNbrIndex = -1;
    //GLint   closestRelIndex = -1;
    cout << "myID = " << ID << endl;
    cout << "+ nRels = " << r.getSize() << endl;
    cout << "+ nNbrs = " << getNNbrs()  << endl;
    vector<GLint> assignedIDs;
    for (GLint i = 0; i < r.getSize(); ++i)
    {
        closestDist     = GLfloat(RAND_MAX);
        closestNbrIndex = -1;
        Vector currRel;
        if (r.getHead(currRel))
        {
            for (GLint j = 0; j < getNNbrs(); ++j)
            {
                Neighbor currNbr;
                if (getHead(currNbr))
                {
                    currDist = (getNbr(0)->relDesired - r[0]).magnitude();
                    if (currDist < closestDist)
                    {
                        bool assignedID = false;
                        for (GLint k = 0; k < assignedIDs.getSize(); ++k)
                        {
                            GLint currID = -1;
                            if ((assignedIDs.getHead(currID)) &&
                                (currID == getNbr(0)->ID))
                            {
                                assignedID = true;
                                break;
                            }
                            ++assignedIDs;
                        }
                        if (!assignedID)
                        {
                            closestDist     = currDist;
                            closestNbrIndex = j;
                        }
                    }
                }
                ++(*this);
            }
            if ((closestNbrIndex >= 0) && (closestNbrIndex < getNNbrs()))
            {
                assignedIDs.insertTail(getNbr(closestNbrIndex)->ID);
                cout << "  -nbrID[" << i << "] = "
                     << getNbr(closestNbrIndex)->ID;
                cout << " | rel = " << r[0] << endl;
                getNbr(closestNbrIndex)->relDesired = r[0];
                ++r;
            }
            else
            {
                cout << "   -nbrID[" << i << "] = " << ID_NO_NBR << endl;
                r.removeHead();
            }
        }
        else ++r;
    }
    --ROSS--*/
    /*--ROSS--
    for (GLint i = 0; i < getNNbrs(); ++i)
    {
        closestDist     = GLfloat(RAND_MAX);
        closestRelIndex = -1;
        if (getHead(currNbr))
        {
            for (GLint j = 0; j < r.getSize(); ++j)
            {
                Vector currRel;
                if (r.getHead(currRel))
                {
                    currDist = (getNbr(0)->relDesired - currRel).magnitude();
                    if (currDist < closestDist)
                    {
                        closestDist     = currDist;
                        closestRelIndex = j;
                    }
                }
                ++r;
            }
            cout << "  -nbrID[" << i << "] = " << getNbr(0)->ID;
            if ((closestRelIndex >= 0) && (closestRelIndex < r.getSize()))
            {
                cout << " | rel = " << r[closestRelIndex] << endl;
                getNbr(0)->relDesired = r[closestRelIndex];
                r.remove(closestRelIndex);
                ++(*this);
            }
            else
            {
                cout << " | [DELETING]" << endl;
                removeHead();
            }
        }
        else ++(*this);
    }
    //setNbrs(nh);
    --ROSS--*/
    }
    Relationship rLeft,rRight;
    if (leftNbr  != NULL) leftNbr->relDesired  = r[LEFT_NBR_INDEX];
    if (rightNbr != NULL) rightNbr->relDesired = r[RIGHT_NBR_INDEX];
    /*if(r.size()>2)
    {
        showNeighbors();
        cout << "Exiting, too many neighbors." << endl;
        exit(1);
    }
    for(int i=0;i<r.size();i++)
    {
        if(r[i].ID == leftNbr->ID)
        {
            rLeft = r[i];
        }else if(r[i].ID== rightNbr->ID)
        {
            rRight = r[i];
        }
    }*/
    /*if (leftNbr  != NULL)
    {

        leftNbr->relDesired = r[lftNbrIndex];
    }
    if (rightNbr != NULL)
    {
        rightNbr->relDesired = r[rghtNbrIndex];
    }*/

    //cout << "Cell["<<ID<<"] has a gradient of " << gradient.magnitude()<< endl;
    //if (leftNbr  != NULL)cout << "just set leftNbr->relDesired to the Vector w/  = " << r[LEFT_NBR_INDEX].angle() << endl;
    //if (rightNbr  != NULL)cout << "just set rightNbr->relDesired to the Vector w/ angle = " << r[RIGHT_NBR_INDEX].angle() << endl;
    //cout << endl;
    //if (leftNbr  != NULL) leftNbr->relDesired  = nbrWithID(lftNbrID)->relDesired;//r[lftNbrID];
    //if (rightNbr != NULL) rightNbr->relDesired = nbrWithID(rghtNbrID)->relDesired;//r[rghtNbrID];
    return true;
}   // changeFormation(const Formation &, Neighbor)


void Cell::setNbrIndex()
{
    vector<Neighbor> n = getNbrs();
    int r=-1,l=-1;
    if(rightNbr)
    {
        r = rightNbr->ID;
    }
    if(leftNbr)
    {
        l = leftNbr->ID;
    }
    for(int i=0;i<n.size();i++)
    {
        if(n[i].ID==r)
        {
            rghtNbrIndex = i;
        } else if (n[i].ID==l)
        {
            lftNbrIndex = i;
        }
    }
}


//
// bool sendStateToNbrs()
// Last modified: 27Aug2006
//
// Attempts to broadcast the state of the cell
// to the neighborhood of the cell, returning
// true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Cell::sendStateToNbrs()
{
    Neighbor curr;
	if(VERBOSE)printf("cellID=%d\n",getID());
    for (GLint i = 0; i < getNNbrs(); ++i)
	{
		if(VERBOSE)printf("sending state to id= %d\n",getNbr(i)->ID);

		//if((getNbr(i)->ID)==NULL
		//if(VERBOSE)printf("

        if (!sendState(getNbr(i)->ID))
		{
			//printf("sendState returned false\n");
			return false;
		}
	}
	//printf("leaving sendStateToNbrs()\n");
    return true;
}   // sendStateToNbrs()



//
// bool sendState(toID)
// Last modified: 27Aug2006
//
// Attempts to send the state of the cell
// to the neighbor with the parameterized ID,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      toID    in      the ID of the receiving neighbor
//
bool Cell::sendState(const GLint toID)
{
	//printf("in sendState()\n");
	State *s = new State(*this);
	//printf("number of relations of %d is %d \n",toID,s->rels.getSize());
	//printf("calling cell = %d\n", this->ID);
	//printf("number of relations of %d is %d \n",this->ID,this->rels.getSize());
	bool answer = sendMsg(s, toID, STATE);
	//printf("leaving sendState()\n");
	return answer;
}   // sendState(const GLint)



//
// bool processPackets()
// Last modified: 30Nov2009
//
// Attempts to process all packets received by the cell,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:  <none>
//
bool Cell::processPackets()
{
    bool   success = true;
    Packet p;
    while (!msgQueue.empty())
    {
        p = msgQueue.front();
        if (!processPacket(p)) success = false;
        msgQueue.pop();
    }
    //cout << " done processing packets " << endl;
    return success;
}   // processPackets()



//
// bool processPacket(p)
// Last modified: 22Dec2006
//
// Attempts to process the parameterized packet,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      p       in/out      the packet to be processed
//
bool Cell::processPacket(Packet &p)
{
    bool success = false;
    if ((p.fromOperator()) && (p.type == CHANGE_FORMATION))
	{
			success  = changeFormation(*((Formation *)p.msg));
	}else if(p.type == PUSH_AUCTION_ANNOUNCEMENT)
	{
		if(ALLOW_CELL_BIDS)
		{
		}
		success = true;
	}
	else if(p.type ==INSERTION_AUCTION_ANNOUNCEMENT)
	{
        insertion_auctions.push_back((Insertion_Auction_Announcement *)p.msg);
        success = true;
	}
	else if(p.type == BID)
	{
		if(p.msg!=NULL)
		{
			bids.push_back((Bid*)p.msg);
			success = true;
			numBids++;
			//cout << "bid received, total = " << numBids << endl;
		}
	}
	else if(p.type == BUMP)
	{
	    if(p.fromID == nbrWithMinGradient(this->gradient)->ID)
	    {
                State* s = (State*)p.msg;
                float a = s->gradient.magnitude() /(float) s->formation.radius;
                this->gradient = (s->gradient *= (a+1/a));
                int next = nbrWithMaxGradient(this->gradient)->ID;
                if(next!=ID && next!=ID_NO_NBR) //if there exists another neighbor, bump it
                {
                    env->sendMsg(&this->getState(), next,ID, BUMP);
                }
	    }
	}
	else if(p.type == NEW_NEIGHBOR)
	{
	    int i = *(int*)p.msg;
	    if(!addNbr(i))
	    {
	        success = false;
	    }
	    else
	    {




	    }

	}
	else if(p.type == DROP_NEIGHBOR)
	{
	    int i = *(int*)p.msg;
        if(!removeNbr(i))
        {
            success = false;
        }
        else
        {
            if(rightNbr->ID == i)
            {
                rightNbr = NULL;
            }
            else if(leftNbr->ID == i)
            {
                leftNbr = NULL;
            }
        }
	}
	else if ((isNbr(p.fromID)) || (p.fromBroadcast()))
	{
        switch(p.type)
        {
            case STATE:
                //cout << "State update..." << endl;
                success = (p.msg == NULL) ?
                    false : updateNbr(p.fromID, *((State *)p.msg));
                delete (State *)p.msg;
                p.msg   = NULL;
                break;
            default: break;
        }
	}
    return success;
}   // processPacket(Packet &)



// <public primitive behaviors>

//
// Behavior moveError()
// Last modified: 07Nov2009
//
// Moves the robot cell using the current translational and
// rotational errors, activating and returning the appropriate
// robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:  <none>
//
Behavior Cell::moveError()
{
    return behavior = moveErrorBehavior(transError, rotError);
}   // moveError()



//
// Behavior moveError(tError, rError)
// Last modified: 07Nov2009
//
// Moves the robot cell using the parameterized translational and
// rotational errors, activating and returning the appropriate
// robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      tError  in      the translational error
//      rError  in      the rotational error
//
Behavior Cell::moveError(const Vector tError, const GLfloat rError)
{
    return behavior = moveErrorBehavior(tError, rError);
}   // moveError(const Vector, const GLfloat)



//
// Behavior moveErrorBehavior(tError, rError)
// Last modified: 07Nov2009
//
// Moves the robot using the parameterized translational and
// rotational errors, returning the appropriate robot behavior.
//
// Returns:     the appropriate robot behavior
// Parameters:
//      tError  in      the translational error
//      rError  in      the rotational error
//
Behavior Cell::moveErrorBehavior(const Vector tError, const GLfloat rError)
{
    if      (transError.magnitude() > threshold())
        return moveArc(transError);
    else if (abs(rotError)          > angThreshold())
        return moveArc(0.0, degreesToRadians(-rotError));
    return moveStop();
}   // moveErrorBehavior(const Vector, const GLfloat)



// <virtual overloaded operators>

//
// Cell& =(s)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized state into this cell.
//
// Returns:     this cell
// Parameters:
//      s       in/out      the state being copied
//
Cell& Cell::operator =(const State &s)
{
    return *this = s;
}   // =(const State &)



//
// Cell& =(nh)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized neighborhood into this cell.
//
// Returns:     this cell
// Parameters:
//      nh      in/out      the neighborhood being copied
//
Cell& Cell::operator =(const Neighborhood &nh)
{
    return *this = nh;
}   // =(const Neighborhood &)



//
// Cell& =(r)
// Last modified: 27Aug2006
//
// Copies the contents of the parameterized robot into this cell.
//
// Returns:     this cell
// Parameters:
//      r       in/out      the robot being copied
//
Cell& Cell::operator =(const Robot &r)
{
    return *this = r;
}   // =(const Robot &)



// <virtual protected utility functions>

//
// bool init(dx, dy, dz, theta, colorIndex)
// Last modified: 30Nov2009
//
// Initializes the cell to the parameterized values,
// returning true if successful, false otherwise.
//
// Returns:     true if successful, false otherwise
// Parameters:
//      dx          in      the initial x-coordinate of the cell (default 0)
//      dy          in      the initial y-coordinate of the cell (default 0)
//      dz          in      the initial z-coordinate of the cell (default 0)
//      theta       in      the initial heading of the cell (default 0)
//      colorIndex  in      the initial array index of the color of the cell
//
bool Cell::init(const GLfloat dx,    const GLfloat dy, const GLfloat dz,
                const GLfloat theta, const Color   colorIndex, bool ins)
{
    showFilled = DEFAULT_CELL_SHOW_FILLED;
    leftNbr    = rightNbr = NULL;
    lftNbrID = rghtNbrID = DEFAULT_NEIGHBOR_ID;
    auctionStepCount = 0;
    insertion = ins;
    converged = -1;
    prevX = 999;
    prevY = 999;
    distanceTraveled = 0.0;

    return true;
}   // init(const GLfloat..<4>, const Color)


void Cell::settleAuction()
{
	//cout << "Cell::settleAuction() entered\n"<< endl;
	auctionStepCount = 0;
	if(bids.size()<=0)
	{
		return;
	}
	Bid* winningBid;
	//srand(time(NULL));
	//winningBid = bids[rand()%bids.size()];




	winningBid = bids[0];
	for(int i=0;i<bids.size();i++)
	{
		if(bids[i]->b_i < winningBid->b_i)
		{
			winningBid = bids[i];
		}
	}
	//cout <<"Robot # "<<winningBid->rID<<" won the auction" << endl;
	if(env->getRobot(winningBid->bID))
	{
        env->settlePushAuction(this,winningBid->bID);
	}
	bids.clear();
}

GLint Cell::getNBids() const
{
    return bids.size();
}

int Cell::getAuctionStepCount() const
{
	return auctionStepCount;
}

bool Cell::setAuctionStepCount(const int& asc)
{
    //cout << ID << " distance = " << distanceTraveled << endl;
	auctionStepCount = asc;
	return true;
}

void Cell::updateDistanceTraveled()
{
    float dist=0.0;
    float xx=0.0,yy=0.0;
    if((prevX!=999)&&(prevY!=999))
    {
        xx = x - prevX;
        yy = y - prevY;
        dist = sqrt((xx*xx)+(yy*yy));
    }
    prevX = x;
    prevY = y;

    distanceTraveled += dist;

    /*if(ID==3)
    {
        cout << "dist = " << dist << endl;
        cout << "total= " << distanceTraveled << endl;
    }*/
}

float Cell::getDistanceTraveled() const
{
    //float answer = 0.0;
    //if(distanceTraveled < 0.001)
    //{
    //    answer = 0.0;
    //}else
    //{
    //    answer = distanceTraveled;
    //}
    return distanceTraveled;//answer;
}


/*bool Cell::neighborsInPosition()
{
    bool rightN = false, leftN = false;
    if(getNNbrs()==1)
    {
        if(rightNbr != NULL)
        {
            if(env->getCell(rightNbr->ID)->getState().transError.magnitude() < MAX_TRANSLATIONAL_ERROR)
            {
                cout << "checking rightNbr transErr - seed has one neighbor -  it is less than MTE at " <<  env->getCell(rightNbr->ID)->getState().transError.magnitude() << endl;
                rightN = leftN = true;
            } else {
                cout << "right not null, but not close enough" << endl;
                rightN = leftN = false;
            }
        }else if(leftNbr != NULL) {
            if(env->getCell(leftNbr->ID)->getState().transError.magnitude() < MAX_TRANSLATIONAL_ERROR)
            {
                cout << "checking leftNbr transErr - seed has one neighbor -  it is less than MTE at " <<  env->getCell(rightNbr->ID)->getState().transError.magnitude() << endl;
                rightN = leftN = true;
            } else {
                cout << "left not null, but not close enough" << endl;
                rightN = leftN = false;
            }
        } else {
            rightN = leftN = false;
            cout << "right AND left null" << endl;
        }
    } else if (getNNbrs() == 2) {

        env->getCell(rightNbr->ID)->updateState();
        env->getCell(leftNbr->ID)->updateState();

        if(env->getCell(rightNbr->ID)->getState().transError.magnitude() < MAX_TRANSLATIONAL_ERROR)
        {
            //cout << "checking rightNbr transErr --seed has 2 neighbors --- it is less than MTE at " <<  env->getCell(rightNbr->ID)->getState().transError.magnitude() << endl;
            rightN =true;
        }
        if(env->getCell(leftNbr->ID)->getState().transError.magnitude() < MAX_TRANSLATIONAL_ERROR)
        {
            //cout << "checking rightNbr transErr --seed has 2 neighbors --  it is less than MTE at " <<  env->getCell(leftNbr->ID)->getState().transError.magnitude() << endl;
            leftN = true;
        }
    } else {
        leftN = rightN = true;
    }

    return rightN && leftN;
}*/

bool Cell::neighborsInPosition() const
{
    bool showStuff = false;
    bool rightN = false, leftN = false;
    if(getNNbrs()==1)
    {
        //cout << endl << endl;
        //cout << "deciding for 1 neighbor" << endl;
        //cout << endl << endl;
        if(rghtNbrID>=0)
        {
            if(env->getCell(rghtNbrID)->getState().transError.magnitude() < max_trans_error)//MAX_TRANSLATIONAL_ERROR)
            {
                if(showStuff) cout << "checking rightNbr transErr - seed has one neighbor -  it is less than MTE at " <<  env->getCell(rghtNbrID)->getState().transError.magnitude() << endl;
                rightN = leftN = true;
            } else {
                if(showStuff)cout << "right not null, but not close enough" << endl;
                rightN = leftN = false;
            }
        }else if(lftNbrID >= 0) {
            if(env->getCell(lftNbrID)->getState().transError.magnitude() < max_trans_error)//MAX_TRANSLATIONAL_ERROR)
            {
                if(showStuff)cout << "checking leftNbr transErr - seed has one neighbor -  it is less than MTE at " <<  env->getCell(lftNbrID)->getState().transError.magnitude() << endl;
                rightN = leftN = true;
            } else {
                if(showStuff)cout << "left not null, but not close enough" << endl;
                rightN = leftN = false;
            }
        } else {
            rightN = leftN = false;
            if(showStuff)cout << "right AND left null" << endl;
        }
    } else if (getNNbrs() == 2) {
        if(showStuff)cout << endl << endl;
        if(showStuff)cout << "deciding for 2 neighbors" << endl;
        if(showStuff)cout << endl << endl;
        //env->getCell(rghtNbrID)->updateState();
        //env->getCell(lftNbrID)->updateState();

        if(env->getCell(rghtNbrID)->getState().transError.magnitude() < max_trans_error)//MAX_TRANSLATIONAL_ERROR)
        {
            //cout << "checking rightNbr transErr --seed has 2 neighbors --- it is less than MTE at " <<  env->getCell(rightNbr->ID)->getState().transError.magnitude() << endl;
            rightN =true;
        }else{
            if(showStuff)cout << "right not close enough" << endl;
        }
        if(env->getCell(lftNbrID)->getState().transError.magnitude() < max_trans_error)//MAX_TRANSLATIONAL_ERROR)
        {
            //cout << "checking rightNbr transErr --seed has 2 neighbors --  it is less than MTE at " <<  env->getCell(leftNbr->ID)->getState().transError.magnitude() << endl;
            leftN = true;
        } else {
            if(showStuff)cout << "left not close enough" << endl;
        }
    } else {
        leftN = rightN = true;
    }

    return rightN && leftN;
}

bool Cell::bidOnInsertionAuction()
{
    bool answer = false;
    //cout << "cStep - insertion stuff" << endl;
    if((getState().transError.magnitude() < max_trans_error))//&&(ID == 0))//formation.getSeedID()))
    {
        //cout << "Cell["<<ID<<"] has "<<insertion_auctions.size() << " auction announcements   ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
        if(insertion_auctions.size()>0)
        {
            //displayInsertionAuctions();
            //if((neighborsInPosition())&&(timeOfLastAuction + AUCTION_BACKOFF < env->stepCount))
            //{
                if(!outstandingBid)
                {
                    //cout << "deeper into bid process" << endl;
                    GLfloat shortestRange = 99999999.0;
                    GLint nearestAuction = -2;
                    if(insertion_auctions.size()>1)
                    {
                        //cout << "heresies" << endl;
                        for(int i=0;i<insertion_auctions.size();i++)
                        {
                            if(!env->getRobot(insertion_auctions[i]->aID))
                            {
                                //cout << "skipping robot #: " << insertion_auctions[i]->aID << endl;
                                continue;
                            }
                            GLfloat range = env->distanceToRobot(this, env->getRobot(insertion_auctions[i]->aID));
                            //cout << "in heresies   range = " << range << endl;
                            if (range<shortestRange)
                            {

                                shortestRange = range;
                                nearestAuction = insertion_auctions[i]->aID;
                                //cout << "Robot["<<nearestAuction<<"] is the nearest auction..." << endl;
                            }
                        }
                    }else{
                        if(!env->getRobot(insertion_auctions[0]->aID))
                        {
                            //cout << "no auctions with existing robots" << endl;
                        } else {
                            nearestAuction = insertion_auctions[0]->aID;
                            shortestRange = env->distanceToRobot(this, env->getRobot(nearestAuction));
                        }
                    }
                    if(nearestAuction == -2)
                    {
                        //cout << "nearestAuction was for a non-existant robot." << endl;
                    } else {
                        //cout << "outside of search for closest auction" << endl;
                        GLfloat range = shortestRange;
                        GLfloat b_j = E * range;
                        Bid    *b   = new Bid(b_j, getID());
                        //cout << "Bid b_i = " << b->b_i << " bID = " << b->bID << endl;
                        env->sendMsg(b, nearestAuction, ID, BID);
                        outstandingBid = 1;
                        //cout << "sent bid to robot["<<nearestAuction<<"] EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"<< endl;
                        //insertion_auctions.clear();
                        answer = true;
                    }
                } else {
                    //cout << "found an outstanding bid, not proceeding" << endl;
                }
            //}
            insertion_auctions.clear();
        }
    }
    return answer;
}

void Cell::displayInsertionAuctions()
{
    cout << endl << endl;
    for(int i=0; i<insertion_auctions.size();i++)
    {
        if(env->getRobot(insertion_auctions[i]->aID))
        {
            cout << "Announcement from robot :   " << insertion_auctions[i]->aID << " at a distance of " << env->distanceToRobot(this, env->getRobot(insertion_auctions[i]->aID)) << endl;
        } else {
            cout << "no robot # " << insertion_auctions[i]->aID << "   could be found." << endl;
        }
    }
    cout << endl << endl;
}

float Cell::getX() const
{
    return x;
}
float Cell::getY() const
{
    return y;
}

int Cell::convergedAt()
{
    return converged;
}

void Cell::showNeighbors()
{
    cout << endl;
    cout << "showNeighbors for cell " << ID << endl;
    vector<Neighbor> n = getNbrs();
    for(int i=0;i<n.size();i++)
    {
        cout << "neighbor at index :  " << i << "    ID :  " << n[i].ID << endl;
    }
    if(rightNbr) cout << "      rightNbr ID :  " << rightNbr->ID << endl;
    if(leftNbr) cout << "       leftNbr ID :  " << leftNbr->ID << endl;
    cout << endl << endl;

}
