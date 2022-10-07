#include "include/params.hpp"
#include "include/kmeans.hpp"


int main(int argvc, char* argv[]){

    char * filename = argv[1];
    char * outfile = argv[2];
    unsigned int dim = atoi(argv[3]);
    unsigned int numClusters = atoi(argv[4]);
    //reading in file
	
    FILE *fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		printf("No such File\n");
		exit(0);
	}
	DTYPE check = 0;
    
    std::vector<DTYPE> dataV;

	while( fscanf(fptr, "%lf, ", &check) == 1 || fscanf(fptr, "%lf ", &check) == 1 || fscanf(fptr, "%lf,", &check) == 1)
	{
		dataV.push_back(check);
	}
	fclose(fptr);

    //convert data to 2d array
    unsigned int numPoints = dataV.size()/dim;
    DTYPE ** data = (DTYPE **)malloc(sizeof(DTYPE *)*numPoints);
    for(unsigned int i = 0; i < numPoints; i++){
        data[i] = (DTYPE *)malloc(sizeof(DTYPE)*dim);
        for(unsigned int j = 0; j < dim; j++){
            data[i][j] = dataV[i*dim+j];
        }
    }

    unsigned int * labels = clusterKMeans(data, numPoints, dim, numClusters);

    std::ofstream myFile;
    myFile.open(outfile, std::ios::out);

    for(int i = 0; i < numPoints; i+=dim){
        myFile << labels[i] << '\n';
    }

    myFile.close();



    return 1;
}