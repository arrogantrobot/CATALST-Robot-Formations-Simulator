#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

float frand(const float min = 0.0f, const float max = 1.0f)
{
    return min + (max - min) * float(rand()) / float(RAND_MAX);
} 

int main()
{
    srand(time(NULL));
    ofstream stuff;
    stuff.open("randOut.txt");
    for(int i=0;i<50;i++)
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
    return 0;
}
