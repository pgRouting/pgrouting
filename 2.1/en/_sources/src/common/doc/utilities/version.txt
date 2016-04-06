.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_version:

pgr_version 
===============================================================================

.. index:: 
	single: version()

Name
-------------------------------------------------------------------------------

``pgr_version`` — Query for pgRouting version information.


Synopsis
-------------------------------------------------------------------------------

Returns a table with pgRouting version information.

.. code-block:: sql

	table() pgr_version();


Description
-------------------------------------------------------------------------------

Returns a table with:

:version: ``varchar`` pgRouting version
:tag: ``varchar`` Git tag of pgRouting build
:hash: ``varchar`` Git hash of pgRouting build
:branch: ``varchar`` Git branch of pgRouting build
:boost: ``varchar`` Boost version


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Query for full version string

.. code-block:: sql

    SELECT pgr_version();

                         pgr_version                      
    ------------------------------------------------------
     (2.0.0-dev,v2.0dev,290,c64bcb9,sew-devel-2_0,1.49.0)
    (1 row)


* Query for ``version`` and ``boost`` attribute

.. code-block:: sql

    SELECT version, boost FROM pgr_version();

      version  | boost  
    -----------+--------
     2.0.0-dev | 1.49.0
    (1 row)


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_versionless` to compare two version numbers
