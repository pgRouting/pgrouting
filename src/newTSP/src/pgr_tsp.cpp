/*PGR-GNU*****************************************************************
 * File: pgr_tsp.cpp
 *
 * Generated with Template by:
 * Copyright (c) 2015 pgRouting developers
 * Mail: project@pgrouting.org
 *
 * Function's developer:
 * Copyright (c) 2015 Celia Virginia Vergara Castillo
 * Mail: vicky_vergara@hotmail.com
 *
 * ------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  ********************************************************************PGR-GNU*/

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif



#include <vector>
#include <algorithm>

#include "../../common/src/pgr_types.h"
#include "../../common/src/pgr_assert.h"
#include "./pgr_tsp.hpp"



static
size_t
rand(size_t n) {
    return static_cast< size_t >(std::rand() % n);
}


static
size_t
succ(size_t i, size_t n) {
    return static_cast<size_t>((i + 1) % n);
}

static
size_t
pred(size_t i, size_t n) {
    return  i == 0? n - 1: i -1;
}


namespace pgRouting {
namespace tsp {


void
TSP::update(std::vector<size_t> new_order) {
    Tour new_tour(new_order);
    update_if_best(new_tour, dist.tourCost(new_tour));
}

void
TSP::update_if_best(const Tour &current_tour, double current_cost) {
#ifndef NDEBUG
    /*
     * if this assertion pops, fix getfooCost
     *
     */
    auto new_cost = dist.tourCost(current_tour);
    pgassert(current_cost == new_cost);
#endif

    if (current_cost < bestCost) {
        best_tour = current_tour;
        bestCost = current_cost;
    }
}


/*
 * Prim's approximated TSP tour
 * See also [Cristophides'92]
 */
bool
TSP::findEulerianPath() {
    std::vector<size_t> jorder(n, 0);
    std::vector<size_t> iorder(n);
    std::vector<size_t> mst(n);
    std::vector<size_t>  arc(n, 0); // fill with 0
    std::vector < double > dis(dist[0]);
    std::iota(std::begin(iorder), std::end(iorder), 0);
    double d;


    auto min_pos = std::min_element(dis.begin() + 1, dis.end());
    auto curr_min_d = *min_pos;
    auto min_idx = min_pos - dis.begin();


    if (curr_min_d == maxd) {
        // PGR_DBG("Error TSP fail to findEulerianPath, check your distance matrix is valid.");
        return false;
    }

    /*
     * O(n^2) Minimum Spanning Trees by Prim and Jarnick 
     * for graphs with adjacency matrix. 
     */

    for (size_t a = 0; a < n - 1; a++) {
        size_t k(0);

        mst[a] = min_idx * n + arc[min_idx]; /* join fragment j with MST */
        dis[min_idx] = -1; 
        d = maxd;
        for (size_t i = 0; i < n; i++) {
            if (dis[i] >= 0) {
                /* not connected yet */
                if (dis[i] > dist[i][min_idx]) {
                    dis[i] = dist[i][min_idx];
                    arc[i] = min_idx;
                }

                if (d > dis[i]) {
                    d = dis[i];
                    k = i;
                }
            }
        }
        min_idx = k;
    }



    size_t l = 0;
    size_t k = 0;
    d = 0;
    arc[l++] = 0;
    while (!(l == 0)) {
        size_t i = arc[--l];

        if (!jorder[i]) {
            iorder[k++] = i;
            jorder[i]  = 1;            
            /* push all kids of i */
            for (size_t j = 0; j < n - 1; j++) {
                if (i == mst[j] % n)
                    arc[l++] = mst[j] % n; 
            }
        }
    }


    update(iorder);
    return true;
}

/*
 *
 * 0 1 2 3 4 5 6 7 8 9
 *     p   f     l
 * slides [4,5,6] to position p
 *
 * 0 1 4 5 6 2 3 7 8 9
 *
 *
 * 0 1 2 3 4 5 6 7 8 9
 *     f     l     p
 * slides [2,3,4] to position p
 *
 * 0 1 6 7 2 3 4 5 8 9
 *
 * 
 */

double
TSP::getDeltaSlide(const Tour &tour, size_t posP, size_t posF, size_t posL) {
    Tour new_tour(tour);
    new_tour.slide(posP, posF, posL);
    return dist.tourCost(new_tour) - dist.tourCost(tour);
}


/*
 *       c..d            c..d
 *       |  |     =>     |  |
 *       |  |            |  |
 *  b -- a  e --f   b -- e  a -- f
 *
 *   a b 1  2   .. n-1 n c d
 *   a c n n-1  ..  2  1 c d
 */
double
TSP::getDeltaSwap(const Tour &tour, size_t posA, size_t posE) const {
    if (succ(posE, n ) == posA) std::swap(posA, posE);
    if (succ(posA, n) == posE) {
        auto b = tour.cities[pred(posA, n)];
        auto a = tour.cities[posA];

        auto e = tour.cities[posE];
        auto f = tour.cities[succ(posE, n)];
        return dist[b][e] + dist[e][a] + dist[a][f]
            - dist[b][a] - dist[a][e]  - dist[e][f] ;
    }

    auto b = tour.cities[pred(posA, n)];
    auto a = tour.cities[posA];
    auto c = tour.cities[succ(posA, n)];

    auto d = tour.cities[pred(posE, n)];
    auto e = tour.cities[posE];
    auto f = tour.cities[succ(posE, n)];

#ifndef NDEBUG
    auto new_tour(tour);
    new_tour.swap(posA, posE);
    pgassert((dist.tourCost(new_tour) - dist.tourCost(tour))
            == (dist[b][e] + dist[e][c] + dist[d][a] + dist[a][f]
                - dist[b][a] - dist[a][c]  - dist[d][e] - dist[e][f]));
#endif

    return dist[b][e] + dist[e][c] + dist[d][a] + dist[a][f]
        - dist[b][a] - dist[a][c]  - dist[d][e] - dist[e][f] ;
}

/*
 *   c..b       c..b
 *    \/    =>  |  |
 *    /\        |  |
 *   a  d       a  d
 *
 *    [                    )
 *   a b 1  2   .. n-1 n c d
 *   a c n n-1  ..  2  1 c d
 */
double
TSP::getDeltaReverse(const Tour &tour, size_t posA, size_t posC) const {
    if (posA == (posC - 1)) return 0;
    auto a = tour.cities[posA];
    auto b = tour.cities[succ(posA, n)];

    auto c = tour.cities[posC];
    auto d = tour.cities[succ(posC, n)];

#ifndef NDEBUG
    auto new_tour(tour);
    new_tour.reverse(posA, posC);
    pgassert((dist.tourCost(new_tour) - dist.tourCost(tour))
            == (dist[a][c] + dist[b][d] - dist[a][b] - dist[c][d]));
#endif

    return dist[a][c] + dist[b][d] - dist[a][b] - dist[c][d];
}


void
TSP::annealing(
        std::ostringstream &log,
        double temperature,
        double final_temperature,
        double cooling_factor,
        size_t tries_per_temperature,
        size_t change_path_per_temperature) {
    pgassert(n == dist.ids.size());
    tries_per_temperature = 500 * n;
    change_path_per_temperature = 60 * n;

    double current_cost(bestCost);
    auto current_tour(best_tour);


    /* annealing schedule */
    for (; final_temperature < temperature; temperature *= cooling_factor) {
        log << "Cycle's Temperature: " << temperature <<"\n";

        /*
           how many times the tour changed in current temperature
           */
        size_t pathchg = 0;
        for (size_t j = 0; j < tries_per_temperature; j++) {
            bool changed(false);

            auto which = rand(3);
            switch (which) {
                case 0: {
                            /*  swap */
                            pgassert(n > 2);

                            auto c1 = std::rand() % n;
                            auto c2 = std::rand() % n;

                            if (c1 == c2) c2 = succ(c2, n);
                            if (c1 > c2) std::swap(c1, c2);

                            pgassert(c1 != c2);
                            pgassert(c1 < n && c2 < n);
                            pgassert(c1 < c2);

                            auto energyChange = getDeltaSwap(current_tour, c1, c2);

                            if (energyChange < 0 
                                    || (0 < energyChange
                                        &&  ((double)std::rand() / (double)RAND_MAX)  < exp(-energyChange / temperature))) {
                                pathchg++;
                                current_cost += energyChange;
                                current_tour.swap(c1,c2);
                                changed = true;
                            }
                        }
                        break;

                case 1: {
                            /* reverse */
                            pgassert(n > 2);

                            auto c1 = std::rand() % n;
                            auto c2 = std::rand() % n;

                            if (c1 == c2) c2 = succ(c2, n);
                            if (c1 == (c2 - 1)) c2 = succ(c2, n);
                            if (c1 > c2) std::swap(c1, c2);

                            pgassert(c1 != c2);
                            pgassert(c1 < n && c2 < n);
                            pgassert(c1 < c2);

                            auto energyChange = getDeltaReverse(current_tour, c1, c2);

                            if (energyChange < 0 
                                    || (0 < energyChange
                                        &&  ((double)std::rand() / (double)RAND_MAX)  < exp(-energyChange / temperature))) {
                                pathchg++;
                                current_cost += energyChange;
                                current_tour.reverse(c1,c2);
                                changed = true;
                            }
                        }
                        break;
                case 2: {
                            /* slide */
                            pgassert(n > 3);

                            auto first = std::rand() % n;
                            auto last = std::rand() % n;

                            if (first > last) std::swap(first, last);
                            pgassert(first <= last);
                            auto place = std::rand() % n;

                            if (first <= place && place <= last) place = succ(last, n);
                            if (place == first) first = succ(first, n);

                            pgassert(place < first || place > last);


                            auto energyChange = getDeltaSlide(current_tour, place, first, last);

                            if (energyChange < 0 
                                    || (0 < energyChange
                                        &&  ((double)std::rand() / (double)RAND_MAX)  < exp(-energyChange / temperature))) {
                                pathchg++;
                                current_cost += energyChange;
                                current_tour.slide(place, first, last);
                                changed = true;
                            }
                        }
                        break;
            }  // switch
            // if the new length is better than best then save it as best
            if (changed) {
                update_if_best(current_tour, current_cost);
            }

            if (change_path_per_temperature < pathchg) {
                log << "\nfinished early temperature" << temperature << "\n";
                break;
            } 
        }
        log << "\n total changes in temperature " << temperature << " = " << pathchg << "\n";
        if (pathchg == 0) break;   /* if no change then quit */
    }
}

}  // namespace tsp
}  // namespace pgRouting

