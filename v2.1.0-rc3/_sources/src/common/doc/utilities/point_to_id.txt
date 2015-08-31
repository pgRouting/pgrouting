.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_point_to_id:

pgr_pointToId
===============================================================================

.. index:: 
	single: pgr_pointToId(geometry,double precision,text,integer) -- deprecated

Name
-------------------------------------------------------------------------------

``pgr_pointToId`` — Inserts a point into a vertices table and returns the corresponig id.

.. note:: This function is intended for the developer's aid.

 Use :ref:`pgr_createTopology <pgr_create_topology>` or :ref:`pgr_createVerticesTable <pgr_create_vert_table>` instead. 

.. warning:: This function is  deprecated in 2.1



Synopsis
-------------------------------------------------------------------------------

This function returns the ``id`` of the row in the vertices table that corresponds to the ``point`` geometry 

.. code-block:: sql

	bigint pgr_pointToId(geometry point, double precision tolerance,text vertname text,integer srid)


Description
-------------------------------------------------------------------------------

:point: ``geometry`` "POINT" geometry to be inserted.
:tolerance: ``float8`` Snapping tolerance of disconnected edges. (in projection unit)
:vertname: ``text`` Vertices table name WITH schema included.
:srid: ``integer`` SRID of the geometry point.

This function returns the id of the row that corresponds to the ``point`` geometry 

  - When the ``point`` geometry already exists in the vertices table ``vertname``, it returns the corresponing ``id``.
  - When the ``point`` geometry is not found in the vertices table ``vertname``, the funcion inserts the ``point`` and returns the corresponing ``id`` of the newly created vertex.
 
.. warning:: The function do not perform any checking of the parameters. Any validation has to be done before calling this function.

.. rubric:: History

* Renamed in version 2.0.0


See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_createVerticesTable <pgr_create_vert_table>` to create a topology based on the geometry.
* :ref:`pgr_createTopology <pgr_create_topology>` to create a topology based on the geometry.


