..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_ithBandwidth - Experimental
===============================================================================

``pgr_ithBandwidth`` — Returns the ith-bandwidth of each vertex of an undirected graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/bandwidth.html#sec:ith-bandwidth

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

* Version 3.6.0

  * New **experimental** signature


Description
-------------------------------------------------------------------------------

ith-bandwidth of a graph is the quantity that is defined for every vertex in the 
graph unlike bandwidth that is defined only for the graph itself. The ith-bandwidth 
of any vertex in the graph is the maximum distance between that vertex and any of 
its adjacent vertex such that all the vertices are placed on a line separated with 
unit intervals.

**The main Characteristics are:**

- ith-Bandwidth of the graph remains same irrespective of whether it is **directed**, **undirected**, **weighted** or **unweighted**.
- Provides the ith-Bandwidth of all the vertices present in the graph.
- ithBandwidth values are in the Range :math:`[1, |V|]`
- The returned rows are ordered in ascending order of the vertex value.

  - where :math:`|V|` is the number of vertices,
  - :math:`d` is the maximum degree of the vertices in the graph,
  - :math:`k` is the number of colors used.

Signatures
------------------------------------------------------------------------------

.. index::
    single: ithBandwidth - Experimental on v3.6

.. parsed-literal::

    pgr_ithBandwidth(`Edges SQL`_)

    RETURNS SET OF (vertex_id, ith_bandwidth)
    OR EMPTY SET

:Example: Graph bandwidth of pgRouting :doc:`sampledata`

.. literalinclude:: doc-pgr_ithBandwidth.queries
   :start-after: -- q1
   :end-before: -- q2

.. Parameters, Inner Queries & result columns

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: bandwidth-family.rst
    :start-after: result columns start
    :end-before: result columns end


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.

.. see also start

* `Boost: ith-Bandwidth documentation
  <https://www.boost.org/libs/graph/doc/bandwidth.html#sec:ith-bandwidth>`__
* `Wikipedia: Graph bandwidth <https://en.wikipedia.org/wiki/Graph_bandwidth>`__

.. see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
