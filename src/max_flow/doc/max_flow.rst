..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _maxflow:

pgr_maxFlow - Maximum Flow Problems
===================================

  - :ref:`pgr_pushRelabel` - Push&relabel algorithm implementation for maximum flow (default).


.. toctree::
        :hidden:

        ./pgr_pushRelabel

Problem definition
==================

A flow network is a directed graph where each edge has a capacity and a flow.
The flow through an edge must not exceed the capacity of the edge.
Additionally, the incoming and outgoing flow of a node must be equal except the for source which only has outgoing flow, and the destination(sink) which only has incoming flow.

Maximum flow algorithms calculate the maximum flow through the graph and the flow of each edge.

Given the following query:

pgr_maxFlow(:math:`edges\_sql, source, sink`)

where :math:`edges\_sql = \{(id_i, tail_i, head_i, capacity_i, reverse\_capacity_i)\}`

:math:`tail_i` and :math:`head_i` identify the source and destination of the i-th edge.

.. rubric:: Graph definition

The weighted directed graph, :math:`G(V,E)`, is defined as:

* the set of vertices  :math:`V`

  - :math:`source  \cup  sink  \bigcup  tail_i  \bigcup  head_i`

* the set of edges :math:`E`

  - :math:`E = \begin{cases} &\{(tail_i, head_i, capacity_i) \text{ when } capacity > 0 \} &\quad  \text{ if } reverse\_capacity = \varnothing \\ \\ &\{(tail_i, head_i, capacity_i) \text{ when } capacity > 0 \} \\ \cup &\{(head_i, tail_i, reverse\_capacity_i) \text{ when } reverse\_capacity_i > 0)\} &\quad \text{ if } reverse\_capacity \neq \varnothing \\ \end{cases}`


.. rubric:: Maximum flow problem

Given:

  - :math:`source \in V` the source vertex
  - :math:`sink \in V` the sink vertex
  - :math:`G(V,E)`

Then:

  :math:`\text{pgr\_maxFlow}(edges\_sql, source, sink, directed) = \boldsymbol{\Phi}`

  :math:`\boldsymbol{\Phi} = \{(id_i, tail_i, head_i, residual\_capacity_i, reverse\_residual\_capacity_i, flow_i, reverse\_flow_i)\}`

where:
  - :math:`id_i = i`
  - :math:`residual\_capacity_i = capacity_i - flow_i`
  - :math:`reverse\_residual\_capacity_i = reverse\_capacity_i - reverse\_flow_i`


:math:`\boldsymbol{\Phi}` maps the i-th edge(and reverse) to their residual capacity and flow. The maximum flow through the graph can be obtained by aggregating on the source or sink and summing the flow from/to it.