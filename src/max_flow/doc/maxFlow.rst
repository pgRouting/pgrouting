..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _maxFlow:

maxFlow - Maximum Flow Problems
===================================

  - :ref:`pgr_maxFlowPushRelabel` - Push&relabel algorithm implementation for maximum flow.
  - :ref:`pgr_maxFlowEdmondsKarp` - Edmonds&Karp algorithm implementation for maximum flow.
  - :ref:`pgr_maxFlowBoykovKolmogorov` - Boykov&Kolmogorov algorithm implementation for maximum flow.

The maximum flow through the graph is guaranteed to be the same with all implementations,
but the actual flow through each edge may vary.

.. toctree::
        :hidden:

        ./pgr_maxFlowPushRelabel
        ./pgr_maxFlowEdmondsKarp
        ./pgr_maxFlowBoykovKolmogorov


Problem definition
==================

A flow network is a directed graph where each edge has a capacity and a flow.
The flow through an edge must not exceed the capacity of the edge.
Additionally, the incoming and outgoing flow of a node must be equal except the for source which only has outgoing flow, and the destination(sink) which only has incoming flow.

Maximum flow algorithms calculate the maximum flow through the graph and the flow of each edge.

Given the following query:

pgr_maxFlow :math:`(edges\_sql, source\_vertex, sink\_vertex)`

where :math:`edges\_sql = \{(id_i, source_i, target_i, capacity_i, reverse\_capacity_i)\}`

.. rubric:: Graph definition

The weighted directed graph, :math:`G(V,E)`, is defined as:

* the set of vertices  :math:`V`

  - :math:`source\_vertex  \cup  sink\_vertex  \bigcup  source_i  \bigcup  target_i`

* the set of edges :math:`E`

  - :math:`E = \begin{cases} &\{(source_i, target_i, capacity_i) \text{ when } capacity > 0 \} &\quad  \text{ if } reverse\_capacity = \varnothing \\ \\ &\{(source_i, target_i, capacity_i) \text{ when } capacity > 0 \} \\ \cup &\{(target_i, source_i, reverse\_capacity_i) \text{ when } reverse\_capacity_i > 0)\} &\quad \text{ if } reverse\_capacity \neq \varnothing \\ \end{cases}`


.. rubric:: Maximum flow problem

Given:


  - :math:`source\_vertex \in V` the source vertex
  - :math:`sink\_vertex \in V` the sink vertex
  - :math:`G(V,E)`

Then:

     :math:`pgr\_maxFlow(edges\_sql, source, sink) = \boldsymbol{\Phi}`

     :math:`\boldsymbol{\Phi} = {(id_i, source_i, target_i, flow_i, residual\_capacity_i)}`

where:

  :math:`\boldsymbol{\Phi}` is a new subset of edges with their residual capacity and flow. The maximum flow through the graph can be obtained by aggregating on the source or sink and summing the flow from/to it. In particular:

  .. math::
        id_i = i
        residual\_capacity_i = capacity_i - flow_i


See Also
--------

* https://en.wikipedia.org/wiki/Maximum_flow_problem
