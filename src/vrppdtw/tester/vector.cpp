#include<stdio.h>
#include<iostream>
#include<vector>
int main()
{
        std::vector<int> path;
        for(int i=0;i<100;i++){
                path.push_back(i);
        }
        for(int i=0;i<100;i++){
                printf("Path array = %d\n",path[i]);
        }
        printf("Size=%d\n",path.size());
        return 0;
}
