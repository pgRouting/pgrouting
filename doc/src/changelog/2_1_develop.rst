.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _changelog_develop

pgRouting devlop Release Notes
===============================================================================

With the release of pgRouting develop fixes some bugs and issues.

For important changes see the following release notes. To see the full list of changes check the list of `Git commits <https://github.com/pgRouting/pgrouting/commits>`_ on Github.



Changes for develop
-------------------------------------------------------------------------------

* ksp - issue #294 fixed (KSP gives compile warning)
      - issue #293 fixed (Cleanup KSP directory)
      - issue #287 fixed (pgr_ksp returns invalid one route when source and target are same)
      - issue #385 fixed (KSP gives erroneous results whenever there are edges that share start/end points)
      - issue #246 ksp part of issue fixed (pgr_Dijkstra/pgr_kdijkstraPath/pgr_ksp differences)
      - issue #246 ksp part of issue fixed (Honor client cancel requests in C /C++ code)
      - issue #207 fixed (pgr_ksp not finding any routes but pgr_dijkstra finds one for the same source and target #207)
      - issue #140 ksp part of issue fixed (Add support for passing bigint as ids and returning them #140) need to be hand-enabled until all other functions support it, not tested on 32bit.
