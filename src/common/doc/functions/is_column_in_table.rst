.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_is_column_in_table:

pgr_isColumnInTable - Check if column exists
===============================================================================

.. index:: 
	single: pgr_isColumnInTable(text,text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_isColumnInTable`` — Check if a column exists in a table.


Synopsis
-------------------------------------------------------------------------------

Returns ``true`` or ``false`` if column “col” exists in table “tab”.

.. code-block:: sql

	boolean pgr_isColumnInTable(tab text, col text);


Description
-------------------------------------------------------------------------------

:tab: ``text`` table name with or without schema component
:col: ``text`` column name to be checked for


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	[TBD]


See Also
-------------------------------------------------------------------------------

* [TBD]
