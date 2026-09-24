:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2026-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Planar Family ; pgr_planarFaces - Experimental
   single: planarFaces - Experimental on v4.1

|

``pgr_planarFaces`` - Experimental
===============================================================================

``pgr_planarFaces`` - Identifies the faces of a planar embedding and lists every
edge-face incidence for an undirected graph.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.1.0

* New experimental function.

Description
-------------------------------------------------------------------------------

In a planar embedding, the graph is drawn in the plane so that edges meet only
at vertices. A face is a maximal connected region of the plane bounded by
edges.

Every bounded face is a cycle of edges enclosing an interior region.
The exterior face, also called the unbounded face, is the region outside the
outermost cycle.

Once an embedding is fixed, each undirected edge has a left and a right
side when walked in the direction the embedding assigns around each vertex.

In a connected planar undirected graph, each edge separates exactly two faces, so it
contributes one row on the left and one on the right.

``pgr_planarFaces`` returns
those incidences: ``face_id`` names the face, ``edge_id`` is the border edge, and
``side`` is ``1`` (left) or ``2`` (right) relative to the embedding.

The function first computes a planar embedding with the Boyer-Myrvold planarity
test, then walks faces with Boost ``planar_face_traversal``.i

.. rubric:: Characteristics

* Works for undirected graphs.
* Loops and paralell edges are preserved.
* Costs are ignored

  * They are used to determine the existance of an edge

* When the graph is not planar it will emit an ``ERROR``

  * Run :doc:`pgr_isPlanar` first

* Each vertex receives exactly one core number
* Results:

  * The result has one row per edge-side pair

    * Graph with :math:`|E|` edges yields :math:`2|E|` rows.

  * Number of faces:
    * Obeys Euler's formula: :math:`|V| - |E| + |F| = 2`
    * On a graph with :math:`C` components: :math:`|V| - |E| + |F| = 2C`.


  * Number of returned rows: :math:`|V|`.
  * Ordered by ``node`` ascending.
  * No rows returned when no edges found. Emits a ``NOTICE``

* Running time: :math:`O(|E| + |V|)`

  * Loop and parallel removal: :math:`O(|E|)`
  * Peeling algorithm: :math:`O(|E|)`
  * Sorting:  :math:`|V| log |V|`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_planarFaces(`Edges SQL`_)

   | Returns set of ``(seq, face_id, edge_id, side)``

:Example: Faces of the full :doc:`sampledata` graph

.. literalinclude:: planarFaces.queries
   :start-after: -- q1
   :end-before: -- q2

* The query returns **36 rows**, twice the **18** edges in ``edges``, consistent
  with :math:`2|E|`.
* ``face_id`` runs from **1** through **7** on this network; each id is one face
  of the embedding, including the unbounded **exterior face** (here ``face_id = 1``
  borders the outer layout of the city block).
* Every ``edge_id`` appears **twice**, once with ``side = 1`` and once with
  ``side = 2``.
* Rows follow the counterclockwise walk of each face in the computed embedding.
* Have :math:`3` components then number if faces = :math:`17 - 18 + 7 = 6 = 2 \times 3`.


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
	* - ``face_id``
	  - ``BIGINT``
	  - Identifier of the face in the computed embedding.
	* - ``edge_id``
	  - ``BIGINT``
	  - Identifier of the edge that borders the face.
	* - ``side``
	  - ``INTEGER``
	  - | ``1`` when the edge borders the face on the left side.
	    | ``2`` when the edge borders the face on the right side.

Additional Examples
-------------------------------------------------------------------------------

Non-planar graphs
...............................................................................

Face extraction requires a planar graph. Subgraphs containing a :math:`K_5` or
:math:`K_{3,3}` minor cannot be embedded without crossings; ``pgr_planarFaces``
raises ``ERROR: Graph is not planar`` in that case. The :doc:`pgr_isPlanar`
documentation shows a non-planar subgraph of ``sampledata`` (edges highlighted in
blue in the figure).

.. graphviz::

   graph G {
    node [shape=circle;style=filled;width=0.3;fixedsize=true;fontsize=8];
    1,2,3,4,5 [color=lightcoral];
    1 [pos="0,0!"];
    2 [pos="1,0!"];
    3 [pos="1,1!"];
    4 [pos="0,1!"];
    5 [pos="0.5,1.5!"];
    1 -- {2, 3, 4, 5};
    2 -- {3, 4, 5};
    3 -- {4, 5};
    4 -- 5;
   }

Create a :math:`K_5` graph.

.. literalinclude:: planarFaces.queries
   :start-after: -- q2
   :end-before: -- test-planarity

Test planarity: dont execute when is not planar.

.. literalinclude:: planarFaces.queries
   :start-after: -- test-planarity
   :end-before: -- will throw an error

Or get an error: (not shown)

.. literalinclude:: planarFaces.queries
   :start-after: -- will throw an error
   :end-before: -- not testing

Planarity of loops
...............................................................................

One loop graph with only one vertex:

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=0.3;fixedsize=true;fontsize=8];
        1 [color=lightblue];
        1 -- 1;
    }

.. literalinclude:: planarFaces.queries
   :start-after: -- not testing
   :end-before: -- end-one-loop

Two loop graph with only one vertex:

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=0.3;fixedsize=true;fontsize=8];
        1 [color=lightblue];
        1 -- 1 -- 1;
    }

.. literalinclude:: planarFaces.queries
   :start-after: -- end-one-loop
   :end-before: -- end-two-loop

Triangular graphs
...............................................................................

.. rubric:: Triangular graph (no duplicate edges)

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=0.5;fixedsize=true;fontsize=8];
        1,2,3 [color=lightblue];
        1 -- 2 -- 3 -- 1;
    }

.. literalinclude:: planarFaces.queries
   :start-after: -- q3
   :end-before: -- tri-no-dup

.. literalinclude:: planarFaces.queries
   :start-after: -- tri-no-dup
   :end-before: -- q4

* Six rows are returned: :math:`2 \times 3` edges.
* :math:`face\_id = 1` is the interior face.
* :math:`face\_id = 2` is the exterior face.
  face.
* Each edge lists ``1`` on one face and ``2`` on the other.

.. rubric:: Triangular graph (duplicate edges)

.. graphviz::

    graph G {
        node [shape=circle;style=filled;width=0.5;fixedsize=true;fontsize=8];
        1,2,3 [color=lightblue];
        1 -- 2 -- 3 -- 1;
        1 -- 2 -- 3 -- 1;
    }

.. literalinclude:: planarFaces.queries
   :start-after: -- q4
   :end-before: -- q5


See Also
-------------------------------------------------------------------------------

* :doc:`pgr_isPlanar`
* :doc:`sampledata`
* Boost Graph Library: `Planar Face Traversal
  <https://www.boost.org/libs/graph/doc/planar_face_traversal.html>`__
* Boost Graph Library: `Boost: Boyer Myrvold planarity
  <https://www.boost.org/libs/graph/doc/boyer_myrvold.html>`__
* Wikipedia: `Planar graph
  <https://en.wikipedia.org/wiki/Planar_graph>`__
* Boyer, J. M. and Myrvold, W. J. (2004). On the Cutting Edge: Simplified O(n)
  Planarity Algorithms by Edge Addition. Journal of Graph Algorithms and
  Applications, 8(3), 241-273.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

