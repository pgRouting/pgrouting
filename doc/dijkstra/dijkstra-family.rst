..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _dijkstra:

Dijkstra - Family of functions
===============================================================================

.. index from here

* :ref:`pgr_dijkstra` - Dijkstra's algorithm for the shortest paths.
* :ref:`pgr_dijkstraCost` - Get the aggregate cost of the shortest paths.
* :ref:`pgr_dijkstraCostMatrix` - Use pgr_dijkstra to create a costs matrix.
* :ref:`pgr_drivingDistance` - Use pgr_dijkstra to calculate catchament information.
* :ref:`pgr_ksp` - Use Yen algorithm with pgr_dijkstra to get the K shortest paths.
* :ref:`pgr_dijkstraVia` - Get a route of a seuence of vertices.

.. index to here


.. toctree::
    :hidden:

    pgr_dijkstra
    pgr_dijkstraCost
    pgr_dijkstraCostMatrix
    pgr_drivingDistance
    pgr_KSP
    pgr_dijkstraVia


The problem definition (Advanced documentation)
-----------------------------------------------


Given the following query:


pgr_dijkstra(:math:`sql, start_{vid}, end_{vid}, directed`)

where  :math:`sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

and

- :math:`source = \bigcup source_i`,
- :math:`target = \bigcup target_i`,

The graphs are defined as follows:

.. rubric:: Directed graph

The weighted directed graph, :math:`G_d(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target \cup {start_{vid}} \cup  {end_{vid}}`

* the set of edges :math:`E`

  - :math:`E = \begin{cases}
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \} & \quad \text{if } reverse\_cost = \varnothing \\
    \text{ }  \text{ }  & \quad \text{ } \\
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \} &  \quad \text{ } \\
    \cup      \{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i>=0 \} & \quad \text{if } reverse\_cost \neq \varnothing \\
    \end{cases}`



.. rubric:: Undirected graph

The weighted undirected graph, :math:`G_u(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target \cup {start_v{vid}} \cup  {end_{vid}}`


* the set of edges :math:`E`

  - :math:`E = \begin{cases}
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \}  & \quad \text{ } \\
    \cup      \{(target_i, source_i, cost_i) \text{ when } cost >=0 \}  & \quad \text{ if } reverse\_cost = \varnothing \\
    \text{ }  \text{ }  & \text{ } \\
    \text{ }  \{(source_i, target_i, cost_i) \text{ when } cost >=0 \} & \text{ } \\
    \cup  \{(target_i, source_i, cost_i) \text{ when } cost >=0 \} & \text{ } \\
    \cup  \{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} & \text{ } \\
    \cup  \{(source_i, target_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} & \quad \text{ if } reverse\_cost \neq \varnothing \\
    \end{cases}`



.. rubric:: The problem

Given:

- :math:`start_{vid} \in V` a starting vertex
- :math:`end_{vid} \in V` an ending vertex
- :math:`G(V,E) = \begin{cases}
  G_d(V,E) & \quad \text{ if6 } directed = true \\
  G_u(V,E) & \quad \text{ if5 } directed = false \\
  \end{cases}`

Then:

.. math::`pgr_dijkstra(sql, start_{vid}, end_{vid}, directed) =
  \begin{cases}
  \text{shortest path } \boldsymbol{\pi} \text{ between } start_{vid} \text{and } end_{vid} & \quad \text{if7 } \exists  \boldsymbol{\pi}  \\
  \varnothing & \quad \text{otherwise} \\
  \end{cases}`


- :math:`\boldsymbol{\pi} = \{(path\_seq_i, node_i, edge_i, cost_i, agg\_cost_i)\}`

where:
  - :math:`path\_seq_i = i`
  - :math:`path\_seq_{| \pi |} = | \pi |`
  - :math:`node_i \in V`
  - :math:`node_1 = start_{vid}`
  - :math:`node_{| \pi |}  = end_{vid}`
  - :math:`\forall i \neq | \pi |, \quad (node_i, node_{i+1}, cost_i) \in E`
  - :math:`edge_i  = \begin{cases}  id_{(node_i, node_{i+1},cost_i)}  &\quad  \text{when } i \neq | \pi | \\ -1 &\quad  \text{when } i = | \pi | \\ \end{cases}`
  - :math:`cost_i = cost_{(node_i, node_{i+1})}`
  - :math:`agg\_cost_i  = \begin{cases}  0   &\quad  \text{when } i = 1  \\ \displaystyle\sum_{k=1}^{i}  cost_{(node_{k-1}, node_k)}  &\quad  \text{when } i \neq 1 \\ \end{cases}`



In other words: The algorithm returns a the shortest path between :math:`start_{vid}` and :math:`end_{vid}` , if it exists, in terms of a sequence of nodes  and of edges,
  - :math:`path\_seq` indicates the relative position in the path of the :math:`node` or :math:`edge`.
  - :math:`cost` is the cost of the edge to be used to go to the next node.
  - :math:`agg\_cost` is the cost from the :math:`start_{vid}` up to the node.


If there is no path, the resulting set is empty.


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`



