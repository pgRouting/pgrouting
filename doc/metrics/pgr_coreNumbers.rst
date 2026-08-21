:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2026-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Metrics Family ; pgr_coreNumbers - Experimental
   single: coreNumbers - Experimental on v4.1

|

``pgr_coreNumbers`` - Experimental
===============================================================================

``pgr_coreNumbers`` - Computes the core number of each vertex in an undirected
graph using k-core decomposition.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.1.0

* New experimental function.

Description
-------------------------------------------------------------------------------

The **core number** of a vertex is the largest value :math:`k` such that the
vertex **belongs to the** :math:`k`-core of the graph.

The :math:`k`-core of a graph is the maximal subgraph in which every vertex has
degree at least :math:`k` within that subgraph. K-core decomposition iteratively
removes vertices of degree less than :math:`k` until no such vertices remain,
increasing :math:`k` at each stage. The process assigns one core number to every
vertex and runs in :math:`O(m)` time on a graph with :math:`m` edges.

The core number says how deeply a vertex sits inside the graph:

* Core :math:`1` vertices are peeled in the first step. They are **terminal
  nodes**, adjacent to at most one other vertex in the induced subgraph: the
  dead-ends and cul-de-sacs of a road network.
* Core :math:`2` vertices survive that step. They lie on cycles, or on the
  chains joining them, like a rural highway that runs on before it branches.
* Core :math:`3` and above mark dense regions where every vertex keeps three or
  more neighbors however much of the graph is peeled away. Urban street grids,
  which offer alternate routes at most intersections, behave this way.

The function applies to **undirected** graphs only. Edge direction and traversal
costs are ignored; only the edge endpoints matter. Each vertex receives exactly
one core number, all vertices in the graph are returned, and the number of rows
is :math:`|V|`. Results are ordered by ``node`` ascending. When the edge SQL
returns no rows, the function emits a notice and returns no rows.

**Parallel edges** between the same pair of vertices are collapsed into a single
edge before the peeling starts, so edge multiplicity does not inflate core
numbers: three parallel edges between two vertices give both vertices core
:math:`1`, the same as a single edge. This matters when importing road networks
that contain duplicate geometry. A **self loop** is not a neighbor of its own
vertex, so it is dropped before peeling and does not contribute to that
vertex's degree. A vertex whose only edge is a self loop has no real
neighbors left, and therefore does not appear in the result.

|Boost| Boost Graph Inside

.. rubric:: References

* Batagelj, V. and Zaversnik, M. (2003). An O(m) Algorithm for Cores Decomposition
  of Networks. arXiv:cs/0310049.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_coreNumbers(`Edges SQL`_)

   | Returns set of ``(seq, node, core)``

:Example: Core numbers on a subgraph of :doc:`sampledata`

.. literalinclude:: coreNumbers.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: Explanation

Vertices are colored by core number in the diagrams below:
``deepskyblue`` for core :math:`1` and ``green`` for core :math:`2`.

* All returned vertices have core number :math:`1`; they belong to the 1-core but
  not to any higher core.
* Vertices :math:`5` and :math:`15` are terminal nodes in this subgraph (degree
  :math:`1`).
* Vertices :math:`6`, :math:`7`, and :math:`10` have higher degree here, but
  peeling degree-:math:`1` vertices first removes the entire subgraph before a
  2-core can form.

The following diagram shows edges :math:`\{1, \ldots, 7\}` from
:doc:`sampledata`; every vertex label shows ``node (core)``.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;fixedsize=true;fontsize=8];
        1,3,5,6,7,10,11,15 [color=deepskyblue];
        5 [pos="0,0!";label="5 (1)"];
        6 [pos="1,0!";label="6 (1)"];
        10 [pos="2,0!";label="10 (1)"];
        15 [pos="3,0!";label="15 (1)"];
        7 [pos="1,1!";label="7 (1)"];
        11 [pos="2,1!";label="11 (1)"];
        1 [pos="0,2!";label="1 (1)"];
        3 [pos="1,2!";label="3 (1)"];
        5 -- 6 [label="1"];
        6 -- 10 [label="2"];
        10 -- 15 [label="3"];
        6 -- 7 [label="4"];
        10 -- 11 [label="5"];
        1 -- 3 [label="6"];
        3 -- 7 [label="7"];
    }

The next subgraph uses edges :math:`\{1, 2, 3, 4\}`, the same set as the main
example of :doc:`pgr_betweennessCentrality`. Every vertex is still core
:math:`1` because the structure is a tree-like path with terminal nodes at
:math:`5` and :math:`15`.

.. graphviz::

    graph G {
        5,7,15,6,10 [shape=circle;style=filled;width=.5;fixedsize=true;fontsize=8];
        5,7,15,6,10 [color=deepskyblue];
        5 [pos="0,0!";label="5 (1)"];
        6 [pos="0,1!";label="6 (1)"];
        7 [pos="0,2!";label="7 (1)"];
        10 [pos="1,1!";label="10 (1)"];
        15 [pos="2,1!";label="15 (1)"];
        5 -- 6 [label="1"];
        6 -- 10 [label="2"];
        10 -- 15 [label="3"];
        6 -- 7 [label="4"];
    }

.. rubric:: K-core peeling (illustration)

On edges :math:`id < 10`, vertices :math:`5` and :math:`15` are removed first
because they have degree :math:`1`. After further peeling, vertices
:math:`6`, :math:`7`, :math:`10`, and :math:`11` remain as the 2-core (shown in
green below).

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;fixedsize=true;fontsize=8];
        1,3,5,15,16 [color=deepskyblue];
        6,7,10,11 [color=green];
        5 [pos="0,0!";label="5 (1)"];
        6 [pos="1,0!";label="6 (2)"];
        10 [pos="2,0!";label="10 (2)"];
        15 [pos="3,0!";label="15 (1)"];
        7 [pos="1,1!";label="7 (2)"];
        11 [pos="2,1!";label="11 (2)"];
        16 [pos="3,2!";label="16 (1)"];
        1 [pos="0,2!";label="1 (1)"];
        3 [pos="1,2!";label="3 (1)"];
        5 -- 6 [label="1"];
        6 -- 10 [label="2"];
        10 -- 15 [label="3"];
        6 -- 7 [label="4"];
        10 -- 11 [label="5"];
        7 -- 11 [label="8"];
        11 -- 16 [label="9"];
        1 -- 3 [label="6"];
        3 -- 7 [label="7"];
    }

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

.. list-table::
	:width: 81
	:widths: auto
	:header-rows: 1

	* - Column
	  - Type
	  - Description
	* - ``seq``
	  - ``BIGINT``
	  - Sequential value starting from 1.
	* - ``node``
	  - ``BIGINT``
	  - Identifier of the vertex.
	* - ``core``
	  - ``BIGINT``
	  - Core number of the vertex: the largest :math:`k` for which the vertex
	    belongs to the :math:`k`-core.

Additional Examples
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.

Sample network overview
...............................................................................

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

   Directed sample network. ``pgr_coreNumbers`` treats every edge as undirected,
   so ``cost`` and ``reverse_cost`` are not used in the calculation.

Comparing with :doc:`pgr_betweennessCentrality` on the same edges
...............................................................................

Both functions use the same ``Edges SQL`` format. The betweenness example on
edges :math:`\{1, 2, 3, 4\}` measures shortest-path influence; core numbers
measure peeling resilience.

1) Core numbers on edges :math:`\{1, 2, 3, 4\}`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: coreNumbers.queries
   :start-after: -- q2
   :end-before: -- q3

.. rubric:: Explanation

* Vertices :math:`5` and :math:`15` are terminal (core :math:`1`).
* Vertex :math:`6` has degree :math:`3` in this subgraph but still has core
  :math:`1`, because removing :math:`5` and :math:`15` first leaves no subgraph
  where every vertex has degree at least :math:`2`.
* Compare with :doc:`pgr_betweennessCentrality` on the same edges: vertex
  :math:`6` has the highest betweenness, but core number does not rank vertices
  by path importance.

Subgraph with core-:math:`2` vertices
...............................................................................

Adding edges :math:`5` through :math:`9` connects the inner vertices into a
2-core. The undirected layout below shows the same network.

.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   Undirected view of the sample network

2) Core numbers on edges :math:`id < 10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: coreNumbers.queries
   :start-after: -- q3
   :end-before: -- q4

.. rubric:: Explanation

* Vertices :math:`6`, :math:`7`, :math:`10`, and :math:`11` have core :math:`2`.
* Vertices :math:`1`, :math:`3`, :math:`5`, :math:`15`, and :math:`16` are
  attached by tree-like branches and remain core :math:`1`.
* Edge :math:`8` (between :math:`7` and :math:`11`) closes a cycle with edges
  :math:`4`, :math:`2`, and :math:`5`, which is why the inner four vertices
  survive peeling.

Filtering results in SQL
...............................................................................

``pgr_coreNumbers`` always returns every vertex. To keep only the dense core,
wrap the call in a standard SQL query.

3) Vertices with core :math:`\geq 2` on edges :math:`id < 10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: coreNumbers.queries
   :start-after: -- q4
   :end-before: -- q5

.. rubric:: Explanation

* The ``WHERE core >= 2`` clause keeps only the 2-core vertices.
* Use it to export only the hub vertices for further analysis.

Full sample graph
...............................................................................

4) Core numbers on all edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: coreNumbers.queries
   :start-after: -- q5
   :end-before: -- q6

.. rubric:: Explanation

* The full network has :math:`17` vertices and maximum core number :math:`2`.
* Terminal branches (vertices :math:`1`, :math:`2`, :math:`3`, :math:`4`,
  :math:`5`, :math:`9`, :math:`13`, :math:`14`) remain core :math:`1`.
* The 2-core is vertices :math:`6`, :math:`7`, :math:`8`, :math:`10`,
  :math:`11`, :math:`12`, :math:`15`, :math:`16` and :math:`17`: the central
  block of the city network. Vertex :math:`9` hangs off :math:`8` by edge
  :math:`14` alone, so it is peeled first and stays core :math:`1`.

Small graphs
...............................................................................

5) Single edge (two terminal nodes)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Both endpoints have core number :math:`1`.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;color=deepskyblue;fixedsize=true;fontsize=8];
        5 [label="5 (1)"];
        6 [label="6 (1)"];
        5 -- 6 [label="1"];
    }

.. literalinclude:: coreNumbers.queries
   :start-after: -- q6
   :end-before: -- q7

.. rubric:: Explanation

* Edge :math:`1` from :doc:`sampledata` connects vertices :math:`5` and :math:`6`.
* Each endpoint has degree :math:`1`, so both are peeled immediately at
  :math:`k = 1`.

6) Triangle (all vertices core :math:`2`)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

In a triangle every vertex has degree :math:`2`, so all three belong to the
2-core.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;color=green;fixedsize=true;fontsize=8];
        1 [label="1 (2)"];
        2 [label="2 (2)"];
        3 [label="3 (2)"];
        1 -- 2 [label="e1"];
        2 -- 3 [label="e2"];
        3 -- 1 [label="e3"];
    }

.. literalinclude:: coreNumbers.queries
   :start-after: -- q7
   :end-before: -- q8

.. rubric:: Explanation

* The ``VALUES`` clause builds a minimal three-edge graph inline.
* No vertex can be peeled before :math:`k = 2`, so every core number is
  :math:`2`.

7) Square cycle from :doc:`sampledata` (all vertices core :math:`2`)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Edges :math:`\{2, 4, 5, 8\}` form a 4-cycle on vertices
:math:`\{6, 7, 10, 11\}`.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;color=green;fixedsize=true;fontsize=8];
        6 [label="6 (2)"];
        7 [label="7 (2)"];
        10 [label="10 (2)"];
        11 [label="11 (2)"];
        6 -- 10 [label="2"];
        6 -- 7 [label="4"];
        10 -- 11 [label="5"];
        7 -- 11 [label="8"];
    }

.. literalinclude:: coreNumbers.queries
   :start-after: -- q8
   :end-before: -- q9

.. rubric:: Explanation

* This is the 2-core heart of example 2) before branches are attached.
* Every vertex has degree :math:`2` in the cycle, so all core numbers are
  :math:`2`.

8) Star graph (hub and spokes, all core :math:`1`)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

A star has a central hub connected to leaf vertices. Every vertex is peeled at
:math:`k = 1`.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;color=deepskyblue;fixedsize=true;fontsize=8];
        1 [label="1 (1)"];
        2 [label="2 (1)"];
        3 [label="3 (1)"];
        4 [label="4 (1)"];
        5 [label="5 (1)"];
        1 -- 2;
        1 -- 3;
        1 -- 4;
        1 -- 5;
    }

.. literalinclude:: coreNumbers.queries
   :start-after: -- q9
   :end-before: -- q10

.. rubric:: Explanation

* The hub has degree :math:`4`, but removing the four leaves leaves an isolated
  vertex, so the hub never belongs to a 2-core.
* Star-like cul-de-sac layouts in road networks often produce core :math:`1`
  vertices throughout.

Empty edge set
...............................................................................

9) No edges in the SQL query
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: coreNumbers.queries
   :start-after: -- q10
   :end-before: -- q11

.. rubric:: Explanation

* When the inner query returns no edges, a notice is emitted and the result is
  empty.
* No exception is raised.

Core numbers and planarity
...............................................................................

Vertices with higher core numbers often lie in densely connected regions. The
workflow below computes core numbers, filters the 2-core, and checks whether the
subgraph can be drawn without edge crossings using :doc:`pgr_isPlanar`.

10) Planarity of the subgraph with edges :math:`id < 10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

First compute core numbers (example 2)), then filter the 2-core (example 3)),
then test planarity:

.. code-block:: sql

   -- Step 1: core numbers on edges id < 10
   SELECT * FROM pgr_coreNumbers(
   'SELECT id, source, target, cost, reverse_cost
   FROM edges WHERE id < 10'
   ) ORDER BY node;

   -- Step 2: keep only vertices in the 2-core
   SELECT node, core
   FROM pgr_coreNumbers(
   'SELECT id, source, target, cost, reverse_cost
   FROM edges WHERE id < 10'
   ) WHERE core >= 2
   ORDER BY node;

   -- Step 3: check planarity of the same edge set
   SELECT pgr_isPlanar(
   'SELECT id, source, target, cost, reverse_cost
   FROM edges WHERE id < 10'
   );

.. rubric:: Explanation

* The subgraph with edges :math:`id < 10` is planar (``pgr_isPlanar`` returns
  ``true``).
* The four vertices with core :math:`2` lie in the densest part of this subgraph.
* Planarity and core number measure different properties: a graph can be planar
  while still having a mix of core-:math:`1` and core-:math:`2` vertices.

11) Complete graph :math:`K_4` (all vertices core :math:`3`)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The examples above reach at most core :math:`2`, because the
:doc:`sampledata` network has degeneracy :math:`2`. Higher cores need a denser
graph. In the complete graph :math:`K_4` every vertex is adjacent to the other
three, so no vertex can ever be peeled at :math:`k \leq 3` and the whole graph
is its own 3-core.

.. literalinclude:: coreNumbers.queries
   :start-after: -- q11
   :end-before: -- q12

.. rubric:: Explanation

* Every vertex has degree :math:`3`, so nothing is removed while peeling at
  :math:`k = 1`, :math:`2` or :math:`3`: all four vertices have core :math:`3`.
* In general the complete graph :math:`K_n` has core number :math:`n - 1` for
  every vertex, which is the maximum possible core number on :math:`n` vertices.
* The degeneracy of this graph is :math:`3`, compared with :math:`2` for the
  :doc:`sampledata` network.

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=.5;fixedsize=true;fontsize=8];
        1,2,3,4 [color=orange];
        1 [pos="0,1!";label="1 (3)"];
        2 [pos="1,1!";label="2 (3)"];
        3 [pos="1,0!";label="3 (3)"];
        4 [pos="0,0!";label="4 (3)"];
        1 -- 2 [label="1"];
        1 -- 3 [label="2"];
        1 -- 4 [label="3"];
        2 -- 3 [label="4"];
        2 -- 4 [label="5"];
        3 -- 4 [label="6"];
    }

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* :doc:`pgr_betweennessCentrality`
* :doc:`pgr_degree`
* :doc:`pgr_isPlanar`
* :doc:`metrics-family`
* Batagelj, V. and Zaversnik, M. (2003). `An O(m) Algorithm for Cores Decomposition
  of Networks <https://arxiv.org/abs/cs/0310049>`__
* `Wikipedia: Degeneracy (graph theory)
  <https://en.wikipedia.org/wiki/Degeneracy_(graph_theory)>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
