#pragma once

#include <deque> 
#include <vector>
#include "order.h"
#include "tw_node.h"
#include "vehicle.h"


class Initial_solution;

class Vehicle_pickDeliver : public Vehicle {
  private:
    double cost;     
    std::set<ID> orders_in_vehicle;    ///< orders inserted in this vehicle
    const Pgr_pickDeliver *problem; ///< The vehicle belongs to this problem


  public:
    friend Initial_solution;

    Vehicle_pickDeliver(
            ID id,
            const Vehicle_node &starting_site,
            const Vehicle_node &ending_site,
            double max_capacity,
            const Pgr_pickDeliver *p_problem);

    Vehicle_pickDeliver(const Vehicle_pickDeliver &v) = default;

    size_t orders_size() const {return orders_in_vehicle.size();}


#if 0

    // return cost: infinity
    // orders are not inserted
    //int   getPosLowLimit(int nid ,int from, const TWC<Vehicle_node> &twc ) const;
    //int   getPosHighLimit(int nid ,int from, int to, const TWC<Vehicle_node> &twc) const;
    double testInsertPUSH(const Order& order, const Orders &orders, int &pickPos,int &delPos,const TWC<Vehicle_node> &twc);
    double testInsertFIFO(const Order& order, const Orders &orders, int &pickPos,int &delPos,const TWC<Vehicle_node> &twc);
    double testInsertLIFO(const Order& order, const Orders &orders, int &pickPos,int &delPos,const TWC<Vehicle_node> &twc);
    double tryInsertPOS(const Order &order, int pickPos, int delPos);
    double insertPOS(const Order &order, int pickPos, int delPos);
    double e_erase(int pickPos, int delPos);
    bool   e_feasable4(const Vehicle_node &n1,const Vehicle_node &n2,const Vehicle_node &n3,const Vehicle_node &n4);

    //*******

    int pos(int nid) const ;
    bool  in(int nid) const;

    void e_push_back(const Vehicle_node &pathstop);
    void e_insert(const Vehicle_node &pathstop,int at);
    void e_erase(int at);
    void e_swap(int i,int j);
    void e_move(int fromi,int toj);
    void e_clean();

    void removeOrder(const Order &order);
    void removeOrder(int orderid);
    void removePickup(int orderid);
    void removeDelivery(int orderid);
    void swapstops(int i,int j);
    bool insertOrderAfterLastPickup(const Order &order, const TWC<Vehicle_node> &twc);
#endif

    bool has_order(const Order &order) const;

    /*! @brief puts an order at the end of the truck
     *
     * Precondition:
     *  !has_order(order)
     *
     * Postcondition:
     *  has_order(order)
     *  !has_cv();
     *
     *  Before: S <nodes> E
     *   After: S <nodes> P D E
     *
     *  Can generate time window violation
     *  No capacity violation
     */
    void push_back(const Order &order);



    /*! @brief Puts an order at the end front of the truck
     *
     * Precondition:
     *  !has_order(order)
     *
     * Postcondition:
     *  has_order(order)
     *  !has_cv();
     *
     *  Before: S <nodes> E
     *   After: S P D <nodes> E
     *
     *  Can generate time window violation
     *  No capacity violation
     */
    void push_front(const Order &order);



    /*! @brief Inserts an order
     *
     *  Precondition:
     *  !has_order(order)
     *
     * Postcondition:
     *  has_order(order)
     *  !has_cv();
     *
     *  Before: S <nodes> E
     *   After: S ....P ....  D .... E
     *
     * push_back is performed when
     *   - pickup 
     *
     *  Can generate time window violation
     *  No capacity violation
     */
    void insert(const Order &order);

    /* @brief erases the order from the vehicle
     *
     * Precondition:
     *  has_order(order)
     *
     * Precondition:
     *  !has_order(order)
     */
    void erase(const Order &order);

    void insert_less_travel_time(const Order&);


    /*!
     * The order that is picked last is removed
     *
     * \returns id of the removed order
     */

    ID pop_back();
    ID pop_front();



#if 0
    void insertPickup(const Order &order, const int at);
    void insertDelivery(const Order &order, const int at);

    Vehicle_node& getnode(int at) {return path[at];};

    bool sameorder(int i,int j){return path[i].getoid()==path[j].getoid();}
    void erase() {path.resize(1);};
    void clean() {path.e_resize(1,maxcapacity); evalLast(); };

    /*algorithm spesific */
    void findBetterForward(int &bestI, int &bestJ);
    bool findImprovment(int i);
    void hillClimbOpt();
    int  findForwardImprovment(const int i,double &bestcost) ;
    double costBetterPickupBackward(int &bppos, int &bdpos);
    double findBestCostBackForw(const int oid,int &bppos,int &bdpos);
    int    findBetterDeliveryForward(const int ppos,const int dpos,double &bestcost);
    bool hasTrip() {return path.size()>1;};
    bool hasNodes() {return !path.empty();};



#endif
};

