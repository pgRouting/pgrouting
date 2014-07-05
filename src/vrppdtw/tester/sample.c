#include<stdio.h>
int main()
{
        int arr[100];
        int i,j,swap;
        for(i=0;i<10;i++)
        {
                scanf("%d",&arr[i]);
        }
        for(i=0;i<10;i++)
        {
                for(j=0;j<10;j++)
                {
                        if(arr[i]>arr[j])
                        {
                                swap=arr[i];
                                arr[i]=arr[j];
                                arr[j]=swap;
                        }
                }
        }
        for(i=0;i<10;i++)
        {
                printf("arr[%d]=%d\n",i,arr[i]);
        }
        return 0;
}
