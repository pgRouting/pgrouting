.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _tsp:

Traveling Sales Person
===============================================================================

                                                                                                                                                                                                                                                                               
  - :ref:`pgr_newTSP` - When input is a cost matrix.
  - :ref:`pgr_eucledianTSP` - When input are coordinates.
                                                                                                                                                                                                                                                                               
.. toctree::
    :hidden:

    ./pgr_tsp
    ./pgr_eucledianTSP


Origin 
..........

The traveling sales person problem was studied in the 18th century by mathematicians
 **Sir William Rowam Hamilton** and **Thomas Penyngton  Kirkman**.

A discussion about the work of Hamilton & Kirkman
can be found in the book **Graph Theory (Biggs et  al. 1976)**.

* ISBN-13: 978-0198539162
* ISBN-10: 0198539169 

It is believed that the general form of the TSP have been first studied by Kalr Menger in Vienna and Harvard.
The problem  was  later promoted by Hassler, Whitney  &  Merrill at Princeton.
A detailed  dscription about the connection between Menger & Whitney, and the development of the  
TSP can be found in  `On the history of combinatorial optimization (till 1960) <http://www.cwi.nl/~lex/files/histco.ps>`_

Problem Definition 
...................

Given a collection of cities and travel cost between each pair,
find the cheapest way for visiting all of the cities and returning to the starting point.

Characteristics 
................

- The travel costs are symmetric:

  - traveling costs from city A to city B are just as much as traveling from B to A.

- This problem is an NP-had optimization problem.

- To calculate the number of different tours through n cities:

  - Given a starting city,
  - There are n-1 choices for the second city,
  - And  n-2 choices for the third city, etc.
  - Multiplying these together we get (n-1)!  = n-1 x n-2 x n-3 x. . . x 3 x 2 x 1.
  - Now since our travel costs do not depend on the direction we take around the tour:

    - this number by 2
    -  (n-1)!/2.

TSP & Simulated Annaleing
.........................

The simulated annealing algorithm was originally inspired from the process of
annealing in metal work.

    Annealing involves heating and cooling a material to
    alter its physical properties due to the changes in its internal structure.
    As the metal cools its new structure becomes fixed,
    consequently causing the metal to retain its newly obtained properties.


.. rubric:: Pseudocode

Given an initial solution, the simulated annaeling process, will start with a high temperature
and gradually cool down until the desired temperature is reached.

For each temperature, a neighbouring new solution **snew** is calculated. The higher the temperature
the higher the probability of accepting the new solution as a possible bester solution.

Once the desired temperature is reached, the best solution found is returned

.. code-block:: none

    Solution ← initial_solution;

    temperature ← initial_temperature;
    while (temperature > final_temperature) {

        do tries_per_temperature times {
            snew ← neighbour(solution);
            If P(E(solution), E(snew), T) ≥ random(0, 1)
                solution ← snew;
        }

        temperature ← temperature * cooling factor;
    }

    Output: the best solution



pgRouting Implementation
.........................

pgRouting's implementation adds some extra parameters to allow some exit controls within the
simuated annealing procces.

To cool down faster to the next temperature:

    - max_changes_per_temperature: limits the number of changes in the solution per temperature 
    - max_consecutive_non_changes: limits the number of consecutive non changes per temperature

This is done by doing some book keeping on the times **solution ← snew;** is executed.

    - max_changes_per_temperature: Increases by one when **solution** changes
    - max_consecutive_non_changes: Reset to 0 when **solution** changes, and increaded each **try**

Aditionally to stop the algorithm at a higher temperature than the desiered one:

    - max_processing_time: limits the time the simulated annealing is performed.
    - book keeping is done to see if there was a change in **solution** on the last temperature

Note that, if no change was found in the first **max_consecutive_non_changes** tries, then the
simulated annealing will stop.

.. code-block:: none

    Solution ← initial_solution;

    temperature ← initial_temperature;
    while (temperature > final_temperature) {

        do tries_per_temperature times {
            snew ← neighbour(solution);
            If P(E(solution), E(snew), T) ≥ random(0, 1)
                solution ← snew;

            when max_changes_per_temperature is reached
                or max_consecutive_non_changes is reached
                BREAK;
        }

        temperature ← temperature * cooling factor;
        when no changes were done in the current temperature
            or max_processing_time has being reached
            BREAK;
    }

    Output: the best solution


Choosing parameters
.........................

There is no exact rule on how the parameters have to be chose, it will depend on the
special characteristics of the problem.

- Your computational time is crucial, then put your time limit to **max_processing_time**.
- Make the **tries_per_temperture** depending on the number of cities, for example:

  - Useful to estimate the time it takes to do one cycle: use `1`

    - this will help to set a reasonable **max_processing_time**

  - `n * (n-1)`
  - `500 * n`

- For a faster decreasing the temperature set **cooling_factor** to a smaller number,
  and set to a higher number for a slower decrease.

- When for the same given data the same resulsi are needed, set **randomize** to `false`.

  - When esimating how long it takes to do one cycle: use `false`


A recomendation is to play with the values and see what fits to the particular data.


..
    description of control parameters


.. include:: tsp_parameters.txt 



See Also
-------------------------------------------------------------------------------

  * http://en.wikipedia.org/wiki/Traveling_salesman_problem
  * http://en.wikipedia.org/wiki/Simulated_annealing
