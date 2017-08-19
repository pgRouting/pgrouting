..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_geom_result:

pgr_geomResult[]
===============================================================================

.. index::
	single: pgr_geomResult[]

Name
-------------------------------------------------------------------------------

``pgr_geomResult[]`` — A set of records to describe a path result with geometry attribute.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_geomResult AS
    (
        seq integer,
        id1 integer,
        id2 integer,
        geom geometry
    );

:seq:   sequential ID indicating the path order
:id1:   generic name, to be specified by the function
:id2:   generic name, to be specified by the function
:geom:  geometry attribute


.. rubric:: History

* New in version 2.0.0
* Replaces ``geoms``


See Also
-------------------------------------------------------------------------------

* :ref:`introduction`


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
