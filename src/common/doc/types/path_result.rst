.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_path_result:

Type: path_result[]
===============================================================================

.. index:: 
	single: path_result[]
	module: types

Name
-------------------------------------------------------------------------------

``path_result[]`` — A set of rows that consists of records with vertex_id, edge_id and cost attributes.


Description
-------------------------------------------------------------------------------

The function returns a set of rows. There is one row for each crossed edge, and an additional one containing the terminal vertex. The columns of each row are:

.. code-block:: sql

    CREATE TYPE path_result AS
    (
        vertex_id integer,
        edge_id integer,
        cost float8
    );

:vertex_id: the identifier of source vertex of each edge. There is one more row after the last edge, which contains the vertex identifier of the target path.
:edge_id: the identifier of the edge crossed
:cost: The cost associated to the current edge. It is 0 for the row after the last edge. Thus, the path total cost can be computated using a sum of all rows in the cost column.


See Also
-------------------------------------------------------------------------------

* :ref:`type_geoms`
* :ref:`type_vertex_result`
