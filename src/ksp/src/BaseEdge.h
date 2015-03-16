#ifndef SRC_KSP_SRC_BASEEDGE_H_
#define SRC_KSP_SRC_BASEEDGE_H_

#include <iostream>
#include "./vrp_assert.h"

/**************************************************************************/
//! BaseEdge    (fromVertex, toVertex, Weight)
/*!
\author Vicky Vergara
\date Feb/2015
\copyright GNU General Public License, version 2
\details  Class to store an edge of a graph.
\sa BaseVertex
********************************************************************** */
class BaseEdge {
 public:
        BaseEdge(UINT id, long  originalId, long start, long end, double weight)
          : m_Start(start), m_End(end),
            m_Weight(weight),
            m_originalID(originalId), m_ID(id),
            m_active(true), m_Main(true)
        {}
        /*! \class compEdge
         * \brief An Edge comparison class for ordering BaseEdge in a set by weight.
        */
        class compBaseEdge {
         public:
          bool operator()(const BaseEdge *e1, const BaseEdge *e2) const {
            return (e1->m_Weight < e2->m_Weight)
                   || ((e1->m_Weight == e2->m_Weight) && (e1->m_ID < e2->m_ID));
            }
        };
        /*! \class compBestEdge
         * \brief An Edge comparison class for keeping the best parallel BaseEdge
        */
        class compBestEdge {
         public:
          bool operator()(const BaseEdge *e1, const BaseEdge *e2) const {
            if ( e1->m_Start <  e2->m_Start) return true;
            if ((e1->m_Start ==  e2->m_Start) && (e1->m_End <  e2->m_End))
                return true;
            return false;
          }
        };
        /** @name accessors */
        ///@{

        //! Returns the original ID of the edge
        long originalID() const { return m_originalID;}
        //! Returns the graph's ID of the edge
        UINT ID() const { return m_ID;}
        //! Returns the graph's ID of the starting vertex of the edge
        UINT  getStart() const { return m_Start;}
        //! Returns the graph's ID of the ending vertex of the edge
        UINT  getEnd() const { return m_End;}
        //! Returns the graph's ID of the weight of the edge
        double Weight() const { return m_Weight;}
        //! Returns true if the edge has being logically removed from the graph
        bool isRemoved() const { return !m_active;}
        //! Returns true if the edge is logically in the graph
        bool isActive() const { return m_active;}
        //! Returns true if the edge is a parallel of the min edge
        bool isParallel() const { return !m_Main;}
        //! Returns true if the edge is the min edge of all the parallels
        bool isMain() const { return m_Main;}
        ///@}

        /** @name mutators */
        ///@{

        //! Logically set the edge as minimal
        void setMain() {m_Main = true;}
        //! Logically set the edge as parallel
        void setParallel() {m_Main = false;}
        //! Logically remove the edge from the graph
        void remove() {m_active = false;}
        //! Logically add the edge to the graph
        void restore() {m_active = true;}
        ///@}

        /** @name debugging Printout */
        ///@{
        void PrintOut(std::ostream& out_stream) const {
            out_stream << "local ID =" << m_ID
                << " ( original ID=" << long(m_originalID) << ", From=" << getStart()
                << ",To=" <<getEnd() << ", Weight=" << Weight()
                << "Active=" << m_active << "Main= "<< m_Main << ")";
        }
        ///@}
 private:
        UINT m_Start;  //!< graph's starting Vertex ID of the edge
        UINT m_End;    //!< graphs's ending vertex ID of the edge
        double m_Weight;  //!< edge's weight
        long m_originalID;  //!< original ID of the edge
        UINT m_ID;     //!< graphs's id of the edge
        bool m_active;   //!< Indicate if edge is logically removed or not
        bool m_Main;   //!< True when its the min edge of all the parallels
};

#endif  // SRC_KSP_SRC_BASEEDGE_H_
