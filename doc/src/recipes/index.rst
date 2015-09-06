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


*No more contributions*


How to contribute.
==================

.. rubric:: To add a recipie or a wrapper

The first steps are:

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

