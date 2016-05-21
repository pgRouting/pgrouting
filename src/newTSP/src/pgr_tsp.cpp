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



#include <iomanip>
#include <limits>
#include <vector>
#include <set>
#include <algorithm>
#include <time.h>

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

void TSP::invariant() const {
    /* the calculated value & the actual value are the same */
    pgassert(std::fabs(dist.tourCost(current_tour) - current_cost) < 10 * epsilon);
    pgassert(std::fabs(dist.tourCost(best_tour) - bestCost) < 10 * epsilon);
    pgassert(n == dist.ids.size());
    pgassert(n == current_tour.size());
    pgassert(n == best_tour.size());
}

void
TSP::update_if_best() {
    ++updatecalls;
    if (updatecalls > 10) {
        current_cost = dist.tourCost(current_tour);
        updatecalls = 0;
    }

    if (current_cost < bestCost) {
        best_tour = current_tour;
        bestCost = dist.tourCost(current_tour);
        current_cost = bestCost;
        updatecalls = 0;
    }

    invariant();
}



size_t
TSP::find_closest_city(
        size_t current_city,
        const std::set<size_t> inserted) const {
    invariant();

    auto distance_row( dist[current_city]);
    size_t best_city = 0;
    auto best_distance = std::numeric_limits<double>::max();
    for (size_t i = 0; i < distance_row.size(); ++i) {
        if (i == current_city) continue;
        if (inserted.find(i) != inserted.end()) continue;
        if (distance_row[i] <  best_distance) {
            best_city = i;
            best_distance = distance_row[i];
        }
    }

    invariant();
    return best_city;
}



void
TSP::greedyInitial() {
    invariant();

    std::set<size_t> pending(best_tour.cities.begin(), best_tour.cities.end());
    std::set<size_t> inserted;
    std::vector<size_t> tour_to_be;

    auto current_city = *pending.begin();

#ifndef NDEBUG
    auto ps(pending.size());
#endif

    pending.erase(pending.begin());
#ifndef NDEBUG
    pgassert(pending.size() == (ps - 1));
#endif

    tour_to_be.push_back(current_city);
    inserted.insert(current_city);

    while (!pending.empty()) {
        auto next_city = find_closest_city(current_city, inserted);
        tour_to_be.push_back(next_city);
        inserted.insert(next_city);

        auto ps(pending.size());
        pending.erase(next_city);
#ifndef NDEBUG
        pgassert(pending.size() == (ps - 1));
#endif

        current_city = next_city;
    }

    pgassert(tour_to_be.size() == n);
    current_tour = Tour(tour_to_be);
    current_cost = dist.tourCost(current_tour);
    update_if_best();
    swapClimb();

    invariant();
    return;
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
TSP::getDeltaSlide(size_t posP, size_t posF, size_t posL) const{
    Tour new_tour(current_tour);
    new_tour.slide(posP, posF, posL);
    return dist.tourCost(new_tour) - dist.tourCost(current_tour);
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
TSP::getDeltaSwap(size_t posA, size_t posE) const {
    invariant();

    if (succ(posE, n ) == posA) std::swap(posA, posE);
    if (succ(posA, n) == posE) {
        auto b = current_tour.cities[pred(posA, n)];
        auto a = current_tour.cities[posA];

        auto e = current_tour.cities[posE];
        auto f = current_tour.cities[succ(posE, n)];
        return dist[b][e] + dist[e][a] + dist[a][f]
            - dist[b][a] - dist[a][e]  - dist[e][f] ;
    }

    auto b = current_tour.cities[pred(posA, n)];
    auto a = current_tour.cities[posA];
    auto c = current_tour.cities[succ(posA, n)];

    auto d = current_tour.cities[pred(posE, n)];
    auto e = current_tour.cities[posE];
    auto f = current_tour.cities[succ(posE, n)];

#ifndef NDEBUG
    auto delta = dist[b][e] + dist[e][c] + dist[d][a] + dist[a][f]
        - dist[b][a] - dist[a][c]  - dist[d][e] - dist[e][f] ;
    auto new_tour(current_tour);
    new_tour.swap(posA, posE);
    pgassert(std::fabs((dist.tourCost(new_tour) - dist.tourCost(current_tour))
                - delta) < (10 * epsilon) );
#endif

    invariant();
    return dist[b][e] + dist[e][c] + dist[d][a] + dist[a][f]
        - dist[b][a] - dist[a][c]  - dist[d][e] - dist[e][f] ;
}

/*
 *   ..A                    C
 *       [                    )
 *   ..a  b 1  2   .. n-1 n c d ..
 *   ..a  c n n-1  ..  2  1 b d ..
 */
double
TSP::getDeltaReverse(size_t posA, size_t posC) const {
    invariant();

    if (posA == (posC - 1)) return 0;
    auto a = current_tour.cities[posA];
    auto b = current_tour.cities[succ(posA, n)];

    auto c = current_tour.cities[posC];
    auto d = current_tour.cities[succ(posC, n)];


#ifndef NDEBUG
    auto delta = dist[a][c] + dist[b][d] - dist[a][b] - dist[c][d];
    auto new_tour(current_tour);
    new_tour.reverse(posA, posC);

    pgassert(std::fabs(dist.tourCost(new_tour) - dist.tourCost(current_tour)
                - delta) < (10 * epsilon));
#endif

    invariant();
    return dist[a][c] + dist[b][d] - dist[a][b] - dist[c][d];
}

void
TSP::swapClimb() {
    pgassert(n > 2);

//    auto first = std::rand() % n;
//    for (size_t first = std::rand() % n; first < n; first++) {
    for (size_t first = 0; first < n; first++) {
        for (size_t last = first + 1; last < n; last++) {
            pgassert(first < last);

            auto energyChange = getDeltaSwap(first, last);

            if (energyChange < 0 && epsilon < std::fabs(energyChange)) {
                current_cost += energyChange;
                current_tour.swap(first, last);
                update_if_best();
            }
        }
    }
}

void
TSP::annealing(
        double temperature,
        double final_temperature,
        double cooling_factor,
        int64_t tries_per_temperature,
        int64_t change_per_temperature,
        bool fix_random) {
    invariant();

    tries_per_temperature = tries_per_temperature * n;
    change_per_temperature = change_per_temperature * n;
    if (fix_random) {
        std::srand(1);
    } else {
        std::srand(static_cast<unsigned int>(time(NULL)));
    }




    /* annealing schedule */
    for (; final_temperature < temperature; temperature *= cooling_factor) {
        invariant();

        log << "Cycle's Temperature: " << temperature <<"\n";

        /*
           how many times the tour changed in current temperature
           */
        size_t pathchg = 0;
        size_t enchg = 0;
        for (size_t j = 0; j < tries_per_temperature; j++) {

            auto which = rand(2);
            // which = 1;
            switch (which) {
                case 0: {
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

                            auto energyChange = getDeltaReverse(c1, c2);

                            if ( (energyChange < 0 && epsilon < std::fabs(energyChange))
                                    || (0 < energyChange
                                        &&  ((double)std::rand() / (double)RAND_MAX)  < exp(-energyChange / temperature))
                               ) {
                                if (energyChange < 0) ++enchg;
                                pathchg++;
                                current_cost += energyChange;
                                current_tour.reverse(c1,c2);
                                update_if_best();
                            }
                        }
                        break;
                case 1: {
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


                            auto energyChange = getDeltaSlide(place, first, last);

                            if ((energyChange < 0 && epsilon < std::fabs(energyChange))
                                    || (0 < energyChange
                                        &&  ((double)std::rand() / (double)RAND_MAX)  < exp(-energyChange / temperature))) {
                                if (energyChange < 0) ++enchg;
                                pathchg++;
                                current_cost += energyChange;
                                current_tour.slide(place, first, last);
                                update_if_best();
                            }
                        }
                        break;
            }  // switch


            if (change_per_temperature < pathchg) {
                break;
            } 
        }
        swapClimb();
        log << "total changes in temperature " << temperature << " = " << pathchg 
            << "\t  total changes because of (energyChange < 0) " << " = " << enchg << "\n";
        if (pathchg == 0) break;   /* if no change then quit */
    }
}

}  // namespace tsp
}  // namespace pgRouting

