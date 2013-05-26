.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_is_column_indexed:

pgr_isColumnIndexed - Check if column is indexed
===============================================================================

.. index:: 
	single: pgr_isColumnIndexed(text,text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_isColumnIndexed`` — Check if a column in a table is indexed.


Synopsis
-------------------------------------------------------------------------------

Returns ``true`` or ``false`` if column “col” in table “tab” is indexed.

.. code-block:: sql

	boolean pgr_isColumnIndexed(tab text, col text);


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
