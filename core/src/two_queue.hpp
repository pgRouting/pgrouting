//  (C) Copyright Jinfu Leng 2012 

#ifndef PGROUTING_TWO_QUEUE_HPP
#define PGROUTING_TWO_QUEUE_HPP

#include <queue>

namespace boost {

template <class _Tp, 
          class _Sequence = std::deque<_Tp> >
class two_queue;

template <class _Tp, class _Sequence>
class two_queue {
public:
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;

protected:
  _Sequence a,b;
public:
  two_queue(){}
  bool empty() const { return a.empty()&&b.empty(); }
  size_type size() const { return a.size()+b.size(); }
  reference top() { 
	if(a.empty()) return b.front();
	else return a.front();	
  }
  const_reference top() const {
	if(a.empty()) return b.front();
	else return a.front();	
  }
  void push_a(const value_type& __x) { a.push_back(__x); }
  void push_b(const value_type& __x) { b.push_back(__x); }
  void pop() { 
	if(a.empty()) b.pop_front();
	else a.pop_front(); 
  }

};
} /* namespace boost */




namespace boost {

template <class _Tp, 
          class _DistanceMap,
          class _Compare,
          class _DistZero>
class two_queue_tm;

template <class _Tp, class _DistanceMap, class _Compare, class _DistZero>
class two_queue_tm {
public:
  typedef std::deque<_Tp> _Sequence;
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;

  typedef typename property_traits<_DistanceMap>::value_type D;
protected:
  _Sequence a,b;
  _DistanceMap* pDistance;
  _Compare compare;
  D minDist;
public:
  two_queue_tm(){}
  void setDistance(_DistanceMap* distance){pDistance = distance;}
  void setCompare(_Compare Compare){compare = Compare;}
  void setZero(_DistZero Zero){minDist = Zero;}
  D GetMinDist() {return minDist;}
  void UpdateMinDist() {	
	typename _Sequence::iterator it;
	it = a.begin();
	while (it != a.end()){
		D d = get(*pDistance, *it);
		if(compare(d,minDist)) minDist = d;
		it++;
	}

	it = b.begin();
	while (it != b.end()){
		D d = get(*pDistance, *it);
		if(compare(d,minDist)) minDist = d;
		it++;
	}

  }
  bool empty() const { return a.empty()&&b.empty(); }
  size_type size() const { return a.size()+b.size(); }
  reference top() { 
	_Sequence *sequence;	
	if(a.empty()) sequence = &b;
	else sequence = &a;
	D d = get(*pDistance, sequence->front());
	if(!compare(d,minDist)&&!compare(minDist,d)) UpdateMinDist();
	return sequence->front();
  }
  const_reference top() const {
	_Sequence *sequence;	
	if(a.empty()) sequence = &b;
	else sequence = &a;
	D d = get(*pDistance, sequence->front());
	if(!compare(d,minDist)&&!compare(minDist,d)) UpdateMinDist();
	return sequence->front();
  }
  void push_a(const value_type& __x) { a.push_back(__x); }
  void push_b(const value_type& __x) { b.push_back(__x); }
  void pop() { 
	if(a.empty()) b.pop_front();
	else a.pop_front(); 
  }

};
} /* namespace boost */

#endif
