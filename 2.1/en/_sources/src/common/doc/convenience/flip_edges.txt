..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_flip_edges:

pgr_flipEdges
==============================================================================

.. index::
        single: pgr_flipEdges(ga geometry[])
        module: common


Name
------------------------------------------------------------------------------

``pgr_flipEdges`` - 


Synopsis
------------------------------------------------------------------------------

The function returns:

  - ``geometry[]`` An array of the input geometries with the geometries flipped end to end such that the geometries are oriented as a path from start to end.

.. code-block:: sql

        geometry[] pgr_flipEdges(ga geometry[])


Description
-----------------------------------------------------------------------------

Given an array of linestrings that are supposedly connected end to end like the results of a route, check the edges and flip any end for end if they do not connect with the previous seegment and return the array with the segments flipped as appropriate.

.. rubric:: Parameters

:ga: ``geometry[]`` An array of geometries, like the results of a routing query.

.. warning::

    * No checking is done for edges that do not connect.
    * Input geometries MUST be LINESTRING or MULTILINESTRING.
    * Only the first LINESTRING of a MULTILINESTRING is considered.

.. rubric:: History

* New in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. code-block:: sql

        select st_astext(e) from (select unnest(pgr_flipedges(ARRAY[
        'LINESTRING(2 1,2 2)'::geometry,
        'LINESTRING(2 2,2 3)'::geometry,
        'LINESTRING(2 2,2 3)'::geometry,
        'LINESTRING(2 2,3 2)'::geometry,
        'LINESTRING(3 2,4 2)'::geometry,
        'LINESTRING(4 1,4 2)'::geometry,
        'LINESTRING(3 1,4 1)'::geometry,
        'LINESTRING(2 1,3 1)'::geometry,
        'LINESTRING(2 0,2 1)'::geometry,
        'LINESTRING(2 0,2 1)'::geometry]::geometry[])) as e) as foo;
              st_astext
        ---------------------
         LINESTRING(2 1,2 2)
         LINESTRING(2 2,2 3)
         LINESTRING(2 3,2 2)
         LINESTRING(2 2,3 2)
         LINESTRING(3 2,4 2)
         LINESTRING(4 2,4 1)
         LINESTRING(4 1,3 1)
         LINESTRING(3 1,2 1)
         LINESTRING(2 1,2 0)
         LINESTRING(2 0,2 1)
        (10 rows)


See Also
-----------------------------------------------------------------------------

