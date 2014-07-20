#include<stdio.h>
#include<vector>
#include<iostream>
using namespace std;
typedef struct mani
{
        int length;
        int path[1000];
        int order[1000];
}M;
int main()
{
        vector<M> ma;
        for(int i=0;i<10;i++)
        {
                ma[i].length=i;
                for(int j=0;j<10;j++)
                {
                        ma[i].path[j]=j;
                        ma[i].order[j]=j;
                }
        }
        return 0;
}
