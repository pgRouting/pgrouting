..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _common-node_network:

=========================
pgRouting Node Network
=========================
:Author: Nicolas Ribot
:Date: $Date: 2013-05-10 20:14:00 -5000 (Fri, 10 May 2013) $
:Revision: $Revision: 0000 $
:Description: This function was developed to node a network of edges for use with pgRouting.
:Copyright: Nicolas Ribot, This is released under the MIT-X license.

Name
------------------------------------------------------------------------------

``pgr_nodeNetwork`` - Nodes an network edge table.

Synopsis
-------------------------------------------------------------------------------

A common problem associated with bring GIS data into pgRouting is the fact
that the data is often not "noded" correctly. This will create invalid
topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network
all the edges will be broken at the intersection. There are cases like an 
over-pass and under-pass intersection where you can not traverse from the
over-pass to the under-pass, but this function does not have the ability to 
detect and accomodate those situations.

This function reads the intab table, that has a primary key column n_pkey
and geometry column named n_geom and intersect all the segments in it against
all the other segments can creates a table outtab. It uses the tolerance
for deciding that multiple node within the toerlance are considered the same
node. The outab table has a structure like:

.. code-block:: sql

    CREATE TABLE noded
    (
      id integer,
      sub_id bigint,
      geom geometry
    );

where id is a reference to the record in intab. sub_id is a seqence number
that identifies all the new segments generated from the original record.

Synopsis
-------------------------------------------------------------------------------

.. code-block:: sql

    text pgr_nodeNetwork(intab text, n_pkey text, n_geom text,  outtab text, tol double precision)

Description
-------------------------------------------------------------------------------

:intab: ``text`` name of the input table to be noded
:n_pkey: ``text`` name of the primary key column for the table
:n_geom: ``text`` name of the geometry column that should be noded
:outtab: ``text`` name of the output table that will be created
:tol: ``float8`` tolerance for coinicident points

Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_nodeNetwork('myedges', 'id', 'the_geom', 'nodededges', 0.000001);


