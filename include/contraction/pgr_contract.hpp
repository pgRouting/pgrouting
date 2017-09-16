/*PGR-GNU*****************************************************************
File: pgr_contract.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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

#ifndef INCLUDE_CONTRACTION_PGR_CONTRACT_HPP_
#define INCLUDE_CONTRACTION_PGR_CONTRACT_HPP_
#pragma once

#include <deque>
#include <vector>
#include "cpp_common/pgr_assert.h"

#include "./pgr_contractionGraph.hpp"
#include "./pgr_linearContraction.hpp"
#include "./pgr_deadEndContraction.hpp"

namespace pgrouting {
namespace contraction {


template < class G >
class Pgr_contract {
    typedef typename G::V V;


    void perform_deadEnd(G &graph,
            Identifiers<V> forbidden_vertices,
            std::ostringstream& debug) {
        Pgr_deadend<G> deadendContractor;
        debug << "Setting forbidden_vertices";
        deadendContractor.setForbiddenVertices(forbidden_vertices);

        deadendContractor.calculateVertices(graph);
        try {
            deadendContractor.doContraction(graph);
        }
        catch ( ... ) {
            debug << "Caught unknown exception!\n";
        }
    }


    void perform_linear(G &graph,
            Identifiers<V>& forbidden_vertices,
            std::ostringstream& debug) {
        std::ostringstream linear_debug;
        Pgr_linear<G> linearContractor;
        linearContractor.setForbiddenVertices(forbidden_vertices);
        linearContractor.calculateVertices(graph);
        try {
            linearContractor.doContraction(graph);
        }
        catch ( ... ) {
            linear_debug << "Caught unknown exception!\n";
        }
        debug << linear_debug.str().c_str() << "\n";
    }


 public:
    Pgr_contract(
            G &graph,
            Identifiers<V> forbidden_vertices,
            std::vector<int64_t> contraction_order,
            int64_t max_cycles,
            Identifiers<int64_t> &remaining_vertices,
            std::vector<pgrouting::CH_edge> &shortcut_edges,
            std::ostringstream& debug) {
        std::deque<int64_t> contract_order;
        //  push -1 to indicate the start of the queue
        contract_order.push_back(-1);
        contract_order.insert(
                contract_order.end(),
                contraction_order.begin(), contraction_order.end());
        for (int64_t i = 0; i < max_cycles; ++i) {
            int64_t front = contract_order.front();
            debug << "Starting cycle " << i+1 << "\n";
            contract_order.pop_front();
            contract_order.push_back(front);
            front = contract_order.front();
            while (front != -1) {
                switch (front) {
                    case -1:
                        debug << "Finished cycle " << i+1 << std::endl;
                        break;
                    default:
                        debug << "contraction "<< front
                            << " asked" << std::endl;
                        if (front == 1) {
#ifndef NDEBUG
                            debug << "Graph before dead end contraction"
                                << std::endl;
                            graph.print_graph(debug);
                            debug << "Performing dead end contraction"
                                << std::endl;
#endif
                            perform_deadEnd(graph, forbidden_vertices, debug);
#ifndef NDEBUG
                            debug << "Graph after dead end contraction"
                                << std::endl;
                            graph.print_graph(debug);
#endif
                        } else if (front == 2) {
#ifndef NDEBUG
                            debug << "Graph before linear contraction"
                                << std::endl;
                            graph.print_graph(debug);
                            debug << "Performing linear contraction"
                                << std::endl;
#endif
                            perform_linear(graph, forbidden_vertices, debug);
#ifndef NDEBUG
                            debug << "Graph after linear contraction"
                                << std::endl;
                            graph.print_graph(debug);
#endif
                        }
                        contract_order.pop_front();
                        contract_order.push_back(front);
                        front = contract_order.front();
                }
            }
        }
        remaining_vertices = graph.get_changed_vertices();
        debug << "Printing shortcuts\n";
        for (auto shortcut : graph.shortcuts) {
            debug << shortcut;
            shortcut_edges.push_back(shortcut);
        }
    }

#if 0
    bool is_valid_contraction_number(int number) {
        switch (number) {
            case -2:
                return false;
                break;
            case -1:
                return false;
                break;
            case 0:
                return true;
                break;
            case 1:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
#endif
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_CONTRACT_HPP_
