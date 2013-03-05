//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek,
//
// Copyright 2007 Orkney, Inc.
// Author: Anton A. Patrushev
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#ifndef BOOST_GRAPH_SHOOTING_STAR_RELAX_HPP
#define BOOST_GRAPH_SHOOTING_STAR_RELAX_HPP

#include <functional>
#include <boost/limits.hpp> // for numeric limits
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

#include <postgres.h>

#define U_TURN_COST 100000

bool is_equal ( int a[], int b[], int size )
{
  for ( int i = 0; i < size; i++ ) 
  {
    if ( a[i] != b[i] )
    return false;
  }
	      
  return true;
}

namespace boost {

    // The following version of the plus functor prevents
    // problems due to overflow at positive infinity.

    template <class T>
    struct closed_plus
    {
      // std::abs just isn't portable :(
      template <class X>
      inline X my_abs(const X& x) const { return x < 0 ? -x : x; }

      T operator()(const T& a, const T& b) const {
        using namespace std;
        T inf = (numeric_limits<T>::max)();
        if (b > 0 && my_abs(inf - a) < b)
          return inf;
        return a + b;
      }
    };
    
    template <class Edge, class Graph, class WeightMap, class EdgeMap,
            class PredecessorMap, class DistanceMap, class CostMap,
            class BinaryFunction, class BinaryPredicate>
    bool relax(Edge e, 
               Edge pe,
               Edge s, 
               const Graph& g, const WeightMap& w, const EdgeMap& em,
               PredecessorMap& p, DistanceMap& d, CostMap& c,
               const BinaryFunction& combine, const BinaryPredicate& compare,
	       int e_max_id)
    {
      typedef typename graph_traits<Graph>::directed_category DirCat;
      bool is_undirected = is_same<DirCat, undirected_tag>::value;

      typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
            
      Vertex u = source(e, g), v = target(e, g), pu = source(pe, g);

      typedef typename property_traits<DistanceMap>::value_type D;
      typedef typename property_traits<WeightMap>::value_type W;
      
      typedef typename property_traits<EdgeMap>::value_type E;
      
      D d_e = get(d, e);
      D d_pe = get(d, pe);
      
      W w_e = get(w, e);

      //edge where we came from
      bool edge_exists;
      
      W w_pe_e;

      E w_pe = get(em, e);
      
      int contains = -1;
	
      Edge ce = pe;
      int e_id;

      std::vector<Edge> edge_h;
      typedef typename std::vector<Edge>::iterator It;

      for(int i=0; i< w_pe[g[e].id].size(); ++i)
      
      if(w_pe[g[e].id].at(i).second.size() < 1 || w_pe[g[e].id].at(i).second.back() > 0)
      {
      
      for(int j=0; j<w_pe[g[e].id].at(i).second.size(); ++j)
      {
	e_id = g[ce].id;
	
        if(w_pe[g[e].id].at(i).second.at(j) == -1)
	  continue;
	
	if(w_pe[g[e].id].at(i).second.at(j) == e_id || w_pe[g[e].id].at(i).second.at(j)+e_max_id == e_id||
	   w_pe[g[e].id].at(i).second.at(j) == e_id+e_max_id || w_pe[g[e].id].at(i).second.at(j)+e_max_id == e_id+e_max_id)
	{
	 contains = i;
	 edge_h.push_back(ce);
	}
	else if(i == contains)
	{
	 contains = -1;
	}
	
	ce = p[g[ce].id];
      }
      
      }
      
      //calculate w_pe_e
      if(contains >= 0)
      {
        w_pe_e = w_pe[g[e].id].at(contains).first;
      }
      //Check if it is a u-turn not in the beginning of route 
      else if( abs(g[e].id-g[pe].id) == e_max_id && g[e].id != g[s].id && g[pe].id != g[s].id )
      {
		w_pe_e = U_TURN_COST;
      }
      else
      {
        w_pe_e = 0;
      }

      //Ugly combination with w_e_pe.

      if ( compare(combine(combine(d_pe, get(w, pe)), w_pe_e), d_e)) 
      {
	put(d, e, combine(combine(d_pe, get(w, pe)), w_pe_e));
	p[g[e].id] = pe;

        return true;
      }
      else
      {
        return false;
      }
    }

    template <class Graph, class WeightMap, class EdgeMap,
      class PredecessorMap, class DistanceMap>
    bool relax(typename graph_traits<Graph>::edge_descriptor e,
               const Graph& g, WeightMap w, EdgeMap em, PredecessorMap p, DistanceMap d)
    {
      typedef typename property_traits<DistanceMap>::value_type D;
      typedef closed_plus<D> Combine;
      typedef std::less<D> Compare;
      return relax(e, g, w, em, p, d, Combine(), Compare());
    }

} // namespace boost

#endif
