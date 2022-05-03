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
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_breadthFirstSearch.html>`__)
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
* Provides the Breadth First Search traversal order from a source node to a target
  depth level.
* Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. parsed-literal:: 

    pgr_breadthFirstSearch(`Edges SQL`_, **Root vid** [, max_depth] [, directed])
    pgr_breadthFirstSearch(`Edges SQL`_, **Root vids** [, max_depth] [, directed])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: breadthFirstSearch(Single vertex) - Experimental on v3.0

Single Vertex
...............................................................................

.. parsed-literal::

    pgr_breadthFirstSearch(`Edges SQL`_, **Root vid** [, max_depth] [, directed])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Breadth First Search traversal with root vertex :math:`2`

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: breadthFirstSearch(Multiple vertices) - Experimental on v3.0

Multiple Vertices
...............................................................................

.. parsed-literal:: 

    pgr_breadthFirstSearch(`Edges SQL`_, **Root vids** [, max_depth] [, directed])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Breadth First Search traverls starting on vertices :math:`\{11, 12\}`
          with :math:`depth <= 2`

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: --q2
   :end-before: --q3


.. Parameters, Inner query & result columns

.. breadthFirstSearch-information-start

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

Edges SQL
...............................................................................

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

                             - In *Multiple Vertices* results are in ascending order.

**node**         ``BIGINT``  Identifier of ``node`` reached using ``edge``.
**edge**         ``BIGINT``  Identifier of the ``edge`` used to arrive to ``node``.

                             - :math:`-1`  when ``node`` = ``start_vid``.

**cost**         ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**     ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
===============  =========== ====================================================

.. result columns end


Additional Examples
...............................................................................

**Undirected Graph**

:Example: The Breadth First Search traverls starting on vertices :math:`\{11, 12\}`
          with :math:`depth <= 2` as well as considering the graph to be undirected.

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: --q3
   :end-before: --q4


**Vertex Out Of Graph**

:Example: The output of the function when a vertex not present in the graph 
          is passed as a parameter.

.. literalinclude:: doc-pgr_breadthFirstSearch.queries
   :start-after: --q4
   :end-before: --q5



See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Breadth First Search algorithm documentation <https://www.boost.org/libs/graph/doc/breadth_first_search.html>`__
* `Wikipedia: Breadth First Search algorithm <https://en.wikipedia.org/wiki/Breadth-first_search>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
