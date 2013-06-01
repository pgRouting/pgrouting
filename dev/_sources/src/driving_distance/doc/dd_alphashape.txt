.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_alphashape:

pgr_alphashape - Alpha shape computation
===============================================================================

.. index:: 
	single: pgr_alphashape(text)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_alphashape`` — Core function for alpha shape computation.

.. note:: 

	This function should not be used directly. Use :ref:`pgr_drivingDistance <pgr_driving_distance>` instead. 


Synopsis
-------------------------------------------------------------------------------

Returns a table with (x, y) rows that describe the vertices of an alpha shape.

.. code-block:: sql

	table() pgr_alphashape(text sql);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_table

    :id: ``int4`` identifier of the vertex
    :x: ``float8`` x-coordinate
    :y: ``float8`` y-coordinate


Returns a vertex record for each row :

:x: x-coordinate
:y: y-coordinate


.. rubric:: History

* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT * FROM pgr_alphashape('SELECT id, x, y FROM vertex_table');

     x | y 
    ---+---
     2 | 0
     4 | 1
     4 | 2
     4 | 3
     2 | 4
     0 | 2
    (6 rows)


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance`
* :ref:`pgr_points_as_polygon`
