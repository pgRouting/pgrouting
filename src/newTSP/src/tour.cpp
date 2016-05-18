
#include "tour.h"

#include <algorithm>

#include "../../common/src/pgr_assert.h"


static
size_t
succ(size_t i, size_t n) {
        return static_cast<size_t>((i + 1) % n);
}

namespace pgRouting {
namespace tsp {


#if 0
void Tour::random_swap(bool with_succ)  {
    auto n = cities.size();
    pgassert(n > 2);

    auto c1 = std::rand() % n;
    auto c2 = with_succ? succ(c1, n) : std::rand() % n;

    if (c1 == c2) c2 = succ(c2, n);

    pgassert(c1 != c2);
    pgassert(c1 < n && c2 < n);
    pgassert(c1 < c2);

    this->swap(c1, c2);
}

void Tour::random_reverse()  {
    auto n = cities.size();
    pgassert(n > 2);

    auto c1 = std::rand() % n;
    auto c2 = std::rand() % n;

    if (c1 == c2) c2 = succ(c2, n);

    pgassert(c1 != c2);
    pgassert(c1 < n && c2 < n);
    pgassert(c1 < c2);

    this->reverse(c1, c2);
}

void Tour::random_rotate()  {
    auto n = cities.size();
    pgassert(n > 3);

    auto c1 = std::rand() % n;
    auto c2 = std::rand() % n;
    auto c3 = std::rand() % n;

    if (c1 == c2) c2 = succ(c2, n);
    if (c2 == c3) c3 = succ(c3, n);
    if (c1 == c3) c3 = succ(succ(c3, n), n);

    pgassert(c1 != c2 && c2 != c3 && c1 != c3);
    pgassert(c1 < n && c2 < n && c3 < n);

    if (c1 > c3) std::swap(c1, c3);
    if (c1 > c2) std::swap(c1, c2);
    if (c2 > c3) std::swap(c2, c3);

    pgassert(c1 < c2 && c2 < c3);

    this->rotate(c1, c2, c3);
}
#endif



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
