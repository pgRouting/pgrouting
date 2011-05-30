#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_V 10000


void generate_random_graph(int V , int T, int* time_intervals , int W, int outDegree)
{	
	srand((unsigned)time(NULL)); 
	
	int adj[MAX_V][MAX_V];
	int c[MAX_V];
	int start[MAX_V];
	int end[MAX_V];
	int E = 0;
	int src = 0;
	int maxOut = 0;
	
	for(int i = 0 ; i < V ; i++)
	{
		c[i] = 0;
		
		
		//To keep something like sliding window for outgoing edges
		if( i < outDegree / 2)
		{
			start[i] = 0;
			end[i] = outDegree; 
		}
		else
		if( i > V - outDegree / 2)
		{
			start[i] = V - outDegree;
			end[i] = V;
		}
		else
		{
			start[i] = i - outDegree / 2;
			end[i] = i + outDegree / 2;
		}
		for(int j = start[i] ; j < end[i] ; j++)
		{
			if(i != j)
			{
				adj[i][j] = rand() % 2;
				if(adj[i][j] == 1)
					c[i]++;
			}
			else
				adj[i][j] = 0;
		}
		
		
		E += c[i];
		if(c[i] > maxOut)
		{
			src = i;
			maxOut = c[i];
		}
	}
	
	cout << V << " " << E << " " << T << endl;
	
	for(int i = 0 ; i < V ; i++)
	{
		cout << c[i] << " ";
		for(int j = start[i] ; j < end[i] ; j++)
		{
			if(adj[i][j] == 1)
				cout << j << " ";
		}
		cout << endl;
	}
	
	for(int i = 0 ; i < E ; i++)
	{
		for(int j = 0 ; j < T ; j++)
		{
			cout << i << " " << time_intervals[j] << " " << rand()%W << endl;
		}
	}
	cout << src << endl;
}


int main()
{
	int N, T , outDegree, W;
	cin >> N;
	cin >> T;
	cin >> W;
	cin >> outDegree;
	int time_intervals[24]; //= {0,20,40,60,80};
	for(int i = 0 ; i < T ; i++)
		cin >> time_intervals[i];
	generate_random_graph(N , T , time_intervals , W, outDegree) ;
	
	
}
