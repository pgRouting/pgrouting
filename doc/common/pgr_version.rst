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

===========  =========== ===============================
Column        Type       Description
===========  =========== ===============================
**version**  ``varchar`` pgRouting version
**tag**      ``varchar`` Git tag of pgRouting build
**hash**     ``varchar`` Git hash of pgRouting build
**branch**   ``varchar`` Git branch of pgRouting build
**boost**    ``varchar`` Boost version
===========  =========== ===============================


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Query for the version string

.. literalinclude:: doc-pgr_version.queries
   :start-after: -- q1
   :end-before: -- q2


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
