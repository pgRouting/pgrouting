#include "maximum_weighted_matching.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <vector>
#include <utility>

std::vector<std::pair<int,int>> maximumWeightedMatching(const std::vector<MWEdge>& edges, bool allow_partial)
{
    using namespace boost;
    typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t,double>> Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;

    // find max vertex id
    int n = 0;
    for (auto &e : edges) {
        if (e.source > n) n = e.source;
        if (e.target > n) n = e.target;
    }
    n += 1; // vertices start from 0

    Graph g(n);
    for (auto &e : edges)
        add_edge(e.source, e.target, e.weight, g);

    std::vector<Vertex> mate(num_vertices(g));
    edmonds_maximum_cardinality_matching(g, &mate[0]);

    std::vector<std::pair<int,int>> matching;
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        if (mate[*vi] != graph_traits<Graph>::null_vertex() && *vi < mate[*vi])
            matching.push_back({*vi, mate[*vi]});
    }

    return matching;
}

