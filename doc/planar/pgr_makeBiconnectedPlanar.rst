:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2020-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Planar Family ; pgr_makeBiconnectedPlanar - Experimental
   single: makeBiconnectedPlanar - Experimental on v4.1

|

``pgr_makeBiconnectedPlanar`` - Experimental
===============================================================================

``pgr_makeBiconnectedPlanar`` — Returns the set of edges needed to make each connected component of a planar graph biconnected.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.1.0

* New experimental function.


Description
-------------------------------------------------------------------------------

``pgr_makeBiconnectedPlanar`` identifies the missing edges that need to be added to each
connected component of an existing planar graph to make those components biconnected,
while ensuring the graph remains planar.

A graph is considered **biconnected** if it is connected and cannot be broken into
disconnected pieces by deleting any single vertex (it has no articulation points).
A planar graph is one that can be drawn in two-dimensional space with no two of
its edges crossing.

The main characteristics are:

* Works for undirected graphs.
* If the graph is not connected it will treat each component as an idividual
  graph.

  * Run :doc:`pgr_makeConnected` to connect all components.

* When the graph is not planar it will emit an ``ERROR``

  * Run :doc:`pgr_isPlanar` first.

* Loops and parallel edges are removed.

  * All vertices are kept.

* Costs are ignored.

* Results:

  * Returns a list of all new edges needed to make biconected graphs out of each
    connected component.
  * Empty results when the graph is already biconnected planar.

* Running time: :math:`O(|V| + |E| + R \log R)` for each component

  * :math:`R` is the number of returned edges.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_makeBiconnectedPlanar(`Edges SQL`_)

   | Returns set of |result-component-make|
   | OR EMPTY SET


:example: List of edges that are needed to make the graph biconnected planar.

.. literalinclude:: makeBiconnectedPlanar.queries
   :start-after: -- q1
   :end-before: -- q2


Sample graph after adding biconnecting edges (olive = original edges, red
dashed = 4 new biconnecting edges).

.. figure:: images/biconnected_planar_sampledata.png
   :scale: 75%

When a graph contains multiple disconnected components, each component
is processed independently. Components (2,4) and (13,14) are already
biconnected (0 articulation points) and require no new edges.

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

:Example: 4-vertex path graph.

Create a 4-vertex tree graph.

.. graphviz::

   graph G {
      node [color=cyan;shape=circle;style=filled;width=0.3;fixedsize=true;fontsize=8];
      1 [pos="1,1!"];
      2 [pos="0,0!"];
      3 [pos="1,0!"];
      4 [pos="2,0!"];
      1 -- {2, 3, 4};
   }


.. literalinclude:: makeBiconnectedPlanar.queries
   :start-after: -- q2
   :end-before: -- q3

Get the edges needed to have biconnected planar graph.

.. literalinclude:: makeBiconnectedPlanar.queries
   :start-after: -- q3
   :end-before: -- q4

Adding the edges to the graph

.. graphviz::

   graph G {
      node [color=cyan;shape=circle;style=filled;width=0.3;fixedsize=true;fontsize=8];
      1 [pos="1,1!"];
      2 [pos="0,0!"];
      3 [pos="1,0!"];
      4 [pos="2,0!"];
      1 -- {2, 3, 4};
      2 -- 3 [color=red];
      3 -- 4 [color=red];
   }

See Also
-------------------------------------------------------------------------------

* `Boost: make_biconnected_planar
  <https://www.boost.org/doc/libs/latest/libs/graph/doc/html/graph/algorithms/planar/make_biconnected_planar.html>`__
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
