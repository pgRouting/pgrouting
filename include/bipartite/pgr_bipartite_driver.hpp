/*PGR-GNU*****************************************************************
File: pgr_bipartite_driver.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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


#ifndef INCLUDE_PGR_BIPARTITE_DRIVER_HPP
#define INCLUDE_PGR_BIPARTITE_DRIVER_HPP
#pragma once
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include <iostream>
#include <algorithm>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>


namespace pgrouting {
    namespace functions {
        template<class G>
        class Pgr_Bipartite : public pgrouting::Pgr_messages {
            public:
            std::vector<pgr_bipartite_rt> pgr_bipartite(
                G &graph ){
                    log<<"Started reading"<<std::endl;
                    std::vector<pgr_bipartite_rt> results(graph.num_vertices());
                    return results;

                    /***Todo 
                     * we might need to define sequance in pgr_.sql file
                     * 




                }

        
        };
    }
}


#endif  // INCLUDE_PGR_BIPARTITE_DRIVER_HPP
