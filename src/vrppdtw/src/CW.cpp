//Savings Algorithm by Clarke and Wright  --- Manikanta

/* Order of Scanning -- number of customers n and number of vehicles v and Dis matrix and Demands */
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<map>
#include<string>
#include<string.h>
using namespace std;


//Struct for sorting purposes 
typedef struct Node
{
        int value;
        int index1;
        int index2;
}node;

//n= customers, v=vehicles dis= distance matrix, Demand[i]= demands of customer i
int n, v, dis[1000][1000]={0},demand[1000]={0},capacity,arr[1000][1000];
node comb[1000];
int length_comb=0,temp,flag[1000]={0};


//function 

int function(int initial, int full)
{
//        printf("**********function call**********\n");
        int length_array=2000,array[2000]={0};
        int total_weight=0,end1=0,end2=0,break_k=0,sat_count=0;

        //Trivial  Condition 
        for(int i=initial;i<=full;i++)
        {
                if(flag[comb[i].index1]==0 && flag[comb[i].index2]==0)
                {
                        end1=500;
                        end2=501;
                        array[end1]=comb[i].index1;
                        array[end2]=comb[i].index2;
                        total_weight=demand[comb[i].index1]+demand[comb[i].index2];
                        flag[comb[i].index1]=1;
                        flag[comb[i].index2]=1;
                        break_k=i;
                        if(i==full)
                        {
                                for(int k=end1;k<=end2;k++)
                                {
                                        if(array[k]!=0)
                                        {
                                                printf("route: %d ",array[k]);
                                        }
                                }
                                printf("\n");
                                return full+1;
                        }
                }
                break;
        }

        if(end1==0 && end2==0)
        {
                return initial+1;
        }
        //Root Building
        for(int i=break_k+1;i<full;i++)
        { 
                if((flag[comb[i].index1]==0 && flag[comb[i].index2]==1) || (flag[comb[i].index1]==1 && flag[comb[i].index2]==0))
                {
                        if(comb[i].index1==array[end1])
                        {

                                if(total_weight+demand[comb[i].index2]<=100)
                                {
                                        end1-=1;
                                        array[end1]=comb[i].index2;
                                        total_weight+=demand[comb[i].index2];
                                        flag[comb[i].index2]=1;
                                }

                        }
                        else if(comb[i].index1==array[end2])
                        {
                                if(total_weight+demand[comb[i].index2]<=100)
                                {
                                        end2+=1;
                                        array[end2]=comb[i].index2;
                                        total_weight+=demand[comb[i].index2];
                                        flag[comb[i].index2]=1;
                                }
                        }
                        else if(comb[i].index2==array[end1])
                        {
                                if(total_weight+demand[comb[i].index1]<=100)
                                {
                                        end1-=1;
                                        array[end1]=comb[i].index1;
                                        total_weight+=demand[comb[i].index1];
                                        flag[comb[i].index1]=1;
                                }
                        }
                        else if(comb[i].index2==array[end2])
                        {
                                if(total_weight+demand[comb[i].index1]<=100)
                                {
                                        end2+=1;
                                        array[end2]=comb[i].index1;
                                        total_weight+=demand[comb[i].index1];
                                        flag[comb[i].index1]=1;
                                }
                        }
                }
                //Saturation Check
                //min of all

                int min;
                for(int j=1;j<n;j++)
                {
                        min=1000;
                        if(flag[j]==0)
                        {
                                if(min > demand[j])
                                {
                                        min=demand[j];
                                }
                        }
                }

  //              printf("end1=%d   end2=%d   total =%d\n",end1,end2,total_weight+min);

                if(total_weight+min>100)
                {
                        for(int k=end1;k<=end2;k++)
                        {
                                if(array[k]!=0)
                                        printf("route: %d ",array[k]);
                        }
                        printf("\n");
                        return i+1;
                }
        }
}

//end function 



int main()
{
        scanf("%d",&n);
        scanf("%d",&v);
        scanf("%d",&capacity);
        char str1[3],str2[3];
        for(int i=0;i<=n;i++)
        {
                for(int j=0;j<=n;j++)
                {
                        scanf("%d",&dis[i][j]);
                }
        }
        for(int i=1;i<=n;i++)
        {
                scanf("%d",&demand[i]);
        }
        //input taken 

        char str_temp[3];
        for(int i=1;i<=n;i++)
        {
                for(int j=i+1;j<=n;j++)
                {
                        arr[i][j]= (dis[0][i]+dis[0][j]-dis[i][j]);
                        length_comb+=1;
                        //integer to string conversion 
                        sprintf(str1,"%d",i);
                        sprintf(str2,"%d",j);
                        strcat(str1,str2);
                        comb[length_comb].value=arr[i][j];
                        comb[length_comb].index1=i;
                        comb[length_comb].index2=j;
                }
        }


        //checking 
        /*
        for(int i=0;i<length_comb;i++)
        {
                printf("i1=%d i2=%d =%d\n",comb[i].index1,comb[i].index2,comb[i].value);
        }*/
        //sort 
        int temp1,temp2;
        for(int j=0;j<length_comb;j++)
        {
                for(int i=0;i<length_comb-j;i++)
                {
                        if(comb[i].value<comb[i+1].value)
                        {
                                //swap
                                temp=comb[i].value;
                                temp1=comb[i].index1;
                                temp2=comb[i].index2;
                                comb[i].value=comb[i+1].value;
                                comb[i].index1=comb[i+1].index1;
                                comb[i].index2=comb[i+1].index2;
                                comb[i+1].value=temp;
                                comb[i+1].index1=temp1;
                                comb[i+1].index2=temp2;
                        }
                }
        }
        printf("After Sorting\n");
        for(int i=0;i<length_comb;i++)
        {
                printf("i1=%d i2=%d =%d\n",comb[i].index1,comb[i].index2,comb[i].value);
        }

        int len=0;
        while(len<length_comb)
        {        
                len=function(len,length_comb-1);
        }


        return 0;
}
