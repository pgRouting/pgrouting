//  (C) Copyright Jeremy Siek 2004 
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef PGROUTING_TWO_QUEUE_HPP
#define PGROUTING_TWO_QUEUE_HPP

#include <queue>

namespace boost {

template <class _Tp, 
          class _Sequence = std::deque<_Tp> >
class two_queue;

template <class _Tp, class _Seq>
inline bool operator==(const two_queue<_Tp, _Seq>&, const two_queue<_Tp, _Seq>&);

template <class _Tp, class _Seq>
inline bool operator<(const two_queue<_Tp, _Seq>&, const two_queue<_Tp, _Seq>&);


template <class _Tp, class _Sequence>
class two_queue {

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
  template <class _Tp1, class _Seq1>
  friend bool operator== (const two_queue<_Tp1, _Seq1>&,
                          const two_queue<_Tp1, _Seq1>&);
  template <class _Tp1, class _Seq1>
  friend bool operator< (const two_queue<_Tp1, _Seq1>&,
                         const two_queue<_Tp1, _Seq1>&);
#endif
public:
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
protected:
#endif
  _Sequence c,a,b;
public:
  two_queue() : c() {}
  //explicit two_queue(const _Sequence& __c) : c(__c) {}

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
  //void push(const value_type& __x) { a.push_back(__x); }
  void push_a(const value_type& __x) { a.push_back(__x); }
  void push_b(const value_type& __x) { b.push_back(__x); }
  void pop() { 
	if(a.empty()) b.pop_front();
	else a.pop_front(); 
  }

  void swap(two_queue& other)
  {
    using std::swap;
    swap(a, other.a);
    swap(b, other.b);
  }
};

template <class _Tp, class _Sequence>
bool 
operator==(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return __x.c == __y.c;
}

template <class _Tp, class _Sequence>
bool
operator<(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return __x.c < __y.c;
}

template <class _Tp, class _Sequence>
bool
operator!=(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return !(__x == __y);
}

template <class _Tp, class _Sequence>
bool 
operator>(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return __y < __x;
}

template <class _Tp, class _Sequence>
bool 
operator<=(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return !(__y < __x);
}

template <class _Tp, class _Sequence>
bool 
operator>=(const two_queue<_Tp, _Sequence>& __x, const two_queue<_Tp, _Sequence>& __y)
{
  return !(__x < __y);
}

template <class _Tp, class _Sequence>
inline void
swap(two_queue<_Tp, _Sequence>& __x, queue<_Tp, _Sequence>& __y)
{ __x.swap(__y); }

} /* namespace boost */

#endif
