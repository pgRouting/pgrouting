// TODO (vicky) license

#include "./node.h"

bool Node::isSamePos(const Node &other) const {
    return comparable_distance(other.point) == 0;
}

double Node::travel_time_to(const Node &other) const {
    return distance(other.point);
}

std::ostream& operator<<(std::ostream &log, const Node &node) {
    log << node.m_original_id << "(" << node.m_id << ")" << static_cast<const Point&>(node); 
    return log;
}

Node::Node(ID id, int64_t original_id, double x, double y)
    : Point(x,y),
    m_id(id),
    m_original_id(original_id) {
    }

bool
Node::operator==(const Node &rhs) const {
    if (&rhs == this) return true;
    return 
        (id() == rhs.id())
        && (original_id() == rhs.original_id()) 
        && (static_cast<Point>(*this) == static_cast<Point>(rhs));
}


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
#endif

