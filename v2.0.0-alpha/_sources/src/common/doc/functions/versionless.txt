.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_versionless:

pgr_versionless - Compare version numbers
===============================================================================

.. index:: 
	single: pgr_versionless(text,text)
	module: utilities

Name
-------------------------------------------------------------------------------

``pgr_version`` — Compare two version numbers and return if smaller.


Synopsis
-------------------------------------------------------------------------------

Returns ``true`` if the first version number is smaller than the second version number. Otherwise returns ``false``.

.. code-block:: sql

	boolean pgr_versionless(text v1, text v2);


Description
-------------------------------------------------------------------------------

:v1: ``text`` first version number
:v2: ``text`` second version number


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_versionless('2.0.1', '2.1');

     pgr_versionless 
    -----------------
     t
    (1 row)


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_version`
