//
//=======================================================================
// Copyright (c) 2004 Kristopher Beevers
//               2007 Anton A. Patrushev, Orkney Inc.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef BOOST_GRAPH_SHOOTING_STAR_SEARCH_HPP
#define BOOST_GRAPH_SHOOTING_STAR_SEARCH_HPP

#define MAX_RULE_LENGTH 5

#include <functional>
#include <boost/limits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/pending/mutable_queue.hpp>
#include <boost/pending/relaxed_heap.hpp>
#include <shooting_star_relax.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/graph/exception.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <queue>
#include "edge_visitors.hpp"

template <class Edge>
struct EdgeRankCompare 
{
  bool operator()(const Edge& LHS, const Edge& RHS) 
  {
    return (LHS.rank < RHS.rank);
  }
		
};

template <class Edge, class Container, class Cmp>
class edge_queue : public std::priority_queue<Edge, Container, Cmp>
{
protected:
  using std::priority_queue< Edge, Container, Cmp >::c;
  using std::priority_queue< Edge, Container, Cmp >::comp;
    
public:
  void sort()
  {
    sort_heap(c.begin(), c.end(), comp);
  }   
};

namespace boost 
{
  
  template <class Heuristic, class Graph>
  struct AStarHeuristicConcept 
  {
    void constraints()
    {
      function_requires< CopyConstructibleConcept<Heuristic> >();
      h(u);
    }
    Heuristic h;
    typename graph_traits<Graph>::vertex_descriptor u;
  };
  
    

  template <class Graph, class CostType>
  class astar_heuristic : public std::unary_function<
    typename graph_traits<Graph>::vertex_descriptor, CostType>
  {
  public:
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    astar_heuristic() {}
    CostType operator()(Vertex u) { return static_cast<CostType>(0); }
  };
  
  
  template <class Visitor, class Graph>
  struct ShootingStarVisitorConcept {
    void constraints()
    {
      function_requires< CopyConstructibleConcept<Visitor> >();
      vis.initialize_vertex(u, g);
      vis.discover_vertex(u, g);
      vis.examine_vertex(u, g);
      vis.examine_edge(e, g, e_max_id);
      vis.black_target(e, pe, s, g, e_max_id);
      vis.gray_target(e, pe, s, g, e_max_id);
      vis.finish_vertex(u, g);
      
      vis.tree_edge(e, pe, s, g, e_max_id);

      vis.initialize_edge(e, g);
      vis.discover_edge(e, g);
      vis.finish_edge(e, g);

    }
    Visitor vis;
    Graph g;
    typename graph_traits<Graph>::vertex_descriptor u;
    typename graph_traits<Graph>::edge_descriptor e, pe, s;
    int e_max_id;
  };
  
  
  // Main visitor function.
  // It decides what to do with an edge of certain color
  
  template <class IncidenceGraph, class Buffer, class BFSVisitor,
            class ColorMap, class EdgeColorMap/*, class HistoryMap*/>
  void shooting_star_edges_visit (const IncidenceGraph& g,
		            typename graph_traits<IncidenceGraph>::edge_descriptor s,
			    Buffer& Q, BFSVisitor vis, ColorMap color, EdgeColorMap edge_color, 
			    int e_max_id)
  {
    function_requires< IncidenceGraphConcept<IncidenceGraph> >();
    typedef graph_traits<IncidenceGraph> GTraits;
    typedef typename GTraits::vertex_descriptor Vertex;
    typedef typename GTraits::edge_descriptor Edge;
    function_requires< ShootingStarVisitorConcept<BFSVisitor, IncidenceGraph> >();
    function_requires< ReadWritePropertyMapConcept<EdgeColorMap, Edge> >();
    
    typedef typename property_traits<ColorMap>::value_type ColorValue;
    typedef color_traits<ColorValue> Color;

    typedef typename property_traits<EdgeColorMap>::value_type EdgeColorValue;
    typedef color_traits<EdgeColorValue> EdgeColor;

    typename GTraits::out_edge_iterator ei, ei_end;

    // Paint source edge gray
    put(edge_color, s, EdgeColor::gray());
    vis.discover_edge(s, g);
    
    // Enqueue the source edge
    Q.push(s);
    
    // While the queue is not empty
    while (! Q.empty()) 
    {
      // Get an edge from the top
      Edge e = Q.top();  Q.pop();            
      
      // Examine the edge
      vis.examine_edge(e, g, e_max_id);
      
      // For all adjacent edges for the current one
      for (tie(ei, ei_end) = out_edges(target(e, g), g); ei != ei_end; ++ei) 
      {
        // Get a color
        EdgeColorValue e_color = get(edge_color, *ei);

	// Discover the edge and paint it grey 
	// and enqueue it if it was white
	if (e_color == EdgeColor::white()) 
	{         
	  vis.tree_edge(*ei, e, s, g, e_max_id);
	  put(edge_color, *ei, EdgeColor::gray());   
	  vis.discover_edge(*ei, g);
          Q.push(*ei);
        } 
	// or execute appropriate function 
	// if it was grey or black
	else 
	{                                     
	  vis.non_tree_edge(*ei, g);
          if (e_color == EdgeColor::gray())
	  {         
	    vis.gray_target(*ei, e, s, g, e_max_id);
          }
	  else
	  {                                      
	    vis.black_target(*ei, e, s, g, e_max_id);
	  }
        }
	
      } // end for

      // Finally paint the parent edge black
      put(edge_color, e, EdgeColor::black());        
      vis.finish_edge(e, g);
      
    } // end while
  }
																							      
  
  template <class Visitors = null_visitor>
  class shooting_star_visitor : public bfs_visitor<Visitors> 
  {
    public:
      shooting_star_visitor() {}
      shooting_star_visitor(Visitors vis)
        : bfs_visitor<Visitors>(vis) {}
  
      template <class Edge, class Graph>
      void edge_relaxed(Edge e, Graph& g) 
      {
        invoke_visitors(this->m_vis, e, g, on_edge_relaxed());
      }
      template <class Edge, class Graph>
      void edge_not_relaxed(Edge e, Graph& g) 
      {
        invoke_visitors(this->m_vis, e, g, on_edge_not_relaxed());      
      }
      template <class Edge, class Graph>
      void initialize_edge(Edge e, Graph& g) 
      {
        invoke_visitors(this->m_vis, e, g, on_initialize_edge());      
      }
    
    private:
      template <class Edge, class Graph>
      void tree_edge(Edge e, Edge pe, Edge s, Graph& g) {}
      template <class Edge, class Graph>
      void non_tree_edge(Edge e, Graph& g) {}
  };

  template <class Visitors>
  shooting_star_visitor<Visitors>
  make_shooting_star_visitor(Visitors vis) 
  {
    return shooting_star_visitor<Visitors>(vis);
  }
  
  typedef shooting_star_visitor<> default_shooting_star_visitor;
  

  namespace detail {

    // Shooting* visitor
    // based on BFS visitor concept from BGL    
    template <class AStarHeuristic, class UniformCostVisitor,
              class UpdatableQueue, class PredecessorMap,
              class CostMap, 
	      class DistanceMap, class WeightMap,
	      class EdgeMap,
              class ColorMap, class EdgeColorMap,
	      class BinaryFunction,
              class BinaryPredicate>
    struct shooting_star_bfs_visitor
    {
      
      typedef typename property_traits<CostMap>::value_type C;
      typedef typename property_traits<ColorMap>::value_type ColorValue;
      typedef color_traits<ColorValue> Color;

      typedef typename property_traits<EdgeColorMap>::value_type EdgeColorValue;
      typedef color_traits<ColorValue> EdgeColor;

      typedef typename property_traits<DistanceMap>::value_type distance_type;
      
      shooting_star_bfs_visitor(AStarHeuristic h, UniformCostVisitor vis,
                        UpdatableQueue& Q, PredecessorMap& p,
                        CostMap c, DistanceMap d, WeightMap w, EdgeMap em,
                        ColorMap col, EdgeColorMap ecol, //HistoryMap his, 
			BinaryFunction combine,
                        BinaryPredicate compare, C zero)
        : m_h(h), m_vis(vis), m_Q(Q), m_predecessor(p), m_cost(c),
          m_distance(d), m_weight(w), m_edge(em), m_color(col), 
	  m_ecolor(ecol), //m_history(his),
          m_combine(combine), m_compare(compare), m_zero(zero) {}

      shooting_star_bfs_visitor(const shooting_star_bfs_visitor &v)
        : m_h(v.m_h), m_vis(v.m_vis), m_Q(v.m_Q), m_predecessor(v.m_predecessor), m_cost(v.m_cost),
          m_distance(v.m_distance), m_weight(v.m_weight), m_edge(v.m_edge), m_color(v.m_color), 
	  m_ecolor(v.m_ecolor), //m_history(his),
          m_combine(v.m_combine), m_compare(v.m_compare), m_zero(v.m_zero) {}
	  
      ~shooting_star_bfs_visitor() {}
      
      template <class Vertex, class Graph>
      void initialize_vertex(Vertex u, Graph& g) 
      {
        m_vis.initialize_vertex(u, g);
      }
      template <class Edge, class Graph>
      void initialize_edge(Edge e, Graph& g) 
      {
        m_vis.initialize_edge(e, g);
      }
      template <class Vertex, class Graph>
      void discover_vertex(Vertex u, Graph& g) 
      {
        m_vis.discover_vertex(u, g);
      }
      template <class Edge, class Graph>
      void discover_edge(Edge e, Graph& g) 
      {
        m_vis.discover_vertex(e, g);
      }
      template <class Vertex, class Graph>
      void examine_vertex(Vertex u, Graph& g) 
      {
        m_vis.examine_vertex(u, g);
      }
      template <class Vertex, class Graph>
      void finish_vertex(Vertex u, Graph& g) 
      {
        m_vis.finish_vertex(u, g);
      }
      template <class Edge, class Graph>
      void examine_edge(Edge e, Graph& g, int e_max_id) 
      { 
        if (m_compare(get(m_weight, e), m_zero))
          throw negative_edge();
        m_vis.examine_edge(e, g, e_max_id);
      }
      template <class Edge, class Graph>
      void non_tree_edge(Edge, Graph&) {}
      
      template <class Edge, class Graph>
      void finish_edge(Edge e, Graph& g) 
      {
        m_vis.finish_edge(e, g);
      }
      
      
      template <class Edge, class Graph>
      void tree_edge(Edge e, Edge pe, Edge s, Graph& g, int e_max_id) 
      {
        m_decreased = relax(e, pe, s, g, m_weight, m_edge, m_predecessor, m_distance,
                            m_cost, m_combine, m_compare, e_max_id);
    
        if(m_decreased) 
	{
          m_vis.edge_relaxed(e, g);
          
	  put(m_cost, e,
              m_combine(get(m_distance, e),
                        m_h(e)));

        } 
	else
          m_vis.edge_not_relaxed(e, g);
      }
      
      
      template <class Edge, class Graph>
      void gray_target(Edge e, Edge pe, Edge s, Graph& g, int e_max_id) 
      {
        m_decreased = relax(e, pe, s, g, m_weight, m_edge, m_predecessor, m_distance,
                            m_cost, m_combine, m_compare, e_max_id);

        if(m_decreased) 
	{
	  put(m_cost, e,
              m_combine(get(m_distance, e),
                        m_h(e)));
			
          m_Q.update(e);
	  	  
          m_vis.edge_relaxed(e, g);
        } 
	else
          m_vis.edge_not_relaxed(e, g);
      }
      
            
      template <class Edge, class Graph>
      void black_target(Edge e, Edge pe, Edge s, Graph& g, int e_max_id) 
      {

        m_decreased = relax(e, pe, s, g, m_weight, m_edge, m_predecessor, m_distance,
                            m_cost, m_combine, m_compare, e_max_id);

        if(m_decreased) 
	{
          m_vis.edge_relaxed(e, g);
	  put(m_cost, e, m_combine(get(m_distance, e), m_h(e)));
          
          m_Q.push(e);
          put(m_ecolor, e, EdgeColor::gray());
          m_vis.black_target(e, g);
        } 
	else
          m_vis.edge_not_relaxed(e, g);
      }  

      AStarHeuristic m_h;
      UniformCostVisitor m_vis;
      UpdatableQueue& m_Q;
      PredecessorMap& m_predecessor;
      CostMap m_cost;
      EdgeMap m_edge;
      DistanceMap m_distance;
      WeightMap m_weight;
      ColorMap m_color;
      EdgeColorMap m_ecolor;
      BinaryFunction m_combine;
      BinaryPredicate m_compare;
      bool m_decreased;
      C m_zero;      
    };
    
  } // namespace detail

  template <typename VertexAndEdgeListGraph, typename AStarHeuristic,
            typename ShootingStarVisitor, typename PredecessorMap,
            typename CostMap, 
	    typename DistanceMap,
            typename WeightMap, 
	    typename EdgeMap,
	    typename ColorMap, typename EdgeColorMap,
	    typename IndexMap,
            typename CompareFunction, typename CombineFunction,
            typename CostInf, typename CostZero>
  inline void
  shooting_star_search_no_init
    (VertexAndEdgeListGraph &g,
     typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor s,
     AStarHeuristic h, ShootingStarVisitor vis,
     PredecessorMap &predecessor, CostMap cost,
     DistanceMap distance, WeightMap weight, EdgeMap edge_map,
     ColorMap color, EdgeColorMap edge_color,
     IndexMap index_map, CompareFunction compare, CombineFunction combine,
     CostInf inf, CostZero zero, int e_max_id)
  {
    typedef indirect_cmp<CostMap, CompareFunction> IndirectCmp;
    IndirectCmp icmp(cost, compare);
  
    typedef typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor
      Edge;
    
    // Queue to store the list of edges to examine.
    // I really hate this queue for what it does with the memory sometimes.
    //
    //And by the way...
    //
    //This place is for rent :)
    //
    typedef mutable_queue<Edge, std::vector<Edge>, IndirectCmp, IndexMap>
      MutableQueue;
    MutableQueue Q(num_edges(g), icmp, index_map);

    detail::shooting_star_bfs_visitor<AStarHeuristic, ShootingStarVisitor,
        MutableQueue, PredecessorMap, CostMap, DistanceMap,
	WeightMap, EdgeMap, ColorMap, EdgeColorMap, /*HistoryMap,*/ CombineFunction, CompareFunction>
      bfs_vis(h, vis, Q, predecessor, cost, distance, weight,
              edge_map, color, edge_color, combine, compare, zero);
  
    shooting_star_edges_visit(g, s, Q, bfs_vis, color, edge_color, e_max_id);
  }
  
  // Non-named parameter interface
  template <typename VertexAndEdgeListGraph, typename AStarHeuristic,
            typename ShootingStarVisitor, typename PredecessorMap,
            typename CostMap, typename DistanceMap,
            typename WeightMap, typename EdgeMap, 
	    typename IndexMap,
            typename ColorMap, typename EdgeColorMap,
	    //typename HistoryMap,
            typename CompareFunction, typename CombineFunction,
            typename CostInf, typename CostZero>
  inline void
  shooting_star_search
    (VertexAndEdgeListGraph &g,
     typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor s,
     AStarHeuristic h, ShootingStarVisitor vis,
     PredecessorMap &predecessor, CostMap cost,
     DistanceMap distance, WeightMap weight,
     EdgeMap edge_map, IndexMap index_map, ColorMap color, EdgeColorMap edge_color,
     CompareFunction compare, CombineFunction combine,
     CostInf inf, CostZero zero, int e_max_id)
  {
    
    typedef typename property_traits<ColorMap>::value_type ColorValue;
    typedef color_traits<ColorValue> Color;
    
    typedef typename property_traits<EdgeColorMap>::value_type EdgeColorValue;
    typedef color_traits<EdgeColorValue> EdgeColor;

    typename graph_traits<VertexAndEdgeListGraph>::vertex_iterator ui, ui_end;
    typename graph_traits<VertexAndEdgeListGraph>::edge_iterator ei, ei_end;

    for (tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui) 
    {
      vis.initialize_vertex(*ui, g);
    }

    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) 
    {
      put(distance, *ei, inf);
      put(edge_color, *ei, EdgeColor::white());
      predecessor[g[*ei].id] = *ei;
      put(cost, *ei, inf);
    }

    put(distance, s, zero);

    put(cost, s, h(s));

    shooting_star_search_no_init
      (g, s, h, vis, predecessor, cost, distance, weight, edge_map,
       color, edge_color, index_map, compare, combine, inf, zero, e_max_id);
    
  }
  
  namespace detail 
  {
    template <class VertexAndEdgeListGraph, class AStarHeuristic,
              class CostMap, class DistanceMap, class WeightMap, class EdgeMap,
              class IndexMap, 
	      class PredecessorMap,
	      class ColorMap, class EdgeColorMap, 
	      class Params>
    inline void
    shooting_star_dispatch2
      (VertexAndEdgeListGraph& g,
       typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor s,
       AStarHeuristic h, CostMap cost, DistanceMap distance,
       WeightMap weight, EdgeMap edge_map, IndexMap index_map, 
       PredecessorMap& predecessor, ColorMap color,
       EdgeColorMap edge_color, const Params& params,
       int e_max_id)
    {
      dummy_property_map p_map;
      typedef typename property_traits<CostMap>::value_type C;
      shooting_star_search
        (g, s, h,
         choose_param(get_param(params, graph_visitor),
                      make_shooting_star_visitor(null_visitor())),
	 predecessor,
         cost, distance, weight, edge_map, index_map, color, edge_color, //history,
         choose_param(get_param(params, distance_compare_t()),
                      std::less<C>()),
         choose_param(get_param(params, distance_combine_t()),
                      closed_plus<C>()),
         choose_param(get_param(params, distance_inf_t()),
                      std::numeric_limits<C>::max BOOST_PREVENT_MACRO_SUBSTITUTION ()),
         choose_param(get_param(params, distance_zero_t()),
                      C()),
	 e_max_id
	);
    }
  
    template <class VertexAndEdgeListGraph, class AStarHeuristic,
              class CostMap, class DistanceMap, class WeightMap,
              class EdgeMap, class IndexMap, 
	      class PredecessorMap,
	      class ColorMap, class EdgeColorMap,
	      class Params>
    inline void
    shooting_star_dispatch1
      (VertexAndEdgeListGraph& g,
       typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor s,
       AStarHeuristic h, CostMap cost, DistanceMap distance,
       WeightMap weight, EdgeMap edge_map, IndexMap index_map, PredecessorMap& predecessor, 
       ColorMap color, EdgeColorMap edge_color, const Params& params,
       int e_max_id)
    {
      typedef typename property_traits<WeightMap>::value_type D;
      typename std::vector<D>::size_type
        n = is_default_param(distance) ? num_vertices(g) : 1;
      std::vector<D> distance_map(n);
      n = is_default_param(cost) ? num_vertices(g) : 1;
      std::vector<D> cost_map(n);

      std::vector<default_color_type> color_map(num_vertices(g));
      default_color_type c = white_color;
      
      detail::shooting_star_dispatch2
        (g, s, h,
         choose_param(cost, make_iterator_property_map
                      (cost_map.begin(), index_map,
                       cost_map[0])),
         choose_param(distance, make_iterator_property_map
                      (distance_map.begin(), index_map, 
                       distance_map[0])),
         weight, edge_map, index_map,
	 predecessor,
	 choose_param(color, make_iterator_property_map
                      (color_map.begin(), index_map, c)),
	 edge_color, 
         params,
	 e_max_id);
    }
  } // namespace detail
  
  // Named parameter interface
  template <typename VertexAndEdgeListGraph,
            typename AStarHeuristic,
            typename P, typename T, typename R, 
	    typename IndexMap,
	    typename DistanceMap,
	    typename CostMap,
	    typename PredecessorMap>
  void
  shooting_star_search
    (VertexAndEdgeListGraph &g,
     typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor s,
     AStarHeuristic h, const bgl_named_params<P, T, R>& params, 
     IndexMap edge_index,
     DistanceMap distance, 
     CostMap cost,
     PredecessorMap &pre_map,
     int e_max_id)
  {
    typedef typename graph_traits<VertexAndEdgeListGraph>::edge_descriptor tEdge;

    detail::shooting_star_dispatch1
      (g, s, h,
       cost,
       distance,
       choose_const_pmap(get_param(params, edge_weight), g, edge_weight), //weight
       choose_const_pmap(get_param(params, edge_weight2), g, edge_weight2), //adjacent edges cost
       edge_index,
       pre_map, //predecessors
       get_param(params, vertex_color), //vertex color (deprecated)
       get_param(params, edge_color), //edge color
       params,
       e_max_id
       );    
  }
  
} // namespace boost

#endif // BOOST_GRAPH_SHOOTING_STAR_SEARCH_HPP
