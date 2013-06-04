.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_point_to_id:

pgr_pointToId - Inserts point into a vertices table
===============================================================================

.. index:: 
	single: pgr_pointToId(geometry,double precision)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_pointToId`` — Inserts a point into a temporary vertices table.

.. note:: 

	This function should not be used directly. Use :ref:`pgr_createTopology <pgr_create_topology>` instead. 


Synopsis
-------------------------------------------------------------------------------

Inserts a point into a temporary vertices table, and returns an id of a new point or an existing point. Tolerance is the minimal distance between existing points and the new point to create a new point.

.. code-block:: sql

	bigint pgr_pointToId(geometry point, double precision tolerance);


Description
-------------------------------------------------------------------------------

:point: ``geometry`` of the existing point
:tolerance: ``float8`` snapping tolerance of disconnected edges (in projection unit)

Returns point id (``bigint``) of a new or existing point.


.. rubric:: History

* Renamed in version 2.0.0


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_create_topology`
* :ref:`topology`
* :ref:`pgr_node_network`
