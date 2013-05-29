.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_find_nearest_link_dwithin:

pgr_findNearestLinkDWithin - Find nearest link within distance
===============================================================================

.. index:: 
	single: pgr_findNearestLinkDWithin(varchar,double precision,varchar)
	module: matching

Name
-------------------------------------------------------------------------------

``pgr_findNearestLinkDWithin`` — Finds the nearest link to a given node.


Synopsis
-------------------------------------------------------------------------------

The function finds and returns the nearest link to a given node.

.. code-block:: sql

  integer pgr_findNearestLinkDWithin(varchar point, double precision distance, varchar geom_table);


Description
-------------------------------------------------------------------------------

:point: ``varchar`` text representation of the point
:distance: ``float8`` function will search for a link within this distance
:geom_table: ``varchar`` network table name (may contain the schema name as well)


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

  [TBD]


See Also
-------------------------------------------------------------------------------

* [TBD]

