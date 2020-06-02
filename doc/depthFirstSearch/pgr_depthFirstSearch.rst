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




See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* `Boost: Depth First Search algorithm documentation <https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/depth_first_search.html>`__
* `Boost: Undirected DFS algorithm documentation <https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/undirected_dfs.html>`__
* `Wikipedia: Depth First Search algorithm <https://en.wikipedia.org/wiki/Depth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
