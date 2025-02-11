..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Metrics Family ; pgr_betweennessCentrality - Experimental
   single: betweennessCentrality - Experimental on v3.7

|

``pgr_betweennessCentrality`` - Experimental
===============================================================================

``pgr_betweennessCentrality`` - Calculates the relative betweenness centrality
using Brandes Algorithm

.. rubric:: Availability

* Version 3.7.0

  * New experimental function.

Description
-------------------------------------------------------------------------------

The Brandes Algorithm takes advantage of the sparse graphs for evaluating the
betweenness centrality score of all vertices.


Betweenness centrality measures the extent to which a vertex lies on the
shortest paths between all other pairs of vertices. Vertices with a high
betweenness centrality score may have considerable influence in a network by the
virtue of their control over the shortest paths passing between them.

The removal of these vertices will affect the network by disrupting the
it, as most of the shortest paths between vertices pass through them.

This implementation work for both directed and undirected graphs.

- Running time:  :math:`\Theta(VE)`
- Running space: :math:`\Theta(VE)`
- Throws when there are no edges in the graph

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr_betweennessCentrality(`Edges SQL`_, [``directed``])

   | Returns set of ``(vid, centrality)``

:Example: For a directed graph with edges :math:`\{1, 2, 3, 4\}`.

.. literalinclude:: betweennessCentrality.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: Explanation

* The betweenness centrality are between parenthesis.
* The leaf vertices have betweenness centrality :math:`0`.
* Betweenness centrality of vertex :math:`6` is higher than of vertex :math:`10`.

  * Removing vertex :math:`6` will create three graph components.
  * Removing vertex :math:`10` will create two graph components.

.. graphviz::

    digraph G {
        5, 7, 15 [shape=circle;style=filled;width=.5;color=deepskyblue;fontsize=8;fixedsize=true;];
        6, 10 [shape=circle;style=filled;width=.5;color=green;fontsize=8;fixedsize=true;];
        5 [pos="0,0!";label="5 (0)"];
        6 [pos="0,1!"label="6 (0.5)"];
        7 [pos="0,2!"label="7 (0)"];
        10 [pos="1,1!"label="10 (0.25)"];
        15 [pos="2,1!"label="15 (0)"];
        5 -> 6 [dir=both;label="1  "];
        6->7 [dir=both;label="4  "];
        10->6 [label="3"];
        15->10 [label="4"];
    }

Parameters
-------------------------------------------------------------------------------

.. include:: allpairs-family.rst
    :start-after: edges_start
    :end-before: edges_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: no_id_edges_sql_start
    :end-before: no_id_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. list-table::
	:width: 81
	:widths: auto
	:header-rows: 1

	* - Column
	  - Type
	  - Description
	* - ``vid``
	  - ``BIGINT``
	  - Identifier of the vertex.
	* - ``centrality``
	  - ``FLOAT``
	  - Relative betweenness centrality score of the vertex (will be in range [0,1])

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: betweenness centrality
  <https://www.boost.org/libs/graph/doc/betweenness_centrality.html>`_

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
