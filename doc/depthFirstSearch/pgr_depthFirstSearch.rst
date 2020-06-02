..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_depthFirstSearch - Experimental
===============================================================================

``pgr_depthFirstSearch`` — Returns the traversal order(s) using Depth
  First Search algorithm. In particular, the Depth First Search algorithm
  and the Undirected DFS algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/depth_first_search.html

   Boost Graph Inside

.. rubric:: Availability

Description
-------------------------------------------------------------------------------

Depth First Search algorithm is a well known traversal algorithm which starts
from a root vertex (``start_vid``) and visits all the nodes in a graph in the
depth-first search traversal order. An optional non-negative maximum depth
parameter (``max_depth``) can be specified to get the results upto a particular
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

.. code-block:: none

    pgr_depthFirstSearch(Edges SQL, Root vid [, max_depth] [, directed])
    pgr_depthFirstSearch(Edges SQL, Root vids [, max_depth] [, directed])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: depthFirstSearch(Single vertex)

Single vertex
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(Edges SQL, Root vid [, max_depth] [, directed])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: From start vertex :math:`2` on a **directed** graph

.. TODO:
.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: depthFirstSearch(Multiple vertices)

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_depthFirstSearch(Edges SQL, Root vids [, max_depth] [, directed])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: From start vertices :math:`\{11, 12\}` with :math:`depth <= 2`
          on a **directed** graph

.. TODO:
.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: --q2
   :end-before: --q3

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
**max_depth**       ``BIGINT``  :math:`9223372036854775807` Upper limit for depth of node in the tree

                                                            - When value is ``Negative`` then **throws error**

**directed**        ``BOOLEAN`` ``true``                    - When ``true`` Graph is considered `Directed`
                                                            - When ``false`` the graph is considered as `Undirected`.
=================== =========== =========================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::Edges SQL

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

**start_vid**    ``BIGINT``  Identifier of the root vertex.

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

.. TODO: CHANGE THIS
   The examples include combinations ... in a directed and undirected graph.

**Directed Graph**

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig1`

.. TODO
.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

**Undirected Graph**

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig2`

.. TODO
.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q4
   :end-before: -- q5

**Vertex Out Of Graph**

:Example: For queries in which starting vertex is not present in the graph

.. TODO:
.. literalinclude:: doc-pgr_depthFirstSearch.queries
   :start-after: --q5
   :end-before: --q6

Equivalences between signatures
...............................................................................

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig1`

.. TODO
.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q6
   :end-before: -- q7

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig2`

.. TODO
.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q7
   :end-before: -- q8


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* `Boost: Depth First Search algorithm documentation <https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/depth_first_search.html>`__
* `Boost: Undirected DFS algorithm documentation <https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/undirected_dfs.html>`__
* `Wikipedia: Depth First Search algorithm <https://en.wikipedia.org/wiki/Depth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
