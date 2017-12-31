..
   ****************************************************************************
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_labelGraph:


pgr_labelGraph - Experimental
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_labelGraph`` — Locates and labels sub-networks within a network which are not topologically connected.

.. include:: proposed.rst
    :start-after: begin-warn-expr
    :end-before: end-warn-expr


Synopsis
-------------------------------------------------------------------------------

Must be run after ``pgr_createTopology()``. No use of ``geometry`` column. Only ``id``, ``source`` and  ``target`` columns are required.

The function returns:

- ``OK`` when a column with provided name has been generated and populated successfully. All connected edges will have unique similar integer values. In case of ``rows_where`` condition, non participating rows will have -1 integer values.
- ``FAIL`` when the processing cannot be finished due to some error. Notice will be thrown accordingly.
- ``rows_where condition generated 0 rows`` when passed SQL condition has not been fulfilled by any row.

.. index::
	single: labelGraph(Complete Signature) - Proposed

.. code-block:: sql

	varchar pgr_labelGraph(text, text, text, text, text, text)



Description
-------------------------------------------------------------------------------

A network behind any routing query may consist of sub-networks completely isolated from each other. Possible reasons could be:

- An island with no bridge connecting to the mainland.
- An edge or mesh of edges failed to connect to other networks because of human negligence during data generation.
- The data is not properly noded.
- Topology creation failed to succeed.

pgr_labelGraph() will create an integer column (with the name provided by the user) and will assign same integer values to all those edges in the network which are connected topologically. Thus better analysis regarding network structure is possible. In case of ``rows_where`` condition, non participating rows will have -1 integer values.

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

The sample data, has 3 subgraphs.

.. literalinclude:: doc-pgr_labelGraph.queries
   :start-after: --q1
   :end-before: --q2


See Also
-------------------------------------------------------------------------------

* `pgr_createTopology <https://github.com/Zia-/pgrouting/blob/develop/src/common/sql/pgrouting_topology.sql>`_ to create the topology of a table based on its geometry and tolerance value.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
