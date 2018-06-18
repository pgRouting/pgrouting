#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

using namespace boost;

template < typename Graph, typename ParentMap >
struct edge_writer
{
	edge_writer(const Graph & g, const ParentMap & p)
		: m_g(g), m_parent(p)
	{
	}

	template < typename Edge >
	void operator() (std::ostream & out, const Edge & e) const
	{
		out << "[label=\"" << get(edge_weight, m_g, e) << "\"";
		typename graph_traits < Graph >::vertex_descriptor
			u = source(e, m_g), v = target(e, m_g);
		if (m_parent[v] == u)
			out << ", color=\"black\"";
		else
			out << ", color=\"grey\"";
		out << "]";
	}
	const Graph & m_g;
	ParentMap m_parent;
};
template < typename Graph, typename Parent >
edge_writer < Graph, Parent >
make_edge_writer(const Graph & g, const Parent & p)
{
	return edge_writer < Graph, Parent >(g, p);
}

struct EdgeProperties {
	int weight;
};

int main()
{
	enum { a, b, c, d, e, N };
	char name[] = { 'A', 'B', 'C', 'D', 'E' };
	typedef std::pair < int, int >E;
	const int n_edges = 8;
	E edge_array[] = { E(a, b), E(a, c), E(b, c), E(b, d),
		E(b, e), E(d, c), E(d, b), E(e, d) };
	int weight[n_edges] = { -1, 4, 3, 2, 2, 5, 1, -4 };

	typedef adjacency_list < vecS, vecS, directedS,
		no_property, EdgeProperties> Graph;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	// VC++ can't handle the iterator constructor
	Graph g(N);
	for (std::size_t j = 0; j < n_edges; ++j)
		add_edge(edge_array[j].first, edge_array[j].second, g);
#else
	Graph g(edge_array, edge_array + n_edges, N);
#endif
	graph_traits < Graph >::edge_iterator ei, ei_end;
	property_map<Graph, int EdgeProperties::*>::type
		weight_pmap = get(&EdgeProperties::weight, g);
	int i = 0;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
		weight_pmap[*ei] = weight[i];

	std::vector<int> distance(N, (std::numeric_limits < short >::max)());
	std::vector<std::size_t> parent(N);
	for (i = 0; i < N; ++i)
		parent[i] = i;
	distance[a] = 0;

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	bool r = bellman_ford_shortest_paths
	(g, int(N), weight_pmap, &parent[0], &distance[0],
		closed_plus<int>(), std::less<int>(), default_bellman_visitor());
#else
	for (int j = 1; j <= N+1; j++)
	{
		bool r = bellman_ford_shortest_paths
		(g, j, weight_map(weight_pmap).distance_map(&distance[0]).
			predecessor_map(&parent[0]).root_vertex(0));
#endif
		std::cout << "Iteration == " << int(j) << "\n";
		if (r)
			for (i = 0; i < N; ++i)
				std::cout << name[i] << ": " << std::setw(3) << distance[i]
				<< " " << name[parent[i]] << std::endl;
		else
		{
			std::cout << "negative cycle" << std::endl;
			for (i = 0; i < N; ++i)
				std::cout << name[i] << ": " << std::setw(3) << distance[i]
				<< " " << name[parent[i]] << std::endl;
		}
	}
	

	while (1)
	{

	}
	return EXIT_SUCCESS;
}
