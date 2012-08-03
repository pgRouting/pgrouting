//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
//
// Revision History:
//   04 April 2001: Added named parameter variant. (Jeremy Siek)
//   01 April 2001: Modified to use new <boost/limits.hpp> header. (JMaddock)
//
#ifndef PGROUTING_MILD_TWO_Q_HPP
#define PGROUTING_MILD_TWO_Q_HPP

#include <functional>
#include <boost/limits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/relax.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/graph/exception.hpp>
#include <boost/pending/relaxed_heap.hpp>
#include <boost/graph/overloading.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/graph/detail/d_ary_heap.hpp>
#include <boost/graph/two_bit_color_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/concept/assert.hpp>

#include <two_queue.hpp>

namespace boost {

  // Core function - compute the shortest path with the two_queue
  template <class Graph,
            class PredecessorMap, class DistanceMap,
            class WeightMap, class Compare, class Combine,
            class DistZero, class ColorMap>
  inline void
  mild_two_q_shortest_paths_no_init
    (const Graph& g,
     typename graph_traits<Graph>::vertex_descriptor s,
     typename graph_traits<Graph>::vertex_descriptor t,
     PredecessorMap predecessor, DistanceMap distance, WeightMap weight,
     Compare compare, Combine combine, DistZero zero,
     ColorMap color)
  {
   
	typedef Graph IncidenceGraph;
	BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept<IncidenceGraph> ));
	typedef graph_traits<IncidenceGraph> GTraits;
	typedef typename GTraits::vertex_descriptor Vertex;
	typedef typename GTraits::edge_descriptor Edge;
	BOOST_CONCEPT_ASSERT(( ReadWritePropertyMapConcept<ColorMap, Vertex> ));
	typedef typename property_traits<ColorMap>::value_type ColorValue;
	typedef color_traits<ColorValue> Color;
	typename GTraits::out_edge_iterator ei, ei_end;
	typedef typename property_traits<DistanceMap>::value_type D;
	typedef typename property_traits<WeightMap>::value_type W;

	two_queue_tm<typename graph_traits<Graph>::vertex_descriptor, DistanceMap, Compare, DistZero> Q;
	Q.setDistance(&distance);
	Q.setCompare(compare);
	Q.setZero(zero);
	Q.push_b(s);
	put(color, s, Color::gray());//gray: is in queue
	while(!Q.empty())
	{
		if(get(color,t)!=Color::white()&&get(distance,t)<=Q.GetMinDist()) break; // when there is no better solution, can stop immediately
		Vertex u = Q.top(); Q.pop();
		put(color, u, Color::black());//black: was in queue
		D d_u = get(distance, u);
		for (boost::tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
        		Vertex v = target(*ei, g);
			D d_v = get(distance,v);
			W w_e = get(weight, *ei);
			if ( compare(combine(d_u, w_e), d_v) ) {
        			put(distance, v, combine(d_u, w_e));
        			put(predecessor, v, u);
				ColorValue c_v = get(color, v);
				if(c_v==Color::white())// white: not reached
				{
					Q.push_b(v);
					put(color, v, Color::gray());			
				}
				else if(c_v==Color::black())
				{
					Q.push_a(v);
					put(color, v, Color::gray());
				}
			}
		}
	}

  }

  // Initialize distances and call breadth first search with default color map
  template <class VertexListGraph,
            class PredecessorMap, class DistanceMap,
            class WeightMap, class IndexMap, class Compare, class Combine,
            class DistInf, class DistZero, typename T, typename Tag, 
            typename Base>
  inline void
  mild_two_q_shortest_paths
    (const VertexListGraph& g,
     typename graph_traits<VertexListGraph>::vertex_descriptor s,
     typename graph_traits<VertexListGraph>::vertex_descriptor t,
     PredecessorMap predecessor, DistanceMap distance, WeightMap weight,
     IndexMap index_map,
     Compare compare, Combine combine, DistInf inf, DistZero zero,
     const bgl_named_params<T, Tag, Base>&
     BOOST_GRAPH_ENABLE_IF_MODELS_PARM(VertexListGraph,vertex_list_graph_tag))
  {
    boost::two_bit_color_map<IndexMap> color(num_vertices(g), index_map);
    mild_two_q_shortest_paths(g, s, t, predecessor, distance, weight, index_map,
                            compare, combine, inf, zero,
                            color);
  }

  // Initialize distances, predecessor, color
  template <class VertexListGraph,
            class PredecessorMap, class DistanceMap,
            class WeightMap, class IndexMap, class Compare, class Combine,
            class DistInf, class DistZero, class ColorMap>
  inline void
  mild_two_q_shortest_paths
    (const VertexListGraph& g,
     typename graph_traits<VertexListGraph>::vertex_descriptor s,
     typename graph_traits<VertexListGraph>::vertex_descriptor t,
     PredecessorMap predecessor, DistanceMap distance, WeightMap weight,
     IndexMap index_map,
     Compare compare, Combine combine, DistInf inf, DistZero zero,
     ColorMap color)
  {
    typedef typename property_traits<ColorMap>::value_type ColorValue;
    typedef color_traits<ColorValue> Color;
    typename graph_traits<VertexListGraph>::vertex_iterator ui, ui_end;
    for (boost::tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui) {
      //vis.initialize_vertex(*ui, g);
      put(distance, *ui, inf);
      put(predecessor, *ui, *ui);
      put(color, *ui, Color::white());
    }
    put(distance, s, zero);

    mild_two_q_shortest_paths_no_init(g, s, t, predecessor, distance, weight,
                            compare, combine, zero, color);
  }

  // Initialize distances and call breadth first search
  template <class VertexListGraph,
            class PredecessorMap, class DistanceMap,
            class WeightMap, class IndexMap, class Compare, class Combine,
            class DistInf, class DistZero>
  inline void
  mild_two_q_shortest_paths
    (const VertexListGraph& g,
     typename graph_traits<VertexListGraph>::vertex_descriptor s,
     typename graph_traits<VertexListGraph>::vertex_descriptor t,
     PredecessorMap predecessor, DistanceMap distance, WeightMap weight,
     IndexMap index_map,
     Compare compare, Combine combine, DistInf inf, DistZero zero)
  {
    mild_two_q_shortest_paths(g, s, t, predecessor, distance, weight, index_map,
                            compare, combine, inf, zero,
                            no_named_parameters());
  }

  namespace detail {

    // Handle defaults for PredecessorMap and
    // Distance Compare, Combine, Inf and Zero
    template <class VertexListGraph, class DistanceMap, class WeightMap,
              class IndexMap, class Params>
    inline void
    mild_two_q_dispatch2
      (const VertexListGraph& g,
       typename graph_traits<VertexListGraph>::vertex_descriptor s,
       typename graph_traits<VertexListGraph>::vertex_descriptor t,
       DistanceMap distance, WeightMap weight, IndexMap index_map,
       const Params& params)
    {
      // Default for predecessor map
      dummy_property_map p_map;

      typedef typename property_traits<DistanceMap>::value_type D;
      D inf = choose_param(get_param(params, distance_inf_t()),
                           (std::numeric_limits<D>::max)());

      mild_two_q_shortest_paths
        (g, s, t,
         choose_param(get_param(params, vertex_predecessor), p_map),
         distance, weight, index_map,
         choose_param(get_param(params, distance_compare_t()),
                      std::less<D>()),
         choose_param(get_param(params, distance_combine_t()),
                      //closed_plus<D>(inf)), //FIXME no matched function, have to change it
                      closed_plus<D>()),
         inf,
         choose_param(get_param(params, distance_zero_t()),
                      D()),
         params);
    }

    template <class VertexListGraph, class DistanceMap, class WeightMap,
              class IndexMap, class Params>
    inline void
    mild_two_q_dispatch1
      (const VertexListGraph& g,
       typename graph_traits<VertexListGraph>::vertex_descriptor s,
       typename graph_traits<VertexListGraph>::vertex_descriptor t,
       DistanceMap distance, WeightMap weight, IndexMap index_map,
       const Params& params)
    {
      // Default for distance map
      typedef typename property_traits<WeightMap>::value_type D;
      typename std::vector<D>::size_type
        n = is_default_param(distance) ? num_vertices(g) : 1;
      std::vector<D> distance_map(n);

      detail::mild_two_q_dispatch2
        (g, s, t, choose_param(distance, make_iterator_property_map
                            (distance_map.begin(), index_map,
                             distance_map[0])),
         weight, index_map, params);
    }
  } // namespace detail

  // Named Parameter Variant
  template <class VertexListGraph, class Param, class Tag, class Rest>
  inline void
  mild_two_q_shortest_paths
    (const VertexListGraph& g,
     typename graph_traits<VertexListGraph>::vertex_descriptor s,
     typename graph_traits<VertexListGraph>::vertex_descriptor t,
     const bgl_named_params<Param,Tag,Rest>& params)
  {
    // Default for edge weight and vertex index map is to ask for them
    // from the graph.  Default for the visitor is null_visitor.
    detail::mild_two_q_dispatch1
      (g, s, t,
       get_param(params, vertex_distance),
       choose_const_pmap(get_param(params, edge_weight), g, edge_weight),
       choose_const_pmap(get_param(params, vertex_index), g, vertex_index),
       params);
  }

} // namespace boost

#ifdef BOOST_GRAPH_USE_MPI
#  include <boost/graph/distributed/dijkstra_shortest_paths.hpp>
#endif

#endif // BOOST_GRAPH_DIJKSTRA_HPP
