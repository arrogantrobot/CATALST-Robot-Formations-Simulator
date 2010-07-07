//
// Filename:        "Cell.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This class describes a robot cell.
//

// preprocessor directives
#ifndef CELL_H
#define CELL_H
#include <vector>
#include "Auctioning.h"
#include "Neighborhood.h"
#include "Robot.h"
using namespace std;



//
#define VERBOSE            (0)
#define AUTONOMOUS_INIT    (1)
#define ALLOW_CELL_BIDS    (0)
#define CELL_INFO_VIEW     (0)
#define AUCTION_STEP_COUNT (3)
#define INSERTION          (0)
#define DEFAULT_NEIGHBOR_ID (-1)



// message type index values
enum MessageType
{
    HEARTBEAT = 0,
    STATE,
    CHANGE_FORMATION,
    PUSH_AUCTION_ANNOUNCEMENT,
    INSERTION_AUCTION_ANNOUNCEMENT,
    BID,
    BUMP,
    NEW_NEIGHBOR,
    DROP_NEIGHBOR
};  // MessageType

enum Direction
{
    LEFT = 0,
    RIGHT
};


// global constants
static const Color   DEFAULT_CELL_COLOR       = WHITE;
static const bool    DEFAULT_CELL_SHOW_FILLED = true;
static const GLint   LEFT_NBR_INDEX           = 0;
static const GLint   RIGHT_NBR_INDEX          = 1;
static const GLint   NEIGHBORHOOD_SIZE        = 2;
static const GLfloat MAX_TRANSLATIONAL_ERROR  = 0.02f;



// describes a robot cell
class Cell: public State, public Neighborhood, public Robot
{
    friend class Environment;

    public:

        // <constructors>
        Cell(const GLfloat dx         = 0.0f,
             const GLfloat dy         = 0.0f,
             const GLfloat dz         = 0.0f,
             const GLfloat theta      = 0.0f,
             const Color   colorIndex = DEFAULT_CELL_COLOR);
        Cell(const Cell &r);

        // <destructors>
        virtual ~Cell();

        // <public mutator functions>
        bool setState(const State &s);
        bool setNbrs(Neighborhood &nh);
        bool setRobot(const Robot &r);
        bool setRobotP(Robot *r);
        bool setAuctionStepCount(const int& asc);

        // <public accessor functions>
        State        getState() const;
        Neighborhood getNbrs()  const;
        Robot        getRobot() const;
        GLint        getNBids() const;
        float        getX() const;
        float        getY() const;
        int          getAuctionStepCount() const;
        float        getDistanceTraveled() const;
        bool         neighborsInPosition() const;
        void         displayInsertionAuctions();
        bool         bidOnInsertionAuction();


        int          outstandingBid;



        // <virtual public utility functions>
        virtual void draw();
        Cell* cStep();
        //virtual  step();
        virtual void updateState();

        // <virtual public neighborhood functions>
        virtual bool changeFormation(const Formation &f,
                                     Neighbor         n = Neighbor());
        virtual bool sendStateToNbrs();
        virtual bool sendState(const GLint);
        virtual bool processPackets();
        virtual bool processPacket(Packet &p);

        // <public primitive behaviors>
        Behavior moveError();
        Behavior moveError(const Vector tError, const GLfloat rError);
        Behavior moveErrorBehavior(const Vector tError, const GLfloat rError);

        // <virtual overloaded operators>
        virtual Cell& operator =(const State &s);
        virtual Cell& operator =(const Neighborhood &nh);
        virtual Cell& operator =(const Robot &r);

    protected:

        // <protected data members>
        vector<Bid *> bids;
        vector<Insertion_Auction_Announcement *> insertion_auctions;
        GLint          numAuctions;
        Neighbor     *leftNbr, *rightNbr;
        int           lftNbrID, rghtNbrID;
        GLint         index;
        GLint         numBids;
        int           auctionStepCount;
        GLfloat       distanceTraveled;
        GLfloat       prevX, prevY;
        bool          insertion;

        // <protected static data members>
        static GLint nCells;

        // <protected utility functions>
        void settleAuction();

        void updateDistanceTraveled();

        // <virtual protected utility functions>
        virtual bool init(const GLfloat dx         = 0.0f,
                          const GLfloat dy         = 0.0f,
                          const GLfloat dz         = 0.0f,
                          const GLfloat theta      = 0.0f,
                          const Color   colorIndex = DEFAULT_CELL_COLOR);
};  // Cell

#endif
