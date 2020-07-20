..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_nodeNetwork
===============================================================================

``pgr_nodeNetwork`` - Nodes an network edge table.

:Author: Nicolas Ribot
:Copyright: Nicolas Ribot, The source code is released under the MIT-X license.

The function reads edges from a not "noded" network table and writes the "noded" edges into a new table.

.. index::
	single: pgr_nodeNetwork

.. code-block:: sql

    pgr_nodenetwork(edge_table, tolerance, id, text the_geom, table_ending, rows_where, outall)
    RETURNS TEXT

.. rubric:: Availability

* Version 2.0.0

  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_nodeNetwork.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_nodeNetwork.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_nodeNetwork.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_nodeNetwork.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_nodeNetwork.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/topology/doc/pgr_nodeNetwork.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/topology/doc/pgr_nodeNetwork.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/common/doc/functions/node_network.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/common/doc/functions/node_network.html>`__


Description
-------------------------------------------------------------------------------

**The main characteristics are:**

A common problem associated with bringing GIS data into pgRouting is the fact that the data is often not "noded" correctly. This will create invalid topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network all the edges will be broken into separate road segments. There are cases like an over-pass and under-pass intersection where you can not traverse from the over-pass to the under-pass, but this function does not have the ability to detect and accommodate those situations.

This function reads the ``edge_table`` table, that has a primary key column ``id`` and geometry column named ``the_geom`` and intersect all the segments in it against all the other segments and then creates a table ``edge_table_noded``. It uses the ``tolerance`` for deciding that multiple nodes within the tolerance are considered the same node.

Parameters
-------------------------------------------------------------------------------

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:tolerance: ``float8`` tolerance for coincident points (in projection unit)dd
:id: ``text`` Primary key column name of the network table. Default value is ``id``.
:the_geom: ``text`` Geometry column name of the network table. Default value is ``the_geom``.
:table_ending: ``text`` Suffix for the new table's. Default value is ``noded``.

The output table will have for  ``edge_table_noded``

:id: ``bigint`` Unique identifier for the table
:old_id: ``bigint``  Identifier of the edge in original table
:sub_id: ``integer`` Segment number of the original edge
:source: ``integer`` Empty source column to be used with  :doc:`pgr_createTopology` function
:target: ``integer`` Empty target column to be used with  :doc:`pgr_createTopology` function
:the geom: ``geometry`` Geometry column of the noded network

Examples
-------------------------------------------------------------------------------

Let's create the topology for the data in :doc:`sampledata`

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q1
   :end-before: --q1.1

Now we can analyze the network.

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q2
   :end-before: --q2.1

The analysis tell us that the network has a gap and an intersection. We try to fix the problem using:

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q3
   :end-before: --q3.1

Inspecting the generated table, we can see that edges 13,14 and 18 has been segmented

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q4
   :end-before: --q4.1

We can create the topology of the new network

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q5
   :end-before: --q5.1

Now let's analyze the new topology

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q6
   :end-before: --q6.1

Images
-------------------------------------------------------------------------------

.. only:: html

	+--------------------------------------+-------------------------------------+
	|.. Rubric:: Before Image              |.. Rubric:: After Image              |
	|                                      |                                     |
	|.. image:: images/before_node_net.png |.. image:: images/after_node_net.png |
	|   :scale: 60%                        |   :scale: 60%                       |
	|   :alt: before image                 |   :alt: after image                 |
	|   :align: left                       |   :align: right                     |
	+--------------------------------------+-------------------------------------+


.. only:: latex

	.. Rubric:: Before Image

	.. image:: images/before_node_net.png
		:scale: 60%
		:alt: before image
		:align: left


	.. Rubric:: After Image

	.. image:: images/after_node_net.png
		:scale: 60%
		:alt: after image
		:align: left


Comparing the results
-------------------------------------------------------------------------------

Comparing with the Analysis in the original edge_table, we see that.

+------------------+-----------------------------------------+--------------------------------------------------------------+
|                  |                Before                   |                        After                                 |
+==================+=========================================+==============================================================+
|Table name        | edge_table                              | edge_table_noded                                             |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Fields            | All original fields                     | Has only basic fields to do a topology analysis              |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Dead ends         | - Edges with 1 dead end: 1,6,24         | Edges with 1 dead end: 1-1 ,6-1,14-2, 18-1 17-1 18-2         |
|                  | - Edges with 2 dead ends  17,18         |                                                              |
|                  |                                         |                                                              |
|                  | Edge 17's right node is a dead end      |                                                              |
|                  | because there is no other edge sharing  |                                                              |
|                  | that same node. (cnt=1)                 |                                                              |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Isolated segments | two isolated segments:  17 and 18 both  | No Isolated segments                                         |
|                  | they have 2 dead ends                   |  - Edge 17 now shares a node with edges 14-1 and 14-2        |
|                  |                                         |  - Edges 18-1 and 18-2 share a node with edges 13-1 and 13-2 |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Gaps              | There is a gap between edge 17 and 14   | Edge 14 was segmented                                        |
|                  | because edge 14 is near to the right    | Now edges: 14-1 14-2 17 share the same node                  |
|                  | node of edge 17                         | The tolerance value was taken in account                     |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Intersections     | Edges 13 and 18 were intersecting       | Edges were segmented, So, now in the interection's           |
|                  |                                         | point there is a node and the following edges share it:      |
|                  |                                         | 13-1 13-2 18-1 18-2                                          |
+------------------+-----------------------------------------+--------------------------------------------------------------+

Now, we are going to include the segments 13-1, 13-2 14-1, 14-2 ,18-1 and 18-2 into our edge-table, copying the data for dir,cost,and reverse cost with tho following steps:

- Add a column old_id into edge_table, this column is going to keep track the id of the original edge
- Insert only the segmented edges, that is, the ones whose max(sub_id) >1

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q7
   :end-before: --q7.1

We recreate the topology:

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q8
   :end-before: --q8.1

To get the same analysis results as the topology of edge_table_noded, we do the following query:

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q9
   :end-before: --q9.1

To get the same analysis results as the original edge_table, we do the following query:

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q10
   :end-before: --q10.1

Or we can analyze everything because, maybe edge 18 is an overpass, edge 14 is an under pass and there is also a street level juction, and the same happens with edges 17 and 13.

.. literalinclude:: doc-pgr_nodeNetwork.queries
   :start-after: --q11
   :end-before: --q11.1

See Also
-------------------------------------------------------------------------------

:doc:`topology-functions` for an overview of a topology for routing algorithms.
:doc:`pgr_analyzeOneWay` to analyze directionality of the edges.
:doc:`pgr_createTopology` to create a topology based on the geometry.
:doc:`pgr_analyzeGraph` to analyze the edges and vertices of the edge table.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
