/*PGR-GNU*****************************************************************
 *

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "cpp_common/basic_vertex.h"

#include <vector>
#include <algorithm>
#include <sstream>
#include <ostream>

#include "cpp_common/pgr_assert.h"


namespace pgrouting {

    std::ostream& operator<<(std::ostream& log, const Basic_vertex &v) {
        log << v.id;
        return log;
    }

    size_t check_vertices(
            std::vector < Basic_vertex > vertices) {
        auto count(vertices.size());
        std::stable_sort(
                vertices.begin(), vertices.end(),
                [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                {return lhs.id < rhs.id;});
        vertices.erase(
                std::unique(
                    vertices.begin(), vertices.end(),
                    [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices.size() - count;
    }


    std::vector < Basic_vertex > extract_vertices(
            std::vector < Basic_vertex > vertices,
            const std::vector <pgr_edge_t > data_edges) {
        if (data_edges.empty()) return vertices;

        vertices.reserve(vertices.size() + data_edges.size() * 2);

        for (const auto edge : data_edges) {
            Basic_vertex vertex;

            vertex.id = edge.source;
            vertices.push_back(vertex);

            vertex.id = edge.target;
            vertices.push_back(vertex);
        }

        /*
         * sort and delete duplicates
         */
        std::stable_sort(vertices.begin(), vertices.end(),
                [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                {return lhs.id < rhs.id;});

        vertices.erase(
                std::unique(vertices.begin(), vertices.end(),
                    [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices;
    }


    std::vector < Basic_vertex > extract_vertices(
            const std::vector <pgr_edge_t > &data_edges) {
        std::vector< Basic_vertex > vertices;
        if (data_edges.empty()) return vertices;

        vertices.reserve(data_edges.size() * 2);

        for (const auto edge : data_edges) {
            Basic_vertex vertex;

            vertex.id = edge.source;
            vertices.push_back(vertex);

            vertex.id = edge.target;
            vertices.push_back(vertex);
        }

        /*
         * sort and delete duplicates
         */
        std::stable_sort(vertices.begin(), vertices.end(),
                [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                {return lhs.id < rhs.id;});

        vertices.erase(
                std::unique(vertices.begin(), vertices.end(),
                    [](const Basic_vertex &lhs, const Basic_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices;
    }

    std::vector < Basic_vertex > extract_vertices(
            const pgr_edge_t *data_edges, int64_t count) {
        return extract_vertices(
                std::vector < pgr_edge_t >(
                    data_edges, data_edges + count));
    }

    std::vector < Basic_vertex > extract_vertices(
            std::vector < Basic_vertex > vertices,
            const pgr_edge_t *data_edges, int64_t count) {
        return extract_vertices(
                vertices, std::vector < pgr_edge_t >(
                    data_edges, data_edges + count));
    }

}  // namespace pgrouting
