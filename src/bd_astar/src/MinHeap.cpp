/*PGR-MIT*****************************************************************

* $Id$
*
* Project: pgRouting bdsp and bdastar algorithms
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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/

#include "MinHeap.h"

MinHeap::MinHeap(int maxNode) {
    init(maxNode);
}

MinHeap::~MinHeap(void) {
    reset();
}

bool MinHeap::reset(void) {
    if (m_HeapTree != NULL) {
        delete [] m_HeapTree;
        m_HeapTree = NULL;
    }
    if (m_Index != NULL) {
        delete [] m_Index;
        m_Index = NULL;
    }
    return true;
}

bool MinHeap::init(int maxNode) {
    m_HeapTree = new PDI[maxNode + 1];
    m_Index = new int[maxNode + 1];
    memset(m_Index, -1, sizeof(int) * (maxNode + 1));
    m_MaxNodeID = maxNode;
    m_CurrentSize = 0;
    return true;
}

void MinHeap::push(PDI node) {
    int nid = node.second;
    double ncost = node.first;
    if (m_Index[nid] != -1) {
        int pos = m_Index[nid];
        if (ncost < m_HeapTree[pos].first) {
            m_HeapTree[pos].first = ncost;
            shift_up(pos);
        } else {
            return;
        }
    } else {
        m_CurrentSize++;
        m_HeapTree[m_CurrentSize] = node;
        m_Index[nid] = m_CurrentSize;
        shift_up(m_CurrentSize);
    }
}

PDI MinHeap::top(void) {
    return(m_HeapTree[1]);
}

bool MinHeap::empty(void) {
    if (m_CurrentSize == 0)
        return true;
    return false;
}

void MinHeap::pop(void) {
    if (m_CurrentSize == 0)
        return;
    int nid = m_HeapTree[1].second;
    m_Index[nid] = -1;
    m_HeapTree[1] = m_HeapTree[m_CurrentSize];
    m_Index[m_HeapTree[1].second] = 1;
    m_CurrentSize--;
    shift_down(1);
}

void MinHeap::shift_up(int node) {
    while (node > 1) {
        int par_node = node / 2;
        if (m_HeapTree[par_node].first <= m_HeapTree[node].first) {
            return;
        }
        PDI temp = m_HeapTree[par_node];
        m_HeapTree[par_node] = m_HeapTree[node];
        m_HeapTree[node] = temp;
        m_Index[m_HeapTree[node].second] = node;
        m_Index[m_HeapTree[par_node].second] = par_node;
        node = par_node;
    }
    return;
}

void MinHeap::shift_down(int node) {
    while (node < m_CurrentSize) {
        int left_child = node * 2;
        int right_child = node * 2 + 1;
        if (left_child > m_CurrentSize) {
            return;
        }
        int smallest = node;
        if (m_HeapTree[left_child] < m_HeapTree[smallest]) {
            smallest = left_child;
        }
        if (right_child <= m_CurrentSize) {
            if (m_HeapTree[right_child] < m_HeapTree[smallest]) {
                smallest = right_child;
            }
        }
        if (node == smallest)
            return;
        PDI temp = m_HeapTree[node];
        m_HeapTree[node] = m_HeapTree[smallest];
        m_HeapTree[smallest] = temp;
        m_Index[m_HeapTree[node].second] = node;
        m_Index[m_HeapTree[smallest].second] = smallest;
        node = smallest;
    }
    return;
}
