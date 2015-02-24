///////////////////////////////////////////////////////////////////////////////
///  GraphElements.h
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 5/28/2010
///
///  $Id: GraphElements.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <deque>
#include <iostream>
#include <limits>


typedef unsigned int POS;

template<class T>
class WeightGreater
{
public:
	// Determine priority.
	bool operator()(const T& a, const T& b) const
	{
		return a.Weight() > b.Weight();
	}

	bool operator()(const T* a, const T* b) const
	{
		return a->Weight() > b->Weight();
	}
};

template<class T>
class WeightLess
{
public:
	// Determine priority.
	bool operator()(const T& a, const T& b) const
	{
		return a.Weight() < b.Weight();
	}

	bool operator()(const T* a, const T* b) const
	{
		return a->Weight() < b->Weight();
	}
};

//////////////////////////////////////////////////////////////////////////
// A class for the object deletion
//////////////////////////////////////////////////////////////////////////
template<class T>
class DeleteFunc
{
public:
	void operator()(const T* it) const
	{
		delete it;
	}
};



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
class BaseEdge;
class BaseVertex
{
	int m_nID;
	unsigned int m_nNID;
	double m_dWeight;
        std::deque< BaseEdge* > m_FaninEdges;
        std::deque< BaseEdge* > m_FanoutEdges;
	bool m_active;   //to indicate if its removed or not
	bool m_visited;   //to indicate if iwe have visited the node

public:

        BaseVertex(unsigned int nid_, int id_, double weight_) 
           : m_nID(id_),
             m_nNID(nid_),
             m_dWeight(weight_),
             m_active(true),
             m_visited(false)
        {
	  m_FaninEdges.clear();
	  m_FanoutEdges.clear();
	}

        BaseVertex(unsigned int nid_, int id_) 
           : m_nID(id_),
             m_nNID(nid_),
             m_dWeight(std::numeric_limits<double>::max()),
             m_active(true),
             m_visited(false)
        {
	  m_FaninEdges.clear();
	  m_FanoutEdges.clear();
	}
        BaseVertex() 
           : m_nID(-1),
             m_nNID(-1),
             m_dWeight(std::numeric_limits<double>::max()),
             m_active(false),
             m_visited(false)
        {
	  m_FaninEdges.clear();
	  m_FanoutEdges.clear();
	}
        
	int getID() const { return m_nID; }
	unsigned int getNID() const { return m_nNID; }
	void setID(int ID_) { m_nID = ID_; }
	void setNID(int NID_) { m_nNID = NID_; }
        const std::deque< BaseEdge* > getFanIn() const  {return m_FaninEdges;}
        const std::deque< BaseEdge* > getFanOut() const {return m_FanoutEdges;}

		
        void push_FanIn(BaseEdge* edge) { m_FaninEdges.push_back(edge);}
        void push_FanOut(BaseEdge* edge) { m_FanoutEdges.push_back(edge); }
	double Weight() const { return m_dWeight; }
	void Weight(double val) {  m_dWeight = m_dWeight > val? val : m_dWeight; }
        bool isRemoved() const { return !m_active; }
        bool isActive() const { return m_active; }
        void reInsert() {m_active=true;}
        void remove() {m_active=false;}

        bool visited() const { return m_visited; }
        void unVisit() {m_visited=false;}
        void visit() {m_visited=true;}

        void clear() {
           m_FaninEdges.clear();
           m_FanoutEdges.clear();
        }
	void PrintOut(std::ostream& out_stream)
	{
		out_stream << m_nID;
	}
};


/**************************************************************************
*  BaseEdge
*  <TODO: insert class description here>
*
*
*  @remarks <TODO: insert remarks here>
*
*  @author Vicky Vergara @date Feb/2015
**************************************************************************/
class BaseEdge
{
        POS m_Start;  // position on the vertices table
        POS m_End;  // position on the vertices table
        int m_nID;
        POS m_nNID;
        double m_dWeight;
	bool m_active;   //to indicate if its removed or not

public:
        BaseEdge(POS nid, int  id_, int start_, int end_, double weight_)
          : m_Start(start_), m_End(end_),
            m_nID(id_), m_nNID(nid),
            m_dWeight(weight_)
        {};
        int getID() const { return m_nID; }
        POS getNID() const { return m_nNID; }
        POS  getStart() const { return m_Start; }
        POS  getEnd() const { return m_End; }
        double Weight() const { return m_dWeight; }
        bool isRemoved() const { return !m_active; }
        bool isActive() const { return m_active; }
        void remove() {m_active=false;}
        void reInsert() {m_active=true;}

        void PrintOut(std::ostream& out_stream)
        {
                out_stream << m_nID<<"("<<getStart()<<","<<getEnd()<<")="<<Weight()<<std::endl;
        }
};


/**************************************************************************
*  BasePath
*  <TODO: insert class description here>
*  edge oriented
*  modified version from the original that is vertex oriented
*
*  @remarks <TODO: insert remarks here>
*
*  @author Yan Qi @date 6/6/2010
*  @modified Vicky Vergara @date Feb/2015
**************************************************************************/
class BasePath
{
protected:

	double m_dWeight;
        int m_start_id;
	std::deque<BaseEdge*> m_vtEdgesList;

public:

        
	BasePath(unsigned int  start_id, unsigned int sink_id)
		:m_dWeight(0),
                 m_start_id(start_id)
	{
		m_vtEdgesList.clear();
	}
	BasePath(unsigned int  start_id)
		:m_dWeight(0),
                 m_start_id(start_id)
	{
		m_vtEdgesList.clear();
	}
	BasePath(const std::deque<BaseEdge*>& edges_list, double weight)
		:m_dWeight(weight),
                 m_start_id(edges_list[0]->getStart())
	{
		m_vtEdgesList.assign(edges_list.begin(), edges_list.end());
	}

	~BasePath(void){}

	double Weight() const { return m_dWeight; }
	void Weight(double val) { m_dWeight = val; }

	POS size() const { return m_vtEdgesList.size(); }
	bool isEmpty() const { return size()==0; }

        int getID(POS i) const {return m_vtEdgesList[i]->getID();} 
	int GetVertex(int i)
	{
		return m_vtEdgesList.at(i)->getStart();
	}

	BaseEdge* GetEdge(POS i)
	{
		return m_vtEdgesList.at(i);
	};

	const BaseEdge* operator[] (POS i) const
	{
		return m_vtEdgesList.at(i);
	}

	BaseEdge* operator[] (POS i) 
	{
		return m_vtEdgesList.at(i);
	}

        bool FromTo (POS from, POS to ) const {
             return from == m_vtEdgesList[0]->getStart()
                    && to == m_vtEdgesList[ size()-1 ]->getEnd();
        }

       bool isEqual(const  BasePath &largerPath ) const {
            if (size() > largerPath.size()) return false;
            for (POS i = 0 ; i < size()-1 ; i++ ) {
                if (! m_vtEdgesList[i]->getID() == largerPath.m_vtEdgesList[i]->getID()) return false;
            }
            return true;
       };


        bool EdgesLessComapre( const BasePath &p2 ) const {
              POS limit = (size() < p2.size()) ? size() : p2.size();
              for (POS i = 0 ; i < limit; i++ ) {
                   if (m_vtEdgesList[i]->getID() < p2.m_vtEdgesList[i]->getID())
                       return true;
              }
              return false;
         };

        void push_back( BaseEdge * edge) {
            if (size() == 0) m_start_id = edge->getStart();
            m_vtEdgesList.push_back(edge);
            m_dWeight += edge->Weight();
        }

        void clear( ) { 
            m_vtEdgesList.clear();
            m_dWeight=0;
        }
 
        //retuns true & a subPath from the begining of the path with upTo edges
        //        (number of nodes is upTo +1
        //returns false and an empty subpath when
        //  the path is empty
        //  its requierd more elements than it has

	bool subPath(BasePath &sub_path, POS upTo )
	{
                if (m_vtEdgesList.size()==0) return false;
                if (upTo >= size()) return false;
                sub_path.m_start_id = m_start_id;
		for (POS i=1; i<upTo; i++)
		{
		   sub_path.push_back( m_vtEdgesList[i] );
		}
	        if (sub_path.size() == upTo) return true;
                sub_path.clear();
		return false;
	}

        void append (const BasePath &trail) {
	    for (POS i=0; i < trail.size(); i++)
	        push_back( trail.m_vtEdgesList[i] );
        }


	// display the content
	void PrintOut(std::ostream& out_stream) const
	{
		out_stream << "Cost: " << m_dWeight << " Length: " << m_vtEdgesList.size() << std::endl;
		for(std::deque<BaseEdge*>::const_iterator pos=m_vtEdgesList.begin(); pos!=m_vtEdgesList.end();++pos)
		{
			(*pos)->PrintOut(out_stream);
			out_stream << "->";
		}
		out_stream << std::endl <<  "*********************************************" << std::endl;
	}
};

