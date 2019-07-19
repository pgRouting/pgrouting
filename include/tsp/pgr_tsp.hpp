/*PGR-GNU*****************************************************************
 * File: tsp_driver.cpp
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

#ifndef INCLUDE_TSP_PGR_TSP_HPP_
#define INCLUDE_TSP_PGR_TSP_HPP_
#pragma once

#include <time.h>

#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <limits>
#include <cmath>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/Dmatrix.h"
#include "tsp/euclideanDmatrix.h"
#include "tsp/tour.h"



static
size_t
rand(size_t n) {
    pgassert(n > 0);
    return static_cast< size_t >(std::rand() % n);
}


static
size_t
succ(size_t i, size_t n) {
    pgassert(n > 0);
    return static_cast<size_t>((i + 1) % n);
}

static
size_t
pred(size_t i, size_t n) {
    return  i == 0? n - 1: i -1;
}


namespace pgrouting {
namespace tsp {

template < typename MATRIX >
class TSP: public MATRIX {
 public:
     using MATRIX::distance;
     using MATRIX::tourCost;
     using MATRIX::get_row;

     /*
      * function members
      */
     explicit TSP(const MATRIX &_costs)
         : MATRIX(_costs),
         current_tour(_costs.size()),
         best_tour(_costs.size()),
         epsilon(0.000001),
         n(_costs.size()),
         updatecalls(0),
         swap_count(0),
         slide_count(0),
         reverse_count(0),
         improve_count(0) {
             pgassert(n == MATRIX::size());
             bestCost = MATRIX::tourCost(best_tour);
             current_cost = MATRIX::tourCost(current_tour);
             pgassert(bestCost == current_cost);
         }


     Tour get_tour() const {return best_tour;}

     std::string get_stats() const {
         std::ostringstream log1;
         log1
             << "\nTotal swaps: " << swap_count
             << "\nTotal slides: " << slide_count
             << "\nTotal reverses: " << reverse_count
             << "\nTimes best tour changed: " << improve_count;
         return log1.str();}

     std::string get_log() const {
         return log.str();}

     void greedyInitial(size_t idx_start = 0);
     void annealing(
             double initial_temperature,
             double final_temperature,
             double cooling_factor,
             int64_t tries_per_temperature,
             int64_t max_changes_per_temperature,
             int64_t max_consecutive_non_changes,
             bool randomize,
             double time_limit);


 private:
     Tour current_tour;
     Tour best_tour;
     double bestCost;
     double current_cost;
     double epsilon;
     size_t n;

     int updatecalls;

     std::ostringstream log;
     size_t swap_count;
     size_t slide_count;
     size_t reverse_count;
     size_t improve_count;

 private:
     void invariant() const;

     size_t find_closest_city(
             size_t current_city,
             const std::set<size_t> &inserted) const;

     double getDeltaSlide(
             size_t posP,
             size_t posF,
             size_t posL) const;

     void swapClimb();

     double getDeltaSwap(
             size_t posA,
             size_t posC) const;

     double getDeltaReverse(
             size_t posA,
             size_t posC) const;

     void update_if_best();
};



template < typename MATRIX >
void TSP<MATRIX>::invariant() const {
    /* the calculated value & the actual value are the same */
    pgassert(std::fabs(tourCost(current_tour) - current_cost) < epsilon);
    pgassert(std::fabs(tourCost(best_tour) - bestCost) < epsilon);
    pgassert(n == MATRIX::ids.size());
    pgassert(n == current_tour.size());
    pgassert(n == best_tour.size());
}

template < typename MATRIX >
void
TSP<MATRIX>::update_if_best() {
    invariant();
    ++updatecalls;

    if (current_cost < bestCost) {
        ++improve_count;
        best_tour = current_tour;
        bestCost = current_cost;
    }

    invariant();
}



template < typename MATRIX >
size_t
TSP<MATRIX>::find_closest_city(
        size_t current_city,
        const std::set<size_t> &inserted) const {
    invariant();

    auto distance_row(get_row(current_city));
    pgassert(distance_row.size() == n);

#ifndef NDEBUG
    std::ostringstream err;
    for (const auto &d : distance_row) {
        err  << d << ", ";
    }
#endif

    size_t best_city = 0;
    auto best_distance = (std::numeric_limits<double>::max)();
#ifndef NDEBUG
    bool found(false);
#endif

    for (size_t i = 0; i < distance_row.size(); ++i) {
        if (i == current_city) continue;
        if (inserted.find(i) != inserted.end()) continue;
        if (distance_row[i] <  best_distance) {
            best_city = i;
            best_distance = distance_row[i];
#ifndef NDEBUG
            found = true;
#endif
        }
    }
    pgassertwm(found, err.str());

    invariant();
    return best_city;
}



template < typename MATRIX >
void
TSP<MATRIX>::greedyInitial(size_t idx_start) {
    invariant();

    std::set<size_t> pending(best_tour.cities.begin(), best_tour.cities.end());
    std::set<size_t> inserted;
    std::vector<size_t> tour_to_be;

    auto current_city = idx_start;

#ifndef NDEBUG
    std::ostringstream err;
    auto ps(pending.size());
#endif

    pending.erase(idx_start);

#ifndef NDEBUG
    pgassert(pending.size() == (ps - 1));
#endif

    tour_to_be.push_back(current_city);
    inserted.insert(current_city);

    while (!pending.empty()) {
        auto next_city = find_closest_city(current_city, inserted);
        tour_to_be.push_back(next_city);
        inserted.insert(next_city);

#ifndef NDEBUG
        auto ps(pending.size());
        err << "before";
        for (const auto p : pending) {
            err << p << ",";
        }
#endif

        pending.erase(next_city);

#ifndef NDEBUG
        err << "\nafter deleting" << next_city << ":\t";
        for (const auto p : pending) {
            err << p << ",";
        }
        pgassertwm(pending.size() == (ps - 1), err.str());
#endif

        current_city = next_city;
    }

    pgassert(tour_to_be.size() == n);
    current_tour = Tour(tour_to_be);
    current_cost = tourCost(current_tour);
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

template < typename MATRIX >
double
TSP<MATRIX>::getDeltaSlide(size_t place, size_t first, size_t last) const {
    invariant();
#ifndef NDEBUG
    std::ostringstream err;
    err  << "\tplace" << place
        << "\tfirst" << first
        << "\tlast" << last
        << "\tn" << n;
#endif

    pgassertwm(place < first || place > last, err.str());
    pgassertwm(first < last, err.str());
    pgassertwm(last < n, err.str());
    pgassertwm(place < n, err.str());
    pgassertwm(first < n, err.str());

    /*
     * Initial state
     * [...f] [f+1 ... l] [l+1 ...p] [p+1 ...]
     *
     * final state
     * [...f] [l+1 ... p] [f+1 ...l] [p+1 ...]
     *
     *
     * Initial state
     *   [f+1 ... l]
     *     :      :
     * [...f]   [l+1 ...p] [p+1 ...]
     *
     * final state
     *               [f+1 ... l]
     *                :       :
     * [...f] [l+1 ...p]   [p+1 ...]
     *
     */

    auto cityP = current_tour.cities[place];
    auto cityF = current_tour.cities[first];
    auto cityL = current_tour.cities[last];
    auto cityP1 = current_tour.cities[succ(place, n)];
    auto cityF1 = current_tour.cities[succ(first, n)];
    auto cityL1 = current_tour.cities[succ(last, n)];

    auto delta(
            distance(cityF, cityL1)
            + distance(cityP, cityF1)
            + distance(cityL, cityP1)
            - distance(cityF, cityF1)
            - distance(cityL, cityL1)
            - distance(cityP, cityP1));

#ifndef NDEBUG
    Tour new_tour(current_tour);
    new_tour.slide(place, first, last);

    err << "\ncurrent_tour:";
    for (const auto id : current_tour.cities) {
        err << id << ", ";
    }

    err << "\nnew_tour:";
    for (const auto id : new_tour.cities) {
        err << id << ", ";
    }

    auto exactDelta = tourCost(new_tour) - tourCost(current_tour);
    err << "\n"
        << exactDelta
        << " - " <<  delta
        << " = "
        << exactDelta - delta
        << " = "
        << std::fabs(exactDelta - delta);
    pgassertwm(std::fabs((exactDelta - delta)) < epsilon, err.str());
#endif

    invariant();
    return delta;
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
template < typename MATRIX >
double
TSP<MATRIX>::getDeltaSwap(size_t posA, size_t posE) const {
    invariant();

    if (succ(posE, n ) == posA) std::swap(posA, posE);
    if (succ(posA, n) == posE) {
        auto b = current_tour.cities[pred(posA, n)];
        auto a = current_tour.cities[posA];

        auto e = current_tour.cities[posE];
        auto f = current_tour.cities[succ(posE, n)];
        return distance(b, e) + distance(e, a) + distance(a, f)
            - distance(b, a) - distance(a, e)  - distance(e, f);
    }

    auto b = current_tour.cities[pred(posA, n)];
    auto a = current_tour.cities[posA];
    auto c = current_tour.cities[succ(posA, n)];

    auto d = current_tour.cities[pred(posE, n)];
    auto e = current_tour.cities[posE];
    auto f = current_tour.cities[succ(posE, n)];

#ifndef NDEBUG
    auto delta = distance(b, e)
        + distance(e, c) + distance(d, a) + distance(a, f)
        - distance(b, a) - distance(a, c)  - distance(d, e) - distance(e, f);
    auto new_tour(current_tour);
    new_tour.swap(posA, posE);
    auto exactDelta = tourCost(new_tour) - tourCost(current_tour);
    std::ostringstream log;
    log << exactDelta
        << " - " <<  delta
        << " = "
        << exactDelta - delta
        << " = "
        << std::fabs(exactDelta - delta);

    pgassertwm(std::fabs((exactDelta - delta)) < epsilon, log.str());
#endif

    invariant();
    return  distance(b, e) + distance(e, c) + distance(d, a) + distance(a, f)
        - distance(b, a) - distance(a, c)  - distance(d, e) - distance(e, f);
}

/*
 *   ..A                    C
 *       [                    )
 *   ..a  b 1  2   .. n-1 n c d ..
 *   ..a  c n n-1  ..  2  1 b d ..
 */
template < typename MATRIX >
double
TSP<MATRIX>::getDeltaReverse(size_t posA, size_t posC) const {
    invariant();

    if (posA == (posC - 1)) return 0;
    auto a = current_tour.cities[posA];
    auto b = current_tour.cities[succ(posA, n)];

    auto c = current_tour.cities[posC];
    auto d = current_tour.cities[succ(posC, n)];


#ifndef NDEBUG
    auto delta =
        distance(a, c) + distance(b, d) - distance(a, b) - distance(c, d);
    auto new_tour(current_tour);
    new_tour.reverse(posA, posC);
    auto exactDelta = tourCost(new_tour) - tourCost(current_tour);

    std::ostringstream log;
    log << "exactDelta(" << exactDelta
        << ") - delta(" <<  delta
        << ") = "
        << exactDelta - delta
        << " = "
        << (exactDelta - delta)
        << " epsilon = " << epsilon;
    pgassertwm(std::fabs((exactDelta - delta)) < epsilon, log.str());
#endif

    invariant();
    return  distance(a, c) + distance(b, d) - distance(a, b) - distance(c, d);
}

template < typename MATRIX >
void
TSP<MATRIX>::swapClimb() {
    invariant();
    if (!(n > 2)) return;
    pgassert(n > 2);

    //    auto first = std::rand() % n;
    //    for (size_t first = std::rand() % n; first < n; first++) {
    for (size_t first = 0; first < n; first++) {
        for (size_t last = first + 1; last < n; last++) {
            pgassert(first < last);

            auto energyChange = getDeltaSwap(first, last);

            if (energyChange < 0 && epsilon < std::fabs(energyChange)) {
                ++swap_count;
                current_cost += energyChange;
                current_tour.swap(first, last);
                update_if_best();
            }
        }
    }
    invariant();
}

template < typename MATRIX >
void
TSP<MATRIX>::annealing(
        double temperature,
        double final_temperature,
        double cooling_factor,
        int64_t tries_per_temperature,
        int64_t max_changes_per_temperature,
        int64_t max_consecutive_non_changes,
        bool randomize,
        double time_limit) {
    invariant();
    if (!(n > 2)) return;

    clock_t start_time(clock());

    if (randomize) {
        std::srand(static_cast<unsigned int>(time(NULL)));
    } else {
        std::srand(1);
    }




    /* annealing schedule */
    for (; final_temperature < temperature; temperature *= cooling_factor) {
        invariant();

        log << "\nCycle(" << temperature <<") ";

        /*
           how many times the tour changed in current temperature
           */
        int64_t pathchg = 0;
        size_t enchg = 0;
        int64_t non_change = 0;
        for (int64_t j = 0; j < tries_per_temperature; j++) {
            ++non_change;

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

                            if ( (energyChange < 0
                                        && epsilon < std::fabs(energyChange))
                                    || (0 < energyChange
                                        &&  (
                                            static_cast<double>(std::rand()) /
                                            static_cast<double>(RAND_MAX))
                                        < exp(-energyChange / temperature))) {
                                if (energyChange < 0) ++enchg;
                                ++reverse_count;
                                ++pathchg;
                                non_change = 0;
                                current_cost += energyChange;
                                current_tour.reverse(c1, c2);
                                update_if_best();
                            }
                        }
                        break;
                case 1: {
                            /* slide */
                            if (n <= 3) {
                                break;
                            }

                            pgassert(n > 3);

                            auto first = std::rand() % n;
                            auto last = std::rand() % n;

                            if (first == last) last = succ(last, n);
                            if (first > last) std::swap(first, last);
                            if (first == 0 && last == (n - 1)) {
                                first = succ(first, n);
                            }

                            pgassert((n - (last - first) - 1) > 0);
                            auto place = std::rand() % (n - (last - first) - 1);
                            place = place < first?  place :
                                last + (place - first) + 1;


                            pgassert((place < first
                                        || place > last)
                                    && (first < last));

                            auto energyChange = getDeltaSlide(
                                    place, first, last);

                            if ((energyChange < 0
                                        && epsilon < std::fabs(energyChange))
                                    || (0 < energyChange
                                        &&  (static_cast<double>(std::rand())
                                            / static_cast<double>(RAND_MAX))
                                        < exp(-energyChange / temperature))) {
                                if (energyChange < 0) ++enchg;
                                ++slide_count;
                                ++pathchg;
                                non_change = 0;
                                current_cost += energyChange;
                                current_tour.slide(place, first, last);
                                update_if_best();
                            }
                        }
                        break;
            }  // switch


            if (max_changes_per_temperature < pathchg
                    && max_consecutive_non_changes < non_change ) {
                break;
            }
        }  // for tries per temperature

        swapClimb();
        clock_t current_time(clock());
        double elapsed_time = static_cast<double>(
                current_time - start_time) / CLOCKS_PER_SEC;
        if (time_limit < elapsed_time) {
            break;
        }
        log << "\ttotal changes =" << pathchg
            << "\t" << enchg << " were because  delta energy < 0";
        if (pathchg == 0) break;   /* if no change then quit */
    }  // for temperatures
}

}  // namespace tsp
}  // namespace pgrouting

#endif   // INCLUDE_TSP_PGR_TSP_HPP_
