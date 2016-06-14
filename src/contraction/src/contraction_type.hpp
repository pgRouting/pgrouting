#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <functional>

class Contraction_type {
 public:
    typedef enum {
        kNone = 0,
        kDeadEnd
    } types;
    Contraction_type() : m_type(types::kNone) { }
    inline bool is_none() const { return m_type == types::kNone; }
    inline bool is_dead_end() const { return m_type == types::kDeadEnd; }
    inline types type() const { return m_type; }
    inline void set_type(Contraction_type ctype) { m_type = ctype.type(); }
    

    const std::string type_str() const {
        switch (Contraction_type::type()) {
            case Contraction_type::types::kNone:
            return "None";
            case Contraction_type::types::kDeadEnd:
            return "DeadEnd Contraction";
            default:
            return "Invalid Contraction Type";
        }
    }
 private:
    types m_type;
};
