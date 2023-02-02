/*PGR-GNU*****************************************************************

Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Nitish Chauhan
Mail: nitishchauhan0022 at gmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef INCLUDE_CIRCUITS_HAWICKCIRCUITS_HPP_
#define INCLUDE_CIRCUITS_HAWICKCIRCUITS_HPP_
#pragma once

#include <iostream>
#include <iterator>
#include <deque>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/hawick_circuits.hpp>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "cpp_common/pgr_assert.h"
#include "c_types/circuits_rt.h"

/**
 * @brief The main file which calls the respective boost function.
 *
 * Contains actual implementation of the function and the calling
 * of the respective boost function.
 */

namespace pgrouting {
namespace functions {

template <typename G, typename E>
class circuit_detector{
 public:
    /** @brief circuit detector to get the results
      *
      * @param graph the graph containing the edges
      * @param data  container for storing the result
      *
      */
    circuit_detector(
        G &graph,
        std::deque<circuits_rt> &data) :
    m_graph(graph),
    m_data(data) {}
    template <typename P, typename Gr>
    void cycle(P const &p, Gr const& Grap) {
        if (p.empty()) {
        return;
        }
        int seq = 0;
        typename P::const_iterator i, before_end = boost::prior(p.end());
        auto start_vid = m_graph[*p.begin()].id;
        auto end_vid = start_vid;
        double agg_cost = 0;
        E edge;
        for (i = p.begin(); i != before_end; ++i, ++seq) {
            auto node = m_graph[*i].id;
            edge = boost::edge(*i, *(i+1), Grap).first;
            auto cost = m_graph[edge].cost;
            auto id = m_graph[edge].id;
            m_data.push_back({circuit_No, seq, start_vid, end_vid, node, id, cost, agg_cost});
            agg_cost = agg_cost + cost;
            }
            auto node = m_graph[*i].id;
            edge = boost::edge(*(boost::prior(p.end())), *(p.begin()), Grap).first;
            auto cost = m_graph[edge].cost;
            auto id = m_graph[edge].id;
            m_data.push_back({circuit_No, seq, start_vid, end_vid, node, id, cost, agg_cost});
            agg_cost = agg_cost + cost;
            ++seq;
            m_data.push_back({circuit_No, seq, start_vid, end_vid, start_vid, -1, 0, agg_cost});
            circuit_No++;
        }

 private:
    G &m_graph;
    std::deque<circuits_rt> &m_data;
    int circuit_No = 1;
};

template <class G>
class pgr_hawickCircuits{
 public:
      typedef typename G::E E;

    /** @name hawickcircuit
      * @{
      *
      */

     /** @brief hawickcircuit function
      *
      * It does all the processing and returns the results.
      *
      * @param graph  the graph containing the edges
      *
      * @returns results, when results are found
      *
      * @see [boost::hawickcircuit]
      * (https://www.boost.org/libs/graph/doc/hawick_circuits.html)
      */

      std::deque<circuits_rt> hawickCircuits(G & graph) {
      // results storing the output
      std::deque<circuits_rt> results;

      // a circuit detector to provide the mechanism to store the circuit
      circuit_detector <G, E> detector(graph, results);

      /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
        CHECK_FOR_INTERRUPTS();
         try {
             boost::hawick_unique_circuits(graph.graph, detector);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }
    return results;
    //@}
}
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_CIRCUITS_HAWICKCIRCUITS_HPP_
