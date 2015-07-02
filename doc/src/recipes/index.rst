.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _recipes:

User's Recipes List
===============================================================================


.. toctree:: 
         
   ./example_recipe.rst
   ./parallel_handling.rst


Unsupported Functions (to become a recipe)
===============================================================================
:ref:`convenience_functions`

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_flip_edges` - flip the edges in an array of geometries so the connect end to end.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
  -  :ref:`pgr_points_to_dmatrix` - Create a distance matrix from an array of points.
  -  :ref:`pgr_vids_to_dmatrix` - Create a distance matrix from an array of ``vertix_id``.
  -  :ref:`pgr_vids_to_dmatrix2` - Create a distance matrix from an array of ``vertix_id``.

.. toctree:: 
   :hidden:
      
   ../../../src/common/doc/convenience/index

*No more contributions*


How to contribute.
==================

.. rubric:: To add a recipie or a wrapper


  - Fork the repository 
  - Create a branch for your recipe or wrapper
  - Create your recipe file

.. code::

    cd doc/src/recipes/
    vi myrecipe.rst
    git add myrecipe.rst
    # include the recipe in this file
    vi index.rst

.. rubric:: To create the test file of your recipe

.. code::

    cd test
    cp myrecipe.rst myrecipe.sql.test
    
    # make your test based on your recipe
    vi myrecipe.sql.test
    git add myrecipe.sql.test

    # create your test results file
    touch myrecipe.result
    git add myrecipe.result

    # add your test to the file
    vi test.conf

Leave only the SQL code on ``myrecipe.sql.test`` by deleting lines or by commenting lines.


.. rubric:: To get the results of your recipe

From the root directory execute:

.. code::

    tools/test-runner.pl -alg recipes -ignorenotice

Copy the results of your recipe and paste them in the file ``myrecipe.result``, and remove the "> " from the file.

.. rubric::  make a pull request.

.. code::

    git commit -a -m 'myrecipe is for this and that'
    git push

From your fork in github make a pull request over develop

