.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_get_table_name:

pgr_getTableName
===============================================================================

.. index:: 
	single: pgr_getTableName(text,text) -- deprecated
	module: common

Name
-------------------------------------------------------------------------------

``pgr_getTableName`` — Retrieves the name of the column as is stored in the postgres administration tables.

.. note:: This function is intended for the developer’s aid.

.. warning:: This function is  deprecated in 2.1
             Use `_pgr_getTableName` instead

Synopsis
-------------------------------------------------------------------------------

Returns a record containing the registered names of the table and of the schema it belongs to.

.. code-block:: sql

	(text sname, text tname)  pgr_getTableName(text tab)


Description
-------------------------------------------------------------------------------

Parameters

:tab: ``text`` table name with or without schema component.

Returns 

:sname:

  - ``text`` containing the registered name of the schema of table "tab".

    * when the schema was not provided in "tab" the current schema is used.

  - ``NULL`` when :

    * The schema is not found in the postgres administration tables.

:tname:

  - ``text`` containing the registered name of the table "tab".
  - ``NULL`` when :

    * The schema is not found in the postgres administration tables.
    * The table "tab" is not registered under the schema ``sname`` in the postgres administration tables


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT * from pgr_getTableName('edge_table');
    
     sname  |   tname    
    --------+------------
     public | edge_table
    (1 row)

    SELECT * from pgr_getTableName('EdgeTable');

     sname  |   tname    
    --------+------------
     public | 
    (1 row)

    SELECT * from pgr_getTableName('data.Edge_Table');
     sname | tname 
    -------+-------
           | 
    (1 row)


The examples use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_is_column_in_table` to check only for the existance of the column.
* :ref:`pgr_get_table_name` to retrieve the name of the table as is stored in the postgres administration tables.

