//
// Filename:        "Packet.h"
//
// Programmer:      Ross Mead
// Last modified:   30Nov2009
//
// Description:     This structure defines a message packet.
//

// preprocessor directives
#ifndef PACKET_H
#define PACKET_H
#include "GLIncludes.h"
using namespace std;



// global constants
static const GLint ID_OPERATOR  = -1;
static const GLint ID_BROADCAST = -2;
static const GLint ID_ROBOT     = -3;



// message type redefinition
typedef void* Message;



// defines a message packet
struct Packet
{

    // <data members>
    Message msg;
    GLint   toID, fromID, type;



    // <constructors>

    //
    // Packet(m, to, from, t)
    // Last modified: 04Sep2006
    //
    // Default constructor that initializes
    // this packet to the parameterized values.
    //
    // Returns:     <none>
    // Parameters:
    //      m       in      the default message
    //      to      in      the default ID of the message recipient
    //      from    in      the default ID of the message sender
    //      t       in      the default message type
    //
    Packet(const Message m    = NULL,
           const GLint   to   = ID_BROADCAST,
           const GLint   from = ID_OPERATOR,
           const GLint   t    = 0)
        : msg(m), toID(to), fromID(from), type(t)
    {
    }   // Packet(const Message, const GLint, const GLint, const GLint)



    //
    // Packet(p)
    // Last modified: 12Aug2006
    //
    // Copy constructor that copies the contents of
    // the parameterized packet into this packet.
    //
    // Returns:     <none>
    // Parameters:
    //      p       in/out      the packet being copied
    //
    Packet(const Packet &p)
        : msg(p.msg), toID(p.toID), fromID(p.fromID), type(p.type)
    {
    }   // Packet(const Packet &)



    // <destructors>

    //
    // ~Packet()
    // Last modified: 07Sep2006
    //
    // Destructor that clears this packet.
    //
    // Returns:     <none>
    // Parameters:  <none>
    //
    ~Packet()
    {
    }   // ~Packet()



    // <utility functions>

    //
    // bool fromOperator() const
    // Last modified: 12Aug2006
    //
    // Returns true if this packet is from an operator, false otherwise.
    //
    // Returns:     true if this packet is from an operator, false otherwise
    // Parameters:  <none>
    //
    bool fromOperator() const
    {
        return fromID == ID_OPERATOR;
    }   // fromOperator() const



    //
    // bool fromBroadcast() const
    // Last modified: 12Aug2006
    //
    // Returns true if this packet is from a broadcast, false otherwise.
    //
    // Returns:     true if this packet is from a broadcast, false otherwise
    // Parameters:  <none>
    //
    bool fromBroadcast() const
    {
        return fromID == ID_BROADCAST;
    }   // fromBroadcast() const
};  // Packet

#endif
