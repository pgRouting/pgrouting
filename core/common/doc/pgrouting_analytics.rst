=========================
pgRouting Graph Analytics
=========================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: This is a collection of tools for analyzing graphs.
It has been contributed to pgRouting by iMaptools.com.
:Copyright: Stephen Woodbridge. This is released under the MIT-X license.

It is common to find problems with graphs that have not been constructed
fully noded or in graphs with zlevels at intersection that have been 
entered incorrectly. An other problem is oneway streets that have been
entered in the wrong direction. We can not detect errors with respect 
to "ground" truth, but we can look for inconsistencies and some 
anomalies in a graph and report them for additional inspections.

We do not current have any visualization tools for these problems, but
I have used mapserver to render the graph and highlight potential 
problem areas. Someone familar with graphviz might contribute tools
for generating images with that.

TODO: add oneway street analysis.

Analyze a Graph
===============

    .. function:: pgr_analyze_graph(edge_tab, geom_col, tol)
       :module: pgrouting_analytics.sql

       Analyzes the "edge_tab" and "vertices_tmp" tables and flags if
       nodes are deadends, ie vertices_tmp.cnt=1 and identifies nodes
       that might be disconnected because of gaps < tol or because of
       zlevel errors in the data. For example:

       ::

           select pgr_analyze_graph('mytab', 'the_geom', 0.000002);

       After the analyzing the graph, deadends are indentified by *cnt=1*
       in the "vertices_tmp" table and potential problems are identified
       with *chk=1*.

       ::

           select * from vertices_tmp where chk = 1;


Utility functions
=================

    .. function:: pgr_is_column_in_table(tab, col)
       :module: pgrouting_analytics.sql

       Return true ot false if column "col" exists in table "tab"

    .. function:: pgr_is_column_indexed(tab, col)
       :module: pgrouting_analytics.sql

       Return true ot false if column "col" in table "tab" is indexed.


