..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_breadthFirstSearch.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_breadthFirstSearch.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_breadthFirstSearch.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_breadthFirstSearch.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_breadthFirstSearch.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_breadthFirstSearch.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_breadthFirstSearch.html>`__

``pgr_breadthFirstSearch`` - Experimental
===============================================================================

``pgr_breadthFirstSearch`` — Returns the traversal order(s) using Breadth First
Search algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/breadth_first_search.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** signature:

    * ``pgr_breadthFirstSearch`` (`Single Vertex`_)
    * ``pgr_breadthFirstSearch`` (`Multiple Vertices`_)

Description
-------------------------------------------------------------------------------

Provides the Breadth First Search traversal order from a root vertex to a
particular depth.

**The main Characteristics are:**

* The implementation will work on any type of graph.
* Provides the Breadth First Search traversal order from a source node to a
  target depth level.
* Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_breadthFirstSearch(`Edges SQL`_, **root vid**, [**options**])
   | pgr_breadthFirstSearch(`Edges SQL`_, **root vids**, [**options**])
   | **options:** ``[max_depth, directed]``

   | RETURNS SET OF |result-bfs|

.. index::
    single: breadthFirstSearch(Single vertex) - Experimental on v3.0

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_breadthFirstSearch(`Edges SQL`_, **root vid**, [**options**])
   | **options:** ``[max_depth, directed]``

   | RETURNS SET OF |result-bfs|

:Example: From root vertex :math:`6` on a **directed** graph with edges in
          ascending order of ``id``

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: breadthFirstSearch(Multiple vertices) - Experimental on v3.0

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_breadthFirstSearch(`Edges SQL`_, **root vids**, [**options**])
   | **options:** ``[max_depth, directed]``

   | RETURNS SET OF |result-bfs|

:Example: From root vertices :math:`\{12, 6\}` on an **undirected** graph with
          **depth** :math:`<= 2` and edges in ascending order of ``id``

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-params_start
   :end-before: mst-bfs-dfs-params_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

DFS optional parameters
...............................................................................

.. include:: BFS-category.rst
   :start-after: max-depth-optional-start
   :end-before: max-depth-optional-end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Return columns
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-dd-result-columns-start
   :end-before: mst-bfs-dfs-dd-result-columns-end

Additional Examples
-------------------------------------------------------------------------------

:Example: Same as `Single vertex`_ with edges in ascending order of ``id``.

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

:Example: Same as `Single vertex`_ with edges in descending order of ``id``.

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

The resulting traversal is different.

The left image shows the result with ascending order of ids and the right image
shows with descending order of the edge identifiers.

|ascending| |descending|

.. |ascending| image:: images/bfs-ascending.png
   :scale: 25%

.. |descending| image:: images/bfs-descending.png
   :scale: 25%

See Also
-------------------------------------------------------------------------------

* :doc:`BFS-category`
* :doc:`sampledata`
* `Boost: Breadth First Search algorithm documentation
  <https://www.boost.org/libs/graph/doc/breadth_first_search.html>`__
* `Wikipedia: Breadth First Search algorithm
  <https://en.wikipedia.org/wiki/Breadth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
