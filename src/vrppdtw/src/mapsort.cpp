#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<map>
#include<string.h>
#include<string>
using namespace std;
typedef struct manikanta
{
        int value;
        char str[3];
}mani;


int main()
{
        mani m[100];
        for(int i=0;i<100;i++)
        {
                m[i].value=i+1;
                strcpy(m[i].str,"10");
        }
        for(int i=0;i<100;i++)
        {
                printf("%d  %s\n",m[i].value,m[i].str);
        }

        return 0;
}
