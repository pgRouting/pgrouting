..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Metrics Family ; pgr_bandwidth - Experimental
   single: bandwidth - Experimental on v4.0

|

``pgr_bandwidth`` - Experimental
===============================================================================

``pgr_bandwidth`` - Calculates the bandwidth of the graph

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubic:: Version 4.0.0

* New experimental function.

Description
-------------------------------------------------------------------------------

Bandwidth measures how "spread out" the connections are in a graph when vertices are arranged in a linear order (like numbering them 1, 2, 3, etc.).

* For each edge in the graph, calculate the distance between the vertex numbers it connects
* The bandwidth is the maximum of all these distances
* The implementation is for undirected graphs
* Run time is 0.160789 seconds

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr_bandwidth(`Edges SQL`_)

   | Returns ``BIGINT``

:Example: For an undirected graph with edges.

.. literalinclude:: bandwidth.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: edges_start
    :end-before: edges_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

Returns a bigint ``(pgr_bandwidth)``

=================  =========== ==========================================
Column             Type        Description
=================  =========== ==========================================
``pgr_bandwidth``   ``BIGINT`` gives the bandwidth of the graph.
=================  =========== ==========================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Undirected graph with edges before optimization.

.. graphviz::

   graph G {
    node [shape=circle, style=filled, fillcolor=white, color=black, fontcolor=black, fontsize=10];
    edge [color=black, penwidth=1];

    4 -- 7;
    7 -- 9;
    7 -- 0;
    0 -- 2;
    2 -- 5;
    5 -- 9;
    9 -- 8;
    9 -- 1;
    5 -- 1;
    9 -- 6;
    6 -- 3;
    1 -- 3;

    {rank=same; 4; 8; 6;}
    {rank=same; 7; 9; 3;}
    {rank=same; 0; 2; 5; 1;}
  }

.. literalinclude:: bandwidth.queries
   :start-after: -- q2
   :end-before: -- q5

:Example: Undirected graph with edges after optimization.

.. graphviz::

   graph G {
    node [shape=circle, style=filled, fillcolor=white, color=black, fontcolor=black, fontsize=12];
    edge [color=black, penwidth=1];

    0 -- 1;
    1 -- 3;
    1 -- 2;
    2 -- 4;
    4 -- 8;
    8 -- 3;
    3 -- 5;
    3 -- 6;
    3 -- 7;
    8 -- 7;
    6 -- 9;
    7 -- 9;

    {rank=same; 0; 5; 6;}
    {rank=same; 1; 3; 9;}
    {rank=same; 2; 4; 8; 7;}

  }

.. literalinclude:: bandwidth.queries
   :start-after: -- q5
   :end-before: -- q8

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: bandwidth
  <https://www.boost.org/libs/graph/doc/bandwidth.html>`_

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
