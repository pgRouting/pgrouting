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
from a root vertex (``Root vid``) and visits all the nodes in a graph in the
depth-first search traversal order. An optional non-negative maximum depth
parameter (``max_depth``) can be specified to get the results up to a particular
depth.

**Note**: The cost of an edge is not used for traversal. If multiple edges
are adjacent to a vertex, then this algorithm traverses that edge which
comes first in the ``Edges SQL``.

**The main Characteristics are:**

- The implementation works for both directed and undirected graphs.
- Provides the Depth First Search traversal order from a root vertex to
  a particular maximum depth level.
- For optimization purposes, any duplicated values in the `Root vids` are
  ignored.
- The returned values are ordered in ascending order of `start_vid`.
- If the root vertex does not exist, an empty row is returned.
- It does not produce the shortest path from a root vertex to a target vertex.
- The aggregate cost of traversal is not guaranteed to be minimal.
- Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_depthFirstSearch(Edges SQL, Root vid [, directed] [, max_depth])
    pgr_depthFirstSearch(Edges SQL, Root vids [, directed] [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_depthFirstSearch(TEXT, BIGINT)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From root vertex :math:`2` on a **directed** graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

:Example: From root vertex :math:`2` on a **directed** graph,
          with :math:`depth <= 2`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: depthFirstSearch(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(TEXT, BIGINT,
    BOOLEAN directed => true, BIGINT max_depth => 9223372036854775807)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From root vertex :math:`2` on an **undirected** graph

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: depthFirstSearch(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(TEXT, ARRAY[ANY_INTEGER],
    BOOLEAN directed => true, BIGINT max_depth => 9223372036854775807)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From root vertices :math:`\{11, 12\}` on a **directed** graph
          with :math:`depth <= 2`

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q4
   :end-before: -- q5

.. Parameters, Inner query & result columns

.. depthFirstSearch-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               SQL query described in `Inner query`_.
**Root vid**        ``BIGINT``             Identifier of the root vertex of the tree.

                                           - Used on `Single Vertex`_.

**Root vids**       ``ARRAY[ANY-INTEGER]`` Array of identifiers of the root vertices.

                                           - Used on `Multiple Vertices`_.
                                           - For optimization purposes, any duplicated value is ignored.
=================== ====================== =================================================

Optional Parameters
...............................................................................

=================== =========== =========================== =================================================
Parameter           Type        Default                     Description
=================== =========== =========================== =================================================
**directed**        ``BOOLEAN`` ``true``                    - When ``true`` Graph is considered `Directed`
                                                            - When ``false`` the graph is considered as `Undirected`.

**max_depth**       ``BIGINT``  :math:`9223372036854775807` Upper limit for depth of node in the tree

                                                            - When value is ``Negative`` then **throws error**
=================== =========== =========================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric:: Edges SQL

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

**start_vid**     ``BIGINT``  Identifier of the start vertex.

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

This example shows that the cost of an edge is not used for traversal. An edge
having a greater cost may be traversed first, if it comes first in the ``Edges SQL``.

.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: -- q5
   :end-before: -- q6

Here, the edge from :math:`2` to :math:`5` has a cost of :math:`1.016` which is
greater than the cost of edge from :math:`2` to :math:`3`, which is :math:`1.004`.
Yet, the edge with greater cost is traversed first, because it comes first in the
``Edges SQL``.


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* `Boost: Depth First Search algorithm documentation <https://www.boost.org/libs/graph/doc/depth_first_search.html>`__
* `Boost: Undirected DFS algorithm documentation <https://www.boost.org/libs/graph/doc/undirected_dfs.html>`__
* `Wikipedia: Depth First Search algorithm <https://en.wikipedia.org/wiki/Depth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
