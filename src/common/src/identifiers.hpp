/*PGR-GNU*****************************************************************

FILE: __FILENAME__

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/


#pragma once
#include <set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
template <typename T>
class Identifiers {
 public:
    Identifiers<T>() = default;
    Identifiers<T>(T* container, size_t size) {
        for (size_t i = 0; i < size; ++i)
        {
            m_ids.insert(container[i]);
        }
    }
    const std::set<T>& ids() const;
    size_t size() const { return m_ids.size(); }
    typedef typename std::set<T>::iterator iterator;
    typedef typename std::set<T>::const_iterator const_iterator;
    //! \brief Returns a boolean value true or false, to indicate whether the set is empty
    inline bool empty() const { return m_ids.empty(); }
    inline void clear() { m_ids.clear(); }
    bool has(const T other) const;
    bool isDisjoint(const T other) const;
    bool isDisjoint(const Identifiers<T> &other) const;
    void insert(const Identifiers<T> &other);
    void insert(const T &other);
    iterator begin() { return m_ids.begin(); }
    const_iterator begin() const { return m_ids.begin(); }
    iterator end() { return m_ids.end(); }
    const_iterator end() const { return m_ids.end(); }
    bool operator ==(const Identifiers<T> &other) const;
    const T& operator [](size_t index) const;
    Identifiers<T> operator +(const T &other) const;
    Identifiers<T> operator *(const T &other) const;
    Identifiers<T> operator -(const T &other) const;
    Identifiers<T> operator +(const Identifiers<T> &other) const;
    Identifiers<T> operator *(const Identifiers<T> &other) const;
    Identifiers<T> operator -(const Identifiers<T> &other) const;
    Identifiers<T>& operator +=(const T &other);
    Identifiers<T>& operator *=(const T &other);
    Identifiers<T>& operator -=(const T &other);
    Identifiers<T>& operator +=(const Identifiers<T> &other);
    Identifiers<T>& operator *=(const Identifiers<T> &other);
    Identifiers<T>& operator -=(const Identifiers<T> &other);
    template<T>
    friend std::ostream& operator << (std::ostream& os, const Identifiers<T>& identifiers);
 private:
    std::set<T> m_ids;
};




//! \brief Returns a set of identifiers of type *set<T>*
template <typename T>
const std::set<T>& Identifiers<T>::ids() const {
    return m_ids;
}

//! \brief Returns a boolean value true or false
/*!
  @param [in] other Identifier of type *T*
  Returns:
  true: If other is in this set
  false: Otherwise
  */
template <typename T>
bool Identifiers<T>::has(const T other) const {
    return (m_ids.find(other) != m_ids.end());
}

//! \brief Returns a boolean value true or false
/*!
  @param [in] other is an identifier of type *T*
  Returns:
  true: If other is not in this set
  false: Otherwise
  */
template <typename T>
bool Identifiers<T>::isDisjoint(const T other) const {
    return (m_ids.find(other) == m_ids.end());
}

//! \brief Returns a boolean value true or false
/*!
  @param [in] other is set of identifiers of type *Identifiers<T>*
  Returns:
  true: If other and this set are disjoint
  false: otherwise
  */
template <typename T>
bool Identifiers<T>::isDisjoint(const Identifiers<T> &other) const {
    for (auto identifier : other.ids()) {
        if (!(m_ids.find(identifier) == m_ids.end())) {
            return false;
        }
    }
    return true;
}

//! \brief Inserts a set of identifiers of type *Identifiers<T>* to this set
/*!
  @param [in] other is a set of identifiers
  */
template <typename T>
void Identifiers<T>::insert(const Identifiers<T> &other) {
    m_ids.insert(other.ids().begin(), other.ids().end());
}

//! \brief Inserts an identifier of type *T* to this set
/*!
  @param [in] other is an identifier of type *T*
  */
template <typename T>
void Identifiers<T>::insert(const T &other) {
    m_ids.insert(other);
}

//! \brief Returns a boolean value 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Returns:
  true: If other and this set are equivalent
  false: Otherwise
  */
template <typename T>
bool Identifiers<T>::operator ==(const Identifiers<T> &other) const {
    return std::equal(m_ids.begin(),m_ids.end(),other.ids().begin());
}


//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is an identifier of type *T*
  Returns the set union of other with this set
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator +(const T &other) const {
    Identifiers<T> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is an identifier of type *T*
  Returns the set intersection of other with this set
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator *(const T &other) const {
    Identifiers<T> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids *= other;
    return intersect_ids;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is an identifier of type *T*
  Returns the set difference between this set and other
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator -(const T &other) const {
    Identifiers<T> diff_ids;
    diff_ids.insert(*this);
    diff_ids -= other;
    return diff_ids;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Returns the set union of other with this set
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator +(const Identifiers<T> &other) const {
    Identifiers<T> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Returns the set intersection of other with this set
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator *(const Identifiers<T> &other) const {
    Identifiers<T> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids *= other;
    return intersect_ids;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Returns the set difference between this set and other
  */
template <typename T>
Identifiers<T> Identifiers<T>::operator -(const Identifiers<T> &other) const {
    Identifiers<T> diff_ids;
    diff_ids.insert(*this);
    diff_ids -= other;
    return diff_ids;
}


//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is an identifiers of type *T*
  Replaces this set with the set union of other with this set
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator +=(const T &other) {
    this->insert(other);
    return *this;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is an identifiers of type *T*
  Replaces this set with the set intersection of other with this set
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator *=(const T &other) {
    if (m_ids.find(other) != m_ids.end()) {
        m_ids.clear();
        m_ids.insert(other);
    } else {
        m_ids.clear();
    }
    return *this;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is a set of identifiers of type *T*
  Replaces this set with the set difference between this set and other
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator -=(const T &other) {
    if (m_ids.find(other) != m_ids.end()) {
        m_ids.erase(m_ids.find(other));
    }
    return *this;
}


//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Replaces this set with the set union of other with this set
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator +=(const Identifiers<T> &other) {
    this->insert(other);
    return *this;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Replaces this set with the set intersection of other with this set
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator *=(const Identifiers<T> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier) == other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

//! \brief Returns a set of identifiers of type *Identifiers<T>&* 
/*!
  @param [in] other is a set of identifiers of type *Identifiers<T>*
  Replaces this set with the set difference between this set and other
  */
template <typename T>
Identifiers<T>& Identifiers<T>::operator -=(const Identifiers<T> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier) != other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

//! \brief Prints the set of identifiers
template <typename T>
std::ostream& operator << (std::ostream& os, const Identifiers<T>& identifiers) {
    os << "{";
    for (auto identifier : identifiers.ids()) {
        os << identifier << ", ";
    }
    os << "}";
    return os;
}

//! \brief Returns an identifier of type *T* at position *index* in the set 
/*!
  @param [in] index is of type size_t 
  */
template <typename T>
const T& Identifiers<T>::operator [](size_t index) const {
        if (index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return *std::next(m_ids.begin(), index);
}
