.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_tsp_wrappers:

PGR_Tsp (Wrappers) — Traveling Sales Person
===============================================================================

.. index:: 
	single: PGR_Tsp(text,varchar,varchar,integer,double precision);
	single: PGR_Tsp(text,varchar,varchar,integer,double precision,boolean,boolean);
	module: driving_distance

Name
-------------------------------------------------------------------------------

``PGR_Tsp`` — Returns the best route from a start node via a list of nodes.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`geoms <type_geoms>` (id, gid, the_geom) rows, that make up a path.

.. code-block:: sql

	geoms[] PGR_Tsp(text method, varchar table_name, varchar ids, integer source, double precision delta);

	geoms[] PGR_Tsp(text method, varchar table_name, varchar ids, integer source, double precision delta, boolean directed, boolean reverse_cost);

.. warning::
	Functions currently don't have a ``method`` parameter.

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

* :ref:`pgr_tsp_core`
* :ref:`type_geoms`

