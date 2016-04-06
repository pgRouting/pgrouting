.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_get_column_name:

pgr_getColumnName
===============================================================================

.. index:: 
	single: pgr_getColumnName(text,text) -- deprecated

Name
-------------------------------------------------------------------------------

``pgr_getColumnName`` — Retrieves the name of the column as is stored in the postgres administration tables.

.. note:: This function is intended for the developer’s aid.


.. warning:: This function is deprecated in 2.1.
             Use `_pgr_getColumnName` instead

Synopsis
-------------------------------------------------------------------------------

Returns a ``text`` contining the registered name of the column.

.. code-block:: sql

	text pgr_getColumnName(tab text, col text);


Description
-------------------------------------------------------------------------------

Parameters

:tab: ``text`` table name with or without schema component.
:col: ``text`` column name to be retrived.

Returns 

  - ``text`` containing the registered name of the column.
  - ``NULL`` when :

    * The table “tab” is not found or
    * Column “col” is not found in table “tab” in the postgres administration tables.

.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_getColumnName('edge_table','the_geom');
    
     pgr_iscolumnintable 
    ---------------------
     the_geom
    (1 row)

    SELECT pgr_getColumnName('edge_table','The_Geom');
    
     pgr_iscolumnintable 
    ---------------------
     the_geom
    (1 row)

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_is_column_in_table` to check only for the existance of the column.
* :ref:`pgr_get_table_name` to retrieve the name of the table as is stored in the postgres administration tables.

