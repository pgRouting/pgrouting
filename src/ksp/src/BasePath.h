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
        double m_Weight; //! weight of the path
        std::deque<BaseEdge> m_EdgesList;

 public:

        class compBasePath {
         public:
           bool operator()(const BasePath &p1, const BasePath &p2) const {
             if (p1.m_Weight < p2.m_Weight) return true;
             if (p1.m_Weight > p2.m_Weight) return false;
             //paths weights are equal now we check by length
             if (p1.m_EdgesList.size() < p2.m_EdgesList.size()) return true;
             if (p1.m_EdgesList.size() > p2.m_EdgesList.size()) return false;
             //paths lengths are equal now we check by edges id's
             for (UINT i = 0; i < p1.m_EdgesList.size(); i++) {
                if (p1.m_EdgesList[i].ID() < p2.m_EdgesList[i].ID()) return true;
                if (p1.m_EdgesList[i].ID() > p2.m_EdgesList[i].ID()) return false;
             }
             // we got here and everything is equal
             return false;
          };
        };
        BasePath() :m_Weight(0) {
                m_EdgesList.clear();
        }


        /** @name accessors */
        ///@{

        //! Returns the weight of the path 
        double Weight() const { return m_Weight;}
        //! Returns the size of the path 
        UINT size() const { return m_EdgesList.size();}
        //! Returns True when size==0
        bool isEmpty() const { return size() == 0;}
        //! Returns the EdgeId of edge at position i 
        int getEdgeID(UINT i) const { 
	    assert(i < m_EdgesList.size());
            return m_EdgesList[i].ID();
        }
        //! Returns the Edge's original Id of edge at position i 
        int getEdgeOriginalID(UINT i) const {
            assert(i < m_EdgesList.size());
            return m_EdgesList[i].originalID();
        }
        //! Returns a copy of edge at position i 
        BaseEdge GetEdge(UINT i) const {
            assert(i < m_EdgesList.size());
            return m_EdgesList.at(i);
        }
        //! Returns a copy of the ith edge of the path
        BaseEdge operator[](UINT i) const {
		assert(i < m_EdgesList.size());
                return (m_EdgesList[i]);
        }
        //! Returns true when path goes fromId -> toId (local id's)
        bool FromTo(UINT fromId, UINT toId) const {
             if (size() == 0) return false;
             return fromId == m_EdgesList[0].getStart()
                    && toId == m_EdgesList[ size()-1 ].getEnd();
        }
        //! Returns true when this path shares the same edges as the largerPath
        bool isEqual(const  BasePath &largerPath) const {
            if (size() > largerPath.size()) return false;
            for (UINT i = 0 ; i < size() ; i++) {
                if (m_EdgesList[i].ID() != largerPath.m_EdgesList[i].ID()) return false;
            }
            return true;
        }

        bool EdgesLessComapre(const BasePath &p2) const {
              UINT limit = (size() < p2.size()) ? size() : p2.size();
              for (UINT i = 0 ; i < limit; i++) {
                   if (m_EdgesList[i].ID() < p2.m_EdgesList[i].ID())
                       return true;
              }
              return false;
        }
        ///@}

        /** @name mutators */
        ///@{

        //! Changes the path's weight to be: val
        void Weight(double val) { m_Weight = val;}
        //! Adds an edge at the end of the path updating the corresponding weight of the path
        void push_back(BaseEdge edge) {
            m_EdgesList.push_back(edge);
            m_Weight += edge.Weight();
        }
        //! Adds an edge at the front of the path updating the corresponding weight of the path
        void push_front(BaseEdge edge) {
            m_EdgesList.push_front(edge);
            m_Weight += edge.Weight();
        }
        //! Adds a path at the end of this path, updates the weight of the path
        void append(const BasePath &trail) {
            for (UINT i=0; i < trail.size(); i++)
                push_back(trail.m_EdgesList[i]);
        }
        //! Deletes all edges of the path, and sets the wigth to 0
        void clear() {
            m_EdgesList.clear();
            m_Weight = 0;
        }
        ///@}

        
        //! returns true & a subPath from the begining of the path with upTo edges
        /*!
          returns false and an empty subpath when
          the path is empty
          its requierd more elements than it has
        */
        bool subPath(BasePath &sub_path, UINT upTo) {
                sub_path.clear();
                if (m_EdgesList.size() == 0) return false;
                if (upTo >= size()) return false;
                for (UINT i = 0; i < upTo; i++) {
                   sub_path.push_back(m_EdgesList[i]);
                }
                if (sub_path.size() == upTo) return true;
                sub_path.clear();
                return false;
        }



        // display the content
        void PrintOut(std::ostream& out_stream) const {
                out_stream << "Cost: " << m_Weight << " Length: " << m_EdgesList.size() << std::endl;
                for (UINT i = 0; i < m_EdgesList.size(); i++) {
                        out_stream << m_EdgesList[i].originalID();
                        //m_EdgesList[i].PrintOut(out_stream);
                        out_stream << "->";
                }
                out_stream << std::endl
                  <<  "*********************************************" << std::endl;
        }
};

#endif  // SRC_KSP_SRC_BASEPATH_H_
