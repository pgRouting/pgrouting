.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_vids_to_dmatrix2:

pgr_vidsToDMatrix
==============================================================================

.. index::
        single: pgr_vidsToDMatrix(IN sql text, IN vids integer[], IN directed boolean, IN has_reverse_cost boolean, IN want_symmetric boolean, OUT dmatrix double precision[]) --proposed


Name
------------------------------------------------------------------------------

``pgr_vidsToDMatrix`` - Creates a distances matrix from an array of ``vertex_id``.

.. warning::  This is a proposed function

     - Is not officially in the release.
     - Name could change.
     - Signature could change.
     - Needs testing.
     - Functionality could change.


Synopsis
------------------------------------------------------------------------------

This function takes an array of ``vertex_id``, a ``sql`` statement to select the edges, and some boolean arguments to control the behavior. It then computes kdijkstra() distances for each vertex to all the other vertices and creates a distance matrix suitable for TSP.

The function returns:

    * :dmatrix: ``float8[]`` - the distance matrix suitable to pass to pgr_TSP() function.

.. code-block:: sql

        pgr_vidsToDMatrix(IN sql text, IN vids integer[], IN directed boolean, IN has_reverse_cost boolean, IN want_symmetric boolean, OUT dmatrix double precision[])



Description
-----------------------------------------------------------------------------

.. rubric:: Paramters

:sql: ``text`` - A SQL statement to select the edges needed for the solution.
:vids: ``integer[]`` - An array of ``vertex_id``.
:directed: ``boolean`` - A flag to indicate if the graph is directed.
:has_reverse_cost: ``boolean`` - A flag to indicate if the SQL has a column ``reverse_cost``.
:want_symmetric: ``boolean`` - A flag to indicate if you want a symmetric or asymmetric matrix. You will need a symmetric matrix for pgr_TSP(). If the matriix is asymmetric, the then the cell(i,j) and cell(j,i) will be set to the average of those two cells except if one or the other are -1.0 then it will take the value of the other cell. If both are negative they will be left alone.

.. warning::

    * kdijkstra() can fail to find a path between some of the vertex ids. We to not detect this other than the cost might get set to -1.0, so the dmatrix shoule be checked for this as it makes it invalid for TSP



.. rubric:: History

* Proposed in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. literalinclude:: doc-matrix.queries
   :start-after: --q2
   :end-before: --q3


This example shows how this can be used in the context of feeding the results into pgr_tsp() function. We convert a text string of ``x,y;x,y;...`` into and array of points, then convert that into an array ``vertex_id``, then create a distance matrix that gets feed into ``pgr_tsp()`` that returns the final result.

.. code-block:: sql

    select * from pgr_tsp(
        (select dmatrix::float8[]
           from pgr_vidstodmatrix(
                    'select id, source, target, cost, reverse_cost from edge_table',
                    pgr_pointstovids(
                        pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                        'edge_table'),
                    true, true, true) as dmatrix
        ),
        1
    );
     seq | id
    -----+----
       0 |  1
       1 |  0
       2 |  6
       3 |  3
       4 |  7
       5 |  5
       6 |  4
       7 |  2
    (8 rows)

This example uses the :ref:`sampledata` network.


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_vids_to_dmatrix` -  - Creates a distances matrix from an array of ``vertex_id``.
* :ref:`pgr_text_to_points` - Create an array of points from a text string.
* :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

