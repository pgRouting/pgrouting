..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Minimum Cost Maximum FLow - Family of functions (Experimental)
==================================================================

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_minCostMaxFlow_Cost` - Only the Min Cost calculation.
* :doc:`pgr_minCostMaxFlow` - Details of flow and cost on edges.

.. index to here


.. toctree::
    :hidden:

    pgr_minCostMaxFlow_Cost
    pgr_minCostMaxFlow

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.0 <https://docs.pgrouting.org/dev/en/costFlow-family.html>`__)

Minimum Cost Maximum Flow Functions General Information
-------------------------------------------------------------------------------

.. characteristics_start

**The main characteristics are:**

- The graph is **directed**.
- Process is done only on edges with positive capacities.
- **The cost value of all input edges must be nonnegative.**
- When the maximum flow is 0 then there is no flow and **EMPTY SET** is returned.

  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Calculates the flow/residual capacity and cost for each edge and agg_cost for the graph. In the output

  - Edges with zero flow are omitted.

- Creates a **super source** and edges to all the source(s), and a **super target** and the edges from all the targets(s).
- The minimum cost through the graph is guaranteed to be the value returned by :doc:`pgr_minCostMaxFlow_Cost <pgr_minCostMaxFlow_Cost>` when executed with the same parameters and can be calculated:

  - By aggregation of the outgoing flow cost from the sources
  - By aggregation of the incoming flow cost to the targets

.. characteristics_end


:doc:`pgr_minCostMaxFlow <pgr_minCostMaxFlow>`  is the minimum cost and that minimum is guaranteed to be the same on the functions :doc:`pgr_minCostMaxFlow <pgr_minCostMaxFlow>`, but the actual flow through each edge may vary.



Problem definition
------------------------

A flow network with cost is a directed graph where each edge has a capacity, a flow and a cost.
For each edge :math:`(u, v)` in the given graph, it has capacity :math:`c(u, v) > 0`, flow :math:`f(u, v) >= 0` and cost :math:`a(u, v) >= 0`.


:math:`f(u, v) <= c(u, v)`


The cost of sending this flow along an edge :math:`(u, v)` is :math:`f(u, v) * a(u, v)`.
Additionally, the incoming and outgoing flow of a node must be equal except the for source which only has outgoing flow, and the destination(sink) which only has incoming flow.

Minimum Cost Maximum flow algorithms calculate the minimum cost maximum flow through the graph and the flow/cost of each edge.

Given the following query:

pgr_minCostMaxFlow :math:`(edges\_sql, source\_vertex, sink\_vertex)`

where :math:`edges\_sql = \{(id_i, source_i, target_i, capacity_i, reverse\_capacity_i, cost_i, reverse\_cost_i)\}`

.. rubric:: Graph definition

The weighted directed graph, :math:`G(V,E)`, is defined as:

* the set of vertices  :math:`V`

  - :math:`source\_vertex  \cup  sink\_vertex  \bigcup  source_i  \bigcup  target_i`

* the set of edges :math:`E`

  - :math:`E = \begin{cases}
    \text{ } \{(source_i, target_i, capacity_i, cost_i) \text{ when } capacity > 0 \} & \quad  \text{ if } reverse\_capacity = \varnothing \\
    \text{ } & \quad \text{ } \\
    \{(source_i, target_i, capacity_i, cost_i) \text{ when } capacity > 0 \} & \text{ } \\
    \cup \{(target_i, source_i, reverse\_capacity_i, reverse\_cost_i) \text{ when } reverse\_capacity_i > 0)\} & \quad \text{ if } reverse\_capacity \neq \varnothing \\ \end{cases}`


.. rubric:: Minimum Cost Maximum Flow problem

Given:


- :math:`G(V,E)`
- :math:`source\_vertex \in V` the source vertex
- :math:`sink\_vertex \in V` the sink vertex

Then:

     :math:`pgr\_minCostMaxFlow(edges\_sql, source, sink) = \boldsymbol{\Phi}`

     :math:`\boldsymbol{\Phi} = {(id_i, edge\_id_i, source_i, target_i, flow_i, residual\_capacity_i, cost_i, agg\_cost_i)}`

Where:

:math:`\boldsymbol{\Phi}` is a subset of the original edges with their residual capacity, flow, cost and aggregate cost. The minimum cost through the graph can be obtained by aggregating on each edge and summing the sending cost of it. In particular:

- :math:`id_i = i`
- :math:`edge\_id = id_i` in edges_sql
- :math:`residual\_capacity_i = capacity_i - flow_i`
- :math:`agg\_cost_i = agg\_cost_{i-1} + cost_i`


See Also
--------
* https://www.boost.org/libs/graph/doc/successive_shortest_path_nonnegative_weights.html


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
