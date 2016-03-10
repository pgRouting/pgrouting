// TODO (vicky) license

#pragma once

#include <basictypes.h>
#include <string>
#include "point.h"

/*! \class Node
 * \brief The Node class defines a point in 2D space with an id.
 *
 * A Node is a point that defines a location in 2D space. It maintains
 * a user \c id and an internal \c nid along with its \c x, \c y location.
 * This is the base object that things like depots, customer locations, etc.
 * are built upon.
 *
 */

class Node : public Point {
    public:
        /** @name accessors */
        ///@{

        inline ID id() const {return m_id;}
        inline int64_t original_id() const {return m_original_id;}

        ///@}

        Node(ID id, int64_t original_id, double x, double y);


        /** @name state */
        ///@{

        bool isSamePos(const Node &other) const;
        double travel_time_to(const Node &node2) const;
        friend std::ostream& operator<<(std::ostream &log, const Node &node);
    private: 
        ID m_id;                ///< internal node number
        int64_t m_original_id;  ///< user supplied node number
};


#if 0
/** @name operators */
///@{

bool operator<(const Node &n) const { return comparable_distance(Point(0,0)nid_ < n.nid_; }
        bool operator==(const Node &n) const {
        return nid_ == n.nid_
        && x_ == n.x_ && y_ == n.y_;
        }
        bool operator!=(const Node &n) const { return !( *this == n ); }
        bool operator>(const Node &n) const { return nid_ > n.nid_; }
        ///@}
        /** @name vector operators */
        ///@{
        Node operator+(const Node &v) const;
        Node operator-(const Node &v) const;
        Node operator*(double f) const;
        double dotProduct(const Node &p) const;
        double length() const;
        double gradient(const Node &pi) const;
        Node unit() const;
        ///@}

        /** @name distances */
        ///@{
double distance(const Node &n) const;
double haversineDistance(const Node &n) const;
double distanceTo(const Node &p) const;
double distanceToSquared(const Node &p) const;
///@}

/** @name distanceToSegment */
/* Shortest distnace from pooit to a segment (v,w) */
///@{
double distanceToSegment(const Node &v, const Node &w) const;
double distanceToSegment(const Node &v, const Node &w, Node &q) const;
double distanceToSegment(double, double, double, double, double &,
        double &) const;
///@}

double positionAlongSegment(const Node &v, const Node &w, double tol) const;

// dump
/** @name log */
///@{
///@}

// constructor


#endif
