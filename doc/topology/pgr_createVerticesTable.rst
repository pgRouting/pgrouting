..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_createVerticesTable
===============================================================================

``pgr_createVerticesTable`` — Reconstructs the vertices table based on the source and target information.

.. rubric:: Availability

* Version 2.0.0

  * Renamed from version 1.x
  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_createVerticesTable.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_createVerticesTable.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_createVerticesTable.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_createVerticesTable.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_createVerticesTable.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/topology/doc/pgr_createVerticesTable.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/topology/doc/pgr_createVerticesTable.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/common/doc/functions/create_vert_table.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/common/doc/functions/create_vert_table.html>`__

Description
-------------------------------------------------------------------------------

The function returns:

- ``OK`` after the vertices table has been reconstructed.
- ``FAIL`` when the vertices table was not reconstructed due to an error.

Signatures
-------------------------------------------------------------------------------

.. code-block:: sql

	pgr_createVerticesTable(edge_table, the_geom, source, target, rows_where)
    RETURNS VARCHAR

Parameters
-------------------------------------------------------------------------------

The reconstruction of the vertices table  function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``.
:rows_where: ``text``   Condition to SELECT a subset or rows.  Default value is ``true`` to indicate all rows.

.. warning::

    The ``edge_table`` will be affected

    - An index will be created, if it doesn't exists, to speed up the process to the following columns:

      * ``the_geom``
      * ``source``
      * ``target``

The function returns:

- ``OK`` after the vertices table has been reconstructed.

  * Creates a vertices table: <edge_table>_vertices_pgr.
  * Fills ``id`` and ``the_geom`` columns of the vertices table based on the source and target columns of the edge table.

- ``FAIL`` when the vertices table was not reconstructed due to an error.

  * A required column of the Network table is not found or is not of the appropriate type.
  * The condition is not well formed.
  * The names of source, target are the same.
  * The SRID of the geometry could not be determined.

.. rubric:: The Vertices Table

The vertices table is a requierment of the :doc:`pgr_analyzeGraph <pgr_analyzeGraph>` and the :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>` functions.

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :doc:`pgr_analyzeGraph <pgr_analyzeGraph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :doc:`pgr_analyzeGraph <pgr_analyzeGraph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:the_geom: ``geometry`` Point geometry of the vertex.

:Example 1: The simplest way to use pgr_createVerticesTable

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q1
   :end-before: --q1.1


Additional Examples
-------------------------------------------------------------------------------


:Example 2: When the arguments are given in the order described in the parameters:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q2
   :end-before: --q2.1


We get the same result as the simplest way to use the function.

.. warning::  An error would occur when the arguments are not given in the appropriate order: In this example, the column source column ``source`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``the_geom`` is passed to the function as the source column.

   .. literalinclude:: doc-pgr_createVerticesTable.queries
      :start-after: --q2.1
      :end-before: --q2.2


.. rubric:: When using the named notation

:Example 3: The order of the parameters do not matter:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q3.1
   :end-before: --q3.2

:Example 4: Using a different ordering

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q4
   :end-before: --q4.1


:Example 5: Parameters defined with a default value can be omitted, as long as the value matches the default:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q5
   :end-before: --q5.1


.. rubric:: Selecting rows using rows_where parameter

:Example 6: Selecting rows based on the id.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q6
   :end-before: --q6.1


:Example 7: Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q7
   :end-before: --q7.1


:Example 8: Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q8
   :end-before: --q8.1

Usage when the edge table's columns DO NOT MATCH the default values:
...............................................................................

Using the following table

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --tab1
   :end-before: --tab2


.. rubric:: Using positional notation:

:Example 9: The arguments need to be given in the order described in the parameters:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q9
   :end-before: --q9.1

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``src`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``mygeom`` is passed to the function as the source column.

    .. literalinclude:: doc-pgr_createVerticesTable.queries
       :start-after: --q9.1
       :end-before: --q9.2

.. rubric:: When using the named notation

:Example 10: The order of the parameters do not matter:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q10
   :end-before: --q10.1

:Example 11: Using a different ordering

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q11
   :end-before: --q11.1


.. rubric:: Selecting rows using rows_where parameter

:Example 12: Selecting rows based on the gid. (positional notation)

.. literalinclude:: doc-pgr_createVerticesTable.queries
    :start-after: --q12
    :end-before: --q12.1

:Example 13: Selecting rows based on the gid. (named notation)

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q13
   :end-before: --q13.1

:Example 14: Selecting the rows where the geometry is near the geometry of row with ``gid`` = 5.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q14
   :end-before: --q14.1


:Example 15: TBD

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q15
   :end-before: --q15.1

:Example 16: Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q16
   :end-before: --q16.1

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q16.1
   :end-before: --q16.2

:Example 17: TBD

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q17
   :end-before: --q17.1

The example uses the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`  for an overview of a topology for routing algorithms.
* :doc:`pgr_createTopology` <pgr_create_topology>` to create a topology based on the geometry.
* :doc:`pgr_analyzeGraph` to analyze the edges and vertices of the edge table.
* :doc:`pgr_analyzeOneWay` to analyze directionality of the edges.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
