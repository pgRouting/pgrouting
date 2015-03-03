#ifndef SRC_KSP_SRC_BASEVERTEX_H_
#define SRC_KSP_SRC_BASEVERTEX_H_


#include <string>
#include <cassert>
#include <set>
#include <deque>
#include <iostream>
#include <limits>

#include "BaseEdge.h"


/**************************************************************************/
//!  BaseVertex    (vertexId, FanInEdges, FanoutEdges)
/*!
\author Dave Potts
\author Vicky Vergara
\date Feb/2015
\copyright GNU General Public License, version 2
\details  Class to store a vertex with FanIn & FanOut Edges.
********************************************************************** */
class BaseVertex {
        int m_originalID;
        UINT m_ID;
        double m_Weight;
        std::set< BaseEdge*, BaseEdge::compBaseEdge> m_FaninEdges;
        std::set< BaseEdge*,  BaseEdge::compBaseEdge> m_FanoutEdges;
        bool m_active;   // to indicate if its removed or not
        bool m_visited;  // to indicate if iwe have visited the node

 public:
        BaseVertex(unsigned int nid_, int id_, double weight_)
           : m_originalID(id_),
             m_ID(nid_),
             m_Weight(weight_),
             m_active(true),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }

        BaseVertex(unsigned int nid_, int id_)
           : m_originalID(id_),
             m_ID(nid_),
             m_Weight(std::numeric_limits<double>::max()),
             m_active(true),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }
        BaseVertex()
           : m_originalID(-1),
             m_ID(0),
             m_Weight(std::numeric_limits<double>::max()),
             m_active(false),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }

        class compBaseEdge {
         public:
          bool operator()(const BaseVertex *v1, const BaseVertex *v2) const {
            return v1->m_Weight < v2->m_Weight 
                   || (v1->m_Weight == v2->m_Weight && v1->m_ID < v2->m_ID);
            }
        };

        /** @name accessors */
        ///@{

        //! Returns the original id of the vertex
        int getOriginalID() const { return m_originalID;}
        //! Returns the id of the vertex
        UINT ID() const { return m_ID;}
        //! Returns a copy of all Incomming edges (regardles of active or visited)
        const std::deque< BaseEdge* > getFanIn() const  {
            std::deque< BaseEdge* > edges;
            edges.assign(m_FaninEdges.begin(), m_FaninEdges.end());
            return edges;
        }
        //! Returns a copy of all outgoing edges (regardles of active or visited)
        const std::deque< BaseEdge* > getFanOut() const  {
            std::deque< BaseEdge* > edges;
            edges.assign(m_FanoutEdges.begin(), m_FanoutEdges.end());
            return edges;
        }
        //const std::deque< BaseEdge* > getFanOut() const {return m_FanoutEdges;}
        //! Returns true if it has being logically removed from the graph
        bool isRemoved() const { return !m_active;}
        //! Returns true if it logically exists in the graph
        bool isActive() const { return m_active;}
        //! Returns true if it has being visited
        bool visited() const { return m_visited;}
        //! Returns the weight of the vertex (max when its not active)
        double Weight() const {
            if (isRemoved()) return std::numeric_limits<double>::max();
            return m_Weight;
        }
        ///@}

        /** @name mutators */
        ///@{

        //! adds the pointer to the edge as Incomming edge
        void push_FanIn(BaseEdge *edge_pt) { m_FaninEdges.insert(edge_pt);}
        //! adds the pointer to the edge as Outgoing  edge
        void push_FanOut(BaseEdge *edge_pt) { m_FanoutEdges.insert(edge_pt);}
        //! Sets the weight of the vertex as the min value of the actual weight and the new value
        void Weight(double val) {  m_Weight = (m_Weight > val)? val : m_Weight;}
        //! Logically inserts the vertex in the graph
        void reInsert() {m_active = true;}
        //! Logically removes the vertex in the graph
        void remove() {m_active = false;}
        //! Logically sets the vertex as unvisited
        void unVisit() {m_visited = false;}
        //! Logically sets the vertex as visited
        void setAsVisited() {m_visited = true;}
        //! Logically activates, unvisits and sets the Vertex weight to max 
        void restore() {
             m_active = true;
             m_Weight = std::numeric_limits<double>::max();
             m_visited = false;
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
                 << "( original ID=" << m_originalID << ", Weight=" << m_Weight
                 << ", Active=" << m_active
                 << ", Visited=" << m_visited << ")";
        }
        ///@}
};


#endif  // SRC_KSP_SRC_BASEVERTEX_H_
