.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _all_pairs:


All pairs
===============================================================================

The following functions work an all vertices pair combinations

    - :ref:`pgr_floydWarshall` - Floyd-Warshall's algorithm.
    - :ref:`pgr_johnson` - Johnson's algorithm

     
.. toctree::
    :hidden: 

    ./pgr_floydWarshall
    ./pgr_johnson


Performance
===============================================================================

The following tests:
 - non server computer
 - with AMD 64 CPU
 - 4M memory
 - trusty
 - posgresql version 9.3

data
----

The following data was used

.. code-block:: none

    BBOX="-122.8,45.4,-122.5,45.6"
    wget --progress=dot:mega -O "sampledata.osm" "http://www.overpass-api.de/api/xapi?*[bbox=${BBOX}][@meta]"


Data procesing was done with osm2pgrouting-alpha

.. code-block:: none

    createdb portland
    psql -c "create extension postgis" portland
    psql -c "create extension pgrouting" portland
    osm2pgrouting -f sampledata.osm -d portland -s 0



The results
-----------

The results of this test are presented as:

:SIZE: is the number of edges given as input
:COUNT: is the average number of records received as output
:AVG TIME: is the average execution time in seconds


pgr_floydWarshall 
-------------------------------------------------------------------------------

This query is not with a bounding box
The conectivity of the passed graph is very low.
For each <SIZE> 30 tests were executed to get the average
The tested query is:

.. code-block:: none

     SELECT count(*) FROM pgr_floydWarshall('SELECTt gid as id, source, target, cost, reverse_cost FROM ways ORDER BY gid LIMIT <SIZE>');



====== =========== ===========
 SIZE     COUNT    AVG TIME(s)
====== =========== ===========
   500      1346    0.03
  1000      2655    0.11
  1500      4110    0.23
  2000      5676    0.39
  2500      7177    0.61
  3000      8778    0.97
  3500     10526    1.65
  4000     12484    2.38
  4500     14354    3.60
  5000     16503    4.92
  5500     18623    6.39
  6000     20710    8.11
  6500     22752    7.42
  7000     24687    8.91
  7500     26861   10.72
  8000     29050   12.77
  8500     31693   15.02
  9000     33879   16.35
  9500     36287   17.72
 10000     38491   19.81
====== =========== ===========

pgr_jhonson 
-------------------------------------------------------------------------------

This query is not with a bounding box
The conectivity of the passed graph is very low.
For each <SIZE> 30 tests were executed to get the average
The tested query is:

.. code-block:: none

     SELECT count(*) FROM pgr_johnson('SELECTt gid as id, source, target, cost, reverse_cost FROM ways ORDER BY gid LIMIT <SIZE>');


====== =========== ===========
 SIZE     COUNT    AVG TIME(s)
====== =========== ===========
   500      1346   0.02
  1000      2655   0.07
  1500      4110   0.15
  2000      5676   0.25
  2500      7177   0.39
  3000      8778   0.52
  3500     10526   0.69
  4000     12484   0.85
  4500     14354   1.07
  5000     16503   1.34
  5500     18623   1.64
  6000     20710   1.95
  6500     22752   2.34
  7000     24687   2.79
  7500     26861   3.41
  8000     29050   4.00
  8500     31693   4.59
  9000     33879   5.19
  9500     36287   6.14
 10000     38491   6.37
====== =========== ===========

See Also
-------------------------------------------------------------------------------

* :ref:`pgr_johnson`
* :ref:`pgr_floydWarshall`
* `Boost floyd-Warshall <http://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html>`_ algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

