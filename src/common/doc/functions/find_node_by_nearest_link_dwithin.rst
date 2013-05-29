.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_find_node_by_nearest_link_dwithin:

pgr_findNodeByNearestLinkDWithin - Find node by nearest link
===============================================================================

.. index:: 
  single: pgr_findNodeByNearestLinkDWithin(varchar,double precision,varchar)
  module: matching

Name
-------------------------------------------------------------------------------

``pgr_findNodeByNearestLinkDWithin`` — Finds a node by nearest link within distance.

Synopsis
-------------------------------------------------------------------------------

The function finds and returns the node searched by the nearest link to a given node.

.. code-block:: sql

  record pgr_findNodeByNearestLinkDWithin(varchar point, double precision distance, varchar geom_table);


Description
-------------------------------------------------------------------------------

:point: ``varchar`` text representation of the point
:distance: ``float8`` function will search for a node within this distance
:geom_table: ``varchar`` network table name (may contain the schema name as well)

Returns a record with

:id: ``integer`` node ID 
:name: ``varchar`` indicates if the nearest node is "source" or "target"


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

  [TBD]


See Also
-------------------------------------------------------------------------------

* [TBD]

