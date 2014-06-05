// sort() Example using arrays.
// By Zereo 04/22/13
#include<stdio.h>
#include <iostream>
#include <algorithm>

using namespace std;

const int SIZE = 7;

int main()
{
            int intArray[SIZE] = {5, 3, 32, -1, 1, 104, 53};

                //Now we call the sort function
                sort(intArray, intArray+SIZE);

                    cout << "Sorted Array looks like this." << endl;
                    for(int i=0;i<SIZE;i++)
                    {
                            printf("%d ",intArray[i]);
                    }
                    printf("\n");

                            return 0;
}
