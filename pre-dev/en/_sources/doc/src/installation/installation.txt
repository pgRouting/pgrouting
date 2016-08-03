..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _installation:

Installation
===============================================================================

This is a basic guide to download and install pgRouting.

Additional notes can be found in `Installation Notes <https://github.com/pgRouting/pgrouting/wiki/Notes-on-Download%2C-Installation-and-building-pgRouting>`__

Also PostGIS provides some information about installation in this `Install Guide <http://www.postgis.us/presentations/postgis_install_guide_22.html>`__

Download
--------

Binary packages are provided for the current version on the following platforms:


Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Winnie Bot Builds:

* `Winnie PostgreSQL 9.2-9.5 32-bit/64-bit <http://postgis.net/windows_downloads>`_

Production Builds:

* Production builds are part of the Spatial Extensions/PostGIS Bundle available via Application StackBuilder
* Can also get PostGIS Bundle from http://download.osgeo.org/postgis/windows/


Ubuntu/Debian
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ubuntu packages are available in Launchpad repositories:

* *stable* https://launchpad.net/~georepublic/+archive/pgrouting
* *unstable* https://launchpad.net/~georepublic/+archive/pgrouting-unstable

.. code-block:: bash

	# Add pgRouting launchpad repository ("stable" or "unstable")
	sudo add-apt-repository ppa:georepublic/pgrouting[-unstable]
	sudo apt-get update

	# Install pgRouting packages
	sudo apt-get install postgresql-9.1-pgrouting

Use `UbuntuGIS-unstable PPA <https://launchpad.net/~ubuntugis/+archive/ubuntugis-unstable>`_ to install PostGIS 2.0.


RHEL/CentOS
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Add repositories for dependencies:

  .. code-block:: bash

     wget http://repo.enetres.net/enetres.repo -O /etc/yum.repos.d/enetres.repo
     wget http://nextgis.ru/programs/centos/nextgis.repo -O /etc/yum.repos.d/nextgis.repo
     yum install epel-release

* Install PostgreSQL and PostGIS according to `this <https://trac.osgeo.org/postgis/wiki/UsersWikiPostGIS21CentOS6pgdg>`__ instructions.

* Install CGAL:

  .. code-block:: bash

     yum install libCGAL10

* Install pgRouting:

  .. code-block:: bash

     yum install pgrouting_94

More info (and packages for CentOS) can be found `here <https://github.com/nextgis/gis_packages_centos/wiki/Using-this-repo>`__.



Fedora
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Fedora RPM's: https://admin.fedoraproject.org/pkgdb/package/rpms/pgRouting/


FreeBSD
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

pgRouting can be installed via ports:

.. code-block:: bash

  cd /usr/ports/databases/pgRouting
  make install clean


OS X
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. See `builds from KingChaos <http://www.kyngchaos.com/software/postgres>`_.

* Homebrew

.. code-block:: bash

	brew install pgrouting


Source Package
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can find all the pgRouting Releases: 

https://github.com/pgRouting/pgrouting/releases

See :ref:`build` to build the binaries from the source.

Using Git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Git protocol (read-only):

.. code-block:: bash

	git clone git://github.com/pgRouting/pgrouting.git


HTTPS protocol (read-only):

.. code-block:: bash

	git clone https://github.com/pgRouting/pgrouting.git

See :ref:`build` to build the binaries from the source.

Installing in the database
--------------------------

pgRouting is an extension. 

.. code-block:: sql

  CREATE EXTENSION postgis;
  CREATE EXTENSION pgrouting;


Upgrading the database
----------------------

To upgrade pgRouting to version 2.x.y use the following command:

.. code-block:: sql

   ALTER EXTENSION pgrouting UPDATE TO "2.x.y";

   For example to upgrade to 2.2.3

   .. code-block:: sql

   ALTER EXTENSION pgrouting UPDATE TO "2.2.3";


