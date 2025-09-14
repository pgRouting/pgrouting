..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. index::
   single: Dijkstra Family ; pgr_dijkstraCost
   single: Cost Category ; pgr_dijkstraCost
   single: dijkstraCost

|

``pgr_dijkstraCost``
===============================================================================

``pgr_dijkstraCost`` - Total cost of the shortest path using Dijkstra
algorithm.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Combinations signature promoted to official.

.. rubric:: Version 3.1.0

* New proposed signature:

  * pgr_dijkstraCost(Combinations)

.. rubric:: Version 2.2.0

* Official function.


Description
-------------------------------------------------------------------------------

The ``pgr_dijkstraCost`` function summarizes of the cost of the shortest path
using Dijkstra Algorithm.

.. include:: dijkstra-family.rst
    :start-after: dijkstra_description_start
    :end-before: dijkstra_description_end

.. include:: dijkstra-family.rst
    :start-after: dijkstra_details_start
    :end-before: dijkstra_details_end

.. include:: cost-category.rst
    :start-after: cost_traits_start
    :end-before: cost_traits_end

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraCost(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_dijkstraCost(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_dijkstraCost(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_dijkstraCost(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_dijkstraCost(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

.. index::
    single: dijkstraCost ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCost(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on a **directed** graph

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q2
    :end-before: -- q3

.. index::
    single: dijkstraCost ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCost(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**
          graph

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q3
    :end-before: -- q4

.. index::
    single: dijkstraCost ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCost(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q4
    :end-before: -- q5

.. index::
    single: dijkstraCost ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCost(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q5
    :end-before: -- q51

.. index::
    single: dijkstraCost ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_dijkstraCost(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q51
    :end-before: -- q52

The query:

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q52
    :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start_vids** the same as **end_vids**

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q8
    :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`sampledata`
* `Boost: Dijkstra shortest paths
  <https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html>`__
* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
