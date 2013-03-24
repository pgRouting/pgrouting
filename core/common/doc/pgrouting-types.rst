=========================
pgRouting Graph Analytics
=========================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: Description of the common postgreSQL types used by many
of the pgRouting functions.
:Copyright: Stephen Woodbridge. This documentation is released under the
MIT-X license.

The following are commonly used data types for some of the pgRouting
functions. There are some additional types that are not documented here
because they are specific to a single or small group of functions and
are documented with those functions.

pgRouting Data Types
====================

    ::

    CREATE TYPE path_result AS
    (
        vertex_id integer,
        edge_id integer,
        cost float8
    );

    CREATE TYPE vertex_result AS
    (
        x float8,
        y float8
    );

    CREATE TYPE geoms AS
    (
        id integer,
        gid integer,
        the_geom geometry
    );
