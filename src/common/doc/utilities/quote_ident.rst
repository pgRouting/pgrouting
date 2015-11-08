.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_quote_ident:

pgr_quote_ident 
===============================================================================

.. index:: 
	single: pgr_quote_ident(text) -- deprecated

Name
-------------------------------------------------------------------------------

``pgr_quote_ident`` — Quotes the input text to be used as an identifier in an SQL statement string.

.. note:: This function is intended for the developer's aid. 

.. warning:: This function is deprecated in 2.1
             Use `_pgr_quote_ident` instead


Synopsis
-------------------------------------------------------------------------------

Returns the given identifier ``idname`` suitably quoted to be used as an identifier in an SQL statement string. 

.. code-block:: sql

	text pgr_quote_ident(text idname);


Description
-------------------------------------------------------------------------------

.. rubric:: Parameters

:idname: ``text`` Name of an SQL identifier. Can include ``.`` dot notation for schemas.table identifiers

Returns the given string suitably quoted to be used as an identifier in an SQL statement string. 

  - When the identifier ``idname`` contains on or more ``.`` separators, each component is suitably quoted to be used in an SQL string.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

Everything is lower case so nothing needs to be quoted.

.. code-block:: sql

	SELECT pgr_quote_ident('the_geom');

	pgr_quote_ident  
	---------------
           the_geom
	(1 row)


	SELECT pgr_quote_ident('public.edge_table');

	  pgr_quote_ident  
	-------------------
	 public.edge_table
	(1 row)

The column is upper case so its double quoted.

.. code-block:: sql

	SELECT pgr_quote_ident('edge_table.MYGEOM');

	   pgr_quote_ident 
	-------------------
	 edge_table."MYGEOM"
	(1 row)

	SELECT pgr_quote_ident('public.edge_table.MYGEOM');

	       pgr_quote_ident 
	---------------------------
	 public.edge_table."MYGEOM"
	(1 row)


The schema name has a capital letter so its double quoted.

.. code-block:: sql

	SELECT pgr_quote_ident('Myschema.edge_table');

	    pgr_quote_ident   
	----------------------
	 "Myschema".edge_table
	(1 row)

Ignores extra ``.`` separators.

.. code-block:: sql

	SELECT pgr_quote_ident('Myschema...edge_table');

	   pgr_quote_ident   
	---------------------
	 "Myschema".edge_table
	(1 row)

See Also
-------------------------------------------------------------------------------

* :ref:`developer` for the tree layout of the project.
* :ref:`pgr_get_table_name` to get the name of the table as is stored in the postgres administration tables.

