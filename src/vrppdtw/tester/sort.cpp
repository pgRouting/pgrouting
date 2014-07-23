#include<stdio.h>
int main()
{
        int arr[100];
        int len;
        scanf("%d",&len);
        for(int i=0;i<len;i++)
        scanf("%d",&arr[i]);

        for(int i=0;i<len;i++)
        {
                printf("arr[%d]=%d\n",i,arr[i]);
        }
        int temp;
        for(int i=0;i<len;i++)
        {
                for(int j=0;j<len;j++)
                {
                        if(arr[i]>arr[j])
                        {
                                temp=arr[i];
                                arr[i]=arr[j];
                                arr[j]=temp;
                        }
                }
        }
        for(int i=0;i<len;i++)
        {
                printf("arr[%d]=%d\n",i,arr[i]);
        }
        return 0;
}
