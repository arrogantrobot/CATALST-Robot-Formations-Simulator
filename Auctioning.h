// preprocessor directives
#ifndef AUCTIONING_H
#define AUCTIONING_H
#include "Vector.h"
#include "State.h"



//
#define MAX_INT   (2147483647)
#define MAX_FLOAT (999999999)



//
#define E         (1.0f)
#define X         (5.0f)



struct Push_Auction_Announcement
{
    Vector p_j;
    State s_i;
    bool right;

    Push_Auction_Announcement(const Vector p = Vector(),
                         const State  s = State(),
                         const bool   r = true): p_j(p), s_i(s), right(r)
    {
    }   // Auction_Announcement(const Vector, const State, const bool)
};  // Auction_Announcement

struct Insertion_Auction_Announcement
{
    GLint aID;
    Insertion_Auction_Announcement(const GLint id = -1):aID(id)
    {
    }
};

//
struct Bid
{
    GLfloat b_i;
    GLint   bID;

    Bid(const GLfloat b = MAX_FLOAT, const GLint id = -1): b_i(b), bID(id)
    {
    }   // Bid(const GLfloat, const GLint)
};  // Bid

#endif

