#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>

int main() {
    using namespace boost;
    using namespace std;

    // Graph typedefs
    typedef adjacency_list<vecS, vecS, directedS,
            no_property,
            property<edge_capacity_t, long,
            property<edge_residual_capacity_t, long,
            property<edge_reverse_t, adjacency_list<>::edge_descriptor,
            property<edge_weight_t, long> > > > > Graph;

    typedef graph_traits<Graph>::edge_descriptor Edge;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;

    Graph g(4); // 4 nodes: 0,1,2,3

    // Add edges: (source, target, capacity, cost)
    auto add_edge_with_properties = [&](int u, int v, long capacity, long cost) {
        Edge e, rev;
        bool inserted;
        tie(e, inserted) = add_edge(u, v, g);
        tie(rev, inserted) = add_edge(v, u, g);

        // property maps
        property_map<Graph, edge_capacity_t>::type capacity_map = get(edge_capacity, g);
        property_map<Graph, edge_weight_t>::type weight_map = get(edge_weight, g);
        property_map<Graph, edge_reverse_t>::type rev_map = get(edge_reverse, g);
        property_map<Graph, edge_residual_capacity_t>::type res_cap_map = get(edge_residual_capacity, g);

        capacity_map[e] = capacity;
        capacity_map[rev] = 0; // reverse edge capacity = 0
        weight_map[e] = cost;
        weight_map[rev] = -cost;
        rev_map[e] = rev;
        rev_map[rev] = e;
        res_cap_map[e] = capacity;
        res_cap_map[rev] = 0;
    };

    add_edge_with_properties(0, 1, 5, 2);
    add_edge_with_properties(0, 2, 4, 4);
    add_edge_with_properties(1, 3, 3, 2);
    add_edge_with_properties(2, 3, 5, 3);

    Vertex s = 0, t = 3;

    // Run Boost min cost flow
    successive_shortest_path_nonnegative_weights(g, s, t);

    property_map<Graph, edge_capacity_t>::type capacity_map = get(edge_capacity, g);
    property_map<Graph, edge_weight_t>::type weight_map = get(edge_weight, g);
    property_map<Graph, edge_residual_capacity_t>::type res_cap_map = get(edge_residual_capacity, g);
    property_map<Graph, edge_reverse_t>::type rev_map = get(edge_reverse, g);

    cout << "Flow results:" << endl;
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        Edge e = *ei;
        long flow = capacity_map[e] - res_cap_map[e];
        if (flow > 0) {
            cout << "Edge " << source(e,g)+1 << "->" << target(e,g)+1
                 << " flow=" << flow
                 << ", cost=" << weight_map[e] << endl;
        }
    }

    cout << "Total cost: " << find_flow_cost(g) << endl;

    return 0;
}

