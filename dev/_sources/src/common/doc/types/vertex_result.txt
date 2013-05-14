.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_vertex_result:

pgr_vertexResult[] - Vertex Result set
===============================================================================

.. index:: 
	single: pgr_vertexResult[]
	module: types

Name
-------------------------------------------------------------------------------

``pgr_vertexResult[]`` — A set of rows that consists of records with x and y attributes.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_vertexResult AS
    (
        x float8,
        y float8
    );


See Also
-------------------------------------------------------------------------------

* :ref:`common_types`
