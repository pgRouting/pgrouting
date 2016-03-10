
#include "./point.h"

Point::Point(double x, double y)
    : point(x,y) {
    }

double Point::x() const {return point.x();}

double Point::y() const {return point.y();}



std::ostream& operator<<(std::ostream& log, const Point &p) {
    log << "(" << p.point.x() << "," << p.point.y() << ")";
    return log;
}


