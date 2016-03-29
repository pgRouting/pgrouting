

#include "../../common/src/pgr_assert.h"
#include "solution.h"
#include "initial_solution.h"
#include "pgr_pickDeliver.h"

void
Initial_solution::invariant() const {
    std::set<ID> orders(assigned);

    orders.insert(unassigned.begin(), unassigned.end());

    /* check the local book keeping is ok */
    pgassert(all_orders == orders);

    /* this checks there is no order duplicated */
    pgassert(all_orders.size() == orders.size());
}


Initial_solution::Initial_solution(
        int kind,
        const Pgr_pickDeliver *p_problem) :
    Solution(p_problem) {

        for (const auto &order : problem->orders()) {
            unassigned.insert(order.id());
        }
        all_orders = unassigned;
        assigned.clear();

        switch  (kind) {
            case 0:
                one_truck_all_orders();
                break;
            case 1:
                one_truck_per_order();
                break;
            case 2:
                push_back_while_feasable();
                break;
            case 3:
                push_front_while_feasable();
                break;
            case 4:
                insert_while_feasable();
                break;
            case 5:
                insert_while_compatible();
                break;
        }

    }




void
Initial_solution::insert_while_compatible() {
    problem->log << "\nInitial_solution::insert_while_compatible\n";
    invariant();

    ID v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    /*
     * orders: keep sorted based on the number of orders it is compatible with
     */
    std::deque<ID> orders(unassigned.begin(), unassigned.end());
    for (const auto &o: orders) {
        problem->log << problem->orders()[o];
    };
    const Pgr_pickDeliver *prob = problem;
    std::sort(orders.begin(), orders.end(), [&prob]
            (const ID &lhs, const ID &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleJ.size()
            < prob->orders()[rhs].m_compatibleJ.size();
            } ); 
    std::stable_sort(orders.begin(), orders.end(), [&prob]
            (const ID &lhs, const ID &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleI.size()
            < prob->orders()[rhs].m_compatibleI.size();
            } ); 


    problem->log << "\n Sorted orders by compatibleI.size\n";
    for (const auto &o: orders) {
        problem->log << "\n|"<< o <<"| = " << problem->orders()[o].m_compatibleI.size();
        problem->log << "\t|"<< o <<"| = " << problem->orders()[o].m_compatibleJ.size();
    };

#if 0
    problem->log << "\nInitial_solution::insert_while_compatible\n";
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.insert(order);

        if (!truck.is_feasable()) {
            truck.erase(order); 
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
#endif
}

void
Initial_solution::insert_while_feasable() {
    invariant();

    ID v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    problem->log << "\nInitial_solution::insert_while_feasable\n";
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.insert(order);

        if (!truck.is_feasable()) {
            truck.erase(order); 
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}

void
Initial_solution::push_front_while_feasable() {
    ID v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_front(order);
        problem->log << truck;
        if (!truck.is_feasable()) {
            truck.pop_front();
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}

void
Initial_solution::push_back_while_feasable() {
    ID v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_back(order);
        if (!truck.is_feasable()) {
            truck.pop_back();
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}



void
Initial_solution::one_truck_per_order() {
    ID v_id(0);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        Vehicle_pickDeliver truck(
                v_id++,
                problem->m_starting_site,
                problem->m_ending_site,
                problem->max_capacity,
                problem);
        truck.push_back(order);
        fleet.push_back(truck);

        assigned.insert(*unassigned.begin());
        unassigned.erase(unassigned.begin());

        invariant();
    };
}




void
Initial_solution::one_truck_all_orders() {
    ID v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_back(order);

        assigned.insert(*unassigned.begin());
        unassigned.erase(unassigned.begin());

        invariant();
    };
    fleet.push_back(truck);
}




