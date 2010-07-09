// preprocessor directives
#ifndef LOG_H
#define LOG_H
#include <vector>
#include "Packet.h"




struct Message_Log
{
    vector<Packet> m;
    int s;

    Message_Log(const vector<Packet> p,
                         const int  ss = 0): m(p), s(ss)
    {
    }
};  // Message Log

struct Distances_Log
{
    vector<float> d;
    int s;

    Distances_Log(const vector<float> p,
                         const int  ss = 0): d(p), s(ss)
    {
    }
};  // Distances Log

struct Error_Log
{
    float trans;
    float rot;
    int s;

    Error_Log(const float t = 0.0,const float r = 0.0,
                         const int  ss = 0): trans(t),rot(r), s(ss)
    {
    }
};  // Error Log


#endif

