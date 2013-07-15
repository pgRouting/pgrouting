.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_node_network:

pgr_nodeNetwork - Nodes an network edge table.
===============================================================================

.. index:: 
	single: pgr_NodeNetwork(text,text,text,text,double precision)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_nodeNetwork`` - Nodes an network edge table.

:Author: Nicolas Ribot
:Copyright: Nicolas Ribot, The source code is released under the MIT-X license.


Synopsis
-------------------------------------------------------------------------------

The function reads edges from a not "noded" network table and writes the "noded" edges into a new table.

.. code-block:: sql

    text pgr_nodeNetwork(text table_in, text gid_cname, text geo_cname,
                         text table_out, double precision tolerance)


Description
-------------------------------------------------------------------------------

A common problem associated with bringing GIS data into pgRouting is the fact that the data is often not "noded" correctly. This will create invalid topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network all the edges will be broken into separate road segments. There are cases like an over-pass and under-pass intersection where you can not traverse from the over-pass to the under-pass, but this function does not have the ability to detect and accomodate those situations.

This function reads the ``table_in`` table, that has a primary key column ``gid_cname`` and geometry column named ``geo_cname`` and intersect all the segments in it against all the other segments and then creates a table ``table_out``. It uses the ``tolerance`` for deciding that multiple nodes within the tolerance are considered the same node. 

:table_in: ``text`` name of the input table to be noded
:gid_cname: ``text`` name of the primary key column for the input table
:geo_cname: ``text`` name of the geometry column that should be noded
:table_out: ``text`` name of the output table that will be created
:tolerance: ``float8`` tolerance for coinicident points (in projection unit)

The ``table_out`` table has a structure like:

:id: ``integer`` is a reference to the record in ``table_in``
:sub_id: ``bigint`` is a sequence number that identifies all the new segments generated from the original record.
:geom: ``geometry`` attribute

.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_nodeNetwork('edge_table', 'id', 'the_geom', 'edge_table_noded', 0.000001);

	               pgr_nodenetwork                
	----------------------------------------------
	 edge_table_noded generated with: 16 segments
	(1 row)


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_create_topology`
