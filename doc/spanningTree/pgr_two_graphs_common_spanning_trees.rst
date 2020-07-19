..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************
.. index::
    single: pgr_two_graphs_common_spanning_trees -Experimental (Complete Doc)
.. _pgr_two_graphs_common_spanning_trees:

pgr_two_graphs_common_spanning_trees -Experimental
===============================================================================

``pgr_two_graphs_common_spanning_trees`` — Set of edges of all common spanning trees.
In particular, the two_graphs_common_spanning_trees algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_54_0/libs/graph/doc/two_graphs_common_spanning_trees.html
   
   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr
   
.. rubric:: Availability


Description
-------------------------------------------------------------------------------
A spanning tree is a subset of Graph G, which has all the vertices covered with the minimum possible number of edges. Hence, a spanning tree does not have cycles and it cannot be disconnected.
Now, imagine there are two graphs that are represented as lists of edges. A common spanning tree is a set of indices that identifies a spanning tree for both the first and for the second of the two graphs. Both graphs can have multiple common spanning trees.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: sql

    pgr_two_graphs_common_spanning_trees(edges_sql_1,edges_sql_2,directed)
    RETURNS SET OF (seq,tree_edges[])


:Example: **TBD**

.. index::
    single: two_graphs_common_spanning_trees

Complete Signature
...............................................................................

.. code-block:: sql


:Example: 

Parameters
-------------------------------------------------------------------------------

============== ============ =================================================
Column         Type           Description
============== ============ =================================================
**edges_sql**   ``TEXT``    SQL query as described above.
**root_vid**   ``BIGINT``  Identifier of the starting vertex.
============== ============ =================================================


Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, vid,idom)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
**seq**        ``INTEGER`` Sequential value starting from **1**.
**vid**        ``BIGINT``  Identifier of vertex .
**idom**       ``BIGINT``  Immediate dominator of vertex.
============== =========== =================================================

Additional Examples
------------------------------------------------------------------------------------------

:Example: To handle the one flag to choose signatures

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-LTDTree.queries
    :start-after: --q1
    :end-before: --q2

See Also
-------------------------------------------------------------------------------

* https://www.boost.org/doc/libs/1_50_0/libs/graph/doc/lengauer_tarjan_dominator.htm
* https://www.cs.princeton.edu/courses/archive/fall03/cs528/handouts/a%20fast%20algorithm%20for%20finding.pdf
* https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/two_graphs_common_spanning_trees.html
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
