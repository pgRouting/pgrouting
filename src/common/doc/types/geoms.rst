.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_geoms:

Type: geoms[]
===============================================================================

.. index:: 
	single: geoms[]
	module: types

Name
-------------------------------------------------------------------------------

``geoms[]`` — A set of rows that consists of records with id, gid and the_geom attributes.


Description
-------------------------------------------------------------------------------

.. note:: 
	Not completed.

.. code-block:: sql

    CREATE TYPE geoms AS
    (
        id integer,
        gid integer,
        the_geom geometry
    );


See Also
-------------------------------------------------------------------------------

* :ref:`type_path_result`
* :ref:`type_vertex_result`

