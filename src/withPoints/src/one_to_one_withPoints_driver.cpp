/*PGR-GNU*****************************************************************
File: withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./one_to_one_withPoints_driver.h"

// #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/memory_func.hpp"
void
adjust_pids(const std::vector< Point_on_edge_t > &points,
        Path &path,
        int64_t start_pid,
        int64_t end_pid) {
    for (auto &path_stop : path.path) {
        path_stop.from = -start_pid;
        path_stop.to = -end_pid;

        for (const auto point: points) {
            if (point.vertex_id == path_stop.vertex) {
                path_stop.vertex = -point.pid;
                break;
            }
        }
    }
}

struct pointCompare {
    bool operator() (const Point_on_edge_t& lhs, const Point_on_edge_t& rhs) const
    { return lhs.fraction < rhs.fraction? true : lhs.pid < rhs.pid;}
};

static 
bool
create_new_edges(
        std::vector< Point_on_edge_t >  &points,
        const std::vector< pgr_edge_t > edges,
        char driving_side,
        bool directed,
        std::vector< pgr_edge_t > &new_edges,
        std::ostringstream &log) {
    for (const auto &point : points){
        log << "point: " 
            << point.pid <<"\t"
            << point.edge_id <<"\t"
            << point.fraction <<"\t"
            << point.side <<"\t"
            << point.vertex_id <<"\n";
    }

    int64_t vertex_id = 1;
    std::vector< Point_on_edge_t >  new_points;
    for (const auto edge : edges) {
        std::set< Point_on_edge_t, pointCompare> points_on_edge;
        for (const auto point : points) {
            if (edge.id == point.edge_id) {
                points_on_edge.insert(point);
                log << "working points: " 
                    << point.pid <<"\t"
                    << point.edge_id <<"\t"
                    << point.fraction <<"\t"
                    << point.side <<"\t"
                    << point.vertex_id <<"\n";
            }
        }
        if (points_on_edge.empty()) {
            log << "For some reason we didnt find a point belonging to the edge, must be an error\n";
            return false;
        }
#if 0
        log << "breaking:  \n"
            << edge.id << "\t" 
            << edge.source << "\t" 
            << edge.target << "\t" 
            << edge.cost << "\t"
            << edge.reverse_cost << "\n";
#endif
        int64_t prev_target = edge.source;
        double prev_fraction = 0;
        double agg_cost = 0;
        double agg_rcost = 0;
        double  last_cost = 0;
        double  last_rcost = 0;
        std::vector< Point_on_edge_t> the_points(points_on_edge.begin(), points_on_edge.end());

        for (auto &point : the_points) {

            log << "\n" << point.pid << "\t" << point.fraction << "\t" << point.side << "\n";
            if (point.fraction < 0 || point.fraction > 1) {
                log << "For some reason an invalid fraction was accepted, must be an error\n";
                return false;
            }
            if (point.fraction == prev_fraction) {
                point.vertex_id = prev_target;
                log << "By equal fraction: vertex_id of the point is " <<  point.vertex_id << "\n";
            }
            if (point.fraction == 0) {
                log << "vertex_id of the point is " << edge.source << "\n";
                point.vertex_id = edge.source;
            }
            if (point.fraction == 1) {
                log << "vertex_id of the point is " << edge.target << "\n";
                point.vertex_id = edge.target;
            }
            if (point.fraction > 0 && point.fraction < 1) {
                log << "vertex_id of the point is " << - vertex_id << "\n";
                point.vertex_id = -vertex_id;
                ++vertex_id;
            }
            new_points.push_back(point);

            if ((edge.cost < 0 or edge.reverse_cost < 0) || driving_side == 'b') {
                if (point.fraction > 0 && point.fraction < 1) {
                    last_cost = (point.fraction - prev_fraction) * edge.cost;
                    last_rcost = (point.fraction - prev_fraction) * edge.reverse_cost;
                    pgr_edge_t new_edge = {edge.id , prev_target, point.vertex_id, last_cost, last_rcost};
                    new_edges.push_back(new_edge);
#if 0
                    log << "new_edge: (id,source,target,cost,reverse_cost) = ("
                        << new_edge.id << "\t"
                        << new_edge.source << "\t"
                        << new_edge.target << "\t"
                        << new_edge.cost << "\t"
                        << new_edge.reverse_cost << ")\n";
#endif
                }
                prev_target = point.vertex_id;
                prev_fraction = point.fraction;
                agg_cost += last_cost;
                agg_rcost += last_rcost;
                continue;
            }
            if ((edge.cost > 0 && edge.reverse_cost > 0) && driving_side == 'r') {
                log << "Edge is two way and driving side is Right\n";
                if (point.fraction > 0 && point.fraction < 1) {
                    last_cost = (point.fraction - prev_fraction) * edge.cost;
                    last_rcost = (point.fraction - prev_fraction) * edge.reverse_cost;
                    log << "new_edge: (id,source,target,cost,reverse_cost) = ("
                        << edge.id << "\t"
                        << prev_target << "\t"
                        << point.vertex_id << "\t"
                        << last_cost << "\t"
                        << last_rcost << ")\n";
                }
                prev_target = point.vertex_id;
                prev_fraction = point.fraction;
                agg_cost += last_cost;
                agg_rcost += last_rcost;
            }

        }
        {
            pgr_edge_t new_edge = {edge.id , prev_target, edge.target,
                (edge.cost - agg_cost), (edge.reverse_cost - agg_rcost)};
            new_edges.push_back(new_edge);
            log << "last edge: (id,source,target,cost,reverse_cost) = ("
                << new_edge.id << "\t"
                << new_edge.source << "\t"
                << new_edge.target << "\t"
                << new_edge.cost << "\t"
                << new_edge.reverse_cost << ")\n";
        }

    }
    points = new_points;
    for (const auto &point : points){
        log << "point: " 
            << point.pid <<"\t"
            << point.edge_id <<"\t"
            << point.fraction <<"\t"
            << point.side <<"\t"
            << point.vertex_id <<"\n";
    }
    return true;

}


// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


void
do_pgr_withPoints(
        pgr_edge_t  *edges,
        size_t total_edges,
        Point_on_edge_t  *points_p,
        size_t total_points,
        pgr_edge_t  *edges_of_points,
        size_t total_edges_of_points,
        int64_t start_pid,
        int64_t end_pid,
        char driving_side,
        bool directed,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {
        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);
        std::vector< pgr_edge_t >
            edges_to_modify(edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;
        log << "start_pid" << start_pid << "\n";
        log << "end_pid" << end_pid << "\n";
        log << "driving_side" << driving_side << "\n";
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                directed,
                new_edges,
                log);

        int64_t start_vid = 0;
        int64_t end_vid = 0;
        for (const auto point : points) {
            if (point.pid == start_pid) {
                start_vid = point.vertex_id;
            }
            if (point.pid == end_pid) {
                end_vid = point.vertex_id;
            }

        }
        log << "start_vid" << start_vid << "\n";
        log << "end_vid" << end_vid << "\n";
        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_edges;

        Path path;


        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
#ifdef DEBUG
            digraph.print_graph(log);
#endif
            pgr_dijkstra(digraph, path, start_vid, end_vid);
        } else {
            log << "Working with Undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
#ifdef DEBUG
            undigraph.print_graph(log);
#endif
            pgr_dijkstra(undigraph, path, start_vid, end_vid);
        }

        size_t count(path.path.size());
        path.print_path(log);
        adjust_pids(points, path, start_pid, end_pid);
        path.print_path(log);



        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return;
        }


        (*return_tuples) = get_memory(count, (*return_tuples));
        size_t sequence = 0;
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;
#ifdef DEBUG
        log << "returning tuples:\n";
        for (size_t i = 0; i < sequence; ++i) {
            log << (*return_tuples)[i].seq << "\t"
                << (*return_tuples)[i].from << "\t"
                << (*return_tuples)[i].to << "\t"
                << (*return_tuples)[i].vertex << "\t"
                << (*return_tuples)[i].edge << "\t"
                << (*return_tuples)[i].cost << "\t"
                << (*return_tuples)[i].tot_cost << "\n";
        }
#endif 


#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
    }
}


void
get_new_queries(
        char *edges_sql,
        char *points_sql,
        int64_t start_pid,
        int64_t end_pid,
        bool strict,
        char **edges_of_points_query,
        char **edges_no_points_query) {

    std::ostringstream edges_of_points_sql;
    std::ostringstream edges_no_points_sql;

    if (strict) {
        edges_of_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << "),"
            << " strict AS (SELECT edge_id FROM points WHERE pid IN (" << start_pid << ", " << end_pid << ")) "
            << " SELECT DISTINCT edges.* FROM edges JOIN strict ON (id = edge_id)";
        *edges_of_points_query = strdup(edges_of_points_sql.str().c_str());

        edges_no_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << "),"
            << " strict AS (SELECT edge_id FROM points WHERE pid IN (" << start_pid << ", " << end_pid << ")) "
            << " SELECT edges.* FROM edges WHERE NOT EXISTS (SELECT edge_id FROM strict WHERE id = edge_id)";
        *edges_no_points_query = strdup(edges_no_points_sql.str().c_str());
    } else {
        edges_of_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << ")"
            << " SELECT DISTINCT edges.* FROM edges JOIN points ON (id = edge_id)";
        *edges_of_points_query = strdup(edges_of_points_sql.str().c_str());

        edges_no_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << ")"
            << " SELECT edges.* FROM edges WHERE NOT EXISTS (SELECT edge_id FROM points WHERE id = edge_id)";
        *edges_no_points_query = strdup(edges_no_points_sql.str().c_str());
    }
}



