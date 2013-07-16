.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_create_topology:

pgr_createTopology - Building a Network Topology
===============================================================================

.. index:: 
	single: pgr_createTopology(varchar,double precision,varchar,varchar)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_createTopology`` — Builds a network topology with source and target information.


Synopsis
-------------------------------------------------------------------------------

The function returns ``OK`` after the network topology has been built.

.. code-block:: sql

	varchar pgr_createTopology(varchar geom_table, double precision tolerance,
                               varchar geo_cname, cvarchar gid_cname);


Description
-------------------------------------------------------------------------------

This function assumes the ``source`` and ``target`` columns exist on table ``geom_table`` and are of type integer or bigint.

.. code-block:: sql

    CREATE TABLE edge_table (
		gid_cname integer,
		source integer, -- or bigint
		target integer, -- or bigint
		geo_cname geometry
    );

It fills the ``source`` and ``target`` id column for all edges. All line ends with a distance less than tolerance, are assigned the same id. 

The topology creation function accepts the following parameters:

:geom_table: ``varchar`` network table name (may contain the schema name as well)
:tolerance: ``float8`` snapping tolerance of disconnected edges (in projection unit)
:geo_cname: ``varchar`` geometry column name of the network table
:gid_cname: ``varchar`` primary key column name of the network table

The function ``OK`` after the network topology has been built.


.. rubric:: History

* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT pgr_createTopology('edge_table', 0.000001, 'the_geom', 'id');

	 pgr_createtopology 
	--------------------
	 OK
	(1 row)


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`topology`
* :ref:`pgr_node_network`
* :ref:`pgr_point_to_id`
