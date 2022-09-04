..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/cuthillMckeeOrdering.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/cuthillMckeeOrdering.html>`__)

cuthillMckeeOrdering - Experimental
===============================================================================

``cuthillMckeeOrdering`` — Returns the reverse Cuthill-Mckee ordering of an undirected
graphs

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/cuthill_mckee_ordering.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

* Version 3.4.0

  * New **experimental** signature:


Description
-------------------------------------------------------------------------------

In numerical linear algebra, the Cuthill-McKee algorithm (CM), named after
Elizabeth Cuthill and James McKee, is an algorithm to permute a sparse
matrix that has a symmetric sparsity pattern into a band matrix form with a
small bandwidth. The reverse Cuthill-McKee algorithm (RCM) due to Alan George
and Joseph Liu is the same algorithm but with the resulting index numbers reversed.
In practice this generally results in less fill-in than the CM ordering when
Gaussian elimination is applied.

The vertices are basically assigned a breadth-first search order, except that at
each step, the adjacent vertices are placed in the queue in order of increasing degree.

There are 3 versions defined in the Boost Graph Library. Here, we have implemented only
version 2 as it is most suitable and practical for users. In this version we don't have to 
provide the starting vertex in the query. Whereas, in the version 1 we have to provide the 
starting vertex and it gives result only for single component. Version 2 is applicable for one 
and more than one components as it finds the starting vertex itself using the pseudo-peripheral
pair heuiristic approach among each component. Maybe in future, we will also implemenent the 
version 1.

**The main Characteristics are:**

- The implementation is for **undirected** graphs.
- The bandwidth minimization problems are considered NP-complete problems. 
- The running time complexity is: :math:`O(m log(m)|V|)`

  - where :math:`|V|` is the number of vertices,
  - :math:`m` is the maximum degree of the vertices in the graph.

Signatures
------------------------------------------------------------------------------

.. index::
    single: cuthillMckeeOrdering - Experimental on v3.4

.. parsed-literal::

    cuthillMckeeOrdering(`Edges SQL`_)

    RETURNS SET OF (seq, node)
    OR EMPTY SET

:Example: Graph ordering of pgRouting :doc:`sampledata`

.. literalinclude:: cuthillMckeeOrdering.queries
   :start-after: -- q1
   :end-before: -- q2

.. Parameters, Inner Queries & result columns

Parameters
-------------------------------------------------------------------------------

.. include:: ordering-family.rst
   :start-after: parameters_start
   :end-before: parameters_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end 

Return columns
-------------------------------------------------------------------------------

.. include:: ordering-family.rst
    :start-after: result_columns_start
    :end-before: result_columns_end

See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.

.. see also start

* `Boost: Cuthill-McKee Ordering
  <https://www.boost.org/libs/graph/doc/cuthill_mckee_ordering.html>`__
* `Wikipedia: Cuthill-McKee Ordering <https://en.wikipedia.org/wiki/Cuthill%E2%80%93McKee_algorithm>`__

.. see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`     
  
      