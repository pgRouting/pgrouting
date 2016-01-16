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


#include <vector>
#include <algorithm>

#include "../../common/src/pgr_types.h"
#include "./pgr_tsp.hpp"



static
size_t
rand(size_t n) {
    return static_cast< size_t >(std::rand() * n);
}


static
size_t
succ(size_t i, size_t n) {
    return static_cast<size_t>((i + 1) % n);
}

static
size_t
MOD(size_t i, size_t n) {
#if 1
return succ(i, n);
#else
return  ((i) % (n) > 0 ? (i) % (n) : (i) % (n) + (n));
#endif
}

double
TSP::D(size_t i, size_t j) {
    return dist[i][j];
}

void
TSP::update(Ids new_order) {
    double blength = dist.pathLength(new_order);
    if (bestlen < blength) {
        border = new_order;
        bestlen = blength;
    }
}

/*
 * Prim's approximated TSP tour
 * See also [Cristophides'92]
 */
bool
TSP::findEulerianPath() {
    Ids iorder(n);
    Ids mst(n);
    Ids arc(n);
    std::vector < double > dis(n);
    double d;
#if 0
    int n, *iorder, *jorder;
    DTYPE d;
    DTYPE maxd;
    DTYPE *dist;
    DTYPE *dis;

    jorder = tsp->jorder;
    iorder = tsp->iorder;
    dist   = tsp->dist;
    maxd   = tsp->maxd;
    n      = tsp->n;

    if (!(mst = (int*) palloc(n * sizeof(int))) ||
            !(arc = (int*) palloc(n * sizeof(int))) ||
            !(dis = (DTYPE*) palloc(n * sizeof(DTYPE))) )
    {
        elog(ERROR, "Failed to allocate memory!");
        return -1;
    }
#endif
    // PGR_DBG("findEulerianPath: 1");

    size_t j(0);
    double curr_maxd = maxd;
    dis[0] = -1;

    for (size_t i = 1; i < n; ++i) {
        dis[i] = dist[i][0];
        arc[i] = 0;
        if (curr_maxd > dis[i]) {
            curr_maxd = dis[i];
            j = i;
        }
    }
    //PGR_DBG("findEulerianPath: j=%d", j);

    if (curr_maxd == maxd) {
        // PGR_DBG("Error TSP fail to findEulerianPath, check your distance matrix is valid.");
        return false;
    }

    /*
     * O(n^2) Minimum Spanning Trees by Prim and Jarnick 
     * for graphs with adjacency matrix. 
     */
    for (size_t a = 0; a < n - 1; a++) {
        size_t k(0);
        mst[a] = j * n + arc[j]; /* join fragment j with MST */
        dis[j] = -1; 
        d = maxd;
        for (size_t i = 0; i < n; i++)
        {
            if (dis[i] >= 0) /* not connected yet */
            {
                if (dis[i] > dist[i][j])
                {
                    dis[i] = dist[i][j];
                    arc[i] = j;
                }
                if (d > dis[i])
                {
                    d = dis[i];
                    k = i;
                }
            }
        }
        j = k;
    }
    //PGR_DBG("findEulerianPath: 3");

    /*
     * Preorder Tour of MST
     */
#if 0
#define VISITED(x) jorder[x]
#define NQ(x) arc[l++] = x
#define DQ()  arc[--l]
#define EMPTY (l==0)
#endif
    for (auto &val : jorder) {
        val = 0;
    }

#if 0
    for (i = 0; i < n; i++) VISITED(i) = 0;
#endif

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

#if 0
    k = 0; l = 0; d = 0; NQ(0);
    while (!EMPTY)
    {
        i = DQ();
        if (!VISITED(i))
        {
            iorder[k++] = i;
            VISITED(i)  = 1;            
            for (j = 0; j < n - 1; j++) /* push all kids of i */
            {
                if (i == mst[j]%n) NQ(mst[j]/n); 
            }    
        }
    }
#endif
    //PGR_DBG("findEulerianPath: 4");

    update(iorder);
    return true;
}

/*
 * Local Search Heuristics
 *  b-------a        b       a
 *  .       .   =>   .\     /.
 *  . d...e .        . e...d .  
 *  ./     \.        .       .
 *  c       f        c-------f
 */

double
TSP::getThreeWayCost(Path p) {
    size_t a, b, c, d, e, f;

    a = iorder[MOD(p[0] - 1, n)];
    b = iorder[p[0]];
    c = iorder[p[1]];
    d = iorder[MOD(p[1] + 1, n)];
    e = iorder[p[2]];
    f = iorder[MOD(p[2] + 1, n)];

    return (D(a,d) + D(e,b) + D(c,f) - D(a,b) - D(c,d) - D(e,f)); 
    /* add cost between d and e if non symetric TSP */ 
}

void
TSP::doThreeWay(Path p) {
    size_t count, m1, m2, m3, a, b, c, d, e, f;

    a = MOD(p[0]-1,n);
    b = p[0];
    c = p[1];
    d = MOD(p[1]+1,n);
    e = p[2];
    f = MOD(p[2]+1,n);    

    m1 = MOD(n + c - b, n) + 1;  /* num cities from b to c */
    m2 = MOD(n + a - f, n) + 1;  /* num cities from f to a */
    m3 = MOD(n + e - d, n) + 1;  /* num cities from d to e */

    count = 0;
    /* [b..c] */
    for (size_t i = 0; i < m1; i++)
        jorder[count++] = iorder[MOD(i + b, n)];

    /* [f..a] */
    for (size_t i = 0; i < m2; i++)
        jorder[count++] = iorder[MOD(i+f,n)];

    /* [d..e] */
    for (size_t i = 0; i < m3; i++)
        jorder[count++] = iorder[MOD(i+d,n)];

    /* copy segment back into iorder */
    for (size_t i = 0; i < n; i++) iorder[i] = jorder[i];
}


/*
 *   c..b       c..b
 *    \/    =>  |  |
 *    /\        |  |
 *   a  d       a  d
 *
 *   a b 1  2   .. n-1 n c d
 *   a c n n-1  ..  2  1 c d
 */
double
TSP::getReverseCost(Path p) {
    int a, b, c, d;

    a = iorder[MOD(p[0] - 1, n)];
    b = iorder[p[0]];
    c = iorder[p[1]];
    d = iorder[MOD(p[1] + 1, n)];

    return (D(d,b) + D(c,a) - D(a,b) - D(c,d));
    /* add cost between c and b if non symetric TSP */ 
}

void
TSP::doReverse(Path p) {

    /* reverse path b...c */
    size_t nswaps = (MOD(p[1] - p[0], n) + 1) / 2;
    for (size_t i = 0; i < nswaps; i++) {
        size_t first = MOD(p[0]+i, n);
        size_t last  = MOD(p[1]-i, n);
        std::swap(iorder[first], iorder[last]);
#if 0
        tmp   = iorder[first];
        iorder[first] = iorder[last];
        iorder[last]  = tmp;
#endif
    }
}


void
TSP::annealing() {
    Path   p;
    int    numOnPath, numNotOnPath;
    double    pathlen;
    double energyChange;

    pathlen = dist.pathLength(iorder); 
    const double T_INIT = 100.0;
    const double FINAL_T = 0.1;
    const double COOLING = 0.9; /* to lower down T (< 1) */
    const size_t TRIES_PER_T = 500 * n;
    const size_t IMPROVED_PATH_PER_T = 60 * n;

    /* annealing schedule */
    for (double T = T_INIT; FINAL_T < T; T *= COOLING) {
        size_t pathchg = 0;
        for (size_t j = 0; j < TRIES_PER_T; j++) {
            do {
                p[0] = rand(n);
                p[1] = rand(n);
                /* non-empty path */
                if (p[0] == p[1]) 
                    p[1] = MOD(p[0] + 1, n);

                numOnPath = MOD(p[1] - p[0], n) + 1;
                numNotOnPath = n - numOnPath;
            } while (numOnPath < 2 || numNotOnPath < 2); /* non-empty path */

            if (rand(2)) {
                /*  threeWay */
                do {
                    p[2] = MOD(rand(numNotOnPath) + p[1] + 1, n);
                } while (p[0] == MOD(p[2] + 1, n)); /* avoids a non-change */

                energyChange = getThreeWayCost(p);
                // if (energyChange < 0 || RREAL < exp(-energyChange / T) )
                if (energyChange < 0 || std::rand() < exp(-energyChange / static_cast<double>(T)) ) {
                    pathchg++;
                    pathlen += energyChange;
                    doThreeWay(p);
                }

            } else {
                /* path Reverse */
                energyChange = getReverseCost(p);
                if (energyChange < 0 || std::rand() < exp(-energyChange / static_cast<double>(T)) ) {
                    pathchg++;
                    pathlen += energyChange;
                    doReverse(p); 
                }
            }
            // if the new length is better than best then save it as best
            update(iorder);
#if 0
            if (pathlen < tsp->bestlen) {
                tsp->bestlen = pathlen;
                for (i=0; i<tsp->n; i++) tsp->border[i] = tsp->iorder[i];
            }
#endif
            if (pathchg > IMPROVED_PATH_PER_T) break; /* finish early */
        }   
        // PGR_DBG("T:%f L:%f B:%f C:%d", T, pathlen, tsp->bestlen, pathchg);
        if (pathchg == 0) break;   /* if no change then quit */
    }
}

