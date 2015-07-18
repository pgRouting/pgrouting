/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#if defined(_MSC_VER) && _MSC_VER < 1600
#define ELOG_H
#endif
#include <boost/lexical_cast.hpp>
#include <boost/config.hpp>
//#include <iostream>
//#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
//#include <boost/graph/johnson_all_pairs_shortest_paths.hpp>
#include "string"
#include "apsp.h"
#include "set"
#include "map"
using namespace std;
using namespace boost;

#ifdef _MSC_VER
#undef max;
#endif // _MSC_VER

/*
//    FIXME: use this to avoid heap allocation ?

void* operator new(size_t size)
{
return palloc(size);
}

void operator delete(void *p)
{
    pfree(p);
}

*/


typedef adjacency_list<vecS, vecS, directedS, no_property,property<edge_weight_t, float, property<edge_weight2_t, float> > > Graph;

int 
boost_apsp(edge_t *edges,
            unsigned int edge_count,
            const int node_cnt,   //TODO deal with this later - node_count
            bool directed,
            bool has_reverse_cost, //TODO this is not implemented
            apsp_element_t **pair,
            int *pair_count,
            char **err_msg)
{
try {
    string msg;
    msg.append("\nStarting with boost_apsp");
    msg.append("\nEdge count is: ");
    msg.append(boost::lexical_cast<std::string>(edge_count));

    set<int> vertices;
    typedef std::pair<int,int> Edge;
    map<int,int> vertex_map;
    map<int,int> echo_map;
    int map_counter = 0;

    //cout<<edge_count;
    Edge *edge_array = (Edge *) malloc(sizeof(Edge) * (edge_count));   
    float *weights = (float*) malloc(sizeof(float) * (edge_count));


    for(int i=0;i<edge_count;i++)
    {
        weights[i] = edges[i].cost;
        vertices.insert(edges[i].source);
        vertices.insert(edges[i].target);

        if(vertex_map.find(edges[i].source) == vertex_map.end())
        {
          vertex_map[edges[i].source] = map_counter;
          echo_map[map_counter++] = edges[i].source;
        }
        if(vertex_map.find(edges[i].target) == vertex_map.end())
        {
          vertex_map[edges[i].target] = map_counter;
          echo_map[map_counter++] = edges[i].target;
        }

        edge_array[i].first = vertex_map.find(edges[i].source)->second;
        edge_array[i].second = vertex_map.find(edges[i].target)->second;

    }

    msg.append("\nPopulated edge_array and vertices");
    //*err_msg = (char*)msg.c_str();
    /*for(int i=0;i<edge_count;i++)
    {
        cout<<edge_array[i].first<<" "<<edge_array[i].second<<"\t";
        cout<<weights[i]<<endl;
    }*/
    int node_count = vertices.size();

    msg.append("\nNode_Count is: ");
    msg.append(boost::lexical_cast<std::string>(node_count));


    Graph g(edge_array, edge_array + edge_count, weights, node_count);

    std::vector<float> d(node_count, std::numeric_limits<float>::max());
    float ** D = (float**) malloc(sizeof(float*) * node_count);

    for(int i = 0; i < node_count; i++)
        {
        D[i] = (float*)malloc(node_count * sizeof(float));
        //D[i] = (double*)malloc(node_count * sizeof(double));
        if(D[i] == NULL)
            {
            //fprintf(stderr, "out of memory\n");
            msg.append("\nout of memory");
            //*err_msg = (char*)msg.c_str();
            //*err_msg = (char*) "out of memory";

            // TODO: free memory cause this is a huge leak
            return -1;
            }
        }


    msg.append("\nNow calling actual boost function");

    floyd_warshall_all_pairs_shortest_paths(g, D, distance_map(&d[0]));

    //msg.append("\nactual boost function call successfull");
    //*err_msg = (char*)msg.c_str();

    /*for(int i=0;i<node_count;i++) {
        for(int j=0;j<node_count;j++) {
            std::cout<<D[i][j]<<" ";
        }
        std::cout<<std::endl;
    } */

    *pair = (apsp_element_t *) malloc(sizeof(apsp_element_t) * (node_count*node_count));
    //pair_count = (int*)malloc(sizeof(int));
    *pair_count = 0;

    graph_traits<Graph>::vertex_iterator vi1, vi_end1, vi2, vi_end2;

    boost::tie(vi1,vi_end1)= boost::vertices(g);

    for(int i = 0; vi1 != vi_end1; vi1++,i++)
    {

        boost::tie(vi2,vi_end2) = boost::vertices(g);
        for(int j = 0; vi2 != vi_end2; vi2++,j++)
        {
            // *SEW* added the follow if to skip over these values
            if (D[i][j] == std::numeric_limits<float>::max()) continue;

            (*pair)[*pair_count].src_vertex_id = echo_map.find(*vi1)->second;
            (*pair)[*pair_count].dest_vertex_id = echo_map.find(*vi2)->second;

            (*pair)[*pair_count].cost = (float8) D[i][j];
            (*pair_count)++;
            //cout<<" "<<(*pair_count);
        }
    }
    //cout<<"Number of pairs: "<<(*pair_count)<<endl;

    return EXIT_SUCCESS;
  }
  catch(...) {
     *err_msg = (char *) "Unknown exception caught!";
     return -1;
  }
}

#ifdef JUNK
/*
void static_warshall()
{

      const int V = 6;
    const int E = 4;
    typedef std::pair<int,int> Edge;

    */
    /*Edge edge_array[ ] =
    { Edge(169,153), Edge(171,172), Edge(171,176), Edge(169,1469)};

    //float weights[] = { 2034374301,215378334,506308873,186331927};
    float weights[] = { 20,21,50,18};*/

    /*Edge edge_array[ ] =
    { Edge(0,1), Edge(0,2), Edge(0,3), Edge(0,4), Edge(0,5),
    Edge(1,2), Edge(1,5), Edge(1,3), Edge(2,4), Edge(2,5),
    Edge(3,2), Edge(4,3), Edge(4,1), Edge(5,4) };


    const int E = sizeof (edge_array)/sizeof (Edge);
    cout<<"Number of edges = "<<E<<endl;
    int weights[] = { 0, 0, 0, 0, 0, 3, -4, 8, 1, 7, 4, -5, 2, 6 };    */
/*
    Edge edge_array[ ] =
    { Edge(0,3), Edge(1,2), Edge(1,4), Edge(0,5)};



    cout<<"Number of edges = "<<E<<endl;
    int weights[] = { 20,21,50,18 };    


    Graph g(edge_array, edge_array + E, weights, V);

    std::vector<int> d(V, std::numeric_limits<int>::max());
    int ** D = (int**) malloc(sizeof(int*) * V);

    for(int i = 0; i < V; i++)
        {
        D[i] = (int*)malloc(V * sizeof(int));
        if(D[i] == NULL)
            {
            fprintf(stderr, "out of memory\n");
            return ;
            }
        }



    floyd_warshall_all_pairs_shortest_paths(g, D, distance_map(&d[0]));
//    johnsons_all_pairs_shortest_paths(g,D,distance_map(&d[0]));

    for(int i=0;i<V;i++)
    {
        for(int j=0;j<V;j++)
        {
            std::cout<<D[i][j]<<" ";

        }
        std::cout<<std::endl;
    } 
}
*/
/*
int main()
{
  static_warshall();
    typedef std::pair<int,int> Edge;
    unsigned int edge_count = 4;
    edge_t *edges = (edge_t*) malloc(sizeof(edge_t)*edge_count);
    /*Edge edge_array[ ] =
    { Edge(0,1), Edge(0,2), Edge(0,3), Edge(0,4), Edge(0,5),
    Edge(1,2), Edge(1,5), Edge(1,3), Edge(2,4), Edge(2,5),
    Edge(3,2), Edge(4,3), Edge(4,1), Edge(5,4) };

    float weights[] = { 0, 0, 0, 0, 0, 3, -4, 8, 1, 7, 4, -5, 2, 6 };    */
  /*  Edge edge_array[ ] =
    { Edge(169,153), Edge(171,172), Edge(171,176), Edge(169,1469)};

    //float weights[] = { 2034374301,215378334,506308873,186331927};
    float weights[] = { 20,21,50,18};
    for(int i=0;i<edge_count;i++)
    {
    edges[i].source = edge_array[i].first;
    edges[i].target = edge_array[i].second;
    edges[i].cost = weights[i];
    }


    unsigned int num_nodes = 6;
    apsp_element_t **pair;
    int pair_count;
    char **err_msg;
    bool directed;
    bool has_reverse_cost;

    //static_warshall();

    boost_apsp(edges, edge_count,0,//num_nodes,
           directed, has_reverse_cost,
           pair, &pair_count, err_msg);

    cout<<endl<<endl;

    cout<<"pairs: "<< (pair_count)<<endl;
    for(int i=0;i<pair_count;i++)
    {
    cout<<(*pair)[i].src_vertex_id<<" "<<(*pair)[i].dest_vertex_id<<" "<<(*pair)[i].cost;
    cout<<endl;

    }
    return 1;
}*/
#endif
