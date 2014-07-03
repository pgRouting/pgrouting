#include<stdio.h>
#include<math.h>
#include<iostream>
#include "./pdp.h"
using namespace std;

int PickupLength=0;


depot d;
customer c[1000];
int len=0;


Route r[500];




int main()
{
        pickup p[600];
        scanf("%d",&VehicleCount);
        scanf("%d",&Capacity);
        scanf("%d",&Speed);
        depot d=ScanDepot(d);
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


        //Sort Pickup's
        int swap;
        double swap1;
        for(int i=1;i<=PickupLength;i++)
        {
                for(int j=1;j<=PickupLength-i;j++)
                {
                        if(p[j].Ddist>p[j+1].Ddist)
                        {
                                swap1=p[j].Ddist;
                                p[j].Ddist=p[j+1].Ddist;
                                p[j+1].Ddist=swap1;
                                swap=p[j].Did;
                                p[j].Did=p[j+1].Did;
                                p[j+1].Did=swap;
                                swap=p[j].Pid;
                                p[j].Pid=p[j+1].Pid;
                                p[j+1].Pid=swap;
                                swap=p[j].id;
                                p[j].id=p[j+1].id;
                                p[j+1].id=swap;
                        }
                }
                p[i].checked=0;
        }
      /*  
        cout << "Pickups after sorted";
       for(int i=1;i<=PickupLength;i++)
       {
               printf("id= %d   Dist=%f  pick=%d  del=%d\n",p[i].id,p[i].Ddist,p[i].Pid,p[i].Did);
       }
       */
        //Sequential Construction 
        for(int v=0;v<36;v++)
        {
                for(int i=PickupLength;i>=1;i--)
                {
                        if(p[i].checked!=1)
                        {
                                State S;
                                S=r[v].append(c,p[i],d,CustomerLength,PickupLength,S);
                                int flag=r[v].HillClimbing(c,d,p[i]) ;
                                if(flag==1)
                                {
                                        //Remove 
                                        r[v].remove(S);
                                }
                                else
                                {
                                        p[i].checked=1;
                                }
                        }
                }
                printf("%d, ",v+1);
                r[v].print();

        }
        int sum=0,rts=0;
        for(int i=0;i<40;i++)
        {
                sum+=r[i].dis;
                if(r[i].dis!=0)
                {
                        rts+=1;
                }
        }
        printf("Sum=%d  Routes=%d\n",sum,rts);


        return 0;
}


















