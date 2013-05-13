.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_apsp_edge:

pgr_apspEdge[] - APSP Edge Result set
===============================================================================

.. index:: 
	single: pgr_apspEdge[]
	module: types

Name
-------------------------------------------------------------------------------

``pgr_apspEdge[]`` — A set of rows that consists of records with source id, target id and cost attributes.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_apspEdge AS
    (
        source_id integer, 
        target_id integer, 
        cost float8
    );


See Also
-------------------------------------------------------------------------------

* :ref:`common_types`

