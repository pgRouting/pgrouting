..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Traveling Sales Person - Family of functions
===============================================================================

.. index from here

* :doc:`pgr_TSP` - When input is given as matrix cell information.
* :doc:`pgr_TSPeuclidean` - When input are coordinates.

.. index to here

.. toctree::
    :hidden:

    pgr_TSP
    pgr_TSPeuclidean

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/TSP-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/TSP-family.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/TSP-family.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/TSP-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/TSP-family.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/tsp/doc/tsp.html>`__

.. contents:: Table of Contents
    :local:


General Information
-------------------------------------------------------------------------------

Problem Definition
...............................................................................

.. tsp problem definition start

The travelling salesman problem (TSP) or travelling salesperson problem asks the
following question:

*Given a list of cities and the distances between each pair of cities, which is
the shortest possible route that visits each city exactly once and returns to the origin city?*

.. tsp problem definition end

Origin
...............................................................................

The traveling sales person problem was studied in the 18th century by mathematicians
 **Sir William Rowam Hamilton** and **Thomas Penyngton  Kirkman**.

A discussion about the work of Hamilton & Kirkman
can be found in the book **Graph Theory (Biggs et  al. 1976)**.

* ISBN-13: 978-0198539162
* ISBN-10: 0198539169

It is believed that the general form of the TSP have been first studied by Kalr Menger in Vienna and Harvard.
The problem was later promoted by Hassler, Whitney & Merrill at Princeton.
A detailed description about the connection between Menger & Whitney, and the development of the
TSP can be found in `On the history of combinatorial optimization (till 1960) <https://homepages.cwi.nl/~lex/files/histco.ps>`__

Characteristics
...............................................................................

- The travel costs are symmetric:

  - traveling costs from city A to city B are just as much as traveling from B to A.

- This problem is an NP-hard optimization problem.

- To calculate the number of different tours through :math:`n` cities:

  - Given a starting city,
  - There are :math:`n-1` choices for the second city,
  - And  :math:`n-2` choices for the third city, etc.
  - Multiplying these together we get :math:`(n-1)!  = (n-1) (n-2) . .  1`.
  - Now since our travel costs do not depend on the direction we take around the tour:

    - this number by 2
    -  :math:`(n-1)!/2`.

.. _simulated-annealing:

Simulated Annealing Algorithm
-------------------------------------------------------------------------------

The simulated annealing algorithm was originally inspired from the process of
annealing in metal work.

Annealing involves heating and cooling a material to
alter its physical properties due to the changes in its internal structure.
As the metal cools its new structure becomes fixed,
consequently causing the metal to retain its newly obtained properties.


.. rubric:: Pseudocode

Given an initial solution, the simulated annealing process, will start with a high temperature
and gradually cool down until the desired temperature is reached.

For each temperature, a neighbouring new solution **newSolution** is calculated. The higher the temperature
the higher the probability of accepting the new solution as a possible bester solution.

Once the desired temperature is reached, the best solution found is returned

.. code-block:: none

    Solution = initial_solution;

    temperature = initial_temperature;
    while (temperature > final_temperature) {

        do tries_per_temperature times {
            newSolution = neighbour(solution);
            If P(E(solution), E(newSolution), T) >= random(0, 1)
                solution = newSolution;
        }

        temperature = temperature * cooling factor;
    }

    Output: the best solution



pgRouting Implementation
...............................................................................

pgRouting's implementation adds some extra parameters to allow some exit controls within the
simulated annealing process.

* ``max_changes_per_temperature``:

  * Limits the number of changes in the solution per temperature
  * Count is reset to :math:`0` when **temperature** changes
  * Count is increased by :math: `1` when **solution** changes

* ``max_consecutive_non_changes``:

  * Limits the number of consecutive non changes per temperature
  * Count is reset to :math:`0` when **solution** changes
  * Count is increased by :math: `1` when **solution** changes

* ``max_processing_time``:

  * Limits the time the simulated annealing is performed.

.. code-block:: none

    Solution = initial_solution;

    temperature = initial_temperature;
    WHILE (temperature > final_temperature) {

        DO tries_per_temperature times {
            newSolution = neighbour(solution);
            If Probability(E(solution), E(newSolution), T) >= random(0, 1)
                solution = newSolution;

            BREAK DO WHEN:
                max_changes_per_temperature is reached
                OR max_consecutive_non_changes is reached
        }

        temperature = temperature * cooling factor;
        BREAK WHILE WHEN:
            no changes were done in the current temperature
            OR max_processing_time has being reached
    }

    Output: the best solution found


Choosing parameters
...............................................................................

There is no exact rule on how the parameters have to be chose, it will depend on the
special characteristics of the problem.

- If the computational time is crucial, then limit execution time with **max_processing_time**.
- Make the **tries_per_temperture** depending on the number of cities (:math:`n`), for example:

  - Useful to estimate the time it takes to do one cycle: use `1`

    - this will help to set a reasonable **max_processing_time**

  - :math:`n * (n-1)`
  - :math:`500 * n`

- For a faster decreasing the temperature set **cooling_factor** to a smaller number,
  and set to a higher number for a slower decrease.

- When for the same given data the same results are needed, set **randomize** to `false`.

  - When estimating how long it takes to do one cycle: use `false`


A recommendation is to play with the values and see what fits to the particular data.


Description of the Control Parameters
...............................................................................

The control parameters are optional, and have a default value.

.. tsp control parameters begin

=============================== ===========  ============  =================================================
Parameter                       Type         Default       Description
=============================== ===========  ============  =================================================
**start_vid**                   ``BIGINT``    `0`          The greedy part of the implementation will use this identifier.
**end_vid**                     ``BIGINT``    `0`          Last visiting vertex before returning to start_vid.
**max_processing_time**         ``FLOAT``    `+infinity`   Stop the annealing processing when the value is reached.
**tries_per_temperature**       ``INTEGER``  `500`         Maximum number of times a neighbor(s) is searched in each temperature.
**max_changes_per_temperature** ``INTEGER``  `60`          Maximum number of times the solution is changed in each temperature.
**max_consecutive_non_changes** ``INTEGER``  `100`         Maximum number of consecutive times the solution is not changed in each temperature.
**initial_temperature**         ``FLOAT``    `100`         Starting temperature.
**final_temperature**           ``FLOAT``    `0.1`         Ending temperature.
**cooling_factor**              ``FLOAT``    `0.9`         Value between between 0 and 1 (not including) used to calculate the next temperature.
**randomize**                   ``BOOLEAN``  `true`        Choose the random seed

                                                           - true: Use current time as seed
                                                           - false: Use `1` as seed. Using this value will get the same results with the same data in each execution.

=============================== ===========  ============  =================================================


.. tsp control parameters end


Description of the return columns
---------------------------------------------------------------------------

.. tsp return values begin

Returns SET OF ``(seq, node, cost, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**node**      ``BIGINT``  Identifier of the node/coordinate/point.
**cost**      ``FLOAT``   Cost to traverse from the current ``node`` to the next ``node`` in the path sequence.
                            - ``0`` for the last row in the path sequence.

**agg_cost**  ``FLOAT``   Aggregate cost from the ``node`` at ``seq = 1`` to the current node.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================

.. tsp return values end

See Also
.........................

.. rubric:: References

* `Simulated annaeling algorithm for beginners <https://www.theprojectspot.com/tutorial-post/simulated-annealing-algorithm-for-beginners/6>`__
* `Wikipedia: Traveling Salesman Problem <https://en.wikipedia.org/wiki/Traveling_salesman_problem>`__
* `Wikipedia: Simulated annealing <https://en.wikipedia.org/wiki/Simulated_annealing>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


