#include<stdio.h>
#include<math.h>
int main()
{
        float t;
        int p;
        int E=1000,k=10,i;
        float e=2.7,z;
        for(i=100;i>=1;i--)
        {
               z=(E/(k*i));
               printf("value of p=%lf where t=%d\n", pow(e,(1/(z))),i);
        }
        return 0;
}
