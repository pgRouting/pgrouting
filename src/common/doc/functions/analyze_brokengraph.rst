.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_brokenGraph:



pgr_brokenGraph
===============================================================================

.. index:: 
	single: pgr_brokenGraph(character varying, character varying)
	module: common



Name
-------------------------------------------------------------------------------

``pgr_brokenGraph`` — Locates and labels sub-networks within a network which are not connected topologically.



Synopsis
-------------------------------------------------------------------------------

The function returns:

  - ``Success`` when a column with the provided name has been generated successfully.
  - ``Column column_name in the table table_name already exists!`` if there is already a column in ways/edge table with the provided name.
  - ``Something went wrong!`` when locating and labeling sub-networks within a network could not occur due to an error. 

.. code-block:: sql

	varchar pgr_brokenGraph(character varying edge_table, character varying graph_id_column)



Description
-------------------------------------------------------------------------------

A network behind any routing query may consist of sub-networks completely isolated from each other. Possible reason could be:

1. An island with no bridge connecting to the mainland.
2. An edge or mesh of edges failed to connect to other networks because of human negligence during data generation.
3. The data is not noded properly.
4. Topology creation failed to succeed. 

pgr_brokenGraph() will create an integer column (with the name provided by the user) and will assign same integer value to all those edges in a network which are connected topologically. Thus better analysis regarding network structure is possible.

Function accepts the following parameters:

:edge_table: ``character varying`` Network table name.
:graph_id_column: ``character varying`` Column name which will hold the integer labels for each sub-graph.

The function returns:

  - ``Success`` when a column with the provided name has been generated successfully.

    * Creates a column with the provided name.
    * Fills that column with integer values starting with 1. Each sub-network will have its own integer value.


  - ``Column column_name in the table table_name already exists!`` if there is already a column in ways/edge table with the provided name.

    * There is already a column with the provided name.

  - ``Something went wrong!`` when locating and labeling sub-networks within a network could not occur due to an error. 

    * Some other internal error occurred.



Examples Output pane Messages
-------------------------------------------------------------------------------

 - When the function went successfully (may take seconds to minutes depending upon the network's size)

 	NOTICE:  Success
	Total query runtime: 171932 ms.
	0 rows retrieved.

 - When there is already a column with the provided name string

	NOTICE:  Column graph_id in the table ways already exists!
    Total query runtime: 24 ms.
    0 rows retrieved.

 - When some internal error occurred

	NOTICE:  Something went wrong!
    Total query runtime: 15 ms.
    0 rows retrieved.