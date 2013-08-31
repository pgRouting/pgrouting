.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _performance:

Performance Tips
===============================================================================


When "you know" that you are going to remove a set of edges from the edges table, and without those edges you are going to use a routing function you can do the following:

Analize the new topology based on the actual topology:
	
.. code-block:: sql

	pgr_analyzegraph('edge_table',rows_where:='id < 17');

Or create a new topology if the change is permanent:
	
.. code-block:: sql

	pgr_createTopology('edge_table',rows_where:='id < 17');
	pgr_analyzegraph('edge_table',rows_where:='id < 17');

Use an SQL that "removes" the edges in the routing function
	
.. code-block:: sql

	SELECT id, source, target from edge_table WHERE id < 17 

When "you know" that the route will not go out of a particular area, to speed up the process you can use a more complex SQL query like
	
.. code-block:: sql

	SELECT id, source, target from edge_table WHERE
        	id < 17 and
        	the_geom  && (select st_buffer(the_geom,1) as myarea FROM  edge_table where id=5)


Note that the  same condition ``id < 17`` is used in all cases.



