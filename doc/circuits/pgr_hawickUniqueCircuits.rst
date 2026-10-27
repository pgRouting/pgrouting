:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2022-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Miscellaneous Algorithms ; pgr_hawickUniqueCircuits
   single: Hawick Unique Circuits - Experimental on v4.1

|

``pgr_hawickUniqueCircuits`` - Experimental
===============================================================================

``pgr_hawickUniqueCircuits`` — Returns the list of unique circuits using hawick
circuits algorithm.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 4.1.0

  * New experimental function.

Description
-------------------------------------------------------------------------------

Hawick Circuit algorithm, is published in 2008 by Ken Hawick and Heath A. James.
This algorithm solves the problem of detecting and enumerating circuits in graphs.

Circuit enumeration in directed graphs, where parallel circuits are listed only
once.

.. include:: pgr_hawickCircuits.rst
   :start-after: circuits_characteristics_start
   :end-before: circuits_characteristics_end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_hawickUniqueCircuits(`Edges SQL`_ [, **options**])
   | pgr_hawickUniqueCircuits(`Edges SQL`_, **start vid** [, **options**])
   | pgr_hawickUniqueCircuits(`Edges SQL`_, **start vids** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

.. index::
    single: pgr_hawickUniqueCircuits ; All circuits

All circuits
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickUniqueCircuits(`Edges SQL`_ [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: Circuits present in the pgRouting :doc:`sampledata` in canonical
          order.

.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: pgr_hawickUniqueCircuits ; Single start vertex

Single start vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickUniqueCircuits(`Edges SQL`_, **start vid** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: All circuits that contain vertex :math:`10`

.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: pgr_hawickUniqueCircuits ; Many start vertices

Many start vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_hawickUniqueCircuits(`Edges SQL`_, **start vids** [, **options**])
   | **options:** ``[min_depth, max_depth, self_loops, u_turn_on_edge]``

   | Returns set of |circuits-result|
   | OR EMPTY SET

:Example: All circuits that contain vertices :math:`\{10, 11\}`

.. literalinclude:: hawickUniqueCircuits.queries
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

.. include:: pgr_hawickCircuits.rst
    :start-after: graph_start
    :end-before: graph_end

.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q4
   :end-before: -- q5

:Example 1: All possible unique ways to return to vertex :math:`1`.

- black is :math:`path\_id = 1`
- purple is :math:`path\_id = 2`
- red is :math:`path\_id = 3`
- green is :math:`path\_id = 4`
- blue is :math:`path\_id = 5`
- orange is :math:`path\_id = 6`

.. graphviz::

   digraph G {
       forcelabels=true;
     graph [splines=line, sep=1];
     node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
     edge [arrowsize=0.5;fontsize=8];

     1 [pos="0,0!"];
     2 [pos="0,2!"];
     3 [pos="2,1!"];
     4 [pos="-1,0!"];
     5 [pos="-1,1!"];

     1 -> 1 [headport=s, tailport=s];
     1 -> 2 [color=purple,dir=both];
     1 -> 2 -> 3 ->1 [color=red];
     1 -> 3 [color=green,dir=both];
     1 -> 3 -> 2 ->1 [color=blue];
     1 -> 4 [color=orange,dir=both]
   }

.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q5
   :end-before: -- q6

:Example 2: Exclude self loops.

- purple is :math:`path\_id = 1`
- red is :math:`path\_id = 2`
- green is :math:`path\_id = 3`
- blue is :math:`path\_id = 4`
- orange is :math:`path\_id = 5`

.. graphviz::

   digraph G {
       forcelabels=true;
     graph [splines=line, sep=1];
     node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
     edge [arrowsize=0.5;fontsize=8];

     1 [pos="0,0!"];
     2 [pos="0,2!"];
     3 [pos="2,1!"];
     4 [pos="-1,0!"];
     5 [pos="-1,1!"];

     1 -> 2 [color=purple,dir=both];
     1 -> 2 -> 3 ->1 [color=red];
     1 -> 3 [color=green,dir=both];
     1 -> 3 -> 2 ->1[color=blue];
     1 -> 4 [color=orange,dir=both]
   }

.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q6
   :end-before: -- q7

:Example 3: Exclude self loops and U-turn on the same edge.

- red is :math:`path\_id = 1`
- green is :math:`path\_id = 2`
- blue is :math:`path\_id = 3`

.. graphviz::

   digraph G {
       forcelabels=true;
     graph [splines=line, sep=1];
     node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
     edge [arrowsize=0.5;fontsize=8];

     1 [pos="0,0!"];
     2 [pos="0,2!"];
     3 [pos="2,1!"];
     4 [pos="-1,0!"];
     5 [pos="-1,1!"];


     1 -> 2 -> 3 -> 1 [color=red];
     1 -> 3 [color=green,penwidth=2];
     3 -> 1 [color=green];
     1 -> 3 [color=blue,penwidth=2];
     3 -> 2 -> 1[color=blue];
   }


.. literalinclude:: hawickUniqueCircuits.queries
   :start-after: -- q7
   :end-before: -- q8

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Hawick Circuit Algorithm
  <https://www.boost.org/doc/libs/1_78_0/libs/graph/doc/hawick_circuits.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
