
#pragma once

#include <deque>
#include <iostream>
#include <algorithm>


#include "./vehicle_node.h"


/*! \class Twpath
 * \brief Twpath class members are auto evaluating.
 * 
 * The intention for this class is to have GENERAL functions that can
 *   be used in different types of problems. Therefore is strongly
 *   recommended that especific problem functions be coded in the
 *   problems vehicle
 *
 * \warning prefix: e_ performs the operation and evaluates
 * \warning prefix: ef_ performs the operation only if the resulting
 *   path is feasable
 * \warning prefix: e__ performs the operation on especific problems and
 *   eventually shall be removed
 * 
 * \note All members return \b true when the operation is succesfull
 * 
 * Twpath also inherits all the non evaluating methods of \ref TwBucket.
 *  
 * A path is an ordered sequence of nodes from starting site to ending site.
 * The problem will define which type of nodes belongs to the twpath and
 * which shall be outside twpath.
 *
 * \sa \ref TwBucket a non evaluating container for nodes
 */

class Vehicle {
 protected:
     ID m_id;
     std::deque< Vehicle_node > m_path;
     double max_capacity;

 public:
     typedef std::tuple< int, int, int, double, double > Cost;

     Vehicle(ID id, const Vehicle_node &starting_site, const Vehicle_node &ending_site, double max_capacity); 



     /*! @name deque like functions 

       \returns True if the operation was performed
       \warning Assertions are performed for out of range operations
       \warning no feasability nor time window or capacity violations 
       checks are performed
       \todo TODO more deque like functions here 
       */

     /*! \brief Invariant
      * The path must:
      *   - have at least 2 nodes
      *   - first node of the path must be Start node
      *   - last node of the path must be End node
      *
      * path: S ..... E
      */
     void invariant() const;


     /// @{

     /*! \brief Insert \bnode at \pos position.
      *
      * \param[in] at The position that the node should be inserted.
      * \param[in] node The node to insert.
      *
      */
     void insert(POS pos, Vehicle_node node);


     /*! \brief Insert \bnode in bes position of the \position_limits.
      *
      * \param[in] position_limits.
      * \param[in] node The node to insert.
      *
      * @returns position where it was inserted
      */
     POS insert(std::pair<POS, POS> position_limits, const Vehicle_node &node);





     /*! \brief Evaluated: push_back a node to the path.
      *
      * before: S <nodes> E
      * after:  S <nodes> n E
      *
      * @param[in] node to be push_back.
      */
     void push_back(const Vehicle_node &node);

     /*! \brief Evaluated: push_back a node to the path.
      *
      * before: S <nodes> E
      * after:  S n <nodes> E
      *
      * \param[in] node to be push_back.
      */
     void push_front(const Vehicle_node &node);


     /*! \brief Evaluated: pop_back a node to the path.
      *
      * before:  S <nodes> n E
      * after: S <nodes> E
      *
      * \param[in] node to be pop_back.
      */
     void pop_back();

     /*! \brief Evaluated: pop_front a node to the path.
      *
      * before: S n <nodes> E
      * after:  S <nodes> E
      *
      * \param[in] node to be pop_front.
      */
     void pop_front();

     /*! @brief Erase node.id()
      *
      * @note start and ending nodes cannot be erased
      *
      * Numbers are positions
      * before: S .... node.id() .... E
      * after: S ....  .... E
      *
      */
     void erase(const Vehicle_node &node);



     /*! @brief Erase node at `pos` from the path.
      *
      * @note start and ending nodes cannot be erased
      *
      * Numbers are positions
      * before: S 1 2 3 4 5 6 pos 8 9 E
      * after: S 1 2 3 4 5 6 8 9 E
      *
      * @param[in] pos to be erased.
      */
     void erase(POS pos);

     /*! @brief Evaluated: erase node at `pos` from the path.
      *
      * @note start and ending nodes cannot be erased
      *
      *  True: S E
      * False: S <nodes> E
      */
     bool empty() const;

     ID id() const {return m_id;}


     /// @{
     Cost cost() const;
     bool cost_compare(const Cost&, const Cost&) const;

     double duration() const { return m_path.back().departure_time(); };
     double total_wait_time() const { return m_path.back().total_wait_time(); };
     int  twvTot() const {return m_path.back().twvTot();}
     int  cvTot() const {return m_path.back().cvTot();}
     bool has_twv() const {return twvTot() != 0;}
     bool has_cv() const {return cvTot() != 0;}
     bool is_feasable() const {return !(has_twv() || has_cv());}
     /// @}




     /*!
      * \brief Swap two nodes in the path.
      *
      * Before: S <nodesA> I <nodesB> J <nodesC> E
      * After: S <nodesA> J <nodesB> I <nodesC> E
      *
      * \param[in] i The position of the first node to swap.
      * \param[in] j The position of the second node to swap.
      */
     void swap(POS i, POS j);


     /*! @name Evaluation 
      *
      *
      *
      * Path evaluation is done incrementally: from a given position to the
      * end of the path, and intermediate values are cached on each node.
      * So, for example, changing the path at position 100:
      * the evaluation function should be called as
      *  \c evaluate(100,maxcapacity)
      * and from that position to the end of the path will be evaluated.
      * None of the "unaffected" positions get reevaluated
      *
      *
      *
      */

     ///@{

     /*! \brief Evaluate: Evaluate the whole path from the start.  */
     void evaluate();

     /*! \brief Evaluate: Evaluate a path from the given position.
      *
      * \param[in] from The starting position in the path for evaluation to
      * the end of the path.
      */
     void evaluate(POS from);

     ///@}

     double deltaTime(const Vehicle_node &node, POS pos) const;
     POS insert_less_travel_time(const Vehicle_node &node, POS after_pos = 0);



     /*! @name accessors */
     ///@{

     std::deque< Vehicle_node > path() const;

     ///@}

     /*! @name operators */
     ///@{


     friend std::ostream& operator<<(std::ostream &log, const Vehicle &v);

     std::string tau() const;

     friend bool operator<(const Vehicle &lhs, const Vehicle &rhs);

     ///@}



     std::pair<POS, POS> position_limits(const Vehicle_node node) const;
    private:
     POS getPosLowLimit(const Vehicle_node &node) const;
     POS getPosHighLimit(const Vehicle_node &node) const;


};
