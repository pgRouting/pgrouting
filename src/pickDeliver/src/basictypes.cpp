
#include "./basictypes.h"
#include <limits>


double d_max() {
        return std::numeric_limits<double>::max();
}

double d_min() {
        return  -std::numeric_limits<double>::max();
}


