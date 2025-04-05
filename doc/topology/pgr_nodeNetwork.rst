..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Topology Family ; pgr_nodeNetwork
   single: nodeNetwork

|

``pgr_nodeNetwork``
===============================================================================

``pgr_nodeNetwork`` - Nodes an network edge table.

:Author: Nicolas Ribot
:Copyright: Nicolas Ribot, The source code is released under the MIT-X license.

The function reads edges from a not "noded" network table and writes the "noded"
edges into a new table.

.. index::
	single: pgr_nodeNetwork

.. parsed-literal::

   | pgr_nodenetwork(edge_table, tolerance, [**options**])
   | **options:** ``[id, text the_geom, table_ending, rows_where, outall]``

   | RETURNS ``TEXT``

.. rubric:: Availability

* Version 2.0.0

  * Official function.


Description
-------------------------------------------------------------------------------

**The main characteristics are:**

A common problem associated with bringing GIS data into pgRouting is the fact
that the data is often not "noded" correctly. This will create invalid
topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network all
the edges will be broken into separate road segments. There are cases like an
over-pass and under-pass intersection where you can not traverse from the
over-pass to the under-pass, but this function does not have the ability to
detect and accommodate those situations.

This function reads the ``edge_table`` table, that has a primary key column
``id`` and geometry column named ``the_geom`` and intersect all the segments in
it against all the other segments and then creates a table ``edge_table_noded``.
It uses the ``tolerance`` for deciding that multiple nodes within the tolerance
are considered the same node.

Parameters
-------------------------------------------------------------------------------

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:tolerance: ``float8`` tolerance for coincident points (in projection unit)dd
:id: ``text`` Primary key column name of the network table. Default value is
     ``id``.
:the_geom: ``text`` Geometry column name of the network table. Default value is
           ``the_geom``.
:table_ending: ``text`` Suffix for the new table's. Default value is ``noded``.

The output table will have for ``edge_table_noded``

:id: ``bigint`` Unique identifier for the table
:old_id: ``bigint`` Identifier of the edge in original table
:sub_id: ``integer`` Segment number of the original edge
:source: ``integer`` Empty source column
:target: ``integer`` Empty target column
:the geom: ``geometry`` Geometry column of the noded network

Examples
-------------------------------------------------------------------------------

Create the topology for the data in :doc:`sampledata`

.. literalinclude:: sampledata.queries
   :start-after: -- q1
   :end-before: -- q1-1

.. literalinclude:: sampledata.queries
   :start-after: -- q3
   :end-before: -- q4

Analyze the network for intersections.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q1
   :end-before: --q2

Analyze the network for gaps.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q2
   :end-before: --q3

The analysis tell us that the network has a gap and an intersection.

Fixing an intersection
...............................................................................

Storing the intersections.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q3
   :end-before: --q4

Calling ``pgr_nodeNetwork``.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q4
   :end-before: --q5

Inspecting the generated table, we can see that edges 13 and 18 have been
segmented.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q5
   :end-before: --q6

.. rubric:: Update the topology

Add new segments to the edges table.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q6
   :end-before: --q7

Insert the intersection as new vertices.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q7
   :end-before: --q8

Update source and target information on the edges table.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q8
   :end-before: --q9

Delete original edge.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q9
   :end-before: --q10

Update the vertex topology

.. literalinclude:: nodeNetwork.queries
   :start-after: --q10
   :end-before: --q11

Analyze the network for intersections.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q11
   :end-before: --q12

Fixing a gap
...............................................................................

Store the deadends

.. literalinclude:: nodeNetwork.queries
   :start-after: --q12
   :end-before: --q13

Calling ``pgr_nodeNetwork``.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q13
   :end-before: --q14

Inspecting the generated table, we can see that edge 14 has been segmented.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q14
   :end-before: --q15

.. rubric:: Update the topology

Add new segments to the edges table.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q15
   :end-before: --q16

Insert the intersection as new vertices.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q16
   :end-before: --q17

Update source and target information on the edges table.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q17
   :end-before: --q18

Delete original edge.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q18
   :end-before: --q19

Update the vertex topology

.. literalinclude:: nodeNetwork.queries
   :start-after: --q19
   :end-before: --q20

Analyze the network for gaps.

.. literalinclude:: nodeNetwork.queries
   :start-after: --q20
   :end-before: --q21

See Also
-------------------------------------------------------------------------------

:doc:`topology-functions` for an overview of a topology for routing algorithms.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
