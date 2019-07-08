..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Chinese Postman Problem - Family of functions (Experimental)
=============================================================

.. index from here

* :doc:`pgr_directedChPP`
* :doc:`pgr_directedChPP_Cost`

.. index to here

.. toctree::
  :hidden:

  pgr_directedChPP
  pgr_directedChPP_Cost



Given the following query:



pgr_directedChPP(:math:`sql`)

where  :math:`sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

and

- :math:`source = \bigcup source_i`,
- :math:`target = \bigcup target_i`,

The graphs are defined as follows:

.. rubric:: Directed graph

The weighted directed graph, :math:`G(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target \cup {start_{vid}} \cup  {end_{vid}}`

* the set of edges :math:`E`

  - :math:`E = \begin{cases}
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \} & \quad \text{if } reverse\_cost = \varnothing \\
    \text{ }  \text{ }  & \quad \text{ } \\
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \} &  \quad \text{ } \\
    \cup      \{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i>=0 \} & \quad \text{if } reverse\_cost \neq \varnothing \\
    \end{cases}`

.. rubric:: The problem

Given:

- :math:`G(V,E)`

Then:

     :math:`pgr\_directedChPP(edges\_sql) = \boldsymbol{\Phi}`

     :math:`\boldsymbol{\Phi} = {(seq_i, node_i, edge\_i, cost_i, agg\_cost_i)}`

Where:

  - :math:`seq_i = i`
  - :math:`node_i \in V`
  - :math:`edge_i  = \begin{cases}  id_{(node_i, node_{i+1},cost_i)}  &\quad  \text{when } i \neq | \pi | \\ -1 &\quad  \text{when } i = | \pi | \\ \end{cases}`
  - :math:`cost_i = cost_{(node_i, node_{i+1})}`
  - :math:`agg\_cost_i  = \begin{cases}  0   &\quad  \text{when } i = 1  \\ \displaystyle\sum_{k=1}^{i}  cost_{(node_{k-1}, node_k)}  &\quad  \text{when } i \neq 1 \\ \end{cases}`


In other words: The algorithm is to find the shortest path which contains every edge in a graph and starts and ends on the same node, if it exists, in terms of a sequence of nodes and of edges,
  - :math:`seq` indicates the relative position in the path of the :math:`node` or :math:`edge`.
  - :math:`cost` is the cost of the edge to be used to go to the next node.
  - :math:`agg\_cost` is the cost from the start node up to the node.


If there is no path, the resulting will be no path found error.


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
