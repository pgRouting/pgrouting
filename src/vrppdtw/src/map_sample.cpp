#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<map>
#include<string>
using namespace std;
int main()
{
        map<string,int> mani;
        mani["12"]=10;
        printf("mani2 = %d\n",mani.find("12")->second);
        return 0;
}
