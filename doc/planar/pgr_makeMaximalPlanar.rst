:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2020-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Planar Family ; pgr_makeMaximalPlanar - Experimental
   single: makeMaximalPlanar - Experimental on v4.1

|

``pgr_makeMaximalPlanar`` - Experimental
===============================================================================

``pgr_makeMaximalPlanar`` — Returns the set of edges needed to make a planar graph maximal planar.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.1.0

* New experimental function.


Description
-------------------------------------------------------------------------------

``pgr_makeMaximalPlanar`` identifies the missing edges that need to be added to
the connected components of a planar graph to make each eligible component maximal planar.

A planar graph is considered **maximal planar** (or fully triangulated) if no additional
edges can be added to it without violating its planarity. In a maximal planar graph,
every face (including the outer face) is a triangle.

The main characteristics are:

* Works for **undirected** graphs.
* Works for **planar** graphs only.
* If the input graph is not planar, it returns an **empty set**.
* Returns a list of all new edges needed to triangulate the eligible components of the graph and make them maximal planar.
* The augmentation applies independently to eligible components (components with 3 or more vertices). Components with fewer than 3 vertices are skipped and returned unchanged.
* The algorithm does not consider traversal costs in the calculations.
* The algorithm does not consider geometric topology in the calculations.
* Running time: :math:`O(|V_G| + |E_G|)` where :math:`G(V_G, E_G)` is the input graph.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_makeMaximalPlanar(`Edges SQL`_)

   | Returns set of |result-component-make|
   | OR EMPTY SET

:Example: List of edges that are needed to make the sample graph maximal planar.

**Sample graph before:**

.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   Sample graph before

**Output:**

.. literalinclude:: makeMaximalPlanar.queries
   :start-after: -- q1
   :end-before: -- q2

**Sample graph after adding triangulation edges:**

.. figure:: images/maximal_planar_sampledata.png
   :scale: 75%

   Sample graph after adding triangulation edges (olive = original edges, red dashed = 17 new triangulation edges).
   Note: When a graph contains multiple disconnected components, each component is processed independently. Components (2,4) and (13,14) have < 3 vertices and cannot be triangulated; the 13-vertex component is triangulated with 17 new edges.

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

Returns set of |result-component-make|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``BIGINT``
     - Sequential value starting from **1**.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the first end point vertex of the edge.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the second end point vertex of the edge.

Additional Examples
-------------------------------------------------------------------------------

:Example: Triangulating a simple 5-cycle ring graph.

.. literalinclude:: makeMaximalPlanar.queries
   :start-after: -- q2
   :end-before: -- q3

**Sample graph before:**

.. figure:: images/maximal_planar_5cycle_before.png
   :scale: 75%

   Sample 5-cycle ring graph before triangulation.

**Output:**

.. literalinclude:: makeMaximalPlanar.queries
   :start-after: -- q3
   :end-before: -- q4

**Sample graph after adding triangulation edges:**

.. figure:: images/maximal_planar_5cycle.png
   :scale: 75%

   Maximal planar triangulation of a simple 5-cycle ring graph (4 red dashed triangulation edges added).

See Also
-------------------------------------------------------------------------------

* `Boost: make_maximal_planar
  <https://www.boost.org/libs/graph/doc/make_maximal_planar.html>`__
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
