#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/range/irange.hpp>
#include <boost/pending/indirect_cmp.hpp>

#include <iostream>

#include "../visitors/dfs_visitor.hpp"


int main() {
    typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS> G;
    typedef typename boost::graph_traits <G>::vertex_descriptor V;
    typedef typename boost::graph_traits <G>::edge_descriptor E;

    enum { v1, v2, v3, v4, v5, v6, v7, v8, v9, N };


    typedef std::pair < V, V > Edge;
    Edge edge_array[] = { Edge(v1, v2), Edge(v2, v1), Edge(v3, v2), Edge(v4, v3),
                          Edge(v2, v5), Edge(v5, v2), Edge(v3, v6), Edge(v7, v8),
                          Edge(v8, v7), Edge(v5, v8), Edge(v8, v5), Edge(v5, v6),
                          Edge(v6, v5), Edge(v6, v9), Edge(v9, v6), Edge(v9, v4),
                          Edge(v4, v9), Edge(v6, v8)
                        };

    G graph(N);
    for (std::size_t j = 0; j < sizeof(edge_array) / sizeof(Edge); ++j)
        add_edge(edge_array[j].first, edge_array[j].second, graph);

    using dfs_visitor = pgrouting::visitors::Dfs_visitor<V, E, G>;

    V root = v2;

    // Here we use std::vector as exterior property storage.
    std::vector<boost::default_color_type> colors(boost::num_vertices(graph));

    auto vis =  dfs_visitor(root, 2, colors, graph);
    auto i_map = get(boost::vertex_index, graph);
    auto color_map = boost::make_iterator_property_map(colors.begin(), i_map);

    boost::depth_first_search(graph, vis, color_map, root);

#if 0
    boost::depth_first_search(graph,
                    boost::root_vertex(v1)
                    .visitor(vis)
                    .color_map(color_map)
    );
#endif

    return 0;
}
