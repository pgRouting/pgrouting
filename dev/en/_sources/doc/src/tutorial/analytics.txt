.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _analytics:

Graph Analytics
===============================================================================

:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Copyright: Stephen Woodbridge. The source code is released under the MIT-X license.


Overview
-------------------------------------------------------------------------------

It is common to find problems with graphs that have not been constructed fully noded or in graphs with z-levels at intersection that have been entered incorrectly. An other problem is one way streets that have been entered in the wrong direction. We can not detect errors with respect to "ground" truth, but we can look for inconsistencies and some anomalies in a graph and report them for additional inspections.

We do not current have any visualization tools for these problems, but I have used mapserver to render the graph and highlight potential problem areas. Someone familiar with graphviz might contribute tools for generating images with that.


Analyze a Graph
-------------------------------------------------------------------------------

With :ref:`pgr_analyze_graph` the graph can be checked for errors. For example for table "mytab" that has "mytab_vertices_pgr" as the vertices table:

.. code-block:: sql

    SELECT pgr_analyzeGraph('mytab', 0.000002);
    NOTICE:  Performing checks, pelase wait...
    NOTICE:  Analyzing for dead ends. Please wait...
    NOTICE:  Analyzing for gaps. Please wait...
    NOTICE:  Analyzing for isolated edges. Please wait...
    NOTICE:  Analyzing for ring geometries. Please wait...
    NOTICE:  Analyzing for intersections. Please wait...
    NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
    NOTICE:                    Isolated segments: 158
    NOTICE:                            Dead ends: 20028
    NOTICE:  Potential gaps found near dead ends: 527
    NOTICE:               Intersections detected: 2560
    NOTICE:                      Ring geometries: 0
    pgr_analyzeGraph 
    ----------
       OK
    (1 row)


In the vertices table "mytab_vertices_pgr":

  - Deadends are indentified by ``cnt=1`` 
  - Potencial gap problems are identified with ``chk=1``.

.. code-block:: sql

    SELECT count(*) as deadends  FROM mytab_vertices_pgr WHERE cnt = 1;
    deadends 
    ----------
        20028
     (1 row)

    SELECT count(*) as gaps  FROM mytab_vertices_pgr WHERE chk = 1;
     gaps 
     -----
       527
     (1 row)
     


For isolated road segments, for example, a segment where both ends are deadends. you can find these with the following query:

.. code-block:: sql

    SELECT *
        FROM mytab a, mytab_vertices_pgr b, mytab_vertices_pgr c
        WHERE a.source=b.id AND b.cnt=1 AND a.target=c.id AND c.cnt=1;


If you want to visualize these on a graphic image, then you can use something like mapserver to render the edges and the vertices and style based on ``cnt`` or if they are isolated, etc. You can also do this with a tool like graphviz, or geoserver or other similar tools.


Analyze One Way Streets
-------------------------------------------------------------------------------

:ref:`pgr_analyze_oneway` analyzes one way streets in a graph and identifies any flipped segments. Basically if you count the edges coming into a node and the edges exiting a node the number has to be greater than one.

This query will add two columns to the vertices_tmp table ``ein int`` and ``eout int`` and populate it with the appropriate counts. After running this on a graph you can identify nodes with potential problems with the following query.


The rules are defined as an array of text strings that if match the ``col`` value would be counted as true for the source or target in or out condition.


Example
-------------------------------------------------------------------------------

Lets assume we have a table "st" of edges and a column "one_way" that might have values like:

   * 'FT'    - oneway from the source to the target node.
   * 'TF'    - oneway from the target to the source node.
   * 'B'     - two way street.
   * ''      - empty field, assume twoway.
   * <NULL>  - NULL field, use two_way_if_null flag.

Then we could form the following query to analyze the oneway streets for errors.

.. code-block:: sql

    SELECT pgr_analyzeOneway('mytab',
                ARRAY['', 'B', 'TF'],
                ARRAY['', 'B', 'FT'],
                ARRAY['', 'B', 'FT'],
                ARRAY['', 'B', 'TF'],
                );

    -- now we can see the problem nodes
    SELECT * FROM mytab_vertices_pgr WHERE ein=0 OR eout=0;

    -- and the problem edges connected to those nodes
    SELECT gid FROM mytab a, mytab_vertices_pgr b WHERE a.source=b.id AND ein=0 OR eout=0
    UNION
    SELECT gid FROM mytab a, mytab_vertices_pgr b WHERE a.target=b.id AND ein=0 OR eout=0;

Typically these problems are generated by a break in the network, the one way direction set wrong, maybe an error related to z-levels or a network that is not properly noded.

The above tools do not detect all network issues, but they will identify some common problems. There are other problems that are hard to detect because they are more global in nature like multiple disconnected networks. Think of an island with a road network that is not connected to the mainland network because the bridge or ferry routes are missing.


See Also
----------------------------------------------------------------------------

* :ref:`pgr_analyze_graph`
* :ref:`pgr_analyze_oneway`
* :ref:`pgr_node_network`

