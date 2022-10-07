#include "include/kmeans.hpp"

unsigned int * clusterKMeans(DTYPE ** data, unsigned int numPoints, unsigned int dim, unsigned int numClusters){

    //first caclualte the mag for each point
    DTYPE * magArray = (DTYPE *)malloc(sizeof(DTYPE)*numPoints);

    #pragma omp parallel for
    for(unsigned int i = 0; i < numPoints; i++){
        for(unsigned int j = 0; j < dim; j++){
            magArray[i] += data[i][j]*data[i][j];
        }
        magArray[i] = sqrt(magArray[i]);
    }

    // place initial centroids
    DTYPE ** centroids = (DTYPE **)malloc(sizeof(DTYPE*)*numClusters);
    DTYPE * centroidMag = (DTYPE *)malloc(sizeof(DTYPE)*numClusters);
    for(unsigned int i = 0; i < numClusters; i ++){
        centroids[i] = (DTYPE *)malloc(sizeof(DTYPE)*dim);
        for(unsigned int j = 0; j < dim; j++){
           centroids[i][j] = (DTYPE)rand()/(DTYPE)RAND_MAX - (DTYPE)rand()/(DTYPE)RAND_MAX; //between -1 and 1
           centroidMag[i] += centroids[i][j]*centroids[i][j];
        }
        centroidMag[i] = sqrt(centroidMag[i]);
    }


    unsigned int maxIterations = MAX_ITER;
    unsigned int iter = 0;

    unsigned int * label = (unsigned int *)malloc(sizeof(unsigned int)*numPoints);
    unsigned int *clusterSize = (unsigned int * )malloc(sizeof(unsigned int)*numClusters);

    while (iter < maxIterations)
    {
        iter++;

        for(unsigned int i = 0; i < numClusters; i++){
            clusterSize[i] = 0;
        }

        //find assigned centroid
        bool movedPoint = false;
        #pragma omp parallel for
        for(unsigned int i = 0; i < numPoints; i++){
            DTYPE shortestDist = RAND_MAX;
            unsigned int lastLabel = label[i];
            for(unsigned int j = 0; j < numClusters; j++){
                DTYPE dist = cosDist(data[i], magArray[i], centroids[j], centroidMag[j], dim);
                if (dist < shortestDist){
                    shortestDist = dist;
                    label[i] = j;
                }
            }
            
            if(label[i] != lastLabel){
                movedPoint = true;
            }

            #pragma omp critical
            {
                clusterSize[label[i]]++;
            }
            

        }

        if(movedPoint == false){
            break;
        }

        // find new mean for each centroid
        
        for(unsigned int i = 0; i < numPoints; i++){
            for(unsigned int j = 0; j < dim; j++){
                centroids[label[i]][j] += data[i][j];
            }
        }

        #pragma omp parallel for
        for(unsigned int i = 0; i < numClusters; i++){
            for(unsigned int j = 0; j < dim; j++){
                centroids[i][j] /= clusterSize[i];
                centroidMag[i] += centroids[i][j]*centroids[i][j]; 
            }
            centroidMag[i] = sqrt(centroidMag[i]);
        }

    }

    for(unsigned int i = 0; i < numClusters; i++){
        free(centroids[i]);
    }

    free(centroids);
    free(centroidMag);
    free(clusterSize);
    free(magArray);

    return label;
    
}


DTYPE cosDist(DTYPE * a, DTYPE magA, DTYPE * b, DTYPE magB, unsigned int dim){
    DTYPE dot = 0;

    for(unsigned int i = 0; i < dim; i++){
        dot += a[i]*b[i];
    }

    return dot/(magA * magB);;
}