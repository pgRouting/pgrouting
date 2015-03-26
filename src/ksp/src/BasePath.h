#ifndef SRC_KSP_SRC_BASEPATH_H_
#define SRC_KSP_SRC_BASEPATH_H_

#include <string>
#include <deque>
#include <iostream>
#include <limits>

#include "./BaseEdge.h"
#include "./BaseVertex.h"
#include "./vrp_assert.h"

/**************************************************************************/
//!  BasePath    (deque of BaseEdges with auto weight update)
/*!
\author Dave Potts
\author Vicky Vergara
\date Feb/2015
\copyright 2015 GNU General Public License, version 2
\details  Class to store a path.
********************************************************************** */
class BasePath {
 public:
     typedef std::deque<BaseEdge*> eDeque;
     typedef std::deque<BaseEdge*>::const_iterator eDequeIt;

     class compBasePath {
      public:
        bool operator()(const BasePath &p1, const BasePath &p2) const {
          if (p1.m_Weight < p2.m_Weight) return true;
          if (p1.m_Weight > p2.m_Weight) return false;
          // paths weights are equal now we check by length
          if (p1.m_EdgesList.size() < p2.m_EdgesList.size()) return true;
          if (p1.m_EdgesList.size() > p2.m_EdgesList.size()) return false;
          // paths weights & lengths are equal now we check by ID
          eDequeIt et1, et2;
          for (et1 = p1.m_EdgesList.begin(), et2 = p2.m_EdgesList.begin();
               (et1 != p1.m_EdgesList.end()) && (et2 != p2.m_EdgesList.end());
               ++et1, ++et2) {
               if ((*et1)->originalID() < (*et2)->originalID()) return true;
               if ((*et1)->originalID() > (*et2)->originalID()) return false;
          }
          // we got here and everything is equal
          return false;
       }
     };
     BasePath() :m_Weight(0) {
             m_EdgesList.clear();
     }


     /** @name accessors */
     ///@{

     //! returns a reference to the EdgesList
     const eDeque& path() const { return m_EdgesList;}
     std::deque<BaseEdge> Path() const {
          std::deque<BaseEdge> localPath;
          eDequeIt et1;
          for (et1 = m_EdgesList.begin(); et1 != m_EdgesList.end(); ++et1) {
             localPath.push_back(*(*et1));
          }
          return localPath;
     }

     //! Returns the weight of the path
     double Weight() const { return m_Weight;}
     //! Returns the size of the path
     UINT size() const { return m_EdgesList.size();}
     //! Returns True when size==0
     bool isEmpty() const { return size() == 0;}
#if 0
     //! Returns the EdgeId of edge at position i
      getEdgeID(UINT i) const {
         assert(i < m_EdgesList.size());
         return m_EdgesList[i]->ID();
     }
#endif
     //! Returns the Edge's->original Id of edge at position i
     long getEdgeOriginalID(UINT i) const {
         assert(i < m_EdgesList.size());
         return m_EdgesList[i]->originalID();
     }
     //! Returns a copy of edge at position i
     BaseEdge* GetEdge(UINT i) const {
         assert(i < m_EdgesList.size());
         return m_EdgesList.at(i);
     }
     //! Returns a copy of the ith edge of the path
     BaseEdge* operator[](UINT i) const {
         assert(i < m_EdgesList.size());
         return (m_EdgesList[i]);
     }
     //! Returns true when path goes fromId -> toId (local id's)
     bool FromTo(UINT fromId, UINT toId) const {
          if (size() == 0) return false;
          return fromId == m_EdgesList[0]->getStart()
                 && toId == m_EdgesList[ size()-1 ]->getEnd();
     }
     //! Returns true when this path shares the vertices as the this path
     bool isEqual(const  BasePath &largerPath) const {
         if (size() > largerPath.size()) return false;
          eDequeIt et1, et2;
          for (et1 = m_EdgesList.begin(), et2 = largerPath.m_EdgesList.begin();
              (et1 != m_EdgesList.end()) && (et2 != largerPath.m_EdgesList.end());
               ++et1, ++et2) {
               if ((*et1)->getStart() != (*et2)->getStart()) return false;
               if ((*et1)->getEnd() != (*et2)->getEnd()) return false;
          }
         return true;
     }
     //! Special comparison function for Yen's algorithm
     bool EdgesLessComapre(const BasePath &p2) const {
          eDequeIt et1, et2;
          for (et1 = m_EdgesList.begin(), et2 = p2.m_EdgesList.begin();
               (et1 != m_EdgesList.end()) && (et2 != p2.m_EdgesList.end());
               ++et1, ++et2) {
               if ((*et1)->ID() < (*et2)->ID()) return false;
          }
           return false;
     }
     ///@}

     /** @name mutators */
     ///@{

     //! Changes the path's weight to be: val
     void Weight(double val) {m_Weight = val;}
     //! Adds an edge at the end of the path updating the corresponding weight of the path
     void push_back(BaseEdge *edge) {
         if (size() > 0) assert(m_EdgesList.back()->getEnd() == edge->getStart());
         m_EdgesList.push_back(edge);
         m_Weight += edge->Weight();
     }
     //! Adds an edge at the front of the path updating the corresponding weight of the path
     void push_front(BaseEdge *edge) {
         if (size() > 0) assert(m_EdgesList.front()->getStart() == edge->getEnd());
         m_EdgesList.push_front(edge);
         m_Weight += edge->Weight();
     }
     //! Adds a path at the end of this path, updates the weight of the path
     void append(const BasePath &trail) {
         if (size() > 0) assert(m_EdgesList.back()->getEnd() == trail.m_EdgesList.front()->getStart());
         m_Weight += trail.m_Weight;
         m_EdgesList.insert(m_EdgesList.end(), trail.m_EdgesList.begin(), trail.m_EdgesList.end());
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
     bool subPath(BasePath &sub_path, UINT upTo) const {
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




     /** @name mutators */
     ///@{

     //! display the content
     void PrintOut(std::ostream& out_stream) const {
             out_stream << "Cost: " << m_Weight << " Length: "
                        << m_EdgesList.size() << std::endl;
             for (UINT i = 0; i < m_EdgesList.size(); i++) {
                     out_stream << m_EdgesList[i]->originalID();
                     out_stream << "->";
             }
             out_stream << std::endl
               <<  "*****************************" << std::endl;
     }
     ///@}
 protected:
     double m_Weight;     //!< weight of the path
     eDeque m_EdgesList;  //!< edges list of the path
};

#endif  // SRC_KSP_SRC_BASEPATH_H_
