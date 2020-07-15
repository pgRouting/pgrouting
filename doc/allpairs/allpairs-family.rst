..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

All Pairs - Family of Functions
===============================================================================

The following functions work on all vertices pair combinations

.. index from here

* :doc:`pgr_floydWarshall` - Floyd-Warshall's algorithm.
* :doc:`pgr_johnson` - Johnson's algorithm

.. index to here

.. toctree::
    :hidden:

    pgr_floydWarshall
    pgr_johnson

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/allpairs-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/allpairs-family.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/allpairs-family.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/allpairs-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/allpairs-family.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/allpairs/doc/allpairs.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/allpairs/doc/allpairs.html>`__



Performance
------------------------------------------------------------------------------

The following tests:
 - non server computer
 - with AMD 64 CPU
 - 4G memory
 - trusty
 - posgreSQL version 9.3

Data
.........................................

The following data was used

.. code-block:: none

    BBOX="-122.8,45.4,-122.5,45.6"
    wget --progress=dot:mega -O "sampledata.osm" "https://www.overpass-api.de/api/xapi?*[bbox=${BBOX}][@meta]"


Data processing was done with osm2pgrouting-alpha

.. code-block:: none

    createdb portland
    psql -c "create extension postgis" portland
    psql -c "create extension pgrouting" portland
    osm2pgrouting -f sampledata.osm -d portland -s 0




Results
.........................................

:Test: One

This test is not with a bounding box
The density of the passed graph is extremely low.
For each <SIZE> 30 tests were executed to get the average
The tested query is:

.. code-block:: none

     SELECT count(*) FROM pgr_floydWarshall(
        'SELECT gid as id, source, target, cost, reverse_cost FROM ways where id <=  <SIZE>');

     SELECT count(*) FROM pgr_johnson(
        'SELECT gid as id, source, target, cost, reverse_cost FROM ways where id <=  <SIZE>');

The results of this tests are presented as:

:SIZE: is the number of edges given as input.
:EDGES: is the total number of records in the query.
:DENSITY: is the density of the data :math:`\dfrac{E}{V \times (V-1)}`.
:OUT ROWS: is the number of records returned by the queries.
:Floyd-Warshall: is the average execution time in seconds of pgr_floydWarshall.
:Johnson: is the average execution time in seconds of pgr_johnson.


====== ======  ========== ======== ============== =============
 SIZE  EDGES    DENSITY   OUT ROWS Floyd-Warshall  Johnson
====== ======  ========== ======== ============== =============
   500    500     0.18E-7     1346   0.14         0.13
  1000   1000     0.36E-7     2655   0.23         0.18
  1500   1500     0.55E-7     4110   0.37         0.34
  2000   2000     0.73E-7     5676   0.56         0.37
  2500   2500     0.89E-7     7177   0.84         0.51
  3000   3000     1.07E-7     8778   1.28         0.68
  3500   3500     1.24E-7    10526   2.08         0.95
  4000   4000     1.41E-7    12484   3.16         1.24
  4500   4500     1.58E-7    14354   4.49         1.47
  5000   5000     1.76E-7    16503   6.05         1.78
  5500   5500     1.93E-7    18623   7.53         2.03
  6000   6000     2.11E-7    20710   8.47         2.37
  6500   6500     2.28E-7    22752   9.99         2.68
  7000   7000     2.46E-7    24687  11.82         3.12
  7500   7500     2.64E-7    26861  13.94         3.60
  8000   8000     2.83E-7    29050  15.61         4.09
  8500   8500     3.01E-7    31693  17.43         4.63
  9000   9000     3.17E-7    33879  19.19         5.34
  9500   9500     3.35E-7    36287  20.77         6.24
 10000  10000     3.52E-7    38491  23.26         6.51
====== ======  ========== ======== ============== =============


:Test: Two

This test is with a bounding box
The density of the passed graph higher than of the Test One.
For each <SIZE> 30 tests were executed to get the average
The tested edge query is:

.. code-block:: none

    WITH
        buffer AS (SELECT ST_Buffer(ST_Centroid(ST_Extent(the_geom)), SIZE) AS geom FROM ways),
        bbox AS (SELECT ST_Envelope(ST_Extent(geom)) as box from buffer)
    SELECT gid as id, source, target, cost, reverse_cost FROM ways where the_geom && (SELECT box from bbox);

The tested queries

.. code-block:: none

    SELECT count(*) FROM pgr_floydWarshall(<edge query>)
    SELECT count(*) FROM pgr_johnson(<edge query>)

The results of this tests are presented as:

:SIZE: is the size of the bounding box.
:EDGES: is the total number of records in the query.
:DENSITY: is the density of the data :math:`\dfrac{E}{V \times (V-1)}`.
:OUT ROWS: is the number of records returned by the queries.
:Floyd-Warshall: is the average execution time in seconds of pgr_floydWarshall.
:Johnson: is the average execution time in seconds of pgr_johnson.


====== =====  ======== ======== ============== =============
 SIZE  EDGES   DENSITY OUT ROWS Floyd-Warshall  Johnson
====== =====  ======== ======== ============== =============
 0.001    44  0.0608       1197     0.10       0.10
 0.002    99  0.0251       4330     0.10       0.10
 0.003   223  0.0122      18849     0.12       0.12
 0.004   358  0.0085      71834     0.16       0.16
 0.005   470  0.0070     116290     0.22       0.19
 0.006   639  0.0055     207030     0.37       0.27
 0.007   843  0.0043     346930     0.64       0.38
 0.008   996  0.0037     469936     0.90       0.49
 0.009  1146  0.0032     613135     1.26       0.62
 0.010  1360  0.0027     849304     1.87       0.82
 0.011  1573  0.0024    1147101     2.65       1.04
 0.012  1789  0.0021    1483629     3.72       1.35
 0.013  1975  0.0019    1846897     4.86       1.68
 0.014  2281  0.0017    2438298     7.08       2.28
 0.015  2588  0.0015    3156007    10.28       2.80
 0.016  2958  0.0013    4090618    14.67       3.76
 0.017  3247  0.0012    4868919    18.12       4.48
====== =====  ======== ======== ============== =============


See Also
.........................................

* :doc:`pgr_johnson`
* :doc:`pgr_floydWarshall`
* `Boost floyd-Warshall <https://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html>`_ algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

