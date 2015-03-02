#ifndef SRC_KSP_SRC_BASEVERTEX_H_
#define SRC_KSP_SRC_BASEVERTEX_H_


#include <string>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>

#include "BaseEdge.h"


/**************************************************************************
*  BaseVertex
*  <TODO: insert class description here>
*
*
*  @remarks <TODO: insert remarks here>
*
*  @author Yan Qi @date 6/6/2010
*  @modified Vicky Vergara @date Feb/2015
**************************************************************************/
class BaseVertex {
        int m_originalID;
        POS m_nNID;
        double m_dWeight;
        std::deque< POS > m_FaninEdges;
        std::deque< POS > m_FanoutEdges;
        bool m_active;   // to indicate if its removed or not
        bool m_visited;  // to indicate if iwe have visited the node

 public:
        BaseVertex(unsigned int nid_, int id_, double weight_)
           : m_originalID(id_),
             m_nNID(nid_),
             m_dWeight(weight_),
             m_active(true),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }

        BaseVertex(unsigned int nid_, int id_)
           : m_originalID(id_),
             m_nNID(nid_),
             m_dWeight(std::numeric_limits<double>::max()),
             m_active(true),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }
        BaseVertex()
           : m_originalID(-1),
             m_nNID(0),
             m_dWeight(std::numeric_limits<double>::max()),
             m_active(false),
             m_visited(false) {
          m_FaninEdges.clear();
          m_FanoutEdges.clear();
        }

        int getOriginalID() const { return m_originalID;}
        POS getNID() const { return m_nNID;}
        void setOriginalID(int ID_) { m_originalID = ID_;}
        void setNID(int NID_) { m_nNID = NID_;}
        const std::deque< POS > getFanIn() const  {return m_FaninEdges;}
        const std::deque< POS > getFanOut() const {return m_FanoutEdges;}


        void push_FanIn(POS edge) { m_FaninEdges.push_back(edge);}
        void push_FanOut(POS edge) { m_FanoutEdges.push_back(edge);}
        double Weight() const { return m_dWeight;}
        void Weight(double val) {  m_dWeight = (m_dWeight > val)? val : m_dWeight;}
        bool isRemoved() const { return !m_active;}
        bool isActive() const { return m_active;}
        void reInsert() {m_active = true;}
        void remove() {m_active = false;}
        void restore() {
             m_active = true;
             m_dWeight = std::numeric_limits<double>::max();
             m_visited = false;
        }


        bool visited() const { return m_visited;}
        void unVisit() {m_visited = false;}
        void setAsVisited() {m_visited = true;}

        void clear() {
           m_FaninEdges.clear();
           m_FanoutEdges.clear();
        }
        void PrintOut(std::ostream& out_stream) const {
             out_stream << "local ID" << m_nNID
                 << "( original ID=" << m_originalID << ", Weight=" << m_dWeight
                 << ", Active=" << m_active
                 << ", Visited=" << m_visited << ")";
        }
        void PrintOut(std::ostream& out_stream, const std::deque<BaseEdge> &edgesTable) const {
                out_stream << "local ID" << m_nNID<< "( original ID=" << m_originalID
                    << ", Weight=" << m_dWeight << ", Active=" << m_active
                    << ", Visited=" << m_visited << ")";
                out_stream << "\n In comming Edges\n";
                for (POS i = 0; i < m_FaninEdges.size(); i++) {
                   edgesTable[m_FaninEdges[i]].PrintOut(out_stream);
                   out_stream << "\n";
                }
                out_stream << "Out Going Edges\n";
                for (POS i = 0; i < m_FanoutEdges.size(); i++) {
                   edgesTable[m_FanoutEdges[i]].PrintOut(out_stream);
                   out_stream << "\n";
                }
        }
};


#endif  // SRC_KSP_SRC_BASEVERTEX_H_
