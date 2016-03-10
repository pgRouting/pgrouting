
#pragma once

#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

class Point {
  public:   
      Point(double x, double y);
      Point(const Point &) = default;

      double x() const;
      double y() const;

      friend
          std::ostream& operator<<(std::ostream& log, const Point &p);

      template <typename T>
          double distance(const T&p2) const {
                  return boost::geometry::distance(point, p2);
          }

      template <typename T>
          double comparable_distance(const T&p2) const {
                  return boost::geometry::comparable_distance(point, p2);
          }



  protected:
      boost::geometry::model::d2::point_xy<double> point;
};
