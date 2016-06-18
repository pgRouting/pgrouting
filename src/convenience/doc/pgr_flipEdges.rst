.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_flip_edges:

pgr_flipEdges - Deprecated Function
==============================================================================


Name
------------------------------------------------------------------------------

``pgr_flipEdges`` - 

.. warning::  This is a deprecated function

     - Is not officially in the release.
     - No longer mantained
     - No testing done.
     - Functionality could change.

Synopsis
------------------------------------------------------------------------------

The function returns:

  - ``geometry[]`` An array of the input geometries with the geometries flipped end to end such that the geometries are oriented as a path from start to end.

.. code-block:: sql

        geometry[] pgr_flipEdges(ga geometry[])


Description
-----------------------------------------------------------------------------

Given an array of linestrings that are supposedly connected end to end like the results of a route, check the edges and flip any end for end if they do not connect with the previous seegment and return the array with the segments flipped as appropriate.

.. rubric:: Parameters

:ga: ``geometry[]`` An array of geometries, like the results of a routing query.

.. warning::

    * No checking is done for edges that do not connect.
    * Input geometries MUST be LINESTRING or MULTILINESTRING.
    * Only the first LINESTRING of a MULTILINESTRING is considered.

.. rubric:: History

* Deprecated in version 2.3.0
* Proposed in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. literalinclude:: doc-conversion.queries
   :start-after: --q2
   :end-before: --q3

See also
--------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

