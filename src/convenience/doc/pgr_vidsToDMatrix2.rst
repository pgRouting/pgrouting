.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_vids_to_dmatrix2:

pgr_vidsToDMatrix - Deprecated Function
==============================================================================

.. warning:: This function is deprecated!!!

    * Is no longer supported.
    * May be removed from future versions.
    * There is no replacement.

Name
------------------------------------------------------------------------------

``pgr_vidsToDMatrix`` - Creates a distances matrix from an array of ``vertex_id``.

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
   :start-after: --q3
   :end-before: --q4


This example shows how this can be used in the context of feeding the results into pgr_tsp() function.

.. literalinclude:: doc-matrix.queries
   :start-after: --q4
   :end-before: --q5


This example uses the :ref:`sampledata` network.


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_vids_to_dmatrix`
* :ref:`pgr_text_to_points`
* :ref:`pgr_tsp-V2.0`


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
