..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _components:

Components - Family of functions
===============================================================================

.. index from here

* :ref:`pgr_connectedComponentsV` - Return the connected components of an undirected graph (Vertex version).
* :ref:`pgr_strongComponentsV` - Return the strongly connected components of a directed graph (Vertex version).
* :ref:`pgr_biconnectedComponents` - Return the biconnected components of an undirected graph.

.. index to here


.. toctree::
    :hidden:

    pgr_connectedComponentsV
    pgr_strongComponentsV
    pgr_biconnectedComponents


The problem definition
-----------------------------------------------

.. rubric:: Connected components

A connected component of an undirected graph is a set of vertices that are all reachable
from each other.

**Notice**: This problem defines on an undirected graph.

Given the following query:


pgr_connectedComponentsV(:math:`sql`)

where  :math:`sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

and

  - :math:`source = \bigcup source_i`,
  - :math:`target = \bigcup target_i`,

The graphs are defined as follows:

The weighted undirected graph, :math:`G(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target`


* the set of edges :math:`E`

  - :math:`E = \begin{cases} &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, cost_i) \text{ when } cost >=0 \}  &\quad  \text{ if } reverse\_cost = \varnothing \\ \\ &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} \\ \cup &\{(source_i, target_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} &\quad \text{ if } reverse\_cost \neq \varnothing \\ \end{cases}`


Given:

  - :math:`G(V,E)`

Then:

.. math:: \text{pgr_connectedComponentsV}(sql) =
  \begin{cases}
  \text{all connected components } \boldsymbol{\pi} \text{ in } G(V,E) &\quad \text{if } \exists  \boldsymbol{\pi}  \\
  \varnothing &\quad \text{otherwise} \\
  \end{cases}

:math:`\boldsymbol{\pi} = \{(component_i, n\_seq_i, node_i)\}`

where:
  - :math:`component_i = \min \{node_j | node_j \in component_i\}`
  - :math:`n\_seq_i` is a sequential value starting from **1** in a component.
  - :math:`node_i \in component_i`
  - The returned values are ordered:

    - `component` ascending
    - `node` ascending

.. rubric:: Strongly connected components

A strongly connected component of a directed graph is a set of vertices that are all reachable
from each other.

**Notice**: This problem defines on a directed graph.

Given the following query:


pgr_strongComponentsV(:math:`sql`)

where  :math:`sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

and

  - :math:`source = \bigcup source_i`,
  - :math:`target = \bigcup target_i`,

The graphs are defined as follows:

The weighted directed graph, :math:`G_d(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target \cup {start_{vid}} \cup  {end_{vid}}`

* the set of edges :math:`E`

  - :math:`E = \begin{cases} &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} &\quad  \text{ if } reverse\_cost = \varnothing \\ \\ &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} &\quad \text{ if } reverse\_cost \neq \varnothing \\ \end{cases}`


Given:

  - :math:`G(V,E)`

Then:

.. math:: \text{pgr_strongComponentsV}(sql) =
  \begin{cases}
  \text{all strongly connected components } \boldsymbol{\pi} \text{ in } G(V,E) &\quad \text{if } \exists  \boldsymbol{\pi}  \\
  \varnothing &\quad \text{otherwise} \\
  \end{cases}

:math:`\boldsymbol{\pi} = \{(component_i, n\_seq_i, node_i)\}`

where:
  - :math:`component_i = \min {node_j | node_j \in component_i}`
  - :math:`n\_seq_i` is a sequential value starting from **1** in a component.
  - :math:`node_i \in component_i`
  - The returned values are ordered:

    - `component` ascending
    - `node` ascending

.. rubric:: Biconnected components

The biconnected components of an undirected graph are the maximal subsets of vertices such that the removal of a vertex from 
particular component will not disconnect the component. Unlike connected components, vertices may belong to multiple biconnected
components. Vertices can be present in multiple biconnected components, but each edge can only be contained in a single biconnected
component. So, the output only has edge version.

**Notice**: This problem defines on an undirected graph.

Given the following query:


pgr_biconnectedComponents(:math:`sql`)

where  :math:`sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

and

  - :math:`source = \bigcup source_i`,
  - :math:`target = \bigcup target_i`,

The graphs are defined as follows:

The weighted undirected graph, :math:`G(V,E)`, is definied by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target`


* the set of edges :math:`E`

  - :math:`E = \begin{cases} &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, cost_i) \text{ when } cost >=0 \}  &\quad  \text{ if } reverse\_cost = \varnothing \\ \\ &\{(source_i, target_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, cost_i) \text{ when } cost >=0 \} \\ \cup &\{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} \\ \cup &\{(source_i, target_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0)\} &\quad \text{ if } reverse\_cost \neq \varnothing \\ \end{cases}`


Given:

  - :math:`G(V,E)`

Then:

.. math:: \text{pgr_biconnectedComponents}(sql) =
  \begin{cases}
  \text{all biconnected components } \boldsymbol{\pi} \text{ in } G(V,E) &\quad \text{if } \exists  \boldsymbol{\pi}  \\
  \varnothing &\quad \text{otherwise} \\
  \end{cases}

:math:`\boldsymbol{\pi} = \{(component_i, n\_seq_i, node_i)\}`

where:
  - :math:`component_i = \min {node_j | node_j \in component_i}`
  - :math:`n\_seq_i` is a sequential value starting from **1** in a component.
  - :math:`edge_i \in component_i`
  - The returned values are ordered:

    - `component` ascending
    - `edge` ascending

.. components_edges_sql_start

Description of the edges_sql query for components functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. components_edges_sql_end


.. components_parameters_start

Description of the parameters of the signatures
...............................................................................

=================== ====================== ========= =========================================
Parameter           Type                   Default   Description
=================== ====================== ========= =========================================
**edges_sql**       ``TEXT``                         SQL query as described above.
=================== ====================== ========= =========================================

.. components_parameters_end


.. return_componentsV_start

Description of the return values for connected components (vertex version) and strongly connected components (vertex version)
.............................................................................................................................

Returns set of ``(seq, component, n_seq, node)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**component**  ``BIGINT`` Component identifier. It is equal to the minimum node identifier in the component.
**n_seq**      ``INT``    It is a sequential value starting from **1** in a component. 
**node**       ``BIGINT`` Identifier of the vertex.
============== ========== =================================================

.. return_componentsV_end


.. return_componentsE_start

Description of the return values for biconnected components, connected components (edge version) and strongly connected components (edge version)
.................................................................................................................................................

Returns set of ``(seq, component, n_seq, edge)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**component**  ``BIGINT`` Component identifier. It is equal to the minimum edge identifier in the component.
**n_seq**      ``INT``    It is a sequential value starting from **1** in a component. 
**edge**       ``BIGINT`` Identifier of the edge.
============== ========== =================================================

.. return_componentsE_end




