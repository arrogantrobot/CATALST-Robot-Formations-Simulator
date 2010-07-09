#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <cmath>

using namespace std;

float frand(const float min = 0.0f, const float max = 1.0f)
{
    //srand(time(NULL));
    return min + (max - min) * float(rand()) / float(RAND_MAX);
} 

bool distance_ok(float x, float y)
{
    bool answer = false;
    float thing = sqrt((x*x)+(y*y));
    //printf("found sqrt of x=%f y=%f\n",x,y);
    //printf("thing = %f\n",thing);
    answer = thing <= 1.0;
    return answer;
}


int get_sign()
{
    int answer;
    if(frand()>0.5)
    {
        answer = 1;
    } else {
        answer = -1;
    }
    return answer;
}

int main(int argc, char * argv[])
{
    int num = atoi(argv[1]);
    string output = argv[2];
    srand(time(NULL));
    ofstream stuff;
    stuff.open(output.c_str());
    for(int i=0;i<num;i++)
    {
        int sign;
        if(frand()>0.5)
        {
            sign = 1;
        } else {
            sign = -1;
        }
        if(i<2)
        {
            sign = 0;
        }
        float x=2.0,y=2.0;
        if(i==0)
        {
            x = y = 0.0;
        } else {
            while(!distance_ok(x,y))
            {
                x = frand()*get_sign();
                y = frand()*get_sign();
            }
        }
        stuff << x << endl;
        stuff << y << endl;
    }
    stuff.close();
    sleep(2);
    return 0;
}
