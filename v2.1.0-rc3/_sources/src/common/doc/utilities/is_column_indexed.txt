.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_is_column_indexed:

pgr_isColumnIndexed
===============================================================================

.. index:: 
	single: pgr_isColumnIndexed(text,text) -- deprecated
	module: common

Name
-------------------------------------------------------------------------------

``pgr_isColumnIndexed`` — Check if a column in a table is indexed.

.. note:: This function is intended for the developer’s aid.

.. warning:: This function is  deprecated in 2.1
             Use `_pgr_isColumnIndexed` instead


Synopsis
-------------------------------------------------------------------------------

Returns ``true`` when the column “col” in table “tab” is indexed.

.. code-block:: sql

	boolean pgr_isColumnIndexed(text tab, text col);


Description
-------------------------------------------------------------------------------

:tab: ``text`` Table name with or without schema component.
:col: ``text`` Column name to be checked for.

Returns:

  -  ``true`` when the column “col” in table “tab” is indexed.
  -  ``false`` when:

    * The table "tab" is not found or
    * Column “col” is not found in table “tab” or
    * Column "col" in table "tab" is not indexed
 
.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_isColumnIndexed('edge_table','x1');

     pgr_iscolumnindexed 
    ---------------------
     f
    (1 row)

    SELECT pgr_isColumnIndexed('public.edge_table','cost');

     pgr_iscolumnindexed 
    ---------------------
     f
    (1 row)



The example use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_is_column_in_table` to check only for the existance of the column in the table.
* :ref:`pgr_get_column_name` to get the name of the column as is stored in the postgres administration tables.
* :ref:`pgr_get_table_name` to get the name of the table as is stored in the postgres administration tables.

