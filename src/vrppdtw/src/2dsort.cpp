#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void sort(int MyArray[8][8]) //you're telling the function that it should expect an 8x8 array as a parameter
{
        for(int row = 0; row < 8; row++)
        {
                for(int i=0;i<8;i++)
                {
                        for(int j=i;j<8;j++)
                        {
                                if(MyArray[row][i]<MyArray[row][j]) //the condition was flipped in the program you gave me, notice that would have sorted them ascendingly
                                {
                                        int temp=MyArray[row][i];
                                        MyArray[row][i]=MyArray[row][j];
                                        MyArray[row][j]=temp;
                                }
                        }
                }
        }
}

void print(int MyArray[8][8]) //you're telling the function that it should expect an 8x8 array as a parameter
{
        for (int i=0; i<8; i++)
        {
                for (int j=0; j<8; j++)
                {
                        cout<<MyArray[i][j]<<" ";
                }
                cout<<endl;
        }
}

int main ()
{
        srand ((unsigned)time(0));
        int array[8][8]; //you don't have to define stuff in the global scope, defining your array here is fine
        for (int i=0; i<8; i++)
        {
                for (int j=0; j<8; j++)
                {
                        array[i][j]=rand()%(99) + 1; //fill the array with elements
                }
        }
        print(array); //print before sorting
        sort (array); //sort
        print(array); //print after sorting
        cout<<endl<<endl;
        return 0;
}
