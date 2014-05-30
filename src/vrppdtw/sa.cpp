#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cmath>


using namespace std;

double f(double x) //I should add another parameter for the energy
{
        return x = pow(x, 4) + (4 / 3)* pow(x, 3) - 4 * pow(x, 2) + 5;  //(I have to 
}


int main()
{

        double first_run, second_run, third_run;        //(first, second and third run) are defined for the purpose of comparing the resulting
        time_t systime;                                // solutions of the three runs will be chosen as the final solution
        time(&systime);
        srand((unsigned int)systime);
        double  alpha = 0.9;                         //alpha is used for the cooling schedule of the temperature            
        const double e = 2.718281828;






        double x ; //setting an initial value of x (state)
        cin >> x;

        cout << "Initial State = " << x << "\t, and F(x)= " << f(x) << endl;

        double L = f(x);

        for (double T = 80; T > 0.00008; T *= alpha) //T = T * alpha which used as a cooling schedule 
        {


                for (int i = 0; i<200; i++) //This loop is for the process of iteration (or searching for new states)
                {
                        double xNew = x + ((rand() / (double)RAND_MAX) * 2 - 1);
                        double LNew = f(xNew);

                        if (LNew < L || (rand() / (double)RAND_MAX) <= pow(e, -(LNew - L) / T))
                        {
                                L = LNew;
                                x = xNew;
                        }
                }


        }

        cout << "Final state = " << x << "\t, total of F(x) = " << f(x) << endl << endl;



        return 0;
}
