#include<stdio.h>
#include<iostream>
#include<vector>
using namespace std;
class Test{
        public:
        int init_sum()
        {
                int sum=0;
                a=10,b=5,c=2;
                sum=a+b+c;
                return sum;
        }
        private:
        int a,b,c;

};
int main()
{
        Test T;
        vector<int> arr;
        printf("Sum=%d\n",T.init_sum());
        for(int i=0;i<100;i++)
        {
                arr.push_back(i);
        }
        int i=100;
       while(i)
       {
               printf("arr[%d]=%d\n",i,arr[i]);
               i--;
       }
       printf("front=%d\n",arr.front());
        return 0;
}
