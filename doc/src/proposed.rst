..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
    ****************************************************************************

|


Proposed Functions
==================================

.. warning-begin

.. collapse:: Proposed

   .. warning:: Proposed functions for next mayor release.

     - They are not officially in the current release.
     - They will likely officially be part of the next mayor release:

       - The functions make use of ANY-INTEGER and ANY-NUMERICAL
       - Name might not change. (But still can)
       - Signature might not change. (But still can)
       - Functionality might not change. (But still can)
       - pgTap tests have being done. But might need more.
       - Documentation might need refinement.

.. end-warning

.. rubric:: Families

:doc:`dijkstra-family`

.. include:: dijkstra-family.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`withPoints-family`

.. include:: withPoints-family.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`transformation-family`

.. include:: transformation-family.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`coloring-family`

.. include:: coloring-family.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`contraction-family`

.. include:: contraction-family.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`traversal-family`

.. include:: traversal-family.rst
   :start-after: official-start
   :end-before: official-end

.. toctree::
   :hidden:

   traversal-family
   coloring-family

.. rubric:: categories

:doc:`cost-category`

.. include:: cost-category.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`costMatrix-category`

.. include:: costMatrix-category.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`drivingDistance-category`

.. include:: drivingDistance-category.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`KSP-category`

.. include:: KSP-category.rst
   :start-after: proposed-start
   :end-before: proposed-end

:doc:`via-category`

.. include:: via-category.rst
   :start-after: proposed start
   :end-before: proposed end

:doc:`withPoints-category`

.. include:: withPoints-category.rst
   :start-after: proposed start
   :end-before: proposed end

.. toctree::
    :hidden:

    withPoints-family
    KSP-category
    via-category
    withPoints-category

See Also
-------------------------------------------------------------------------------

* :doc:`experimental`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
