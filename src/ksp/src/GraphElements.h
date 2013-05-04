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

#ifndef __GraphElements_h
#define __GraphElements_h

#include <string>
#include <iostream>

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
**************************************************************************/
class BaseVertex
{
  size_t m_nID;
  double m_dWeight;

public:

  size_t getID() const { return m_nID; }
  void setID(size_t ID_) { m_nID = ID_; }

  double Weight() const { return m_dWeight; }
  void Weight(double val) { m_dWeight = val; }

  void PrintOut(std::ostream& out_stream)
  {
    out_stream << m_nID;
  }
};
#endif
