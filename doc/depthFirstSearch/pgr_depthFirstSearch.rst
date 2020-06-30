..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_depthFirstSearch - Experimental
===============================================================================

``pgr_depthFirstSearch`` — Returns the traversal order(s) using Depth First
Search algorithm. In particular, the Depth First Search algorithm and the
Undirected DFS algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/depth_first_search.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

Description
-------------------------------------------------------------------------------

Depth First Search algorithm is a well known traversal algorithm which starts
from a start vertex (``start_vid``) and visits all the nodes in a graph in the
depth-first search traversal order. An optional non-negative maximum depth
parameter (``max_depth``) can be specified to get the results up to a particular
depth.

**The main Characteristics are:**

- The implementation works for both undirected and directed graphs.
- Provides the Depth First Search traversal order from a source node to
  a particular maximum depth level.
- For optimization purposes, any duplicated values in the `start_vids` are
  ignored.
- The returned values are ordered in ascending order of `start_vid`.
- If the starting vertex does not exist, empty row is returned.
- Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_depthFirstSearch(edges_sql, start_vid [, max_depth] [, directed])
    pgr_depthFirstSearch(edges_sql, start_vids [, max_depth] [, directed])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_depthFirstSearch(TEXT edges_sql, BIGINT start_vid)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From start vertex :math:`2` on a **directed** graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: depthFirstSearch(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(TEXT edges_sql, BIGINT start_vid,
    BIGINT max_depth => 9223372036854775807, BOOLEAN directed => true)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From start vertex :math:`2` on an **undirected** graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: depthFirstSearch(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids,
    BIGINT max_depth => 9223372036854775807, BOOLEAN directed => true)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From start vertices :math:`\{11, 12\}` with :math:`depth <= 2`
          on a **directed** graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

.. Parameters, Inner query & result columns

.. depthFirstSearch-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**edges_sql**       ``TEXT``               SQL query described in `Inner query`_.
**start_vid**       ``BIGINT``             Identifier of the start vertex of the tree.

                                           - Used on `Single Vertex`_.

**start_vids**      ``ARRAY[ANY-INTEGER]`` Array of identifiers of the start vertices.

                                           - Used on `Multiple Vertices`_.
                                           - For optimization purposes, any duplicated value is ignored.
=================== ====================== =================================================

Optional Parameters
...............................................................................

=================== =========== =========================== =================================================
Parameter           Type        Default                     Description
=================== =========== =========================== =================================================
**max_depth**       ``BIGINT``  :math:`9223372036854775807` Upper limit for depth of node in the tree

                                                            - When value is ``Negative`` then **throws error**

**directed**        ``BOOLEAN`` ``true``                    - When ``true`` Graph is considered `Directed`
                                                            - When ``false`` the graph is considered as `Undirected`.
=================== =========== =========================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric:: edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result columns start

Returns SET OF ``(seq, depth, start_vid, node, edge, cost, agg_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from :math:`1`.
**depth**        ``BIGINT``  Depth of the ``node``.

                             - :math:`0`  when ``node`` = ``start_vid``.

**start_vid**    ``BIGINT``  Identifier of the start vertex.

                             - In `Multiple Vertices`_ results are in ascending order.

**node**         ``BIGINT``  Identifier of ``node`` reached using ``edge``.
**edge**         ``BIGINT``  Identifier of the ``edge`` used to arrive to ``node``.

                             - :math:`-1`  when ``node`` = ``start_vid``.

**cost**         ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**     ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
===============  =========== ====================================================

.. result columns end


Additional Examples
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.

The examples include the traversal with starting vertices as 6, 8 and 15 in a
directed and undirected graph, for both single vertex and multiple vertices.

**Directed Graph**

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig1`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q5
   :end-before: -- q6

**Undirected Graph**

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig2`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q7
   :end-before: -- q8

**Vertex Out Of Graph**

:Example: For queries in which starting vertex is not present in the graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q9
   :end-before: -- q10

Equivalences between signatures
...............................................................................

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig1`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q11
   :end-before: -- q12

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig2`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q13
   :end-before: -- q14


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* `Boost: Depth First Search algorithm documentation <https://www.boost.org/
libs/graph/doc/depth_first_search.html>`__
* `Boost: Undirected DFS algorithm documentation <https://www.boost.org/
libs/graph/doc/undirected_dfs.html>`__
* `Wikipedia: Depth First Search algorithm <https://en.wikipedia.org/wiki/Depth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
