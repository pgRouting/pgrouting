/*********************************************************************
*
*  $Id: tsplib.c,v 1.1 2006/05/13 23:39:56 woodbri Exp $
*
*  tsplib
*  Copyright 2005,2013, Stephen Woodbridge, All rights Reserved
*  This file is released under MIT-X license as part of pgRouting.
*
**********************************************************************
*
*  tsplib.c
*
*  Routine to calculate the node order for a Traveling Salesman Problem
*  from a distance matrix.
*
*  $Log: tsplib.c,v $
*  Revision 1.1  2006/05/13 23:39:56  woodbri
*  Initial checkin of IMT::TSP module.
*
*  Initial revision created from router tsp2.c
*
**********************************************************************
*
* Simulated annealing and a non symetric
* Euclidian Traveling Salesman Problem.
*
* Solution based on local search heuristics for
* non-crossing paths and nearest neighbors 
*
* Storage Requirements: 4n ints
*
* Problem: given an nXn matrix of costs for n cities in the plane, 
* find a permutation pi_1, pi_2, ..., pi_n of 1, 2, ..., n that 
* minimizes sum for 1<=i<n D(pi_i,pi_i+1), where D(i,j) is the 
* euclidian distance between cities i and j from the matrix
*
* This means that the ith row is the "from" city and the jth column
* is the cost to the "to" city.
*
* NOTE: costs are assumed to be ints, so scaling them to meters or
* some other int would be advisable.
*
* NOTE: the internal cost calculations ASSUME a symetric matrix. There
* are notes of what needs to be updated, but all D(i,j)s need to be
* reviewed because order is important for asymetric cases.
*
* Note: with n cities, there is (n-1)!/2 possible tours.
* factorial(10)=3628800  factorial(50)=3E+64  factorial(150)=5.7E+262
* If we could check one tour per clock cycle on a 100 MHZ computer, we
* would still need to wait approximately 10^236 times the age of the
* universe to explore all tours for 150 cities. 
*
* Derivied from code 1995 by Maugis Lionel (Sofreavia) on sid1
* that was placed in the public domain.
*************************************************************************/

//#ifdef __MINGW64__
//#define ELOG_H
//#include <winsock2.h>
//#endif
#include <postgres.h>
#include <string.h>    /* memcpy */
#include <math.h>      /* exp    */

#include "tsp.h"

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif


#define T_INIT                        100
#define FINAL_T                       0.1
#define COOLING                       0.9 /* to lower down T (< 1) */
#define TRIES_PER_T                   500*n  
#define IMPROVED_PATH_PER_T           60*n   

/*
 * Portable Uniform Integer Random Number in [0-2^31] range
 * Performs better than ansi-C rand() 
 * D.E Knuth, 1994 - The Stanford GraphBase
 */
#define RANDOM()        (*rand_fptr >= 0 ? *rand_fptr-- : flipCycle ()) 
#define two_to_the_31   ((unsigned long)0x80000000) 
#define RREAL           ((double)RANDOM()/(double)two_to_the_31)

static long A[56]= {-1};
long *rand_fptr = A;

#define mod_diff(x,y)   (((x)-(y))&0x7fffffff) 
long flipCycle()
{
    register long *ii,*jj;
    for (ii = &A[1], jj = &A[32]; jj <= &A[55]; ii++, jj++)
    *ii= mod_diff (*ii, *jj);

    for (jj = &A[1]; ii <= &A[55]; ii++, jj++)
    *ii= mod_diff (*ii, *jj);
    rand_fptr = &A[54];
    return A[55];
}

void initRand (long seed)
{
    register long i;
    register long prev = seed, next = 1;
    seed = prev = mod_diff (prev,0);
    A[55] = prev;
    for (i = 21; i; i = (i+21)%55)
    {
        A[i] = next;
        next = mod_diff (prev, next);
        if (seed&1) seed = 0x40000000 + (seed >> 1);
        else seed >>= 1;
        next = mod_diff (next,seed);
        prev = A[i];
    }
    
    for (i = 0; i < 7; i++) flipCycle(); 
}

long unifRand (long m)
{
    register unsigned long t = two_to_the_31 - (two_to_the_31%m);
    register long r;
    do {
        r = RANDOM();
    } while (t <= (unsigned long)r);
    return r%m;
}

/*
 * Defs
 */

typedef int Path[3];      /* specify how to change path */

typedef struct tspstruct {
    int n;
    DTYPE maxd;
    DTYPE *dist;
    int *iorder;
    int *jorder;
    float b[4];
} TSP;

#define MOD(i,n)    ((i) % (n) >= 0 ? (i) % (n) : (i) % (n) + (n))
#define D(x,y) dist[(x)*n+y]        

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define sqr(x)   ((x)*(x))

/*
 * Prim's approximated TSP tour
 * See also [Cristophides'92]
 */
int findEulerianPath(TSP *tsp)
{
    int *mst, *arc;    
    int d, i, j, k, l, a;
    int n, *iorder, *jorder;
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
    DBG("findEulerianPath: 1");

    j = -1;
    d = maxd;
    dis[0] = -1;
    for (i = 1; i < n; i++)
    {
        dis[i] = D(i,0);
        arc[i] = 0;
        if (d > dis[i])
        {
            d = dis[i];
            j = i;
        }
    }
    DBG("findEulerianPath: j=%d", j);

    if (j == -1)
        elog(ERROR, "Error TSP fail to findEulerianPath, check your distance matrix is valid.");

    /*
     * O(n^2) Minimum Spanning Trees by Prim and Jarnick 
     * for graphs with adjacency matrix. 
     */
    for (a = 0; a < n - 1; a++)
    {
        mst[a] = j * n + arc[j]; /* join fragment j with MST */
        dis[j] = -1; 
        d = maxd;
        for (i = 0; i < n; i++)
        {
            if (dis[i] >= 0) /* not connected yet */
            {
                if (dis[i] > D(i,j))
                {
                    dis[i] = D(i,j);
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
    DBG("findEulerianPath: 3");

    /*
     * Preorder Tour of MST
     */
#define VISITED(x) jorder[x]
#define NQ(x) arc[l++] = x
#define DQ()  arc[--l]
#define EMPTY (l==0)
        
    for (i = 0; i < n; i++) VISITED(i) = 0;
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
    DBG("findEulerianPath: 4");

    return 0;
}

DTYPE pathLength (TSP *tsp)
{
    int i, j, k;
    DTYPE len = 0;

    int *iorder = tsp->iorder;
    DTYPE *dist   = tsp->dist;
    int  n      = tsp->n;
    
    for (i = 0; i < n-1; i++)
    {
        len += D(iorder[i], iorder[i+1]);
    }
    len += D(iorder[n-1], iorder[0]); /* close path */
    return (len);
}

/*
 * Local Search Heuristics
 *  b-------a        b       a
 *  .       .   =>   .\     /.
 *  . d...e .        . e...d .  
 *  ./     \.        .       .
 *  c       f        c-------f
 */
DTYPE getThreeWayCost (TSP *tsp, Path p)
{
    int a, b, c, d, e, f;
    int *iorder = tsp->iorder;
    DTYPE *dist   = tsp->dist;
    int n       = tsp->n;

    a = iorder[MOD(p[0]-1,n)];
    b = iorder[p[0]];
    c = iorder[p[1]];
    d = iorder[MOD(p[1]+1,n)];
    e = iorder[p[2]];
    f = iorder[MOD(p[2]+1,n)];

    return (D(a,d) + D(e,b) + D(c,f) - D(a,b) - D(c,d) - D(e,f)); 
        /* add cost between d and e if non symetric TSP */ 
}

void doThreeWay (TSP *tsp, Path p)
{
    int i, count, m1, m2, m3, a, b, c, d, e, f;
    int *iorder = tsp->iorder;
    int *jorder = tsp->jorder;
    int n       = tsp->n;
    
    a = MOD(p[0]-1,n);
    b = p[0];
    c = p[1];
    d = MOD(p[1]+1,n);
    e = p[2];
    f = MOD(p[2]+1,n);    
    
    m1 = MOD(n+c-b,n)+1;  /* num cities from b to c */
    m2 = MOD(n+a-f,n)+1;  /* num cities from f to a */
    m3 = MOD(n+e-d,n)+1;  /* num cities from d to e */

    count = 0;
    /* [b..c] */
    for (i = 0; i < m1; i++) jorder[count++] = iorder[MOD(i+b,n)];

    /* [f..a] */
    for (i = 0; i < m2; i++) jorder[count++] = iorder[MOD(i+f,n)];

    /* [d..e] */
    for (i = 0; i < m3; i++) jorder[count++] = iorder[MOD(i+d,n)];

    /* copy segment back into iorder */
    for (i = 0; i < n; i++) iorder[i] = jorder[i];
}

/*
 *   c..b       c..b
 *    \/    =>  |  |
 *    /\        |  |
 *   a  d       a  d
 */
DTYPE getReverseCost (TSP *tsp, Path p)
{
    int a, b, c, d;
    int *iorder = tsp->iorder;
    DTYPE *dist   = tsp->dist;
    int n       = tsp->n;
    
    a = iorder[MOD(p[0]-1,n)];
    b = iorder[p[0]];
    c = iorder[p[1]];
    d = iorder[MOD(p[1]+1,n)];
    
    return (D(d,b) + D(c,a) - D(a,b) - D(c,d));
    /* add cost between c and b if non symetric TSP */ 
}

void doReverse(TSP *tsp, Path p)
{
    int i, j, nswaps, first, last, tmp;
    int *iorder = tsp->iorder;
    int n       = tsp->n;
    

    /* reverse path b...c */
    nswaps = (MOD(p[1]-p[0],n)+1)/2;
    for (i = 0; i < nswaps; i++)
    {
        first = MOD(p[0]+i, n);
        last  = MOD(p[1]-i, n);
        tmp   = iorder[first];
        iorder[first] = iorder[last];
        iorder[last]  = tmp;
    }
}

void annealing(TSP *tsp)
{
    Path   p;
    int    i=1, j, pathchg;
    int    numOnPath, numNotOnPath;
    DTYPE    pathlen, bestlen;
    int    n = tsp->n;
    double energyChange, T;

    pathlen = pathLength (tsp); 
    bestlen = pathlen;

    for (T = T_INIT; T > FINAL_T; T *= COOLING)  /* annealing schedule */
    {
        pathchg = 0;
        for (j = 0; j < TRIES_PER_T; j++)
        {
            do {
                p[0] = unifRand (n);
                p[1] = unifRand (n);
                /* non-empty path */
                if (p[0] == p[1]) p[1] = MOD(p[0]+1,n);
                numOnPath = MOD(p[1]-p[0],n) + 1;
                numNotOnPath = n - numOnPath;
            } while (numOnPath < 2 || numNotOnPath < 2); /* non-empty path */
            
            if (RANDOM() % 2) /*  threeWay */
            {
                do {
                    p[2] = MOD(unifRand (numNotOnPath)+p[1]+1,n);
                } while (p[0] == MOD(p[2]+1,n)); /* avoids a non-change */

                energyChange = getThreeWayCost (tsp, p);
                if (energyChange < 0 || RREAL < exp(-energyChange/T) )
                {
                    pathchg++;
                    pathlen += energyChange;
                    doThreeWay (tsp, p);
                }
            }
            else            /* path Reverse */
            {
                energyChange = getReverseCost (tsp, p);
                if (energyChange < 0 || RREAL < exp(-energyChange/T))
                {
                    pathchg++;
                    pathlen += energyChange;
                    doReverse(tsp, p); 
                }
            }
            if (pathlen < bestlen) bestlen = pathlen;
            if (pathchg > IMPROVED_PATH_PER_T) break; /* finish early */
        }   
        DBG("T:%f L:%d B:%d C:%d", T, pathlen, bestlen, pathchg);
        if (pathchg == 0) break;   /* if no change then quit */
    }
}


void reverse(int num, int *ids)
{
    int i, j, t;
    for (i=0, j=num-1; i<j; i++, j--) {
        t = ids[j];
        ids[j] = ids[i];
        ids[i] = t;
    }
}


int find_tsp_solution(int num, DTYPE *cost, int *ids, int start, int end, DTYPE *total_len, char *err_msg)
{
    int   i, j;
    int   istart = 0;
    int   jstart = 0;
    int   iend = -1;
    int   jend = -1;
    int   rev = 0;
    TSP   tsp;
    long  seed = -314159L;

    initRand (seed);

    /* initialize tsp struct */
    tsp.n = num;
    tsp.dist   = NULL;
    tsp.iorder = NULL;
    tsp.jorder = NULL;

    if (!(tsp.iorder = (int*) palloc (tsp.n * sizeof(int)))   ||
        !(tsp.jorder = (int*) palloc (tsp.n * sizeof(int))) ) {
            elog(FATAL, "Memory allocation failed!");
            return -1;
        }

    tsp.dist = cost;
    tsp.maxd = 0;
    for (i=0; i<tsp.n*tsp.n; i++) {
        tsp.maxd = MAX(tsp.maxd, cost[i]);
    }

    /* identity permutation */
    for (i = 0; i < tsp.n; i++) tsp.iorder[i] = i;

    DBG("Initial Path Length: %d", pathLength(&tsp));

    /*
     * Set up first eulerian path iorder to be improved by
     * simulated annealing.
     */
    if(findEulerianPath(&tsp))
        return -1;

    DBG("Approximated Path Length: %d", pathLength(&tsp));

    annealing(&tsp);

    *total_len = pathLength(&tsp);

    DBG("Best Path Length: %.4f", *total_len);

    // reorder ids[] with start as first

#ifdef DEBUG
    for (i=0; i<tsp.n; i++) {
        DBG("i: %d, ids[i]: %d, io[i]: %d, jo[i]: %d",
            i, ids[i], tsp.iorder[i], tsp.jorder[i]);
    }
#endif

    // get index of start node in ids
    for (i=0; i < tsp.n; i++) {
        if (ids[i] == start) istart = i;
        if (ids[i] == end)   iend = i;
    }
    DBG("istart: %d, iend: %d", istart, iend);

    // get the idex of start in iorder
    for (i=0; i < tsp.n; i++) {
        if (tsp.iorder[i] == istart) jstart = i;
        if (tsp.iorder[i] == iend)   jend = i;
    }
    DBG("jstart: %d, jend: %d", jstart, jend);

    /*
     * If the end is specified and the end point is the first in the list
     * or if it is after start node then we need to reverse everything
     * to extract the nodes in the reverse order
     * and later we will reverse them again so they are correct
    */
    if (jend > 0 && jend == jstart+1 || jend == 0) {
        reverse(tsp.n, ids);
        reverse(tsp.n, tsp.iorder);
        reverse(tsp.n, tsp.jorder);
        jstart = tsp.n - jstart - 1;
        jend = tsp.n - jend -1;
        rev = 1;
        DBG("reversed ids: jstart: %d, jend: %d", jstart, jend);
#ifdef DEBUG
        for (i=0; i<tsp.n; i++) {
            DBG("i: %d, ids[i]: %d, io[i]: %d, jo[i]: %d",
                i, ids[i], tsp.iorder[i], tsp.jorder[i]);
        }
#endif
    }

    // copy ids to tsp.jorder so we can rewrite ids
    memcpy(tsp.jorder, ids, tsp.n * sizeof(int));

    // write reordered ids into ids[]
    for (i=jstart, j=0; i < tsp.n; i++, j++)
        ids[j] = tsp.jorder[tsp.iorder[i]];

    for (i=0; i < jstart; i++, j++)
        ids[j] =tsp.jorder[tsp.iorder[i]];

    // if we reversed the order above so put it correct now.
    if (rev)
        reverse(tsp.n, ids);

#ifdef DEBUG
    DBG("ids getting returned!");
    for (i=0; i<tsp.n; i++) {
        DBG("i: %d, ids[i]: %d, io[i]: %d, jo[i]: %d",
            i, ids[i], tsp.iorder[i], tsp.jorder[i]);
    }
#endif

    DBG("tsplib: jstart=%d, jend=%d, n=%d, j=%d", jstart, jend, tsp.n, j);

    return 0;
}

/* EOF */
