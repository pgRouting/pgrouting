..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPointsDD.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_withPointsDD.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPointsDD.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPointsDD.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_withPointsDD.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsDD.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsDD.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsDD.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsDD.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsDD.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsDD.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsDD.html>`__

``pgr_withPointsDD`` - Proposed
===============================================================================

``pgr_withPointsDD`` - Returns the driving **distance** from a starting point.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * New **proposed** function


Description
-------------------------------------------------------------------------------

Modify the graph to include points and using Dijkstra algorithm, extracts all
the nodes and points that have costs less than or equal to the value
``**distance**`` from the starting point.
The edges extracted will conform the corresponding spanning tree.

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vid**, **distance**, [**options A**])
   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vids**, **distance**, [**options B**])
   | **options A:** ``[directed, driving_side, details]``
   | **options B:** ``[directed, driving_side, details, equicost]``

   | RETURNS SET OF |result-generic-no-seq|
   | OR EMPTY SET

.. index::
    single: withPointsDD(Single Vertex) - Proposed on v2.2

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vid**, **distance**, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | RETURNS SET OF |result-1-1-no-seq|
   | OR EMPTY SET

:Example: Right side driving topology, from point :math:`1` within a distance of
          :math:`3.3` with details.

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: withPointsDD(Multiple Vertices) - Proposed on v2.2

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vids**, **distance**, [**options**])
   | **options:** ``[directed, driving_side, details, equicost]``

   | RETURNS SET OF |result-m-1-no-seq|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`16` within a distance of
          :math:`3.3` with ``equicost`` on a directed graph

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: -- q3
   :end-before: -- q4

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: 14 19 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below
   * - `Points SQL`_
     - ``TEXT``
     - `Points SQL`_ as described below
   * - **Root vid**
     - ``BIGINT``
     - Identifier of the root vertex of the tree.

       - Negative values represent a point
   * - **Root vids**
     - ``ARRAY`` [**ANY-INTEGER**]
     - Array of identifiers of the root vertices.


       - Negative values represent a point
       - :math:`0` values are ignored
       - For optimization purposes, any duplicated value is ignored.
   * - **distance**
     - ``FLOAT``
     - Upper limit for the inclusion of a node in the result.

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. include:: withPoints-family.rst
   :start-after: withPoints_optionals_start
   :end-before: withPoints_optionals_end

Driving distance optional parameters
...............................................................................

.. include:: pgr_drivingDistance.rst
    :start-after: equicost_start
    :end-before: equicost_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SET OF |result-generic-no-seq|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - ``seq``
     - ``BIGINT``
     - Sequential value starting from :math:`1`.
   * - ``[start_vid]``
     - ``BIGINT``
     - Identifier of the root vertex.

   * - ``node``
     - ``BIGINT``
     - Identifier of ``node`` within the limits from ``from_v``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the ``edge`` used to arrive to ``node``.

       - :math:`0` when ``node`` = ``from_v``.

   * - ``cost``
     - ``FLOAT``
     - Cost to traverse ``edge``.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``from_v`` to ``node``.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, FLOAT, NUMERIC

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the driving distance from the two closest locations on the graph of point
`(2.9, 1.8)`.

.. literalinclude:: doc-pgr_withPointsDD.queries
    :start-after: -- q5
    :end-before: -- q6

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9,1.8)`.

Driving side does not matter
...............................................................................

From point :math:`1` within a distance of :math:`3.3`, does not matter driving
side, with details.

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: -- q4
   :end-before: -- q5


See Also
-------------------------------------------------------------------------------

* :doc:`pgr_drivingDistance`
* :doc:`pgr_alphaShape`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

