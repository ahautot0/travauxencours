
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

// turing.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include <sstream> 
#include <hash_map>
#include <algorithm>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>


class MyClass_Hasher {
    public:
     static const size_t bucket_size = 10; // mean bucket size that the container should try not to exceed
     static const size_t min_buckets = (1 << 10); // minimum number of buckets, power 
     MyClass_Hasher() {
          // should be default-constructible
     }
     size_t operator()(const std::vector<int>& v) const {
             size_t hash = 5381;

            std::vector<int>::const_iterator iter = v.begin();
            std::vector<int>::const_iterator end  = v.end();
            for(; iter != end; ++iter) {
                hash = ((hash << 5) + hash) + *iter;
            }

            return hash;
     }

     bool operator()(const std::vector<int>& v1, const std::vector<int>& v2) const {
           if (v1.size() != v2.size()) {
                return true;
            }
            std::vector<int>::const_iterator iter1 = v1.begin();
            std::vector<int>::const_iterator iter2 = v2.begin();
            std::vector<int>::const_iterator end1 = v1.end();
            for (; iter1 != end1; ++iter1, ++iter2) {
                if (*iter1 != *iter2) {
                    return true;
                }
            }

            return false;
     }
 };
   


__global__ void
run_turing_machine(const long long machineNumBegin, bool* machineEnded, int* nbIteration, int* printedTape, int* sizeTape, int tapeLength, int numElements)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index >= numElements) {
        return;
    }
    if (index%2 == 0) {
        return;
    }

    int* currentTapepointer = printedTape + index*tapeLength;
    for (int* tmp = currentTapepointer; tmp  != currentTapepointer+tapeLength; ++tmp) {
        *tmp = 0;
    }

        
    long long machineNumber = machineNumBegin + index;
    int currentStateIndex_ = 0;
    int currentIndexInTape_ = 0;
    int iterationNumber_ = 0;
    int maxIndexWriteInTape_ = 0;
    machineEnded[index] = false;
    sizeTape[index] = 0;

    
    int transitionsTable[3*10];
    {
        int rest;
        int subNb;
        for (int i = 0; i < 10; ++i) {
            rest = machineNumber % 20;

            // inverse order !!!
            subNb = rest;
            int j = 0;
            for (; j < 2; ) {
                int subRest = subNb % 2;
                transitionsTable[3*i+j] = subRest;

                subNb = (subNb - subRest)/2;

                j++;
            }
            transitionsTable[3*i+j] = subNb;

            machineNumber = (machineNumber - rest)/20;
        }
    }

    

    for (int iterationCounter = 0; iterationCounter < 1000; ++iterationCounter) {
        int charRead = currentTapepointer[currentIndexInTape_];

        int* pointerOnInstructionToFollow = NULL;
        if (currentStateIndex_ == 0) {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*0];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*1];
            }
        }
        else if (currentStateIndex_ == 1) {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*2];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*3];
            }
        }
        else if (currentStateIndex_ == 2) {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*4];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*5];
            }
        }
        else if (currentStateIndex_ == 3) {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*6];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*7];
            }
        }
        else {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*8];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*9];
            }
        }
        
        // write on the tape
        currentTapepointer[currentIndexInTape_] = pointerOnInstructionToFollow[1];

        // move that in the if... see if it give the same.

        // Update the index in tape
        if (pointerOnInstructionToFollow[0] == 0) {
            currentIndexInTape_++;
            if (currentIndexInTape_ > maxIndexWriteInTape_) {
                maxIndexWriteInTape_ = currentIndexInTape_;
            }

            if (currentIndexInTape_ >= tapeLength) {
                return;
            }
        }
        else {
            currentIndexInTape_--;
            if (currentIndexInTape_ < 0) {
                machineEnded[index] = true;
                nbIteration[index] = iterationCounter+1;
                sizeTape[index] = maxIndexWriteInTape_;
                return;
            }
        }

        // Update state to go to.
        currentStateIndex_ = pointerOnInstructionToFollow[2];
    }

//end:
    
}



int main(int argc, char* argv[])
{

    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

    const int maxSimultaneously = 1024*100;
    const int tapeLength = 200;

    int* nbIteration = (int*)malloc(maxSimultaneously*sizeof(int));
    bool* machineEnded = (bool*)malloc(maxSimultaneously*sizeof(bool));
    int* tapeSize = (int*) malloc(maxSimultaneously*sizeof(int));
    int* printedTape = (int*)malloc(tapeLength*maxSimultaneously*sizeof(int));
        
    int* d_nbIteration = NULL;
    bool* d_machineEnded = NULL;
    int* d_tapeSize = NULL;
    int* d_printedTape = NULL;
    cudaMalloc((void **)&d_nbIteration, maxSimultaneously*sizeof(int));
    cudaMalloc((void **)&d_machineEnded, maxSimultaneously*sizeof(bool));
    cudaMalloc((void **)&d_tapeSize, maxSimultaneously*sizeof(int));
    cudaMalloc((void **)&d_printedTape, tapeLength*maxSimultaneously*sizeof(int));


    long long limit = 4294967;//2;//96;//90000000;//;
    for (long long currentMachineNum = 0; currentMachineNum < limit; /*++i*/) {

        

        int maxNbelements = maxSimultaneously;
        if (currentMachineNum+maxSimultaneously > limit) {
            long long tmp = limit - currentMachineNum;
            maxNbelements = (int) tmp;
        }


        const int threadsPerBlock = 1024;
        int blocksPerGrid = maxSimultaneously/threadsPerBlock;//(maxSimultaneously + threadsPerBlock - 1) / threadsPerBlock;
        //printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
        run_turing_machine<<<blocksPerGrid, threadsPerBlock>>>(currentMachineNum, d_machineEnded, d_nbIteration, d_printedTape, d_tapeSize, tapeLength, maxNbelements);
        //long long test = 67686362;
        //run_turing_machine<<<, threadsPerBlock>>>(test, d_machineEnded, d_nbIteration, d_printedTape, d_tapeSize, tapeLength, maxNbelements);

        cudaError_t err = cudaGetLastError();

        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        
        cudaMemcpy(nbIteration, d_nbIteration, maxSimultaneously*sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(machineEnded, d_machineEnded, maxSimultaneously*sizeof(bool), cudaMemcpyDeviceToHost);
        cudaMemcpy(tapeSize, d_tapeSize, maxSimultaneously*sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(printedTape, d_printedTape, tapeLength*maxSimultaneously*sizeof(int*), cudaMemcpyDeviceToHost);
        
        for (int k = 0; k < maxNbelements; k++) {
            if (machineEnded[k] == true && tapeSize[k] >= 0) {
                std::vector<int> infosToStore;
                infosToStore.reserve(tapeSize[k]+1);

                int* pointerBegin = &printedTape[k*tapeLength];
                int* pointerEnd = &printedTape[k*tapeLength]+tapeSize[k]+1;
                for (;pointerBegin != pointerEnd; ++pointerBegin) {
                    infosToStore.push_back(*pointerBegin);
                }

                long long curMachine = currentMachineNum+k;
                std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator mapIter = map.find(infosToStore);
                if (mapIter == map.end()) {
                    map[infosToStore] = std::make_pair(curMachine, nbIteration[k]);
                }
                else {
                    if (curMachine < mapIter->second.first) {
                        map[infosToStore] = std::make_pair(curMachine, nbIteration[k]);
                    }
                }
            }
        }

        currentMachineNum += maxSimultaneously;
        
    }

    cudaFree(d_nbIteration);
    cudaFree(d_machineEnded);
    cudaFree(d_tapeSize);
    cudaFree(d_printedTape);

    free(nbIteration);
    free(machineEnded);
    free(tapeSize);
    free(printedTape);

    FILE* f = fopen("MT5.txt", "w");
    
    std::vector<std::pair<long long, std::pair<int, std::vector<int> > > > allMachines;

    {
        std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator iter = map.begin();
        std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator end = map.end();
        for (; iter != end; ++iter) {
            allMachines.push_back(std::make_pair(iter->second.first, std::make_pair(iter->second.second, iter->first)));
        }
    }

    std::sort(allMachines.begin(), allMachines.end());

    {
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator iter = allMachines.begin();
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator end  = allMachines.end();
        for (; iter != end; ++iter) {
            const std::vector<int>& vect = (iter->second.second);
            fprintf(f, "{%lld, %d, {", iter->first, iter->second.first);

            if (vect.size() >= 0) {
                std::vector<int>::const_reverse_iterator tapeIter = vect.rbegin();
                std::vector<int>::const_reverse_iterator tapeEnd = vect.rend();
                for (; tapeIter != tapeEnd-1; ++tapeIter) {
                    fprintf(f, "%d, ", *tapeIter);
                }
                fprintf(f, "%d", *tapeIter);
            }

            fprintf(f, "}}\n");
        }        
    }

    fclose(f);
    std::cout << "process terminated" << std::endl;
    std::cin;
}




