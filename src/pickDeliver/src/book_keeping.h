
#ifndef _FOO_
#define _FOO_
#pragma once

#include <queue>
#include <iostream>
#include "./vehicle_pickDeliver.h"
/**
 * Book keeping class for swapping orders between vehicles
 */

namespace pgrouting {
namespace vrp {

class Swap_bk;
class Swap_info {
    friend class Swap_bk;
    public:

        Swap_info() = default;
        Swap_info(const Swap_info &) = default;
        Swap_info(const Vehicle_pickDeliver &from,
                const Vehicle_pickDeliver &to,
                size_t from_o,
                size_t to_o,
                double delta);
#if 0
            from_truck(from),
            to_truck(to),
            from_order(from_o),
            to_order(to_o),
            estimated_delta(delta)
    { }
#endif
        friend std::ostream& operator << (std::ostream& log, const Swap_info &d);
#if 0
        {
            log   
                << "\n" << d.from_truck.tau() << " --> "
                << d.from_order << "(" << d.from_truck.orders()[d.from_order].pickup().original_id() << ")"
                << "\n" << d.to_truck.tau() << " --> "
                << d.to_order << "(" << d.to_truck.orders()[d.to_order].pickup().original_id() << ")"
                << "\n" << "delta = " << d.estimated_delta;
            return log;
        }
#endif
    public:
        Vehicle_pickDeliver from_truck;
        Vehicle_pickDeliver to_truck;
        size_t from_order;
        size_t to_order;
        double estimated_delta;
};


class Swap_bk {
    private:
        class Compare {
            public:
                bool operator() (const Swap_info &lhs, const Swap_info rhs) {
                    return lhs.estimated_delta > rhs.estimated_delta;
                }
        };
    public:
    typedef typename std::priority_queue<
            Swap_info,
            std::vector<Swap_info>,
            Compare >  Swaps_queue;


        Swaps_queue  m_swaps;
        friend std::ostream& operator<< (std::ostream &log, const Swap_bk &data);
#if 0
        {
            auto q = data.m_swaps;
            while(!q.empty()) {
                log << q.top() << " ";
                q.pop();
            }
            log << '\n';
            return log;
        }
#endif
    public:
        Swap_bk() = default;
        Swap_bk(const Swap_bk &bk) = default;
        Swaps_queue& possible_swaps() {return m_swaps;}
        void push(const Swap_info& data) {m_swaps.push(data);}
        Swap_info top() {return m_swaps.top();}
        void pop() {return m_swaps.pop();}
        bool empty() {return m_swaps.empty();}

};

}  // namespace vrp
}  // namespace pgrouting

#endif  // _FOO_
