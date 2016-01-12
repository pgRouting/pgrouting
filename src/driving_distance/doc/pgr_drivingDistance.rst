.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_drivingdistance:

pgr_drivingDistance
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_drivingDistance`` - Returns the driving distance from a start node.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Using Dijkstra algorithm, extracts all the nodes that have costs less than or equal to the value ``distance``.
The edges extracted will conform the corresponding spanning tree.

.. index::
	single: drivingDistance(edges_sql, start_vid, distance)

.. rubric:: The minimal signature:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_v bigint, distance float8)
     RETURNS SET OF (seq, node, edge, cost, agg_cost)


.. index::
	single: drivingDistance(edges_sql, start_vid, distance, directed)

.. rubric:: Driving Distance from a single starting point:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_vid bigint, distance float8, directed boolean)
     RETURNS SET OF (seq, node, edge, cost, agg_cost)


.. index::
	single: drivingDistance(edges_sql, start_vids, distance, directed, equiCost)

.. rubric:: Driving Distance from a multiple starting points:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_vids anyarray, distance float8,
         directed boolean default true,
         equicost boolean default false)
     RETURNS SET OF (seq, start_vid, node, edge, cost, agg_cost)

Description of the SQL query
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

        .. code-block:: sql

                SELECT id, source, target, cost [,reverse_cost] FROM edge_table


        :id: ``ANY-INTEGER`` identifier of the edge.
        :source: ``ANY-INTEGER`` identifier of the source vertex of the edge.
        :target: ``ANY-INTEGER`` identifier of the target vertex of the edge.
        :cost: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
        :reverse_cost: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

Description of the parameters of the signatures
-------------------------------------------------------------------------------

:sql: SQL query as decribed above.
:start_v: ``BIGINT`` id of the starting vertex.
:start_v: ``array[ANY-INTEGER]`` array of id of starting vertices.
:distance: ``FLOAT`` Upper limit for the inclusion of the node in the result.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
:equicost: ``boolean`` (optional). When ``true`` the node will only appear in the closest ``start_v`` list.  Default is ``false`` which resembles several calls using the single starting point signatures. Tie brakes are arbitrarely.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_v], node, edge, cost, agg_cost)``

:seq: ``INT`` row sequence.
:start_v: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:node: ``BIGINT`` id of the node within the limits from ``start_v``.
:edge: ``BIGINT`` id of the edge used to arrive to ``node``. ``0`` when the ``node`` is the ``start_v``.
:cost: ``FLOAT`` cost to traverse ``edge``.
:agg_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.



Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_drivingDistance.result
   :start-after: --q1
   :end-before: --q2

Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`


.. literalinclude:: doc-pgr_drivingDistance.result
   :start-after: --q2
   :end-before: --q3

Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_drivingDistance.result
   :start-after: --q3
   :end-before: --q4



Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_drivingDistance.result
   :start-after: --q4
   :end-before: --q5

The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0
* Added functionality in version 2.1


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_alphashape` - Alpha shape computation
* :ref:`pgr_points_as_polygon` - Polygon around set of points

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

