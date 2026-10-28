:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2022-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Miscellaneous Algorithms ; pgr_hawickCircuits
   single: Hawick Circuits - Experimental on v3.4

|

``pgr_hawickCircuits`` - Experimental
===============================================================================

``pgr_hawickCircuits`` — Returns the list of circuits using hawick circuits algorithm.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.1.0

* New optional parameters: ``min_depth``, ``max_depth``, ``self_loops`` and
  ``u_turn_on_edge``.
* New signatures to select starting vertices.
* Deprecated signature: ``pgr_hawickCircuits(text)``.

.. rubric:: Version 3.4.0

* New experimental function.

Description
-------------------------------------------------------------------------------

Hawick Circuit algorithm, is published in 2008 by Ken Hawick and Heath A. James.
This algorithm solves the problem of detecting and enumerating circuits in graphs.

Circuit enumeration in directed graphs, where circuits with parallel arcs are
enumerated.

* Each combination of parallel arcs is returned as a different circuit.

.. circuits_characteristics_start

**The main characteristics are:**

- Works for directed graphs.
- The algorithm outputs the distinct circuits present in the graph.
- The circuits are elementary: a vertex is not repeated within a circuit, except
  the starting vertex that closes the circuit.

- When no initial vertices are given all circuits are in canonical order of the
  vertices insertion of the graph: A circuit with vertices ``u``, ``v``: ``u``
  starting circuits containing ``v`` will not be listed on ``start_vid = v`` rows.

  - Different edge ordering gives different canonical ordering.

- When start vertices are given, all circuits are returned for each requested
  vertex it contains, and the results are grouped by ``start_vid``.
- The graph passed to the algorithm discard parallel edges with higher costs.
- Time Complexity: :math:`O((V + E) (c + 1))`

  - where :math:`|E|` is the number of edges in the graph,
  - where :math:`|V|` is the number of vertices in the graph.
  - where :math:`|c|` is the number of circuits in the graph.

|Boost| Boost Graph Inside

.. circuits_characteristics_end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_hawickCircuits(`Edges SQL`_)
   | pgr_hawickCircuits(`Edges SQL`_, **start vid** [, **options**])
   | pgr_hawickCircuits(`Edges SQL`_, **start vids** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

.. index::
    single: pgr_hawickCircuits ; All circuits

All circuits
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickCircuits(`Edges SQL`_ [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: All the circuits of the pgRouting :doc:`sampledata`

When no starting vertex is given, all the circuits of the graph are returned.
The result has 94 rows, one ``path_id`` for each circuit:

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: pgr_hawickCircuits ; Single start vertex

Single start vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickCircuits(`Edges SQL`_, **start vid** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: Circuits that contain vertex :math:`10`

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: pgr_hawickCircuits ; Many start vertices

Many start vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickCircuits(`Edges SQL`_, **start vids** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: Circuits that contain vertices :math:`\{10, 11\}`

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q3
   :end-before: -- q4

Parameters
-------------------------------------------------------------------------------

.. include:: circuits-family.rst
    :start-after: circuits_parameters_start
    :end-before: circuits_parameters_end

Optional parameters
...............................................................................

.. include:: circuits-family.rst
    :start-after: circuits_optionals_start
    :end-before: circuits_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: circuits-family.rst
    :start-after: circuits_result_columns_start
    :end-before: circuits_result_columns_end

Additional Examples
-------------------------------------------------------------------------------

.. graph_start

The examples use the following table ``c_example``, that represent a cul_de_sac
where the exit is on vertex :math:`5` and the vehicle is on vertex :math:`1` but
pointing at the wrong direction of the exit.

.. graphviz::

   digraph G {
     graph [splines=false, sep=1];
     node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
     edge [arrowsize=0.5,fontsize=8];

     1 [pos="0,0!"];
     2 [pos="0,2!"];
     3 [pos="2,0!"];
     4 [pos="-1,0!"];
     5 [pos="-1,1!"];

     1 -> 2 [label="1", dir=both];
     2 -> 3 [label="2", dir=both];
     1 -> 3 [label="3", dir=both];
     1 -> 3 [label="4", dir=both,penwidth=4];
     1 -> 4 [label="5", color=orange,dir=both];
     1 -> 5 [label="6"];
     1 -> 1 [label="7", headport=s, tailport=s];
   }

.. graph_end

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q4
   :end-before: -- q5

:Example 1: All possible ways to return to vertex :math:`1`.

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q5
   :end-before: -- q6

:Example 2: Exclude self loops.

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q6
   :end-before: -- q7

:Example 3: Exclude self loops and U-turn on the same edge.

.. literalinclude:: hawickCircuits.queries
   :start-after: -- q7
   :end-before: -- q8

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Hawick Circuit Algorithm
  <https://www.boost.org/doc/libs/latest/libs/graph/doc/html/graph/algorithms/cycle_detection/hawick_circuits.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
