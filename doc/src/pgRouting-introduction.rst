..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Introduction
===============================================================================

pgRouting is an extension of `PostGIS <https://postgis.net>`__ and `PostgreSQL
<https://www.postgresql.org>`__ geospatial database and adds routing and other
network analysis functionality. A predecessor of pgRouting – pgDijkstra, written
by Sylvain Pasche from `Camptocamp <https://camptocamp.com>`__, was later
extended by Orkney and renamed to pgRouting. The project is
now supported and maintained by `Georepublic <https://georepublic.info>`__,
`Paragon Corporation <https://www.paragoncorporation.com/>`__ and a broad user
community.

pgRouting is part of `OSGeo Community Projects
<https://wiki.osgeo.org/wiki/OSGeo_Community_Projects>`__ from the `OSGeo
Foundation <https://www.osgeo.org>`__ and included on `OSGeoLive
<http://live.osgeo.org/>`__.


.. _license:

Licensing
-------------------------------------------------------------------------------

The following licenses can be found in pgRouting:

.. list-table::
   :widths: 250 500

   * - **License**
     -
   * - GNU General Public License v2.0 or later
     - Most features of pgRouting are available under `GNU General Public
       License v2.0 or later
       <https://spdx.org/licenses/GPL-2.0-or-later.html>`_.
   * - Boost Software License - Version 1.0
     - Some Boost extensions are available under `Boost Software License -
       Version 1.0 <https://www.boost.org/LICENSE_1_0.txt>`_.
   * - MIT-X License
     - Some code contributed by iMaptools.com is available under MIT-X license.
   * - Creative Commons Attribution-Share Alike 3.0 License
     - The pgRouting Manual is licensed under a `Creative Commons
       Attribution-Share Alike 3.0 License
       <https://creativecommons.org/licenses/by-sa/3.0/>`_.

In general license information should be included in the header of each source
file.

Contributors
-------------------------------------------------------------------------------

This Release Contributors
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Individuals in this release v3.8.x (in alphabetical order)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Aurélie Bousquet,
Regina Obe,
Vicky Vergara


And all the people that give us a little of their time making comments, finding
issues, making pull requests etc. in any of our products: osm2pgrouting,
pgRouting, pgRoutingLayer, workshop.

Translators (in alphabetical order)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Dapeng Wang

Corporate Sponsors in this release (in alphabetical order)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are corporate entities that have contributed developer time, hosting, or
direct monetary funding to the pgRouting project:

- `OSGeo <https://www.osgeo.org>`__
- `OSGeo UK <https://uk.osgeo.org>`__
- `Google Summer of Code <https://summerofcode.withgoogle.com>`__
- `HighGo Software <https://www.highgo.com/>`__
- `Paragon Corporation <https://www.paragoncorporation.com/>`__

Contributors Past & Present:
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Individuals (in alphabetical order)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Aasheesh Tiwari,
Abhinav Jain,
Aditya Pratap Singh,
Adrien Berchet,
Akio Takubo,
Andrea Nardelli,
Anthony Tasca,
Anton Patrushev,
Aryan Gupta,
Ashraf Hossain,
Ashish Kumar,
Aurélie Bousquet,
Cayetano Benavent,
Christian Gonzalez,
Daniel Kastl,
Dapeng Wang,
Dave Potts,
David Techer,
Denis Rykov,
Ema Miyawaki,
Esteban Zimanyi,
Florian Thurkow,
Frederic Junod,
Gerald Fenoy,
Gudesa Venkata Sai Akhil,
Hang Wu,
Himanshu Raj,
Imre Samu,
Jay Mahadeokar,
Jinfu Leng,
Kai Behncke,
Kishore Kumar,
Ko Nagase,
Mahmoud Sakr,
Manikata Kondeti,
Mario Basa,
Martin Wiesenhaan,
Maxim Dubinin,
Maoguang Wang,
Mohamed Bakli,
Mohamed Zia,
Mukul Priya,
Nitish Chauhan,
Rajat Shinde,
Razequl Islam,
Regina Obe,
Rohith Reddy,
Sarthak Agarwal,
Shobhit Chaurasia,
Sourabh Garg,
Stephen Woodbridge,
Swapnil Joshi,
Sylvain Housseman,
Sylvain Pasche,
Veenit Kumar,
Vidhan Jain,
Virginia Vergara,
Yige Huang

Corporate Sponsors (in alphabetical order)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are corporate entities that have contributed developer time, hosting, or
direct monetary funding to the pgRouting project:

- Camptocamp
- CSIS (University of Tokyo)
- Georepublic
- Google Summer of Code
- HighGo Software
- iMaptools
- Leopark
- Orkney
- OSGeo
- OSGeo UK
- Paragon Corporation
- Versaterm Inc.


More Information
-------------------------------------------------------------------------------

* The latest software, documentation and news items are available at the
  pgRouting web site https://pgrouting.org.
* PostgreSQL database server at the PostgreSQL main site
  https://www.postgresql.org.
* PostGIS extension at the PostGIS project web site https://postgis.net.
* Boost C++ source libraries at https://www.boost.org.
* :doc:`migration`
