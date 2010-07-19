#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

struct robot {
    float x,y;
    bool selected;
};

struct cell {
    float x,y;
    robot* r;
};

int main(int argc, char * argv[])
{
    int num_bots;
    string cell_pos,robot_pos;
    if(argc==4)
    {
        num_bots = atoi(argv[1]);
        cell_pos = argv[2];
        robot_pos = argv[3];
        cout << "cell_pos = " << cell_pos << " and robot_pos = " << robot_pos << endl;
    } else {
        cout << "improper argument specification." << endl;
        exit(1);
    }
    ifstream robot_f,cell_f;
    robot_f.open(robot_pos.c_str());

    vector<robot> robots;

    cout << "Robots: " << endl;
    for(int i=0;i<num_bots;i++)
    {
        float xx=0.0,yy=0.0;
        robot_f >> xx;
        robot_f >> yy;
        robot r;
        r.x = xx;
        r.y = yy;
        r.selected = false;
        //cout << "x="<<xx<<" y="<<yy<<endl;
        robots.push_back(r);
    }
    robot_f.close();

    for(int i=0;i<robots.size();i++)
    {
        cout <<" r.x="<< robots[i].x<<" r.y="<<robots[i].y<<endl;
    }

    cell_f.open(cell_pos.c_str());
    vector<cell> cells;
    cout << "Cells: " << endl;
    for(int i=0;i<num_bots;i++)
    {
        float xx=0.0,yx=0.0;
        cell_f >> xx;
        cell_f >> yx ;
        cell c;
        c.x = xx;
        c.y = yx;
        c.r = NULL;
        cout << "x="<<xx<<" y="<<yx<<endl;
        cells.push_back(c);
    }
    cell_f.close();

    bool answerFound = false;
    while(!answerFound)
    {
        for(int c=0;c<cells.size();c++)
        {
            float distance = 9999999.9;
        }
    }            

    return 0;
}

float distance(float x,  float y, float xx, float yy)
{
    float answer = 99999999.9;
    float xxx = x-xx;
    float yyy = y-yy;
    answer = sqrt((xxx*xxx)+(yyy*yyy));
    return answer;
}

