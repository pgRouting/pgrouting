#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "./Dmatrix.h"


namespace pgRouting {
namespace tsp {


class Tour {

 public:

     Tour(const Tour &) = default;

     explicit Tour(const std::vector<size_t> &cities_order) : 
         cities(cities_order){
     }

     explicit Tour(size_t n) {
         cities.resize(n);
         std::iota(std::begin(cities), std::end(cities), 0);
     }

     inline size_t size() const {return cities.size();};

     friend double Dmatrix::tourCost(const Tour &tour) const;


     /* @brief slides range [first + 1, last + 1) into place + 1
      *
      * 0 1 2 3 4 5 6 7 8 9
      *     p   f     l
      * slides [4,5,6] to position p
      *
      * 0 1 4 5 6 2 3 7 8 9
      *
      *
      * 0 1 2 3 4 5 6 7 8 9
      *     f     l     p
      * slides [2,3,4] to position p
      *
      * 0 1 6 7 2 3 4 5 8 9
      *
      * uses std::reverse 
      *
      * http://en.cppreference.com/w/cpp/algorithm/rotate
      *
      * first    -   the beginning of the original range
      * last     -   the end of the original range
      * place    -   location where to slide
      *
      *
      *
      * @params[IN] place index of place
      * @params[IN] first - index of first
      * @params[IN] last - index of last
      *
      * precondition:
      * pgassert(first < cities.size();
      * pgassert(last  < cities.size();
      * pgassert(place < cities.size();
      *
      */

     void slide(
             size_t place,
             size_t first,
             size_t last);


     /* @brief std::reverse on the cities
      *
      * http://en.cppreference.com/w/cpp/algorithm/reverse
      *
      * first    -   the beginning of the original range
      * last     -   the end of the original range
      *
      * @params[IN] c1 - index of first
      * @params[IN] c2 - index of lasst
      *
      * precondition:
      * pgassert(c1 < c2);
      *
      */

     void reverse(
             size_t c1,
             size_t c2);


     /* @brief std::rotate on the cities
      *
      * http://en.cppreference.com/w/cpp/algorithm/rotate
      *
      * first    -   the beginning of the original range
      * n_first  -   the element that should appear at the beginning of the rotated range
      * last     -   the end of the original range
      *
      * @params[IN] c1 - index of first
      * @params[IN] c2 - index of n_first
      * @params[IN] c3 - index of last
      *
      * precondition:
      * pgassert(c2 && c2 < c3 && c3 < n);
      *
      */
     void rotate(
             size_t c1,
             size_t c2,
             size_t c3);


     void swap(
             size_t c1,
             size_t c2);

 public:
     std::vector<size_t> cities;

};

}  // namespace tsp
}  // namespace pgRouting
