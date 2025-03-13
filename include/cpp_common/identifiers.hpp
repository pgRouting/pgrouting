/*PGR-GNU*****************************************************************
File: identifiers.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Rohith Reddy
Mail:

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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_IDENTIFIERS_HPP_
#define INCLUDE_CPP_COMMON_IDENTIFIERS_HPP_
#pragma once

#include <set>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <stdexcept>

/* TODO(vicky)
 * compiler check that type T is a integral type
 */
namespace pgrouting {

template <typename T>
class Identifiers {
 public:
    typedef typename std::set<T>::iterator iterator;
    typedef typename std::set<T>::const_iterator const_iterator;

    /*!
    \brief define ids
     */
    void set_ids(std::set<T> ids) {
        m_ids = ids;
    }

    //! @name constructors
    //@{
    Identifiers() = default;
    explicit Identifiers(const std::set<T>& data) : m_ids(data) {
    }

    /* @brief initializes with {1 ~ number}
     *
     * @params [in] number
     */
    explicit Identifiers(const size_t number) {
        size_t i(0);
        std::generate_n(std::inserter(m_ids, m_ids.begin()),
                number,
                [&i](){ return i++; });
    }

    //@}

    //! @name set like operators
    //@{
    size_t size() const {return m_ids.size(); }
    inline bool empty() const {return m_ids.empty(); }
    inline T front() const {return *m_ids.begin();}
    const_iterator begin() const {return m_ids.begin();}
    const_iterator end() const {return m_ids.end();}
    inline void pop_front() {m_ids.erase(m_ids.begin());}
    inline void clear() {m_ids.clear();}
    iterator begin() {return m_ids.begin();}
    iterator end() {return m_ids.end();}
    //@}


 private:
    std::set<T> m_ids;

 public:
    //! \brief true ids() has element
    /*!
     * @param [in] other Identifier of type *T*
     */
    bool has(const T other) const {
        return (m_ids.find(other) != m_ids.end());
    }


    //! \brief true when both sets are equal
    /*!
     * @param [in] rhs set of identifiers to be compared
     */
    bool operator==(const Identifiers<T> &rhs) const {
        return std::equal(m_ids.begin(), m_ids.end(), rhs.m_ids.begin());
    }

    //! @name  set UNION
    /// @{

    /*! \brief set UNION set
     *
     * @param[in] lhs Identifiers
     * @param[in] rhs Identifiers
     */
    friend Identifiers<T> operator +(
            const Identifiers<T> &lhs,
            const Identifiers<T> &rhs) {
        Identifiers<T> union_ids(lhs);
        union_ids += rhs;
        return union_ids;
    }

    //! \brief compound set UNION set
    /*!
     * @param [in] other set of identifiers
     */
    Identifiers<T>& operator +=(
            const Identifiers<T> &other) {
        m_ids.insert(other.m_ids.begin(), other.m_ids.end());
        return *this;
    }
    //! \brief compound set UNION element
    /*!
     * @param [in] element of type *T*
     */
    Identifiers<T>& operator +=(const T &element) {
        m_ids.insert(element);
        return *this;
    }

    /// @}



    //! @name  set INTERSECTION
    /// @{

    /*! \brief set INTERSECTION
     *
     *
     * @param[in] lhs  Identifiers
     * @param[in] rhs  Identifiers
     */

    friend Identifiers<T> operator *(
                const Identifiers<T> &lhs,
                const Identifiers<T> &rhs) {
            std::set<T> result;
            std::set_intersection(
                    lhs.m_ids.begin(), lhs.m_ids.end(),
                    rhs.m_ids.begin(), rhs.m_ids.end(),
                    std::inserter(result, result.begin()));
            return Identifiers<T>(result);
        }

    //! \brief coumpound set INTERSECTION set
    /*!
     * @param [in] other is a set of identifiers of type *Identifiers<T>*
     */
    Identifiers<T>& operator *=(
            const Identifiers<T> &other) {
        *this = *this * other;
        return *this;
    }

    //! \brief compund set INTERSECTION element
    /*!
     * @param[in] element is an identifiers of type *T*
     */
    Identifiers<T>& operator *=(const T &element) {
        if (has(element)) {
            m_ids.clear();
            m_ids.insert(element);
        } else {
            m_ids.clear();
        }
        return *this;
    }

    /// @}


    //! @name  set DIFFERENCE
    /// @{

    /* \brief set DIFFERENCE set
     *
     * @param[in] lhs Identifiers
     * @param[in] rhs Identifiers
     */
    friend
        Identifiers<T> operator -(
                const Identifiers<T> &lhs,
                const Identifiers<T> &rhs) {
            std::set<T> result;
            std::set_difference(
                    lhs.m_ids.begin(), lhs.m_ids.end(),
                    rhs.m_ids.begin(), rhs.m_ids.end(),
                    std::inserter(result, result.begin()));
            return Identifiers<T>(result);
        }



    //! \brief compound set DIFFERENCE set
    /*!
      @param [in] other is a set of identifiers of type *Identifiers<T>*
      Replaces this set with the set difference between this set and other
      */
    Identifiers<T>& operator -=(const Identifiers<T> &other) {
        *this = *this - other;
        return *this;
    }

    //! \brief compund set DIFFERENCE element
    /*!
      @param[in] element to be removed
      */
    Identifiers<T>& operator -=(const T &element) {
            m_ids.erase(element);
        return *this;
    }

    /// @}

    //! \brief Prints the set of identifiers
    friend
        std::ostream&
        operator<<(std::ostream& os, const Identifiers<T>& identifiers) {
            os << "{";
            for (auto identifier : identifiers.m_ids) {
                os << identifier << ", ";
            }
            os << "}";
            return os;
        }
};

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_IDENTIFIERS_HPP_
