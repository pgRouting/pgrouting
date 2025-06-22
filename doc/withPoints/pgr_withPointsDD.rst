..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: withPoints Family ; pgr_withPointsDD
   single: With Points Category ; pgr_withPointsDD
   single: Driving Distance Category ; pgr_withPointsDD
   single: withPointsDD

|

``pgr_withPointsDD``
===============================================================================

``pgr_withPointsDD`` - Returns the driving **distance** from a starting point.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.

.. Breaking change

* Breaking change, signatures no longer available:

  * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
  * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

.. rubric:: Version 3.6.0

* Output columns standardized to |result-spantree|
* **Driving side** parameter is positional unnamed and compulsory.

  * Valid values depend on kind of graph

* When ``details`` is ``false``:

  * Points reached within the distance are not included.

* Deprecated signatures:

  * pgr_withpointsdd(text,text,bigint,double precision,boolean,character,boolean)
  * pgr_withpointsdd(text,text,anyarray,double precision,boolean,character,boolean,boolean)

.. rubric:: Version 2.2.0

* New proposed function.


Description
-------------------------------------------------------------------------------

Modify the graph to include points and using Dijkstra algorithm, extracts all
the nodes and points that have costs less than or equal to the value
``**distance**`` from the starting point.
The edges extracted will conform the corresponding spanning tree.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vid**, **distance**, **driving side**, [**options A**])
   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vids**, **distance**, **driving side**, [**options B**])
   | **options A:** ``[directed, details]``
   | **options B:** ``[directed, details, equicost]``

   | Returns set of |result-spantree|
   | OR EMPTY SET

.. index::
    single: withPointsDD ; Single Vertex

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vid**, **distance**, **driving side**, [**options**])
   | **options:** ``[directed, details]``

   | Returns set of |result-spantree|
   | OR EMPTY SET

:Example: Right side driving topology, from point :math:`1` within a distance of
          :math:`3.3` with details.

.. literalinclude:: withPointsDD.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: withPointsDD ; Multiple Vertices

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsDD(`Edges SQL`_, `Points SQL`_, **root vids**, **distance**, **driving side**, [**options**])
   | **options:** ``[directed, details, equicost]``

   | Returns set of |result-spantree|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`16` within a distance of
          :math:`3.3` with ``equicost`` on a directed graph

.. literalinclude:: withPointsDD.queries
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
   * - **driving side**
     - ``CHAR``
     - - Value in [``r``, ``R``, ``l``, ``L``, ``b``, ``B``] indicating if the driving side is:

         - ``r``, ``R`` for right driving side,
         - ``l``, ``L`` for left driving side.
         - ``b``, ``B`` for both.

       - Valid values differ for directed and undirected graphs:

         - In directed graphs: [``r``, ``R``, ``l``, ``L``].
         - In undirected graphs: [``b``, ``B``].

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``details``
     - ``BOOLEAN``
     - ``false``
     - - When ``true`` the results will include the points that are in the path.
       - When ``false`` the results will not include the points that are in the
         path.

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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: spantree-result-columns-start
   :end-before: spantree-result-columns-end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the driving distance from the two closest locations on the graph of point
`(2.9, 1.8)`.

.. literalinclude:: withPointsDD.queries
    :start-after: -- q5
    :end-before: -- q6

* Point :math:`-1` corresponds to the closest edge from point :math:`(2.9, 1.8)`.
* Point :math:`-2` corresponds to the next close edge from point :math:`(2.9, 1.8)`.

Driving side does not matter
...............................................................................

From point :math:`1` within a distance of :math:`3.3`, does not matter driving
side, with details.

.. literalinclude:: withPointsDD.queries
   :start-after: -- q4
   :end-before: -- q5


See Also
-------------------------------------------------------------------------------

* :doc:`pgr_drivingDistance`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

