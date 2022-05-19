..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_drivingDistance.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_drivingDistance.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_drivingDistance.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_drivingDistance.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_drivingDistance.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_drivingDistance.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_drivingDistance.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_drivingDistance.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/driving_distance/doc/pgr_drivingDistance.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/driving_distance/doc/pgr_drivingDistance.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/driving_distance/doc/dd_driving_distance_v3.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/driving_distance/doc/dd_driving_distance.html>`__

pgr_drivingDistance
===============================================================================

``pgr_drivingDistance`` - Returns the driving distance from a start node.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.1.0:

  * Signature change pgr_drivingDistance(single vertex)
  * New **Official** pgr_drivingDistance(multiple vertices)

* Version 2.0.0:

  * **Official** pgr_drivingDistance(single vertex)


Description
-------------------------------------------------------------------------------

Using the Dijkstra algorithm, extracts all the nodes that have costs less than
or equal to the value ``distance``.
The edges extracted will conform to the corresponding spanning tree.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vid,  distance [, directed])
    pgr_drivingDistance(edges_sql, start_vids, distance [, directed] [, equicost])
    RETURNS SET OF (seq, [start_vid,] node, edge, cost, agg_cost)

.. index::
	single: drivingDistance(Single vertex)

Single Vertex
...............................................................................

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vid, distance [, directed])
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

:Example: From vertex :math:`6` for a distance of :math:`3.0`

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q1
   :end-before: --q2

.. index::
	single: drivingDistance(Multiple vertices)

Multiple Vertices
...............................................................................

.. code-block:: sql

    pgr_drivingDistance(edges_sql, start_vids, distance, [, directed] [, equicost])
    RETURNS SET OF (seq, start_vid, node, edge, cost, agg_cost)

:Example: From vertices :math:`\{6, 9\}` for a distance of :math:`3.0` with
          equi-cost on a directed graph

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q2
   :end-before: --q3

Parameters
-------------------------------------------------------------------------------

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

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_v], node, edge, cost, agg_cost)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
**seq**        ``INTEGER`` Sequential value starting from **5**.
**start_vid**  ``INTEGER`` Identifier of the starting vertex.
**node**       ``BIGINT``  Identifier of the node in the path within the limits from ``start_vid``.
**edge**       ``BIGINT``  Identifier of the edge used to arrive to ``node``. ``0`` when the ``node`` is the ``start_vid``.
**cost**       ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**   ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
============== =========== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: From vertices :math:`\{6, 9\}` for a distance of :math:`3.0` on an
          undirected graph

.. literalinclude:: doc-pgr_drivingDistance.queries
   :start-after: --q3
   :end-before: --q4

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_alphaShape` - Alpha shape computation
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

