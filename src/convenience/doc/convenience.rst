.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _convenience_functions:

Convenience Functions
===============================================================================

.. warning::  This are proposed function

     - Is not officially in the release.
     - Name could change.
     - Signature could change.
     - Needs testing.
     - Functionality could change.

The following functions are general purpose convenience functions that might
be useful when building a larger application or handling input from say an
Ajax handler.

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.

Distance Matrix Functions
===============================================================================

These function my be helpful when you need to create or manipulate distance matricies, like for TSP or VRP related problems.

 - :ref:`pgr_points_to_dmatrix` - Create a distance matrix from an array of points.
 - :ref:`pgr_vids_to_dmatrix` - Create a distance matrix from an array of ``vertix_id``.
 - :ref:`pgr_vids_to_dmatrix2` - Create a distance matrix from an array of ``vertix_id``.

See also
--------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


.. toctree::
  :hidden:

  pgr_pointToEdgeNode
  pgr_textToPoints
  pgr_pointsToVids

  pgr_pointsToDMatrix
  pgr_vidsToDMatrix
  pgr_vidsToDMatrix2

