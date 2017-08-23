..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_drivingDistance:

pgr_drivingDistance
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_drivingDistance`` - Returns the driving distance from a start node.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability

* pgr_drivingDistance(single vertex) 2.0.0, signature change 2.1.0
* pgr_drivingDistance(multiple vertices) 2.1.0

Synopsis
-------------------------------------------------------------------------------

Using the Dijkstra algorithm, extracts all the nodes that have costs less than or equal to the value ``distance``.
The edges extracted will conform to the corresponding spanning tree.

Signature Summary
-------------------------------------------------------------------------------

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vid, distance)
    pgr_drivingDistance(edges_sql, start_vid, distance, directed)
    pgr_drivingDistance(edges_sql, start_vids, distance, directed, equicost)

    RETURNS SET OF (seq, [start_vid,] node, edge, cost, agg_cost)


Signatures
-------------------------------------------------------------------------------

.. index::
	single: drivingDistance(Minimal Use)

Minimal Use
...................

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vid, distance)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)



.. index::
	single: drivingDistance(Single Start Vertex)

Driving Distance From A Single Starting Vertex
..............................................

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vid, distance, directed)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)


.. index::
	single: drivingDistance(Multiple Starting Vertices)

Driving Distance From Multiple Starting Vertices
.................................................

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vids, distance, directed, equicost)
    RETURNS SET OF (seq, start_vid, node, edge, cost, agg_cost)

Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


Description of the parameters of the signatures
.................................................

============== ====================== =================================================
Column          Type                  Description
============== ====================== =================================================
**edges_sql**  ``TEXT``               SQL query as described above.
**start_vid**  ``BIGINT``             Identifier of the starting vertex.
**start_vids** ``ARRAY[ANY-INTEGER]`` Array of identifiers of the starting vertices.
**distance**   ``FLOAT``              Upper limit for the inclusion of the node in the result.
**directed**   ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**equicost**   ``BOOLEAN``            (optional). When ``true`` the node will only appear in the closest ``start_vid`` list.  Default is ``false`` which resembles several calls using the single starting point signatures. Tie brakes are arbitrary.
============== ====================== =================================================



Description of the return values
.................................................

Returns set of ``(seq [, start_v], node, edge, cost, agg_cost)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
**seq**        ``INTEGER`` Sequential value starting from **1**.
**start_vid**  ``INTEGER`` Identifier of the starting vertex.
**node**       ``BIGINT``  Identifier of the node in the path within the limits from ``start_vid``.
**edge**       ``BIGINT``  Identifier of the edge used to arrive to ``node``. ``0`` when the ``node`` is the ``start_vid``.
**cost**       ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**   ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
============== =========== =================================================



Additional Examples
-------------------------------------------------------------------------------

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
................................................................................................

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q1
   :end-before: --q2

Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
................................................................................................

The examples in this section use the following :ref:`fig2`


.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q2
   :end-before: --q3

Examples for queries marked as ``directed`` with ``cost`` column
......................................................................................

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q3
   :end-before: --q4



Examples for queries marked as ``undirected`` with ``cost`` column
......................................................................................

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q4
   :end-before: --q5


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_alphashape` - Alpha shape computation
* :ref:`pgr_points_as_polygon` - Polygon around set of points
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

