#include<stdio.h>
#include<iostream>
#include<vector>
using namespace std;
class route{
        public:
                route(){
                }
                int arr[100];
                int arr_length;
};
class Solution{
        public:
                Solution(){
                }
                ~Solution(){
                }
                int twv;
                int cost; 
                int route_length;
                route r[100];
};
int main(){
        route r;
        for(int i=0;i<100;i++){
                r.arr[i]=i;
        }
        for(int i=0;i<100;i++){
                printf("r.arr[%d]=%d\n",i,r.arr[i]);
        }
        Solution S;      
        S.route_length=10;
        for(int i=0;i<10;i++) {
                for(int j=0;j<100;j++)
                {
                        S.r[i].arr[j]=j;
                }
        }
        for(int i=0;i<10;i++) {
                for(int j=0;j<100;j++)
                {
                        printf("S.r[%d].arr[%d]=%d\n",i,j,S.r[i].arr[j]);
                }
        }
        return 0;
}
