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

.. note:: additional notes can be found in `Installation Notes`_

.. _Installation Notes: https://github.com/pgRouting/pgrouting/wiki/Notes-on-Download%2C-Installation-and-building-pgRouting

Download
--------

Binary packages are provided for the current version on the following platforms:


Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Winnie Bot Experimental Builds:

* `Winnie PostgreSQL 9.2-9.5 32-bit/64-bit <http://postgis.net/windows_downloads>`_

Production Builds:

* Production builds are part of the Spatial Extensions/PostGIS Bundle available via Application StackBuilder
* `Can also get PostGIS Bundle from <http://download.osgeo.org/postgis/windows/>`_


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


RHEL/CentOS/Fedora
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Fedora RPM's: https://admin.fedoraproject.org/pkgdb/acls/name/pgRouting


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

To upgrade pgRouting to version 2.1.0 use the following command:

.. code-block:: sql

   ALTER EXTENSION pgrouting UPDATE TO "2.1.0";


