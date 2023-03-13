..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPointsKSP.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_withPointsKSP.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_withPointsKSP.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPointsKSP.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPointsKSP.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_withPointsKSP.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsKSP.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsKSP.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsKSP.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsKSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsKSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsKSP.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsKSP.html>`__

pgr_withPointsKSP - Proposed
===============================================================================

``pgr_withPointsKSP`` — Yen's algorithm for K shortest paths using Dijkstra.

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

Modifies the graph to include the points defined in the `Points SQL`_ and
using Yen algorithm, finds the :math:`K` shortest paths.

.. index::
    single: withPointsKSP - Proposed on v2.2

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_ **start vid**, **end vid**, **K**, [**options**])
   | **options:**  ``[directed, heap_paths, driving_side, details]``

   | RETURNS SET OF |ksp-result|
   | OR EMPTY SET

:Example: Get 2 paths from Point :math:`1` to point :math:`2` on a directed
          graph.

* For a directed graph.
* The driving side is set as **b** both. So arriving/departing to/from the
  point(s) can be in any direction.
* No details are given about distance of other points of the query.
* No heap paths are returned.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q1
   :end-before: --q2

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: 17 22 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ query as described.
   * - `Points SQL`_
     - ``TEXT``
     - `Points SQL`_ query as described.
   * - **start vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.

       - Negative values represent a point
   * - **end vid**
     - **ANY-INTEGER**
     - Identifier of the destination vertex.

       - Negative values represent a point
   * - **K**
     - **ANY-INTEGER**
     - Number of required paths

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

KSP Optional parameters
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
    :start-after: ksp_optionals_start
    :end-before: ksp_optionals_end

With points optional parameters
...............................................................................

.. include:: withPoints-family.rst
    :start-after: withPoints_optionals_start
    :end-before: withPoints_optionals_end

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

.. include:: pgr_KSP.rst
    :start-after: ksp_returns_start
    :end-before: ksp_returns_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Get :math:`2` paths using left side driving topology, from vertex :math:`1` to
the closest location on the graph of point `(2.9, 1.8)`.

.. literalinclude:: doc-pgr_withPointsKSP.queries
    :start-after: --q4
    :end-before: -- q5

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.

Left driving side
...............................................................................

Get :math:`2` paths using left side driving topology, from point :math:`1` to
point :math:`2` with details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q2
   :end-before: --q3

Right driving side
...............................................................................

Get :math:`2` paths using right side driving topology from, point :math:`1` to
point :math:`2` with heap paths and details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q3
   :end-before: --q4

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`KSP-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

