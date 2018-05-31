

// Boost DFS example on an undirected graph.
// Create a sample graph, traverse its nodes
// in DFS order and print out their values.

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <iostream>

#if 0
typedef std::map<vertex_t, boost::default_color_type> ColorMap;
typedef boost::associative_property_map<ColorMap> Color;




using ColorPropertyType = boost::property<boost::vertex_color_t, boost::default_color_type>;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> MyGraph;
//using Color_map_t =  boost::property_map<MyGraph, boost::vertex_color_t>;
typedef boost::property_map<MyGraph, boost::vertex_color_t> Color_map_t;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> MyGraph;

typedef boost::graph_traits<MyGraph>::vertex_descriptor MyVertex;
typedef boost::graph_traits<MyGraph>::edge_descriptor MyEdge;
#endif

class MyVisitor : public boost::default_dfs_visitor
{
public:
#if 0
  std::vector<boost::vertex_color_t>  &m_color;
  MyVisitor(std::vector<boost::vertex_color_t> &color) : m_color(color) {}
#endif

  void initialize_vertex(MyVertex v, const MyGraph& g) const
  {
#if 0
    if (v == 1) {
    discover_vertex(v, g);
    }
    cerr << "initialize_vertex" << v << endl;
    return;
#endif
  }
  void start_vertex(MyVertex v, const MyGraph& g) const
  {
       std::cout << ' ';
  }
  void discover_vertex(MyVertex v, const MyGraph& g) const
  {
//      const auto& colorMapGraph = get(boost::vertex_color_t(), g);
      std::cout << "(" << v
          << " color["
  //        << get(colorMapGraph, vertex(v, g))
          << "] ";
  }
  void examine_edge(MyEdge e, const MyGraph& g) const
  {
      //cerr << "examine_edge" << e << endl;
      return;
  }
  void tree_edge(MyEdge e, const MyGraph& g) const
  {
      //cerr << "tree_edge" << e << endl;
      return;
  }
  void back_edge(MyEdge e, const MyGraph& g) const
  {
      //cerr << "back_edge" << e << endl;
      return;
  }
  void forward_or_cross_edge(MyEdge e, const MyGraph& g) const
  {
      //cerr << "forward_or_cross_edge" << e << endl;
      return;
  }
  void finish_vertex(MyVertex v, const MyGraph& g) const
  {
      //const auto& colorMapGraph = get(boost::vertex_color_t(), g);
      std::cout << ' ' << v
          << " color["
        //  << get(colorMapGraph, vertex(v, g))
          << "])";
  }

};

int main()
{
  typedef std::pair < int, int >E;
  E edge_array[] = {
      E(1, 2), E(2, 1),
      E(3,2),
      E(4,3),
      E(2, 5), E(5, 2),
      E(3, 6),
      E(7, 8), E(8, 7),
      E(8, 5), E(5, 8),
      E(5, 6), E(6, 5),
      E(6, 9), E(9, 6),
      E(5, 10), E(10, 5),
      E(6,11),
      E(10,11),
      E(11,12),
      E(10,13), E(13,10),
      E(9,12), E(12,9),
      E(4, 9), E(9, 4),
      E(14,15), E(15,14),
      E(16,17), E(17,16),
  };

  MyGraph g(edge_array, edge_array + sizeof(edge_array) / sizeof(E), 18);
  std::vector<boost::default_color_type> vertex_color(boost::num_vertices(g));
  auto idmap = get(vertex_index, g);
  auto vcmap = make_iterator_property_map(vertex_color.begin(), idmap);
  MyGraph::vertex_descriptor const start = 0;

  boost::depth_first_search(g,
          boost::visitor(vis).color_map(vertex_color));

#if 0
    ColorMap colorMap;
    Color color(colorMap);
    MyVisitor vis();
    MyVertex start = 1;
  //std::vector<boost::vertex_color_t> colorMap(num_vertices(g), boost::white_color);



//  auto  color = boost::get(boost::vertex_color_t(), g);
//  Color_map_t colorMap = colorMap;



//MyVisitor vis(colorMap);
  return 0;
#endif
}


