#include<stdio.h>
#include<iostream>
#include<vector>
void TabuSearch(){
        int n=0,maxitr=max(30,Customer/2);
        int s,s0,sbest;
        int cbest;
        while(1)
        {
                s=s.bestneighborhood();
                if(s.getcost() < cbest)
                {
                        sbest=s; 
                        cbest=s.getcost();
                }
                if(n>maxitr)
                        break;
                //Reactive TABU part
                /* theory : Checking iterations, if the solution is repeated after n0 times, Tabu length(maxitr) is increased */
        }
}

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
        TabuSearch();
        return 0;
}
