.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _installation:

Installation
===============================================================================

Binary packages are provided for the current version on the following platforms:

Windows
********************************************************************************

Winnie Bot Experimental Builds: 

* `PostgreSQL 9.2 32-bit, 64-bit <http://winnie.postgis.net/download/windows/pg92/buildbot/>`_


Ubuntu
********************************************************************************

Ubuntu packages are available in a `Launchpad repository <https://launchpad.net/~georepublic/+archive/pgrouting>`_:

.. code-block:: bash

	# Add pgRouting launchpad repository
	sudo add-apt-repository ppa:georepublic/pgrouting
	sudo apt-get update

	# Install pgRouting packages
	sudo apt-get install 
			postgresql-9.1-pgrouting \
	        postgresql-9.1-pgrouting-dd 


RHEL/CentOS
********************************************************************************



OSX
********************************************************************************

See `builds from KingChaos <http://www.kyngchaos.com/software/postgres>`_.


Build from Source
================================================================================

See :ref:`build` for notes on compiling from source.


Source Package
********************************************************************************

* Git 2.0.0 release: `tar.gz <http:/download.sample>`_ `zip <http:/download.sample>`_
* Git master branch: `tar.gz <http:/download.sample>`_ `zip <http:/download.sample>`_
* Git develop branch: `tar.gz <http:/download.sample>`_ `zip <http:/download.sample>`_


Using Git
********************************************************************************

Git protocol (read-only):

.. code-block:: bash

	git clone git://github.com/qgis/Quantum-GIS.git


HTTPS protocol (read-only):

.. code-block:: bash

	git clone https://github.com/qgis/Quantum-GIS.git

