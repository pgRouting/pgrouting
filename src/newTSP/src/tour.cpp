
#include "tour.h"

#include <algorithm>

#include "../../common/src/pgr_assert.h"


#if 0
static
size_t
succ(size_t i, size_t n) {
        return static_cast<size_t>((i + 1) % n);
}
#endif

namespace pgRouting {
namespace tsp {





void Tour::reverse(
        size_t c1,
        size_t c2) {
    pgassert(c1 < c2 && c2 < cities.size());
    std::reverse(
            cities.begin() + (c1 + 1),
            cities.begin() + (c2 + 1));
}

void Tour::slide(
        size_t place,
        size_t first,
        size_t last) {
    pgassert(first < cities.size());
    pgassert(last  < cities.size());
    pgassert(place < cities.size());

    if (place < first) {
        std::rotate(
                cities.begin() + (place + 1),
                cities.begin() + (first + 1),
                cities.begin() + (last + 1));
    } else {
        std::rotate(
                cities.begin() + (first + 1),
                cities.begin() + (last + 1),
                cities.begin() + (place + 1));
    }
}

void Tour::rotate(
        size_t c1,
        size_t c2,
        size_t c3) {
    pgassert(c1 < c2 && c2 < c3 && c3 < cities.size());

    std::rotate(
            cities.begin() + (c1 + 1),
            cities.begin() + (c2 + 1),
            cities.begin() + (c3 + 1));
}


void Tour::swap(
        size_t c1,
        size_t c2 ) {
    pgassert(c1 < c2);

    std::iter_swap(cities.begin() + c1, cities.begin() + c2 );
}


} // namespace tsp
} // namespace pgRouting
