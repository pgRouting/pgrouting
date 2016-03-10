
#pragma once
#include <string>

#include "./node.h"
extern "C" {
#include "./../../common/src/pgr_types.h"
}

/*! \class Twnode
 * \brief Extends the \ref Node class to create a Node with time window attributes.
 *
 * A Time Window node is a Node with addition attributes and methods to
 * to support Time Windows and to model a more complex Node need in many
 * vehicle routing problems.
 *
 * Most application specific code will extend this class and define the specific
 * values and requirements for \c type and \c streetid.
 *
 */
class Tw_node: public Node {
 public:
     typedef enum {
         kStart = 0, ///< starting site
         kPickup,    ///< pickup site
         kDelivery,  ///< delivery site
         kDump,      ///< dump site, empties truck
         kLoad,      ///< load site, fills the truck
         kEnd        ///< ending site
     } NodeType;

     /** @name accessors */
     ///@{


     /*! \brief Returns the opening time.*/
     inline double opens() const {return m_opens;}

     /*! \brief Returns the closing time.  */
     inline double closes() const {return m_closes;}

     /*! \brief Returns the demand associated with this node.  */
     inline double demand() const {return m_demand;}

     /*!  * \brief Returns the service time for this node.  */
     inline double service_time() const { return m_service_time;}

     /*!  * \brief Returns the service time for this node.  */
     inline NodeType type() const { return m_type;}

     /*!  \brief Returns the length of time between the opening and closeing.  */
     inline double window_length() const {return  m_closes - m_opens;}

     ///@}

     /** @name kind of node */
     ///@{
     
     inline bool is_start() const {return m_type == kStart;}
     inline bool is_pickup() const {return m_type == kPickup;}
     inline bool is_delivery() const {return m_type == kDelivery;}
     inline bool is_dump() const {return m_type == kDump;}
     inline bool is_load() const {return m_type == kLoad;}
     inline bool is_end() const {return m_type == kEnd;}
     std::string type_str() const;

     ///@}



     /*!  * \brief Print the contents of a Twnode object.  */
     friend std::ostream& operator<<(std::ostream &log, const Tw_node &node);

     bool is_valid() const;


     /*! \brief True when \b arrivalTime  is before it \b opens */
     inline bool is_early_arrival(double arrival_time) const {
         return arrival_time < m_opens;
     }

     /*! \brief True when \b arrivalTime  is after it \b closes */
     inline bool is_late_arrival(double arrival_time) const {
         return arrival_time > m_closes;
     }
     /*! \brief True when \b arrivalTime in the time window */
     inline bool is_on_time(double arrival_time) const {
         return !is_early_arrival(arrival_time) && !is_late_arrival(arrival_time);
     }

     bool is_ok_after_visiting(const Tw_node &other) const;

     /** @name document functions */
     ///@{
     /*!
      *  The actual arrival time at \b This node,  given that:
      *  \b this node is visited directly after \b other node
      *   and that the actual arrival time at \b other node was opens(other)
      **/
     double arrival_i_opens_j(const Tw_node &other) const;

     /*!
      * The actual arrival time at \b This node,  given that:
      * \bthis node is visited directly after \b other node
      * and that the actual arrival time at \b other node was closes(other)
      **/
     double arrival_i_closes_j(const Tw_node &other) const;


     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as early as possible from \bother it can arrives to \b this
      */
     bool isCompatibleIJ(const Tw_node &other) const;

     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as late as possible from \bother it can arrives to \b this
      */
     bool isFullyCompatibleIJ(const Tw_node &other) const;

     /*
      * is compatible to arrive to \bthis after visiting \bother
      *  - is fully compatible
      *  - does not have a waiting time when arriving as earliest as possible after
      */
     bool isTightCompatibleIJ(const Tw_node &other) const;


     ///@}



     Tw_node(const Tw_node &other) = default;
     Tw_node(
             ID id,
             Customer_t data,
             NodeType type
            );

     Tw_node(
             ID id,
             int64_t p_original_id,
             double x,
             double y,
             double opens,
             double closes,
             double service_time,
             double demand,
             NodeType type
            );
    
    protected:
     double m_opens;        ///< opening time of the node
     double m_closes;       ///< closing time of the node
     double m_service_time; ///< time it takes to be served
     double m_demand;       ///< The demand for the Node
     NodeType m_type;         ///< The demand for the Node


};
