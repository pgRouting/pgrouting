.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _changelog_2_1_develop:

pgRouting devlop Release Notes
===============================================================================

With the release of pgRouting develop fixes some bugs and issues.

For important changes see the following release notes. To see the full list of changes check the list of `Git commits <https://github.com/pgRouting/pgrouting/commits>`_ on Github.

..



Changes for develop
-------------------------------------------------------------------------------

* ksp
      - Uses boost::Graph and the boost's Dijstra's algorithm
      - issue #294 fixed (KSP gives compile warning)
      - issue #293 fixed (Cleanup KSP directory)
      - issue #287 fixed (pgr_ksp returns invalid one route when source and target are same)
      - issue #385 fixed (KSP gives erroneous results whenever there are edges that share start/end points)
      - issue #246 fixed for ksp only (pgr_Dijkstra/pgr_kdijkstraPath/pgr_ksp differences)
      - issue #232 fixed for ksp only (Honor client cancel requests in C /C++ code)
             - c++ class does this task, might only be usable in c++ code
             - not tested on c code
             - must be interesting to put this class in a "shared c or c++ code" directory so any pgRouting code can use it and avoid copy/paste code
      - issue #207 fixed (pgr_ksp not finding any routes but pgr_dijkstra finds one for the same source and target)
      - issue #140 fixed for ksp only (Add support for passing bigint as ids and returning them #140)
             - need to be hand-enabled until all other functions support it
             - not tested on 32bit.
             - added pgr_costResult3Big type
             - added _pgr_parameter check for sql with: id,source,target,cost and optional reverse_cost
                   - the sql: column existance and type checking is no longer requiered in the c code
             - this new type and function are located under ksp/sql (move to common?)
      
