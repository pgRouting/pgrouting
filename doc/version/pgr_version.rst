..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_version.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_version.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_version.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_version.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_version.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_version.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_version.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_version.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_version.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_version.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/common/doc/pgr_version.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/common/doc/pgr_version.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/common/doc/utilities/version.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/common/doc/utilities/version.html>`__

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
