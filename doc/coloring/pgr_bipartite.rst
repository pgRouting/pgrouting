..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bipartite -Experimental
===============================================================================

``pgr_bipartite`` — If graph is bipartite then function returns the vertex id along with color (0 and 1) else it will return an empty set.
In particular, the is_bipartite() algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_53_0/libs/graph/doc/is_bipartite.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_bipartite.html>`__)



Description
-------------------------------------------------------------------------------
A bipartite graph is a graph with two sets of vertices which are connected to each other, but not within themselves.
A bipartite graph is possible if the graph coloring is possible using two colors such that vertices in a set are colored with the same color.

**The main Characteristics are:**

- The algorithm works in undirected graph only.
- The returned values are not ordered.
- The algorithm checks graph is bipartite or not. If it is bipartite then it returns the node along with two colors `0` and `1` which represents two different sets.
- If graph is not bipartite then algorithm returns empty set.
- Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------


.. code-block:: sql

    pgr_bipartite(Edges SQL) -- Experimental on v3.2
    
    RETURNS SET OF (vertex_id, color_id)
    OR EMPTY SET



:Example: The pgr_bipartite algorithm with and edge_sql as a parameter when graph is bipartite:

.. literalinclude:: doc-bipartite.queries
   :start-after: --q1
   :end-before: --q2


.. index::
    single: bipartite (Single Vertex) - Experimental on v3.2


.. Parameters, Inner query & result columns

Parameters
-------------------------------------------------------------------------------

.. include:: coloring-family.rst
    :start-after: parameters start
    :end-before: parameters end

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query of an **undirected** graph, which should return
            a set of rows with the following columns:

.. include:: traversal-family.rst
   :start-after: edges_sql_start
   :end-before: edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: coloring-family.rst
    :start-after: result columns start
    :end-before: result columns end


Additional Examples
------------------------------------------------------------------------------------------

:Example: The odd length cyclic graph can not be bipartite.
Adding the edge (1,7) will make odd length cyclic graph with 5 veritices, as follows:

.. literalinclude:: doc-bipartite.queries
   :start-after: --q2
   :end-before: --q3
   
The new graph is not bipartite because it has a odd length cycle of 5 vertices. Edges in blue represent odd length cycle.

.. image:: images/bipartite.png
   :scale: 60%

.. literalinclude:: doc-bipartite.queries
    :start-after: --q3
    :end-before: --q4






See Also
-------------------------------------------------------------------------------

.. see also start

* `Boost: is_bipartite algorithm documentation <https://www.boost.org/doc/libs/1_50_0/libs/graph/doc/is_bipartite.html>`__
* `Wikipedia: bipartite graph <https://en.wikipedia.org/wiki/Bipartite_graph>`__

.. see also end

* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
