#ifndef MAXIMUM_WEIGHTED_MATCHING_HPP
#define MAXIMUM_WEIGHTED_MATCHING_HPP

#include <vector>
#include <utility> // for std::pair

// Structure to represent an edge
struct MWEdge {
    int source;
    int target;
    double weight;
};

// Function declaration
std::vector<std::pair<int,int>> maximumWeightedMatching(const std::vector<MWEdge>& edges, bool allow_partial);

#endif // MAXIMUM_WEIGHTED_MATCHING_HPP

