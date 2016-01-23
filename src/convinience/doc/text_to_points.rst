.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_text_to_points:

pgr_textToPoints
==============================================================================

.. index::
        single: pgr_textToPoints(pnts text, srid integer DEFAULT(4326)) --proposed


Name
------------------------------------------------------------------------------

``pgr_textToPoints`` - Converts a text string of the format "x,y;x,y;x,y;..." into and array of point geometries.

.. warning::  This is a proposed function

     - Is not officially in the release.
     - Name could change.
     - Signature could change.
     - Needs testing.
     - Functionality could change.

Synopsis
------------------------------------------------------------------------------

Given a text string of the format "x,y;x,y;x,y;..." and the srid to use, split the string and create and array point geometries. 

The function returns:

  - 

.. code-block:: sql

        integer pgr_textToPoints(pnts text, srid integer DEFAULT(4326))


Description
-----------------------------------------------------------------------------

.. rubric:: Paramters

:pnts: ``text`` A text string of the format "x,y;x,y;x,y;..." where x is longitude and y is latitude if use values in lat-lon.
:srid: ``integer`` The SRID to use when constructing the point geometry. If the paratmeter is absent it defaults to ``SRID:4326``.

.. rubric:: History

* Proposed in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. literalinclude:: doc-conversion.queries
   :start-after: --q2
   :end-before: --q3


Examples
-----------------------------------------------------------------------------

.. code-block:: sql

        select st_astext(g) from (
            select unnest(
                pgr_texttopoints('0,0;1,1;1,0;0,1;1,4;1,5;0,4;0,5', 0)
            ) as g
        ) as foo;
          st_astext
         ------------
          POINT(0 0)
          POINT(1 1)
          POINT(1 0)
          POINT(0 1)
          POINT(1 4)
          POINT(1 5)
          POINT(0 4)
          POINT(0 5)
         (8 rows)
                  


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``node_id`` based on closest edge.
* :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
