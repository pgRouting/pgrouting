#ifndef SRC_KSP_SRC_BASEPATH_H_
#define SRC_KSP_SRC_BASEPATH_H_


#include <string>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>


#include "BaseEdge.h"
#include "BaseVertex.h"



/**************************************************************************/
//!  BasePath    (deque of BaseEdges with auto weight update)
/*!
\author Dave Potts
\author Vicky Vergara
\date Feb/2015
\copyright GNU General Public License, version 2
\details  Class to store a path.
********************************************************************** */
class BasePath {
 protected:
        double m_dWeight; //! weight of the path
        std::deque<BaseEdge> m_vtEdgesList;

 public:
        BasePath() :m_dWeight(0) {
                m_vtEdgesList.clear();
        }


        /** @name accessors */
        ///@{

        //! Returns the weight of the path 
        double Weight() const { return m_dWeight;}
        //! Returns the size of the path 
        POS size() const { return m_vtEdgesList.size();}
        //! Returns True when size==0
        bool isEmpty() const { return size() == 0;}
        //! Returns the EdgeId of edge at position i 
        int getEdgeID(POS i) const { 
	    assert(i < m_vtEdgesList.size());
            return m_vtEdgesList[i].ID();
        }
        //! Returns the Edge's original Id of edge at position i 
        int getEdgeOriginalID(POS i) const {
            assert(i < m_vtEdgesList.size());
            return m_vtEdgesList[i].originalID();
        }
        //! Returns a copy of edge at position i 
        BaseEdge GetEdge(POS i) const {
            assert(i < m_vtEdgesList.size());
            return m_vtEdgesList.at(i);
        }
        //! Returns a copy of the ith edge of the path
        BaseEdge operator[](POS i) const {
		assert(i < m_vtEdgesList.size());
                return (m_vtEdgesList[i]);
        }
        //! Returns true when path goes fromId -> toId (local id's)
        bool FromTo(POS fromId, POS toId) const {
             if (size() == 0) return false;
             return fromId == m_vtEdgesList[0].getStart()
                    && toId == m_vtEdgesList[ size()-1 ].getEnd();
        }
        //! Returns true when this path shares the same edges as the largerPath
        bool isEqual(const  BasePath &largerPath) const {
            if (size() > largerPath.size()) return false;
            for (POS i = 0 ; i < size() ; i++) {
                if (m_vtEdgesList[i].ID() != largerPath.m_vtEdgesList[i].ID()) return false;
            }
            return true;
        }

        bool EdgesLessComapre(const BasePath &p2) const {
              POS limit = (size() < p2.size()) ? size() : p2.size();
              for (POS i = 0 ; i < limit; i++) {
                   if (m_vtEdgesList[i].ID() < p2.m_vtEdgesList[i].ID())
                       return true;
              }
              return false;
        }
        ///@}

        /** @name mutators */
        ///@{

        //! Changes the path's weight to be: val
        void Weight(double val) { m_dWeight = val;}
        //! Adds an edge at the end of the path updating the corresponding weight of the path
        void push_back(BaseEdge edge) {
            m_vtEdgesList.push_back(edge);
            m_dWeight += edge.Weight();
        }
        //! Adds an edge at the front of the path updating the corresponding weight of the path
        void push_front(BaseEdge edge) {
            m_vtEdgesList.push_front(edge);
            m_dWeight += edge.Weight();
        }
        //! Adds a path at the end of this path, updates the weight of the path
        void append(const BasePath &trail) {
            for (POS i=0; i < trail.size(); i++)
                push_back(trail.m_vtEdgesList[i]);
        }
        //! Deletes all edges of the path, and sets the wigth to 0
        void clear() {
            m_vtEdgesList.clear();
            m_dWeight = 0;
        }
        ///@}

        
        //! returns true & a subPath from the begining of the path with upTo edges
        /*!
          returns false and an empty subpath when
          the path is empty
          its requierd more elements than it has
        */
        bool subPath(BasePath &sub_path, POS upTo) {
                sub_path.clear();
                if (m_vtEdgesList.size() == 0) return false;
                if (upTo >= size()) return false;
                for (POS i = 0; i < upTo; i++) {
                   sub_path.push_back(m_vtEdgesList[i]);
                }
                if (sub_path.size() == upTo) return true;
                sub_path.clear();
                return false;
        }



        // display the content
        void PrintOut(std::ostream& out_stream) const {
                out_stream << "Cost: " << m_dWeight << " Length: " << m_vtEdgesList.size() << std::endl;
                for (POS i = 0; i < m_vtEdgesList.size(); i++) {
                        m_vtEdgesList[i].PrintOut(out_stream);
                        out_stream << "->";
                }
                out_stream << std::endl
                  <<  "*********************************************" << std::endl;
        }
};

#endif  // SRC_KSP_SRC_BASEPATH_H_
