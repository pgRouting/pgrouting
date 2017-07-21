/*PGR-GNU*****************************************************************
File: line_vertex.cpp
Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

#include "cpp_common/line_vertex.h"

#include <vector>
#include <algorithm>
#include <sstream>
#include <ostream>

#include "cpp_common/pgr_assert.h"


namespace pgrouting {

    std::ostream& operator<<(std::ostream& log, const Line_vertex &v) {
        log << "\nid = " << v.id;
        log << " | source = " << v.source;
        log << " | target = " << v.target;
        log << " | cost = " << v.cost;
        return log;
    }

    size_t check_vertices(
            std::vector < Line_vertex > vertices) {
        auto count(vertices.size());
        std::stable_sort(
                vertices.begin(), vertices.end(),
                [](const Line_vertex &lhs, const Line_vertex &rhs)
                {return lhs.id < rhs.id;});
        vertices.erase(
                std::unique(
                    vertices.begin(), vertices.end(),
                    [](const Line_vertex &lhs, const Line_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices.size() - count;
    }


    std::vector < Line_vertex > extract_vertices(
            std::vector < Line_vertex > vertices,
            const std::vector <pgr_edge_t > data_edges) {
        if (data_edges.empty()) return vertices;

        vertices.reserve(vertices.size() + data_edges.size() * 2);

        for (const auto edge : data_edges) {
            Line_vertex vertex(edge);

            auto max_element = std::max_element(vertices.begin(), vertices.end(),
                                                [](const Line_vertex& a,
                                                   const Line_vertex& b) {
                                                       return a.id < b.id;
                                                   });
            int64_t max_id = (max_element == vertices.end)?1:(*max_element).id;
            ++max_id;

            if (edge.cost > 0) {
                vertex.id = max_id;
                vertices.push_back(vertex);
            }

            if (edge.reverse_cost > 0) {
                ++max_id;
                vertex.id  = max_id;
                vertex.cost = edge.reverse_cost;
                std::swap(vertex.source, vertex.target);
                vertices.push_back(vertex);
            }
        }

        /*
         * sort and delete duplicates
         */
        std::stable_sort(vertices.begin(), vertices.end(),
                [](const Line_vertex &lhs, const Line_vertex &rhs)
                {return lhs.id < rhs.id;});

        vertices.erase(
                std::unique(vertices.begin(), vertices.end(),
                    [](const Line_vertex &lhs, const Line_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices;
    }


    std::vector < Line_vertex > extract_vertices(
            const std::vector <pgr_edge_t > data_edges) {
        std::vector< Line_vertex > vertices;
        if (data_edges.empty()) return vertices;

        vertices.reserve(data_edges.size() * 2);

        for (const auto edge : data_edges) {
            Line_vertex vertex(edge);

            auto max_element = std::max_element(vertices.begin(), vertices.end(),
                                                [](const Line_vertex& a,
                                                   const Line_vertex& b) {
                                                       return a.id < b.id;
                                                   });
            int64_t max_id = (max_element == vertices.end)?1:(*max_element).id;
            ++max_id;

            if (edge.cost > 0) {
                vertex.id = max_id;
                vertices.push_back(vertex);
            }

            if (edge.reverse_cost > 0) {
                ++max_id;
                vertex.id  = max_id;
                vertex.cost = edge.reverse_cost;
                std::swap(vertex.source, vertex.target);
                vertices.push_back(vertex);
            }
        }

        /*
         * sort and delete duplicates
         */
        std::stable_sort(vertices.begin(), vertices.end(),
                [](const Line_vertex &lhs, const Line_vertex &rhs)
                {return lhs.id < rhs.id;});

        vertices.erase(
                std::unique(vertices.begin(), vertices.end(),
                    [](const Line_vertex &lhs, const Line_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());

        return vertices;
    }

    std::vector < Line_vertex > extract_vertices_from_edges(
            const pgr_edge_t *data_edges, int64_t count) {
        return extract_vertices_from_edges(
                std::vector < pgr_edge_t >(
                    data_edges, data_edges + count));
    }

    std::vector < Line_vertex > extract_vertices_from_edges(
            std::vector < Line_vertex > vertices,
            const pgr_edge_t *data_edges, int64_t count) {
        return extract_vertices_from_edges(
                vertices, std::vector < pgr_edge_t >(
                    data_edges, data_edges + count));
    }

}  // namespace pgrouting
