.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_quote_ident:

pgr_quote_ident - Quote table name with Schema Component
===============================================================================

.. index:: 
	single: pgr_quote_ident(text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_quote_ident`` — Quote table name with or without schema component.


Synopsis
-------------------------------------------------------------------------------

Function to split a string on ``.`` characters and then quote the components as postgres identifiers and then join them back together with ``.`` characters. 
Multile ``.`` will get collapsed into a single ``.``, so ``schema...table`` till get returned as ``schema."table"`` and ``Schema.table`` becomes ``"Schema"."table"``.

.. code-block:: sql

	text pgr_quote_ident(text tab);


Description
-------------------------------------------------------------------------------

:tab: ``text`` table name with or without schema component

Returns table name with or without schema as ``text``.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT pgr_quote_ident('public.edge_table');

	  pgr_quote_ident  
	-------------------
	 public.edge_table
	(1 row)


	SELECT pgr_quote_ident('edge_table');
	 pgr_quote_ident 
	-----------------
	 edge_table
	(1 row)


	SELECT pgr_quote_ident('Public...edge_table');
	   pgr_quote_ident   
	---------------------
	 "Public".edge_table
	(1 row)


See Also
-------------------------------------------------------------------------------

* [TBD]
