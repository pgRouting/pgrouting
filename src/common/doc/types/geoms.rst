.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_geoms:

pgr_geoms[] - Geometry Result set
===============================================================================

.. index:: 
	single: pgr_geoms[]
	module: types

Name
-------------------------------------------------------------------------------

``pgr_geoms[]`` — A set of rows that consists of records with id, gid and the_geom attributes.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_geoms AS
    (
        id integer,
        gid integer,
        the_geom geometry
    );


See Also
-------------------------------------------------------------------------------

* :ref:`common_types`

