..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _maxFlow:

Flow - Family of functions
===================================

.. index from here

* :ref:`pgr_maxFlow` - Only the Max flow calculation using Push and Relabel algorithm.
* :ref:`pgr_BoykovKolmogorov` - Boykov and Kolmogorov with details of flow on edges.
* :ref:`pgr_EdmondsKarp` - Edmonds and Karp algorithm with details of flow on edges.
* :ref:`pgr_PushRelabel` - Push and relabel algorithm with details of flow on edges.
* Applications

  * :ref:`pgr_edgeDisjointPaths` - Calculates edge disjoint paths between two groups of vertices.
  * :ref:`pgr_maxCardinalityMatch` - Calculates a maximum cardinality matching in a graph.

.. index to here


.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. toctree::
    :hidden:

    pgr_maxFlow
    pgr_pushRelabel
    pgr_edmondsKarp
    pgr_boykovKolmogorov
    pgr_maxCardinalityMatch
    pgr_edgeDisjointPaths


Flow Functions General Information
-----------------------------------

.. characteristics_start

.. rubric:: Characteristics


- The graph is **directed**.
- Process is done only on edges with positive capacities.
- When the maximum flow is 0 then there is no flow and **EMPTY SET** is returned.

  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Calculates the flow/residual capacity for each edge. In the output

  - Edges with zero flow are omitted.

- Creates a **super source** and edges to all the source(s), and a **super target** and the edges from all the targets(s).
- The maximum flow through the graph is guaranteed to be the value returned by :ref:`pgr_maxFlow <pgr_maxFlow>` when executed with the same parameters and can be calculated:

  - By aggregation of the outgoing flow from the sources
  - By aggregation of the incoming flow to the targets

.. characteristics_end


:ref:`pgr_maxFlow <pgr_maxFlow>`  is the  maximum Flow and that maximum is guaranteed to be the same on the functions :ref:`pgr_pushRelabel <pgr_pushRelabel>`, :ref:`pgr_edmondsKarp <pgr_edmondsKarp>`, :ref:`pgr_boykovKolmogorov <pgr_boykovKolmogorov>`, but the actual flow through each edge may vary.



Problem definition
------------------------

A flow network is a directed graph where each edge has a capacity and a flow.
The flow through an edge must not exceed the capacity of the edge.
Additionally, the incoming and outgoing flow of a node must be equal except the for source which only has outgoing flow, and the destination(sink) which only has incoming flow.

Maximum flow algorithms calculate the maximum flow through the graph and the flow of each edge.

The maximum flow through the graph is guaranteed to be the same with all implementations,
but the actual flow through each edge may vary.
Given the following query:

pgr_maxFlow :math:`(edges\_sql, source\_vertex, sink\_vertex)`

where :math:`edges\_sql = \{(id_i, source_i, target_i, capacity_i, reverse\_capacity_i)\}`

.. rubric:: Graph definition

The weighted directed graph, :math:`G(V,E)`, is defined as:

* the set of vertices  :math:`V`

  - :math:`source\_vertex  \cup  sink\_vertex  \bigcup  source_i  \bigcup  target_i`

* the set of edges :math:`E`

  - :math:`E = \begin{cases}
    \text{ } \{(source_i, target_i, capacity_i) \text{ when } capacity > 0 \} & \quad  \text{ if } reverse\_capacity = \varnothing \\
    \text{ } & \quad \text{ } \\
    \{(source_i, target_i, capacity_i) \text{ when } capacity > 0 \} & \text{ } \\
    \cup \{(target_i, source_i, reverse\_capacity_i) \text{ when } reverse\_capacity_i > 0)\} & \quad \text{ if } reverse\_capacity \neq \varnothing \\ \end{cases}`


.. rubric:: Maximum flow problem

Given:


- :math:`G(V,E)`
- :math:`source\_vertex \in V` the source vertex
- :math:`sink\_vertex \in V` the sink vertex

Then:

     :math:`pgr\_maxFlow(edges\_sql, source, sink) = \boldsymbol{\Phi}`

     :math:`\boldsymbol{\Phi} = {(id_i, edge\_id_i, source_i, target_i, flow_i, residual\_capacity_i)}`

Where:

:math:`\boldsymbol{\Phi}` is a subset of the original edges with their residual capacity and flow. The maximum flow through the graph can be obtained by aggregating on the source or sink and summing the flow from/to it. In particular:

- :math:`id_i = i`
- :math:`edge\_id = id_i` in edges_sql
- :math:`residual\_capacity_i = capacity_i - flow_i`


See Also
--------

* https://en.wikipedia.org/wiki/Maximum_flow_problem

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
