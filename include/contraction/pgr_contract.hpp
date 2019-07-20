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

#include "contraction/pgr_contractionGraph.hpp"
#include "contraction/ch_graphs.hpp"
#include "contraction/pgr_linearContraction.hpp"
#include "contraction/pgr_deadEndContraction.hpp"

namespace pgrouting {
namespace contraction {

bool is_valid_contraction(int number);

template < class G >
class Pgr_contract {
    typedef typename G::V V;

 public:
    Pgr_contract(
            G &graph,
            Identifiers<V> forbidden_vertices,
            std::vector<int64_t> contraction_order,
            int64_t max_cycles
            ) {
        std::deque<int64_t> contract_order;
        //  push -1 to indicate the start of the queue
        contract_order.push_back(-1);
        contract_order.insert(
                contract_order.end(),
                contraction_order.begin(), contraction_order.end());
        for (int64_t i = 0; i < max_cycles; ++i) {
            int64_t front = contract_order.front();
            contract_order.pop_front();
            contract_order.push_back(front);
            auto kind = contract_order.front();
            while (kind != -1) {
                one_cycle(graph, kind, forbidden_vertices);
                contract_order.pop_front();
                contract_order.push_back(front);
                kind = contract_order.front();
            }
        }
    }


 private:
    void one_cycle(
            G &graph,
            int64_t kind,
            Identifiers<V> &forbidden_vertices) {
        switch (kind) {
            case -1:
                pgassert(false);
                break;

            case 1:
                perform_deadEnd(graph, forbidden_vertices);
                break;


            case 2:
                perform_linear(graph, forbidden_vertices);
                break;
            default:
                pgassert(false);
                break;
        }
    }

    void perform_deadEnd(G &graph,
            Identifiers<V> forbidden_vertices) {
        Pgr_deadend<G> deadendContractor;
        deadendContractor.setForbiddenVertices(forbidden_vertices);

        deadendContractor.calculateVertices(graph);
        try {
            deadendContractor.doContraction(graph);
        }
        catch ( ... ) {
            throw;
        }
    }


    void perform_linear(G &graph,
            Identifiers<V>& forbidden_vertices) {
        Pgr_linear<G> linearContractor;
        try {
            linearContractor(graph, forbidden_vertices);
        }
        catch ( ... ) {
            throw;
        }
    }
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_CONTRACT_HPP_
