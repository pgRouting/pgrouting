.. 
   ****************************************************************************
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
	single: pgr_brokenGraph(text, text, text, text, text, text)
	module: common



Name
-------------------------------------------------------------------------------

``pgr_brokenGraph`` — Locates and labels sub-networks within a network which are not topologically connected. Must be run after ``pgr_createTopology()``. No use of ``geometry`` column. Only ``id``, ``source`` and  ``target`` columns are required.



Synopsis
-------------------------------------------------------------------------------

The function returns:

  - ``OK`` when a column with provided name has been generated and populated successfully. All connected edges will have unique similar integer values. In case of ``rows_where`` condition, non participating rows will have -1 integer values.
  - ``FAIL`` when the processing cannot be finished due to some error. Notice will be thrown accordingly.
  - ``rows_where condition generated 0 rows`` when passed SQL condition has not been fulfilled by any row. 

.. code-block:: sql

	varchar pgr_brokenGraph(text, text, text, text, text, text)



Description
-------------------------------------------------------------------------------

A network behind any routing query may consist of sub-networks completely isolated from each other. Possible reasons could be:

- An island with no bridge connecting to the mainland.
- An edge or mesh of edges failed to connect to other networks because of human negligence during data generation.
- The data is not properly noded.
- Topology creation failed to succeed. 

pgr_brokenGraph() will create an integer column (with the name provided by the user) and will assign same integer values to all those edges in the network which are connected topologically. Thus better analysis regarding network structure is possible. In case of ``rows_where`` condition, non participating rows will have -1 integer values.

Prerequisites:
Must run ``pgr_createTopology()`` in order to generate ``source`` and ``target`` columns. Primary key column ``id`` should also be there in the network table.

Function accepts the following parameters:

:edge_table: ``text`` Network table name, with optional schema name.
:id: ``text`` Primary key column name of the network table. Default is ``id``.
:source: ``text`` Source column name generated after ``pgr_createTopology()``. Default is ``source``.
:target: ``text`` Target column name generated after ``pgr_createTopology()``. Default is ``target``.
:subgraph: ``text`` Column name which will hold the integer labels for each sub-graph. Default is ``subgraph``.
:rows_where: ``text`` The SQL where condition. Default is ``true``, means the processing will be done on the whole table. 




Example Usage
-------------------------------------------------------------------------------

.. code-block:: sql

  select pgr_brokenGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<100');


Example Output pane Message
-------------------------------------------------------------------------------

.. code-block:: sql

  NOTICE:  Processing:
  NOTICE:  pgr_brokenGraph('ways','id','source','target','subgraph','true')
  NOTICE:  Performing initial checks, please hold on ...
  NOTICE:  Starting - Checking table ...
  NOTICE:  Ending - Checking table
  NOTICE:  Starting - Checking columns
  NOTICE:  Ending - Checking columns
  NOTICE:  Starting - Checking rows_where condition
  NOTICE:  Ending - Checking rows_where condition
  NOTICE:  Starting - Calculating subgraphs
  NOTICE:  Successfully complicated calculating subgraphs
  NOTICE:  Ending - Calculating subgraphs

  Total query runtime: 5426 ms.
  1 row retrieved.

  pgr_brokenGraph
  character varying
  --------------------
  OK
  (1 row)




See Also
-------------------------------------------------------------------------------

* `pgr_createTopology <https://github.com/Zia-/pgrouting/blob/develop/src/common/sql/pgrouting_topology.sql>`_ to create the topology of a table based on its geometry and tolerance value.
