#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>

using namespace std;

float frand(const float min = 0.0f, const float max = 1.0f)
{
    //srand(time(NULL));
    return min + (max - min) * float(rand()) / float(RAND_MAX);
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
        float r = rand();
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
        stuff << sign*frand() << endl;
    }
    stuff.close();
    sleep(2);
    return 0;
}
