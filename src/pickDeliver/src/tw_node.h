
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


     inline void set_Pid(ID id) {
         assert (is_delivery());
         m_otherid = id;
     }

     inline void set_Did(ID id) {
         assert (is_pickup());
         m_otherid = id;
     }

     inline ID Did() const {
         assert (is_pickup());
         return m_otherid;
     }

     inline ID Pid() const {
         assert (is_delivery());
         return m_otherid;
     }


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

     /*!  * \brief Returns the type of this node.  */
     inline NodeType type() const { return m_type;}

     /*!  \brief Returns the length of time between the opening and closeing.  */
     inline double window_length() const {return  m_closes - m_opens;}

     ///@}

     /** @name kind of node 
      *
      * A true value when;
      *
      *   - 0 < opens < closes
      *   - the type is the requested type
      *   - the demand are valid for the requested type
      */

     ///@{

     
     /*!  \brief is_start
      *
      * To be a start node:
      *   - type is kStart
      *   - demand == 0
      *
      **/
     bool is_start() const;


     /*!  \brief is_pickup
      *
      * To be a pickup node:
      *   - type is kPickup
      *   - demand > 0
      *
      **/
     bool is_pickup() const;


     /*!  \brief is_delivery
      *
      * To be a delivery node:
      *   - type is kDelivery
      *   - demand < 0
      *
      **/
     bool is_delivery() const;


     /*!  \brief is_dump
      *
      * To be a dump node:
      *   - type is kDump
      *   - demand <= 0
      *
      **/
     bool is_dump() const;


     /*!  \brief is_Load
      *
      * To be a Load node:
      *   - type is kLoad
      *   - demand >= 0
      *
      **/
     bool is_load() const;


     /*!  \brief is_end
      *
      * To be a End node:
      *   - type is kEnd
      *   - demand == 0
      *
      **/
     bool is_end() const;


     std::string type_str() const;

     ///@}



     /*!  * \brief Print the contents of a Twnode object.  */
     friend std::ostream& operator<<(std::ostream &log, const Tw_node &node);

     bool operator==(const Tw_node &rhs) const;



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





     /** @name document functions */
     ///@{
     /*!
      *  The actual arrival time at \b This node,  given that:
      *  \b this node is visited directly after \b other node
      *   and that the actual arrival time at \b other node was opens(other)
      **/
     double arrival_j_opens_i(const Tw_node &I) const;

     /*!
      * The actual arrival time at \b This node,  given that:
      * \bthis node is visited directly after \b other node
      * and that the actual arrival time at \b other node was closes(other)
      **/
     double arrival_j_closes_i(const Tw_node &I) const;


     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as early as possible from \bother it can arrives to \b this
      */
     bool is_compatible_IJ(const Tw_node &I) const;

     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as late as possible from \bother it can arrives to \b this
      */
     bool is_partially_compatible_IJ(const Tw_node &I) const;

     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as late as possible from \bother it can arrives to \b this
      */
     bool is_tight_compatible_IJ(const Tw_node &I) const;

     /*
      * is possible to arrive to \bthis after visiting \bother
      *   - departing as late as possible from \bother it can arrives to \b this
      */
     bool is_partially_waitTime_compatible_IJ(const Tw_node &I) const;

     /*
      * is compatible to arrive to \bthis after visiting \bother
      *  - is fully compatible
      *  - does not have a waiting time when arriving as earliest as possible after
      */
     bool is_waitTime_compatible_IJ(const Tw_node &I) const;


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
     bool is_valid() const;

     double m_opens;        ///< opening time of the node
     double m_closes;       ///< closing time of the node
     double m_service_time; ///< time it takes to be served
     double m_demand;       ///< The demand for the Node
     ID m_otherid;          ///< the other's id
     NodeType m_type;       ///< The demand for the Node
};
