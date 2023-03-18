..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_version``
===============================================================================

``pgr_version`` — Query for pgRouting version information.

.. rubric:: Availability

* Version 3.0.0

  * Breaking change on result columns
  * Support for old signature ends

* Version 2.0.0

  * **Official** function


Description
-------------------------------------------------------------------------------

Returns pgRouting version information.

.. index::
    single: version

Signature
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_version()

   | RETURNS ``TEXT``

:Example: pgRouting Version for this documentation

.. literalinclude:: doc-version.queries
   :start-after: -- q1
   :end-before: -- q2

Result Columns
-------------------------------------------------------------------------------

=========== ===============================
 Type       Description
=========== ===============================
``TEXT``    pgRouting version
=========== ===============================

See Also
-------------------------------------------------------------------------------

* :doc:`reference`
* :doc:`pgr_full_version`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
