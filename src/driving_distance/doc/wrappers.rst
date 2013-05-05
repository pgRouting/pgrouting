.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_driving_distance_wrappers:

PGR_DrivingDistance (Wrappers) — Driving Distance
===============================================================================

.. index:: 
	single: PGR_DrivingDistance(varchar,integer,double precision,double precision,boolean,boolean)
	single: PGR_DrivingDistance(varchar,double precision,double precision,double precision,varchar,varchar,boolean,boolean)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``PGR_DrivingDistance`` — Returns the driving distance from a start node.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`geoms <type_geoms>` (id, gid, the_geom) rows, that make up a path.

.. code-block:: sql

	geoms[] PGR_DrivingDistance(varchar table_name, integer source, double precision distance, double precision delta, boolean directed, boolean reverse_cost);

	geoms[] PGR_DrivingDistance(varchar table_name, double precision x, double precision y, double precision distance, varchar cost_column, varchar reverse_cost_column, boolean directed, boolean reverse_cost);


Description
-------------------------------------------------------------------------------

.. note:: 
	Not completed.


Examples
-------------------------------------------------------------------------------

.. note:: 
	Not completed.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance_core`
* :ref:`type_geoms`

