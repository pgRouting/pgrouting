.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_alphashape:

pgr_alphashape - Alpha shape computation
===============================================================================

.. index:: 
	single: pgr_alphashape(text)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_alphashape`` — Core function for alpha shape computation.

.. note:: 

	This function should not be used directly. Use :ref:`pgr_drivingDistance <pgr_driving_distance>` instead. 


Synopsis
-------------------------------------------------------------------------------

Returns a table with (x, y) rows that describe a vertex result.

.. code-block:: sql

	table() pgr_alphashape(text sql);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

  .. code-block:: sql

    SELECT id, source, target, cost [,reverse_cost] FROM edge_table


  :id: ``int4`` identifier of the edge
  :source: ``int4`` identifier of the source vertex
  :target: ``int4`` identifier of the target vertex
  :cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
  :reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).


Returns a table with attributes:

:x: x-coordinate
:y: y-coordinate


.. rubric:: History

* Renamed in version 2.0.0


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance`
