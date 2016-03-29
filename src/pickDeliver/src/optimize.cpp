

#include "solution.h"
#include "optimize.h"
#include "pgr_pickDeliver.h"

Optimize::Optimize(
        int kind,
        const Solution &old_solution) :
    Solution(old_solution) {

        switch  (kind) {
            case 1:
                decrease_truck();
                break;
        }
    }

/*
 * Optimize decreasing truck
 *
 * - Objective: remove shortest truck
 * 
 */
void
Optimize::decrease_truck() {
    /* sort fleet by truck.size smallest is last*/

    problem->log << "Before sort" << tau();


    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs) -> bool {
            return rhs.orders_size() < lhs.orders_size();
            }
            );

    problem->log << "After first sort" << tau();

    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs) -> bool {
            return rhs.duration() < lhs.duration();
            }
            );
    problem->log << "After sort" << tau();


}


