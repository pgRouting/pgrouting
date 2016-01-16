BEGIN;
/*.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************
*/
--.. _example_recipe:

--Comparing topology of a unnoded network with a noded network
-------------------------------------------------------------

--:Author: pgRouting team.
--:Licence: Open Source

--This recipe uses the :ref:`sampledata` network.

--The purpose of this recipe is to compare a not nodded network with a nodded network.

--.. code-block:: sql
set client_min_messages = warning;

	SELECT pgr_createTopology('edge_table', 0.001);
	SELECT pgr_analyzegraph('edge_table', 0.001);
	SELECT pgr_nodeNetwork('edge_table', 0.001);
	SELECT pgr_createTopology('edge_table_noded', 0.001);
	SELECT pgr_analyzegraph('edge_table_noded', 0.001); 
ROLLBACK;
