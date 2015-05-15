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

* `PostgreSQL 9.2-9.4beta1 32-bit, 64-bit <http://postgis.net/windows_downloads>`_

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

======================== ======================= ====================
Git 2.0.0-rc1 release    `v2.0.0-rc1.tar.gz`_    `v2.0.0-rc1.zip`_
Git 2.0.0-beta release   `v2.0.0-beta.tar.gz`_   `v2.0.0-beta.zip`_
Git 2.0.0-alpha release  `v2.0.0-alpha.tar.gz`_  `v2.0.0-alpha.zip`_
Git master branch        `master.tar.gz`_        `master.zip`_
Git develop branch       `develop.tar.gz`_       `develop.zip`_
======================== ======================= ====================

.. _v2.0.0-rc1.tar.gz: https://github.com/pgRouting/pgrouting/archive/v2.0.0-rc1.tar.gz
.. _v2.0.0-rc1.zip: https://github.com/pgRouting/pgrouting/archive/v2.0.0-rc1.zip

.. _v2.0.0-beta.tar.gz: https://github.com/pgRouting/pgrouting/archive/v2.0.0-beta.tar.gz
.. _v2.0.0-beta.zip: https://github.com/pgRouting/pgrouting/archive/v2.0.0-beta.zip

.. _v2.0.0-alpha.tar.gz: https://github.com/pgRouting/pgrouting/archive/v2.0.0-alpha.tar.gz
.. _v2.0.0-alpha.zip: https://github.com/pgRouting/pgrouting/archive/v2.0.0-alpha.zip

.. _master.tar.gz: https://github.com/pgRouting/pgrouting/archive/master.tar.gz
.. _master.zip: https://github.com/pgRouting/pgrouting/archive/master.zip

.. _develop.tar.gz: https://github.com/pgRouting/pgrouting/archive/develop.tar.gz
.. _develop.zip: https://github.com/pgRouting/pgrouting/archive/develop.zip


Using Git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Git protocol (read-only):

.. code-block:: bash

	git clone git://github.com/pgRouting/pgrouting.git


HTTPS protocol (read-only):
.. code-block:: bash

	git clone https://github.com/pgRouting/pgrouting.git


See :ref:`build` for notes on compiling from source.
