..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_analyzeOneWay
===============================================================================

``pgr_analyzeOneWay`` — Analyzes oneway Sstreets and identifies flipped segments.

This function analyzes oneway streets in a graph and identifies any flipped
segments.

.. rubric:: Availability

* Version 2.0.0

  * **Official** function


Description
-------------------------------------------------------------------------------

The analyses of one way segments is pretty simple but can be a powerful tools to
identifying some the potential problems created by setting the direction of a
segment the wrong way. A node is a `source` if it has edges the exit from that
node and no edges enter that node. Conversely, a node is a `sink` if all edges
enter the node but none exit that node. For a `source` type node it is logically
impossible to exist because no vehicle can exit the node if no vehicle and enter
the node. Likewise, if you had a `sink` node you would have an infinite number
of vehicle piling up on this node because you can enter it but not leave it.

So why do we care if the are not feasible? Well if the direction of an edge was
reversed by mistake we could generate exactly these conditions. Think about a
divided highway and on the north bound lane one segment got entered wrong or
maybe a sequence of multiple segments got entered wrong or maybe this happened
on a round-about. The result would be potentially a `source` and/or a `sink`
node.

So by counting the number of edges entering and exiting each node we can
identify both `source` and `sink` nodes so that you can look at those areas of
your network to make repairs and/or report the problem back to your data vendor.

.. rubric:: Prerequisites

The edge table to be analyzed must contain a source column and a target column
filled with id's of the vertices of the segments and the corresponding vertices
table <edge_table>_vertices_pgr that stores the vertices information.

- Use :doc:`pgr_createVerticesTable` to create the vertices table.
- Use :doc:`pgr_createTopology` to create the topology and the vertices table.

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_analyzeOneWay(**geom_table**, **s_in_rules**, **s_out_rules**, **t_in_rules**, **t_out_rules**, [**options**])
   | **options:** ``[oneway, source, target, two_way_if_null]``

   | RETURNS ``TEXT``

Parameters
-------------------------------------------------------------------------------

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:s_in_rules: ``text[]`` source node **in** rules
:s_out_rules: ``text[]`` source node **out** rules
:t_in_rules: ``text[]`` target node **in** rules
:t_out_rules: ``text[]`` target node **out** rules
:oneway: ``text`` oneway column name name of the network table. Default value is
         ``oneway``.
:source: ``text`` Source column name of the network table. Default value is
         ``source``.
:target: ``text`` Target column name of the network table. Default value is
         ``target``.
:two_way_if_null: ``boolean`` flag to treat oneway NULL values as
                  bi-directional. Default value is ``true``.

.. note::
   It is strongly recommended to use the named notation. See
   :doc:`pgr_createVerticesTable` or :doc:`pgr_createTopology` for examples.

The function returns:

- ``OK`` after the analysis has finished.

  * Uses the vertices table: <edge_table>_vertices_pgr.
  * Fills completely the ``ein`` and ``eout`` columns of the vertices table.

- ``FAIL`` when the analysis was not completed due to an error.

  * The vertices table is not found.
  * A required column of the Network table is not found or is not of the
    appropriate type.
  * The names of source , target or oneway are the same.

The rules are defined as an array of text strings that if match the ``oneway``
value would be counted as ``true`` for the source or target **in** or **out**
condition.

.. rubric:: The Vertices Table

The vertices table can be created with :doc:`pgr_createVerticesTable` or
:doc:`pgr_createTopology`

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this
      vertex. See :doc:`pgr_analyzeGgraph <pgr_analyzeGraph>`.
:chk: ``integer`` Indicator that the vertex might have a problem. See
      :doc:`pgr_analyzeGraph <pgr_analyzeGraph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this
      vertex as incoming.
:eout: ``integer`` Number of vertices in the edge_table that reference this
       vertex as outgoing.
:the_geom: ``geometry`` Point geometry of the vertex.

Additional Examples
-------------------------------------------------------------------------------

.. literalinclude:: analyzeOneWay.queries
   :start-after: --q1
   :end-before: --q1.1


See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`
* :doc:`pgr_analyzeGraph`
* :doc:`pgr_createVerticesTable`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
