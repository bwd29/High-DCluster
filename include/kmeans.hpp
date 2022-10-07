

#pragma once
#include "include/params.hpp"

unsigned int * clusterKMeans(DTYPE ** data, unsigned int numPoints, unsigned int dim, unsigned int numClusters);
DTYPE cosDist(DTYPE * a, DTYPE magA, DTYPE * b, DTYPE magB, unsigned int dim);