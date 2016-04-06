.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _developer_functions:

Developers's Functions
===============================================================================

Functions to help you develop a wrapper or a recipe 

   -  :ref:`pgr_get_column_name` - to get the name of the column as is stored in the postgres administration tables.
   -  :ref:`pgr_get_table_name` - to retrieve the name of the table as is stored in the postgres administration tables.
   -  :ref:`pgr_is_column_indexed` - to check if the column is indexed. 
   -  :ref:`pgr_is_column_in_table` - to check only for the existance of the column.
   -  :ref:`pgr_point_to_id` -to insert/get the id of the inserted point in a vertices table.
   -  :ref:`pgr_quote_ident` - to quotes the input text to be used as an identifier in an SQL statement string.
   -  :ref:`pgr_versionless` - to compare two version numbers.
   -  :ref:`pgr_start_point` - to get the start point of a (multi)linestring.
   -  :ref:`pgr_end_point` - to get the end point of a (multi)linestring.

.. toctree::
   :hidden:

	pgr_getColumnName - to get the name of the column as is stored in the postgres administration tables. <get_column_name> 
	pgr_getTableName - to retrieve the name of the table as is stored in the postgres administration tables.<get_table_name>
	pgr_isColumnIndexed - to check if the column is indexed. <is_column_indexed>
	pgr_isColumnInTable - to check only for the existance of the column.<is_column_in_table> 
	pgr_pointToId -to insert/get the id of the inserted point in a vertices table.<point_to_id>
	pgr_quote_ident - to quotes the input text to be used as an identifier in an SQL statement string.<quote_ident>
	pgr_versionless - to compare two version numbers<versionless>
	pgr_startPoint - to get the start point of a (multi)linestring<start_point>
	pgr_endPoint - to get the end point of a (multi)linestring<end_point>



