
#ifndef _PDP_H
#define _PDP_H

int VehicleCount;
int Capacity;
int Speed;

typedef struct Depot{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
}depot;



typedef struct Customer{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
        int Ddist;
        int P;
        int D;
}customer;


typedef struct Pickup{
        int id;
        int Pid;
        int Ddist;
        int Did;
        int checked;
}pickup; 



typedef struct Delivery{
        int id;
        int Did;
        int Ddist;
        int Pid;
}delivery; 

int CalculateDistance(int x1,int y1,int x2,int y2)
{
        return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}

depot ScanDepot(depot d)
{

        scanf("%d",&d.id);
        scanf("%d",&d.x);
        scanf("%d",&d.y);
        scanf("%d",&d.demand);
        scanf("%d",&d.Etime);
        scanf("%d",&d.Ltime);
        scanf("%d",&d.Stime);
        scanf("%d",&d.Pindex);
        scanf("%d",&d.Dindex);
        return d;
}

customer ScanCustomer(int id,customer c,depot d)
{
        c.id=id;
        scanf("%d",&c.x);
        scanf("%d",&c.y);
        scanf("%d",&c.demand);
        scanf("%d",&c.Etime);
        scanf("%d",&c.Ltime);
        scanf("%d",&c.Stime);
        scanf("%d",&c.Pindex);
        scanf("%d",&c.Dindex);
        if(c.Pindex==0)
        { 
                c.P=1;
                c.D=0;
        }  
        else if(c.Dindex==0)
        {
                c.D=1;
                c.P=0;
        }
        c.Ddist=CalculateDistance(c.x,c.y,d.x,d.y);
        return c;
}


//temp variables
int temp=0;

int CustomerLength=0;
int OrderLength=0;

typedef struct cs{
        int twv;
        int cv;
        int dis;
        int path[1000];
        int order[1000];
        int path_length;
}State;

// Class 
class Route
{
        public:
                int twv;
                int cv;
                int dis;
                int path[1000];
                int order[1000];
                int path_length;
                Route()
                {
                        twv=0;
                        cv=0;
                        dis=0;
                        path_length=0;
                        for(int i=0;i<1000;i++)
                        {
                                path[i]=0;
                                order[i]=0;
                        }
                }

                State  append(customer *c, Pickup p, depot d,int CustomerLength, int PickupLength, State S)
                {
                        //Save State;
                        S.twv=twv;
                        S.cv=cv;
                        S.dis=dis;
                        S.path_length=path_length;
                        for(int i=0;i<path_length;i++)
                        {
                                S.path[i]=path[i];
                                S.order[i]=order[i];
                        }

                        // Insert Order 
                                printf("Path_length=%d\n",path_length);
                                path[path_length]=p.Pid;
                                printf("Pid=%d\n",p.Pid);
                                order[path_length]=p.id;
                                path[path_length+1]=p.Did;
                                printf("Did=%d\n",p.Did);
                                order[path_length+1]=p.id;
                                path_length+=2;

                        return S;
                }

                void remove( State S)
                {
                        printf("Remove Called\n");
                        twv=S.twv;
                        cv=S.cv;
                        dis=S.dis;
                        path_length=S.path_length;
                        for(int i=0;i<path_length;i++)
                        {
                                path[i]=S.path[i];
                                order[i]=S.order[i];
                        }
                        return;
                }
                void path_print(int path[],int path_length)
                {
                        printf("Path  ");
                        for(int i=0;i<path_length;i++)
                        {
                                printf("%d ",path[i]);
                        }
                        printf("\n");
                        return;
                }

                void print()
                {
                        printf("TWV=%d\n",twv);
                        printf("dis=%d\n",dis);
                        printf("cv=%d\n",cv);
                        printf("Path ");
                        for(int i=0;i<path_length;i++)
                        {
                                printf("%d ",path[i]);
                        }
                        printf("\n");
                        return;
                }
};




#endif










