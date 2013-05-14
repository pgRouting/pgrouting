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

Binary packages are provided for the current version on the following platforms:


Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Winnie Bot Experimental Builds: 

* `PostgreSQL 9.2 32-bit, 64-bit <http://winnie.postgis.net/download/windows/pg92/buildbot/>`_


Ubuntu/Debian
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ubuntu packages are available in a `Launchpad repository <https://launchpad.net/~georepublic/+archive/pgrouting>`_:

.. code-block:: bash

	# Add pgRouting launchpad repository
	sudo add-apt-repository ppa:georepublic/pgrouting
	sudo apt-get update

	# Install pgRouting packages
	sudo apt-get install postgresql-9.1-pgrouting postgresql-9.1-pgrouting-dd 

Use `UbuntuGIS-unstable PPA <https://launchpad.net/~ubuntugis/+archive/ubuntugis-unstable>`_ to install PostGIS 2.0.


RHEL/CentOS/Fedora
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Fedora RPM's: https://admin.fedoraproject.org/pkgdb/acls/name/pgRouting


OSX
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

See `builds from KingChaos <http://www.kyngchaos.com/software/postgres>`_.


Build from Source
===============================================================================

Source Package
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Git 2.0.0 release: `v2.0.0.tar.gz <https://github.com/pgRouting/pgrouting/archive/v2.0.0.tar.gz>`_  | `v2.0.0.zip <https://github.com/pgRouting/pgrouting/archive/v2.0.0.zip>`_
* Git master branch: `master.tar.gz <https://github.com/pgRouting/pgrouting/archive/master.tar.gz>`_  | `master.zip <https://github.com/pgRouting/pgrouting/archive/master.zip>`_
* Git develop branch: `develop.tar.gz <https://github.com/pgRouting/pgrouting/archive/develop.tar.gz>`_  | `develop.zip <https://github.com/pgRouting/pgrouting/archive/develop.zip>`_


Using Git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Git protocol (read-only):

.. code-block:: bash

	git clone git://github.com/pgRouting/pgrouting.git


HTTPS protocol (read-only):

.. code-block:: bash

	git clone https://github.com/pgRouting/pgrouting.git


See :ref:`build` for notes on compiling from source.
