#pragma once
#include <set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

template <class ID>
class Identifiers {
 public:
    const std::set<ID>& ids() const;
    bool isDisjoint(const ID other) const;
    bool isDisjoint(const Identifiers<ID> &other) const;
    void insert(const Identifiers<ID> &other);
    void insert(const ID &other);
    Identifiers<ID> operator +(const ID &other) const;
    Identifiers<ID> operator *(const ID &other) const;
    Identifiers<ID> operator -(const ID &other) const; 
    Identifiers<ID> operator +(const Identifiers<ID> &other) const;
    Identifiers<ID> operator *(const Identifiers<ID> &other) const;
    Identifiers<ID> operator -(const Identifiers<ID> &other) const;
    Identifiers<ID>& operator +=(const ID &other);
    Identifiers<ID>& operator *=(const ID &other);
    Identifiers<ID>& operator -=(const ID &other);     
    Identifiers<ID>& operator +=(const Identifiers<ID> &other);
    Identifiers<ID>& operator *=(const Identifiers<ID> &other);
    Identifiers<ID>& operator -=(const Identifiers<ID> &other);
    template<ID> 
    friend std::ostringstream& operator << (std::ostringstream& os, const Identifiers<ID>& identifiers);
 private:
    std::set<ID> m_ids;

};


template <class ID>
const std::set<ID>& Identifiers<ID>::ids() const {
    return m_ids;
}

template <class ID>
bool Identifiers<ID>::isDisjoint(const ID other) const {
    return (m_ids.find(other) == m_ids.end());
}

template <class ID>
bool Identifiers<ID>::isDisjoint(const Identifiers<ID> &other) const {
    for (auto identifier : other.ids()) {
        if (!(m_ids.find(identifier) == m_ids.end())) {
            return false;
        }
    }
    return true;
}

template <class ID>
void Identifiers<ID>::insert(const Identifiers<ID> &other) {
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
    
template <class ID>
void Identifiers<ID>::insert(const ID &other) {
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

template <class ID>
Identifiers<ID> Identifiers<ID>::operator +(const ID &other) const {
    Identifiers<ID> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

template <class ID>
Identifiers<ID> Identifiers<ID>::operator *(const ID &other) const {
    Identifiers<ID> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids*=other;
    return intersect_ids;
}

template <class ID>
Identifiers<ID> Identifiers<ID>::operator -(const ID &other) const {
    Identifiers<ID> diff_ids;
    diff_ids.insert(*this);
    diff_ids-=other;
    return diff_ids;
}


template <class ID>
Identifiers<ID> Identifiers<ID>::operator +(const Identifiers<ID> &other) const {
    Identifiers<ID> union_ids;
    union_ids.insert(*this);
    union_ids.insert(other);
    return union_ids;
}

template <class ID>
Identifiers<ID> Identifiers<ID>::operator *(const Identifiers<ID> &other) const {
    Identifiers<ID> intersect_ids;
    intersect_ids.insert(*this);
    intersect_ids*=other;
    return intersect_ids;
}

template <class ID>
Identifiers<ID> Identifiers<ID>::operator -(const Identifiers<ID> &other) const {
    Identifiers<ID> diff_ids;
    diff_ids.insert(*this);
    diff_ids-=other;
    return diff_ids;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator +=(const ID &other) {
    this->insert(other);
    return *this;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator *=(const ID &other) {

    if (m_ids.find(other)!=m_ids.end()) {
        m_ids.clear();
        m_ids.insert(other);
    }
    else {
        m_ids.clear();
    }
    return *this;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator -=(const ID &other) {
    if (m_ids.find(other)!=m_ids.end()) {
        m_ids.erase(m_ids.find(other));
    }
    return *this;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator +=(const Identifiers<ID> &other) {
    this->insert(other);
    return *this;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator *=(const Identifiers<ID> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier)==other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

template <class ID>
Identifiers<ID>& Identifiers<ID>::operator -=(const Identifiers<ID> &other) {
    for (auto identifier : m_ids) {
        if (other.ids().find(identifier)!=other.ids().end()) {
            m_ids.erase(m_ids.find(identifier));
        }
    }
    return *this;
}

template <class ID>
std::ostringstream& operator << (std::ostringstream& os, const Identifiers<ID>& identifiers) {
    os << "Set of IDS : {";
    for (auto identifier : identifiers.ids()) {
        os << identifier << ", ";
    }
    os << "}";
    return os;
}