.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_is_column_in_table:

pgr_isColumnInTable
===============================================================================

.. index:: 
	single: pgr_isColumnInTable(text,text) -- deprecated
	module: common

Name
-------------------------------------------------------------------------------

``pgr_isColumnInTable`` — Check if a column is in the table.

.. note:: This function is intended for the developer’s aid.

.. warning:: This function is  deprecated in 2.1
             Use `_pgr_isColumnInTable` instead



Synopsis
-------------------------------------------------------------------------------

Returns ``true`` when the column “col” is in table “tab”.

.. code-block:: sql

	boolean pgr_isColumnInTable(text tab, text col);


Description
-------------------------------------------------------------------------------

:tab: ``text`` Table name with or without schema component.
:col: ``text`` Column name to be checked for.

Returns:

  -  ``true`` when the column “col” is in table “tab”.
  -  ``false`` when:

    * The table "tab" is not found or
    * Column “col” is not found in table “tab” 
 
.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_isColumnInTable('edge_table','x1');

     pgr_iscolumnintable 
    ---------------------
     t
    (1 row)

    SELECT pgr_isColumnInTable('public.edge_table','foo');

     pgr_iscolumnintable
    ---------------------
     f
    (1 row)



The example use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_is_column_indexed` to check if the column is indexed.
* :ref:`pgr_get_column_name` to get the name of the column as is stored in the postgres administration tables.
* :ref:`pgr_get_table_name` to get the name of the table as is stored in the postgres administration tables.

