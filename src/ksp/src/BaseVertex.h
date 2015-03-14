#ifndef SRC_KSP_SRC_BASEVERTEX_H_
#define SRC_KSP_SRC_BASEVERTEX_H_


#include <string>
#include <set>
#include <deque>
#include <iostream>
#include <limits>
#include "./vrp_assert.h"


/**************************************************************************/
//!  BaseVertex    (vertexId, FanInEdges, FanoutEdges)
/*!
\author Dave Potts
\author Vicky Vergara
\date Feb/2015
\copyright 2015 GNU General Public License, version 2
\details  Class to store a vertex with FanIn & FanOut Edges.
********************************************************************** */
class BaseEdge;
class BaseVertex {
 public:
  typedef typename  std::set< BaseEdge*, BaseEdge::compBaseEdge> eSetPt;
  typedef typename  std::set< BaseEdge*, BaseEdge::compBaseEdge>::iterator eSetPtIt;

        /*! @name constructors
            Copy constructor is the Default
        */
        ///@{

        BaseVertex(UINT nid_, long id_, double weight_)
           : m_originalID(id_),
             m_ID(nid_),
             m_active(true) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }

        BaseVertex(UINT nid_, long id_)
           : m_originalID(id_),
             m_ID(nid_),
             m_active(true) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }
        BaseVertex()
           : m_originalID(-1),
             m_ID(0),
             m_active(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }
        ///@}

        /** @name comparisons */
        ///@{


        //! Order By original ID
        class compBaseVertexID {
         public:
          bool operator()(const BaseVertex *v1, const BaseVertex *v2) const {
            return v1->m_originalID < v2->m_originalID;
            }
        };
        ///@}

        /** @name accessors */
        ///@{

        //! Returns the original id of the vertex
        long getOriginalID() const { return m_originalID;}
        //! Returns the id of the vertex
        UINT ID() const { return m_ID;}
        //! Returns a reference of incomming edges
        const eSetPt& getFanIn() const  {
            return m_FaninEdges;
        }
        //! Returns a reference to outgoing edges
        const eSetPt& getFanOut() const  {
            return m_FanoutEdges;
        }
        //! Returns true if it has being logically removed from the graph
        bool isRemoved() const { return !m_active;}
        //! Returns true if it logically exists in the graph
        bool isActive() const { return m_active;}
        ///@}

        /** @name mutators */
        ///@{

        //! adds the pointer to the edge as Incomming edge
        void push_FanIn(BaseEdge *edge_pt) { m_FaninEdges.insert(edge_pt);}
        //! adds the pointer to the edge as Outgoing  edge
        void push_FanOut(BaseEdge *edge_pt) { m_FanoutEdges.insert(edge_pt);}
        //! Logically inserts the vertex in the graph
        void reInsert() {m_active = true;}
        //! Logically removes the vertex in the graph
        void remove() {m_active = false;}
        //! Logically activates, unvisits and sets the Vertex weight to max
        void restore() {
             m_active = true;
        }
        //! Clears all incomming and outgoing edges of the vertex
        void clear() {
           m_FaninEdges.clear();
           m_FanoutEdges.clear();
        }
        ///@}

        /** @name Debugging */
        ///@{

        void PrintOut(std::ostream& out_stream) const {
             out_stream << "local ID" << m_ID
                 << "( original ID=" << m_originalID
                 << ", Active=" << m_active << ")";
        }
        ///@}

 private:
        long m_originalID;      //!< Original id
        UINT m_ID;             //!< Local id
        eSetPt m_FaninEdges;    //!< Fan in edges
        eSetPt m_FanoutEdges;  //!< Fan out edges
        bool m_active;   //!< to indicate if its removed or not
};


#endif  // SRC_KSP_SRC_BASEVERTEX_H_
