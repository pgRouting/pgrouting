/*PGR-GNU*****************************************************************

File: basePath_SSEC.hpp
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

#pragma once
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif


#include <deque>
#include <iostream>
#include <algorithm>
#include "./pgr_types.h"

class Path {
    typedef std::deque< Path_t >::iterator pthIt;
    typedef std::deque< Path_t >::const_iterator ConstpthIt;

 private:
    std::deque< Path_t > path;
    int64_t m_start_id;
    int64_t m_end_id;
    double m_tot_cost;

 public:
    Path(): m_tot_cost(0) {}
    Path(int64_t s_id, int64_t e_id)
        : m_start_id(s_id), m_end_id(e_id), m_tot_cost(0)
    {}
    int64_t start_id() const {return m_start_id;}
    void start_id(int64_t value) {m_start_id = value;}
    int64_t end_id()  const {return m_end_id;}
    void end_id(int64_t value) {m_end_id = value;}
    double tot_cost()  const {return m_tot_cost;}

    size_t size() const {return path.size();}
    bool empty() const {return path.empty();}

    void push_front(Path_t data);
    void push_back(Path_t data);
    const Path_t& operator[](size_t i) const {return path[i];}
    Path_t& operator[](size_t i) {return path[i];}

    pthIt begin() {return path.begin();}
    pthIt end() {return path.end();}
    ConstpthIt begin() const {return path.begin();}
    ConstpthIt end() const {return path.end();}


    void erase(pthIt pos) {path.erase(pos);}
    const Path_t& back() const {return path.back();}
    Path_t& back() {return path.back();}
    const Path_t& front() const {return path.front();}
    Path_t& front() {return path.front();}


    Path_t set_data(
            int64_t d_from,
            int64_t d_to,
            int64_t d_vertex,
            int64_t d_edge,
            double d_cost,
            double d_tot_cost);

    void push_front(
            int64_t d_vertex,
            int64_t d_edge,
            double d_cost,
            double d_tot_cost);
    void clear();

    friend std::ostream& operator<<(std::ostream &log, const Path &p);


    void reverse();

    Path  getSubpath(unsigned int j) const;


    bool isEqual(const Path &subpath) const;
    void appendPath(const Path &o_path);
    void empty_path(unsigned int d_vertex);

    void get_pg_dd_path(
            General_path_element_t **ret_path,
            size_t &sequence) const;

    void get_pg_ksp_path(
            General_path_element_t **ret_path,
            size_t &sequence, int routeId) const;

    void generate_postgres_data(
            General_path_element_t **postgres_data,
            size_t &sequence) const;

    friend size_t collapse_paths(
            General_path_element_t **ret_path,
            const std::deque< Path > &paths) {
        size_t sequence = 0;
        for (const Path &path : paths) {
            if (path.path.size() > 0)
                path.generate_postgres_data(ret_path, sequence);
        }
        return sequence;
    }



    /*
     * sort the paths by size from greater to smaller
     *        and sort each path by node
     * all the nodes on p2 are going to be compared
     * with the nodes of p1
     *
     * When both paths reach the node and p1.agg_cost > p2.agg_cost
     *    erase the node of p1
     *    (cant erase from p2 because we loose the iterators
     *     so in a future cycle it will be deleted)
     *
     * sort the paths by start_id,
     */

    friend void equi_cost(std::deque< Path > &paths) {
        /* sort paths by size: largest first */
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e2.size() < e1.size();
                });

        /* sort each path by node: smaller id first */
        for (auto &p : paths) {
            if (p.size() < 2) continue;
            std::sort(p.begin(), p.end(),
                    [](const Path_t &e1, const Path_t &e2)->bool {
                    return e1.node < e2.node;
                    });
        }

        for (auto &p1 : paths) {
            for (const auto &p2 : paths) {
                if (p1.start_id() == p2.start_id()) continue;
                for (const auto &stop : p2.path) {
                    /* find the node of p2 in p1 */
                    auto pos = lower_bound(p1.begin(), p1.end(), stop,
                            [](const Path_t &l, const Path_t &r)->bool {
                            return l.node < r.node;
                            });

                    if (pos != p1.end() && stop.node == pos->node && stop.agg_cost < pos->agg_cost) {
                        /* both share the same node &
                         * the second path has the smallest
                         *  So erasing from the first path */
                        p1.erase(pos);
                    }
                }
            }
        }

        /* sort paths by start_id */
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.start_id() < e2.start_id();
                });

        /* sort each path by agg_cost, node */
        for (auto &path : paths) {
            /* least influential data first */
            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    { return l.node < r.node;});
            /* preserve the order of what we did before */
            std::stable_sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    { return l.agg_cost < r.agg_cost;});
        }
    }

    friend size_t count_tuples(const std::deque< Path > &paths) {
        size_t count(0);
        for (const Path &e : paths) {
            count += e.path.size();
        }
        return count;
    }
};


