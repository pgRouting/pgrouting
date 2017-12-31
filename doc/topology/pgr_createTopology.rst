..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_create_topology:

pgr_createTopology
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_createTopology`` — Builds a network topology based on the geometry information.


Synopsis
-------------------------------------------------------------------------------

The function returns:

- ``OK`` after the network topology has been built and the vertices table created.
- ``FAIL`` when the network topology was not built due to an error.

.. index::
    single: createTopology(Complete Signature)

.. code-block:: none

    varchar pgr_createTopology(text edge_table, double precision tolerance,
                           text the_geom:='the_geom', text id:='id',
                           text source:='source',text target:='target',
                           text rows_where:='true', boolean clean:=false)



Description
-------------------------------------------------------------------------------

.. rubric:: Parameters

The topology creation function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name AS well)
:tolerance: ``float8`` Snapping tolerance of disconnected edges. (in projection unit)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.
:id: ``text``  Primary key column name of the network table. Default value is ``id``.
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``.
:rows_where: ``text``   Condition to SELECT a subset or rows.  Default value is ``true`` to indicate
 all rows that where ``source`` or ``target`` have a null value, otherwise the condition is used.
:clean: ``boolean`` Clean any previous topology.  Default value is ``false``.

.. warning::

    The ``edge_table`` will be affected

    - The ``source`` column values will change.
    - The ``target`` column values will change.

      - An index will be created, if it doesn't exists, to speed up the process to the following columns:

        * ``id``
        * ``the_geom``
        * ``source``
        * ``target``

The function returns:

- ``OK`` after the network topology has been built.

  * Creates a vertices table: <edge_table>_vertices_pgr.
  * Fills ``id`` and ``the_geom`` columns of the vertices table.
  * Fills the source and target columns of the edge table referencing the ``id`` of the vertices table.


- ``FAIL`` when the network topology was not built due to an error:

  * A required column of the Network table is not found or is not of the appropriate type.
  * The condition is not well formed.
  * The names of source , target or id are the same.
  * The SRID of the geometry could not be determined.


.. rubric:: The Vertices Table

The vertices table is a requirement of the :ref:`pgr_analyzeGraph <pgr_analyze_graph>` and the :ref:`pgr_analyzeOneway <pgr_analyze_oneway>` functions.

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex AS incoming. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex AS outgoing. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:the_geom: ``geometry`` Point geometry of the vertex.

.. rubric:: History

* Renamed in version 2.0.0

Usage when the edge table's columns MATCH the default values:
-------------------------------------------------------------------------------

.. rubric:: The simplest way to use pgr_createTopology is:

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q1
   :end-before: --q1.1



.. rubric:: When the arguments are given in the order described in the parameters:

We get the same result AS the simplest way to use the function.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q2
   :end-before: --q2.1


.. warning::  | An error would occur when the arguments are not given in the appropriate order:
    | In this example, the column ``id`` of the table ``ege_table`` is passed to the function as the geometry column,
    | and the geometry column ``the_geom`` is passed to the function as the id column.

    .. literalinclude:: doc-pgr_createTopology.queries
       :start-after: --q3
       :end-before: --q3.1


.. rubric:: When using the named notation

Parameters defined with a default value can be omitted, as long as the value matches the default
And The order of the parameters would not matter.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q4
   :end-before: --q4.1

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q5
   :end-before: --q5.1

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q6
   :end-before: --q6.1

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q7
   :end-before: --q7.1

Selecting the rows where the geometry is near the geometry of row with ``id = 5``.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q8
   :end-before: --q8.1

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q9
   :end-before: --q9.1




Usage when the edge table's columns DO NOT MATCH the default values:
-------------------------------------------------------------------------------

For the following table

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q10
   :end-before: --q10.1

.. rubric:: Using positional notation:

The arguments need to be given in the order described in the parameters.

Note that this example uses clean flag. So it recreates the whole vertices table.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q11
   :end-before: --q11.1

.. warning::  | An error would occur when the arguments are not given in the appropiriate order:
    | In this example, the column ``gid`` of the table ``mytable`` is passed to the function AS the geometry column,
    | and the geometry column ``mygeom`` is passed to the function AS the id column.

    .. literalinclude:: doc-pgr_createTopology.queries
       :start-after: --q12
       :end-before: --q12.1



.. rubric:: When using the named notation

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.
The order of the parameters do not matter:

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q13
   :end-before: --q13.1

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q14
   :end-before: --q14.1


.. rubric:: Selecting rows using rows_where parameter

Based on id:

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q15
   :end-before: --q15.1


Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q16
   :end-before: --q16.1


Examples with full output
-------------------------------------------------------------------------------

This example start a clean topology, with 5 edges, and then its incremented to the rest of the edges.

.. literalinclude:: doc-pgr_createTopology.queries
   :start-after: --q17
   :end-before: --q17.1

The example uses the :doc:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`topology` for an overview of a topology for routing algorithms.
* :ref:`pgr_create_vert_table` to reconstruct the vertices table based on the source and target information.
* :ref:`pgr_analyze_graph` to analyze the edges and vertices of the edge table.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

