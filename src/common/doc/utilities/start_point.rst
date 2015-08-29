.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_start_point:

pgr_startPoint 
===============================================================================

.. index:: 
	single: pgr_startPoint(geometry) -- deprecated

Name
-------------------------------------------------------------------------------

``pgr_startPoint`` — Returns a start point of a (multi)linestring geometry.

.. note:: This function is intended for the developer's aid. 

.. warning:: This function is deprecated in 2.1
             Use `_pgr_startPoint` instead

Synopsis
-------------------------------------------------------------------------------

Returns the geometry of the start point of the first LINESTRING of ``geom``. 

.. code-block:: sql

	geometry pgr_startPoint(geometry geom);


Description
-------------------------------------------------------------------------------

.. rubric:: Parameters

:geom: ``geometry`` Geometry of a MULTILINESTRING or LINESTRING.

Returns the geometry of the start point of the first LINESTRING of ``geom``. 



.. rubric:: History

* New in version 2.0.0



See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_end_Point` to get the end point of a (multi)linestring.

