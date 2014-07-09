#include<stdio.h>
int main()
{
        int a=101234;
        int sum=0;
        while(a!=0)
        {
                sum+=a%10;
                a/=10;
        }
        printf("sum=%d\n",sum);
        return 0;
}
