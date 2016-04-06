#pragma once
#include <set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

template <class T>
class Identifiers {
 public:
    const std::set<T>& ids() const;
    bool isDisjoint(const T other) const;
    bool isDisjoint(const Identifiers<T> &other) const;
    void insert(const Identifiers<T> &other);
    void insert(const T &other);
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
    friend std::ostringstream& operator << (std::ostringstream& os, const Identifiers<T>& identifiers);
 private:
    std::set<T> m_ids;
};


template <class T>
const std::set<T>& Identifiers<T>::ids() const {
    return m_ids;
}

template <class T>
bool Identifiers<T>::isDisjoint(const T other) const {
    return (m_ids.find(other) == m_ids.end());
}

template <class T>
bool Identifiers<T>::isDisjoint(const Identifiers<T> &other) const {
    for (auto identifier : other.ids()) {
        if (!(m_ids.find(identifier) == m_ids.end())) {
            return false;
        }
    }
    return true;
}

template <class T>
void Identifiers<T>::insert(const Identifiers<T> &other) {
    #if 0
    std::cout << "Before Insertion" << std::endl;
    for (auto identifier : m_ids) {
        std::cout << identifier << ", ";
    }
    std::cout << std::endl;
    #endif
    m_ids.insert(other.ids().begin(), other.ids().end());
     #if 0
    std::cout << "After Insertion" << std::endl;
    for (auto identifier : m_ids) {
        std::cout << identifier << ", ";
    }
    std::cout << std::endl;
    #endif
}

template <class T>
void Identifiers<T>::insert(const T &other) {
    #if 0
    std::cout << "Before Insertion" << std::endl;
    for (auto identifier : m_ids) {
        std::cout << identifier << ", ";
    }
    std::cout << std::endl;
    #endif
    m_ids.insert(other);
    #if 0
    std::cout << "After Insertion" << std::endl;
    for (auto identifier : m_ids) {
        std::cout << identifier << ", ";
    }
    std::cout << std::endl;
    #endif
}

template <class T>
Identifiers<T> Identifiers<T>::operator +(const T &other) const {
    Identifiers<T> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

template <class T>
Identifiers<T> Identifiers<T>::operator *(const T &other) const {
    Identifiers<T> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids *= other;
    return intersect_ids;
}

template <class T>
Identifiers<T> Identifiers<T>::operator -(const T &other) const {
    Identifiers<T> diff_ids;
    diff_ids.insert(*this);
    diff_ids -= other;
    return diff_ids;
}


template <class T>
Identifiers<T> Identifiers<T>::operator +(const Identifiers<T> &other) const {
    Identifiers<T> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

template <class T>
Identifiers<T> Identifiers<T>::operator *(const Identifiers<T> &other) const {
    Identifiers<T> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids *= other;
    return intersect_ids;
}

template <class T>
Identifiers<T> Identifiers<T>::operator -(const Identifiers<T> &other) const {
    Identifiers<T> diff_ids;
    diff_ids.insert(*this);
    diff_ids -= other;
    return diff_ids;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator +=(const T &other) {
    this->insert(other);
    return *this;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator *=(const T &other) {
    if (m_ids.find(other) != m_ids.end()) {
        m_ids.clear();
        m_ids.insert(other);
    } else {
        m_ids.clear();
    }
    return *this;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator -=(const T &other) {
    if (m_ids.find(other) != m_ids.end()) {
        m_ids.erase(m_ids.find(other));
    }
    return *this;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator +=(const Identifiers<T> &other) {
    this->insert(other);
    return *this;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator *=(const Identifiers<T> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier) == other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

template <class T>
Identifiers<T>& Identifiers<T>::operator -=(const Identifiers<T> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier) != other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

template <class T>
std::ostringstream& operator << (std::ostringstream& os, const Identifiers<T>& identifiers) {
    os << "Set of TS : {";
    for (auto identifier : identifiers.ids()) {
        os << identifier << ", ";
    }
    os << "}";
    return os;
}