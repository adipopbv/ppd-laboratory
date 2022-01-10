#include "cuda_runtime.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

//  b,g,r,b,g,r,b,g,r
// [0,1,2,3,4,5,6,7,8]

__global__ void applyFilterOnGPU(const int *inputPixelArray, int *outputPixelArray, int lines, int cols, int channels) {
    int id = blockDim.x * blockIdx.x + threadIdx.x;
    if (id < lines * cols * channels) {
        if (id % 3 != 0) {
            // BGR representation!
            int inputBlue = inputPixelArray[id];
            int inputGreen = inputPixelArray[id + 1];
            int inputRed = inputPixelArray[id + 2];
            int blueValue = 0;
            int greenValue = 0;
            int redValue = 0;

            // should be sepia
            blueValue = (inputRed * .272) + (inputGreen *.534) + (inputBlue * .131);
            greenValue = (inputRed * .349) + (inputGreen *.686) + (inputBlue * .168);
            redValue = (inputRed * .393) + (inputGreen *.769) + (inputBlue * .189);

            if (blueValue > 255)
                blueValue = 255;
            if (greenValue > 255)
                greenValue = 255;
            if (redValue > 255)
                redValue = 255;

            outputPixelArray[id] = blueValue;
            outputPixelArray[id + 1] = greenValue;
            outputPixelArray[id + 2] = redValue;
        }
    }
}

int *flatten(int ***matrix, int lines, int cols, int channels) {
    int *flat = (int *) malloc(lines * cols * channels * sizeof(int));
    int id = 0;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            for (int c = 0; c < channels; c++) {
                flat[id] = matrix[i][j][c];
                id++;
            }
        }
    }
    return flat;
}

int ***unflatten(const int *array, int lines, int cols, int channels) {
    int ***img = (int ***) malloc(lines * sizeof(int **));
    int id = 0;

    for (int i = 0; i < lines; i++) {
        img[i] = (int **) malloc(cols * sizeof(int *));

        for (int j = 0; j < cols; j++) {
            img[i][j] = (int *) malloc(channels * sizeof(int));

            for (int c = 0; c < channels; c++) {
                img[i][j][c] = array[id];
                id++;
            }
        }
    }
    return img;
}

void applyFilter() {
    //read the matrix
    ifstream in("pixels.txt");
    int lines, cols, channels = 3;
    in >> lines >> cols;

    int BLOCK_SIZE = 1000;
    int blockCount = ((lines * cols * channels) / BLOCK_SIZE) + 1;

    //matrix read
    int ***matrix = (int ***) malloc(lines * sizeof(int **));
    for (int i = 0; i < lines; i++) {
        matrix[i] = (int **) malloc(cols * sizeof(int *));

        for (int j = 0; j < cols; j++) {
            int *line = (int *) malloc(channels * sizeof(int));

            in >> line[0] >> line[1] >> line[2];

            matrix[i][j] = line;
        }
    }

    int size = lines * cols * channels;
    //flatten matrix
    int *flatMatrix = flatten(matrix, lines, cols, channels);
    int *result = (int *) malloc(size * sizeof(int));

    //copy flatMatrix to the device array
    int *deviceMatrix, *deviceResult;
    cudaMalloc(&deviceMatrix, size * sizeof(int));
    cudaMalloc(&deviceResult, size * sizeof(int));

    cudaMemcpy(deviceMatrix, flatMatrix, size * sizeof(int), cudaMemcpyHostToDevice);

    //call the filter function
    applyFilterOnGPU <<< blockCount, 1000 >>>(deviceMatrix, deviceResult, lines, cols, channels);

    //copy from deviceResult in result
    cudaMemcpy(result, deviceResult, size * sizeof(int), cudaMemcpyDeviceToHost);

    int ***img = unflatten(result, lines, cols, channels);

    ofstream out("pixels.txt");
    out << lines << " " << cols << " " << channels << "\n";
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < channels; k++) {
                out << img[i][j][k] << " ";
            }
            out << "\n";
        }
    }

    out.close();
}

int main() {
    char *inCommand = "python ../img_to_px.py ../img.jpg";
    char *outCommand = "python ../px_to_img.py ../sepia.jpg";

    cout << "converting image to pixel values matrix" << endl;
    system(inCommand); //read pixel values to pixels.txt
    cout << "applying filter" << endl;
    applyFilter();
    cout << "converting pixel values matrix to image" << endl;
    system(outCommand); //write pixel values to image

    return 0;
}
