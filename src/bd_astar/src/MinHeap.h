/*PGR-MIT*****************************************************************

* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/

#ifndef SRC_BD_ASTAR_SRC_MINHEAP_H_
#define SRC_BD_ASTAR_SRC_MINHEAP_H_
#pragma once

#include <utility>

typedef std::pair<double, int> PDI;

class MinHeap {
 public:
     explicit MinHeap(int maxNode);
     ~MinHeap(void);

     bool reset(void);
     bool init(int maxNode);
     void push(PDI node);
     PDI top();
     void pop();
     bool empty();

 private:
     void shift_up(int node);
     void shift_down(int node);

 private:
     PDI *m_HeapTree;
     int *m_Index;
     int m_MaxNodeID;
     int m_CurrentSize;
};

#endif  // SRC_BD_ASTAR_SRC_MINHEAP_H_
