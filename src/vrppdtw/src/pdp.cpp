#include<stdio.h>
#include<math.h>
#include<iostream>
#include "./pdp.h"
using namespace std;

int PickupLength=0;


depot d;
customer c[1000];
int len=0;
int main()
{
        pickup p[600];
        scanf("%d",&VehicleCount);
        scanf("%d",&Capacity);
        scanf("%d",&Speed);
        ScanDepot(d);
        while((scanf("%d",&temp))!=EOF)
        {
                CustomerLength+=1;
                len=CustomerLength;
                c[CustomerLength]=ScanCustomer(temp,c[CustomerLength],d);
        }

        //From customers put aside all the pickup's;
        for(int i=1;i<=106;i++)
        {
                if(c[i].P==1)
                {
                        PickupLength+=1;
                        p[PickupLength].id=PickupLength;
                        p[PickupLength].Did=c[i].Dindex;
                        p[PickupLength].Ddist=c[i].Ddist;
                        p[PickupLength].Pid=c[i].id;
                }
        }
        printf("Pickup Struct length=%d\n",PickupLength);


        //Sort Pickup's
      int swap1,swap2,swap3,swap4;
        for(int i=1;i<=PickupLength;i++)
        {
                for(int j=1;j<=PickupLength-i;j++)
                {
                        if(p[j].Ddist>p[j+1].Ddist)
                        {
                                //swap Ddist
                                swap1=p[j].Ddist;
                                p[j].Ddist=p[j+1].Ddist;
                                p[j+1].Ddist=swap1;
                                //swap Did
                                swap2=p[j].Did;
                                p[j].Did=p[j+1].Did;
                                p[j+1].Did=swap2;
                                //swap Pid
                                swap3=p[j].Pid;
                                p[j].Pid=p[j+1].Pid;
                                p[j+1].Pid=swap3;
                                //swap id
                                swap4=p[j].id;
                                p[j].id=p[j+1].id;
                                p[j+1].id=swap4;
                        }
                }
        }

        printf("After Sorting:  \n");
        for(int i=1;i<=PickupLength;i++)
        {
                printf("Ddist =%d  id=%d   Pid=%d\n",p[i].Ddist,p[i].id,p[i].Pid);
        }


        return 0;
}
