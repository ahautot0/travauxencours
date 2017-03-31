
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

   

class Machine 
{
public:

    Machine(long long machineNumber)
        :transitionsTable_(),
        currentStateIndex_(0),
        tape_(),
        currentIndexInTape_(0),
        iterationNumber_(0),
        machineNumber_(machineNumber),
        maxIndexWriteInTape_(0)
    {
        long long nb = machineNumber;
        transitionsTable_.resize(8);

        {
            // inverse order!!!!!
            std::vector<std::vector<int> >::iterator iter = transitionsTable_.begin();
            std::vector<std::vector<int> >::iterator end = transitionsTable_.end();
            for (; iter != end; ++iter) {
                int rest = nb % 16;

                iter->resize(3);
                // inverse order !!!
                std::vector<int>::iterator subIter = iter->begin();
                std::vector<int>::iterator subEnd = iter->end();
                int subNb = rest;
                for (; subIter != subEnd-1; ++subIter) {
                    int subRest = subNb % 2;
                    *subIter = subRest;

                    subNb = (subNb - subRest)/2;
                }
                *subIter = subNb;


                nb = (nb - rest)/16;
            }

        }

        tape_.resize(300, 0);            

    }

    void process(std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >& map)
    {
        for (int iterationCounter = 0; iterationCounter < 300; ++iterationCounter) {
            int charRead = tape_[currentIndexInTape_];

            std::vector<int>* pointerOnInstructionToFollow = NULL;
            if (currentStateIndex_ == 0) {
                if (charRead == 0) {
                    pointerOnInstructionToFollow = &transitionsTable_[0];
                }
                else {
                    pointerOnInstructionToFollow = &transitionsTable_[1];
                }
            }
            else if (currentStateIndex_ == 1) {
                if (charRead == 0) {
                    pointerOnInstructionToFollow = &transitionsTable_[2];
                }
                else {
                    pointerOnInstructionToFollow = &transitionsTable_[3];
                }
            }
            else if (currentStateIndex_ == 2) {
                if (charRead == 0) {
                    pointerOnInstructionToFollow = &transitionsTable_[4];
                }
                else {
                    pointerOnInstructionToFollow = &transitionsTable_[5];
                }
            }
            else {
                if (charRead == 0) {
                    pointerOnInstructionToFollow = &transitionsTable_[6];
                }
                else {
                    pointerOnInstructionToFollow = &transitionsTable_[7];
                }
            }

            // write on the tape
            tape_[currentIndexInTape_] = (*pointerOnInstructionToFollow)[1];

            // move that in the if... see if it give the same.
            

            // Update the index in tape
            if ((*pointerOnInstructionToFollow)[0] == 0) {
                currentIndexInTape_++;
                if (currentIndexInTape_ > maxIndexWriteInTape_) {
                    maxIndexWriteInTape_ = currentIndexInTape_;
                }
            }
            else {
                currentIndexInTape_--;
                if (currentIndexInTape_ < 0) {
                    
                    std::vector<int> infosToStore;
                    infosToStore.reserve(maxIndexWriteInTape_);

                    if (maxIndexWriteInTape_ >= 0) {
                        for (int i = maxIndexWriteInTape_; i > 0; --i) {
                            infosToStore.push_back(tape_[i]);
                        }
                        infosToStore.push_back(tape_[0]);
                    }

                    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator mapIter = map.find(infosToStore);
                    if (mapIter == map.end()) {
                        map[infosToStore] = std::make_pair(machineNumber_, (iterationCounter+1));
                    }
                    else {
                        if (machineNumber_ < mapIter->second.first) {
                            map[infosToStore] = std::make_pair(machineNumber_, (iterationCounter+1));
                        }
                    }

                    return;
                }
            }

            // Update state to go to.
            currentStateIndex_ = (*pointerOnInstructionToFollow)[2];
        }

    }


    long long machineNumber_;

    int currentStateIndex_;
	
    int currentIndexInTape_;

    int iterationNumber_;

    std::vector<int> tape_;

    int maxIndexWriteInTape_;

    std::vector<std::vector<int> > transitionsTable_; 

};


// La fonction qui tourne sur la carte graphique en parallèle
__global__ void
run_turing_machine(const long long machineNumBegin, bool* machineEnded, int* nbIteration, int* printedTape, int* sizeTape, int tapeLength, int numElements)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index >= numElements) {
        return;
    }

    // Calculer le numéro de la machine courante.
    long long machineNumber = machineNumBegin + index;

    // Rejeter les machines impaires.
    if (machineNumber%2 == 1) {
        sizeTape[index] = -1;  // pour forcer le traitement ultérieur de cette machine.
        machineEnded[index] = true;
        return;
    }
    

	// currentTapepointer est un pointeur qui se met au début de la bande allouée pour la machine courante
    int* currentTapepointer = printedTape + index*tapeLength;
    for (int* tmp = currentTapepointer; tmp  != currentTapepointer+tapeLength; ++tmp) {
		// On initialise toute la bande à 0.
        *tmp = 0;
    }


    int currentStateIndex_ = 0;
    int currentIndexInTape_ = 0;
    int iterationNumber_ = 0;
    int maxIndexWriteInTape_ = 0;
    machineEnded[index] = false;
    sizeTape[index] = 0;

    
    int transitionsTable[3*8];
    {
        int rest;
        int subNb;
        for (int i = 0; i < 8; ++i) {
            rest = machineNumber % 16;

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

            machineNumber = (machineNumber - rest)/16;
        }
    }

    

    for (int iterationCounter = 0; iterationCounter < 300; ++iterationCounter) {
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
        else {
            if (charRead == 0) {
                pointerOnInstructionToFollow = &transitionsTable[3*6];
            }
            else {
                pointerOnInstructionToFollow = &transitionsTable[3*7];
            }
        }
        
        // write on the tape
        currentTapepointer[currentIndexInTape_] = pointerOnInstructionToFollow[1];
		

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
	    
}



/*bool sym4 (long long int nb)
	{
	long long int quo, nx, min; 
	short i,j,nb16[6][8]={};

	quo=min=nb;
	
	for (i=7; quo>0; i--) {nb16[0][i] = quo%16; quo=quo/16;}

	nb16[1][0]=nb16[2][2]=nb16[3][4]=nb16[4][2]=nb16[5][4]=nb16[0][0];
	nb16[1][1]=nb16[2][3]=nb16[3][5]=nb16[4][3]=nb16[5][5]=nb16[0][1];
	nb16[1][4]=nb16[2][0]=nb16[3][0]=nb16[4][4]=nb16[5][2]=nb16[0][2];
	nb16[1][5]=nb16[2][1]=nb16[3][1]=nb16[4][5]=nb16[5][3]=nb16[0][3];
	nb16[1][2]=nb16[2][4]=nb16[3][2]=nb16[4][0]=nb16[5][0]=nb16[0][4];
	nb16[1][3]=nb16[2][5]=nb16[3][3]=nb16[4][1]=nb16[5][1]=nb16[0][5];
	nb16[1][6]=nb16[2][6]=nb16[3][6]=nb16[4][6]=nb16[5][6]=nb16[0][6];
	nb16[1][7]=nb16[2][7]=nb16[3][7]=nb16[4][7]=nb16[5][7]=nb16[0][7];

	
	for (i=0; i<8; i++) 
	{
		switch (nb16[1][i]/4)
	{case 0 : break;
	 case 1 : nb16[1][i]=nb16[1][i]+4; break;
	 case 2 : nb16[1][i]=nb16[1][i]-4; break;
	 case 3 : break;}

		switch (nb16[2][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb16[2][i]=nb16[2][i]+4; break;
	 case 3 : nb16[2][i]=nb16[2][i]-4; break;}
	
		switch (nb16[3][i]/4)
	{case 0 : break;
	 case 1 : nb16[3][i]=nb16[3][i]+4; break;
	 case 2 : nb16[3][i]=nb16[3][i]+4; break;
	 case 3 : nb16[3][i]=nb16[3][i]-8; break;}

		switch (nb16[4][i]/4)
	{case 0 : break;
	 case 1 : nb16[4][i]=nb16[4][i]+8; break;
	 case 2 : nb16[4][i]=nb16[4][i]-4; break;
	 case 3 : nb16[4][i]=nb16[4][i]-4; break;}
		
		switch (nb16[5][i]/4)
	{case 0 : break;
	 case 1 : nb16[5][i]=nb16[5][i]+8; break;
	 case 2 : break;
	 case 3 : nb16[5][i]=nb16[5][i]-8; break;}
	
	}
	
	for (j=1; j<6; j++) {nx=0;for (i=0; i<8; i++) {nx=16*nx + nb16[j][i];} if (nx<min) min=nx; }
	if (nb == min) return (false); else return (true);
}*/







int main(int argc, char* argv[])
{

    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

    // Ce nombre représente le nombre de machines qui vont tourner en parrallèle avant chaque resynchronisation sur le cpu.
    // A priori plus il est grand plus rapide ce sera mais il faut veiller à ne pas dépasser une certaine limite sous peine de ne 
    // plus avoir de mémoire et que le programme plante.
    // Une marge d'erreur est préférable d'autant plus si le pc fait tourner d'autres choses en même temps.
    const int maxSimultaneously = 1024*100;

    // longueur de la bande d'impression relative à chaque machine de turing.
    const int tapeLength = 20;

	// Allocation des structures sur le processeur.
    int* nbIteration = (int*)malloc(maxSimultaneously*sizeof(int));
    bool* machineEnded = (bool*)malloc(maxSimultaneously*sizeof(bool));
    int* tapeSize = (int*) malloc(maxSimultaneously*sizeof(int));
    int* printedTape = (int*)malloc(tapeLength*maxSimultaneously*sizeof(int));
        
    int* d_nbIteration = NULL;
    bool* d_machineEnded = NULL;
    int* d_tapeSize = NULL;
    int* d_printedTape = NULL;
	// Allocation des structures sur le GPU.
    cudaMalloc((void **)&d_nbIteration, maxSimultaneously*sizeof(int));
    cudaMalloc((void **)&d_machineEnded, maxSimultaneously*sizeof(bool));
    cudaMalloc((void **)&d_tapeSize, maxSimultaneously*sizeof(int));
    cudaMalloc((void **)&d_printedTape, tapeLength*maxSimultaneously*sizeof(int));

    long long numberMachinesStopped = 0;

	// Remarque : Pour effectuer des petits tests, on peut réduire le nombre de machine à calculer....
    long long limit = 4294967296;
    for (long long currentMachineNum = 0; currentMachineNum < limit; ) {/*if (sym4(currentMachineNum)) continue;*/

        int maxNbelements = maxSimultaneously;
        if (currentMachineNum+maxSimultaneously > limit) {
            long long tmp = limit - currentMachineNum;
            maxNbelements = (int) tmp;
        }

		// threadsPerBlock et blocksPerGrid sont 2 paramètres demandé par cuda: Ils peuvent être modifié dépendant
		// de la carte graphique.
        int threadsPerBlock = 1024;
        int blocksPerGrid =maxSimultaneously/threadsPerBlock;//(maxSimultaneously + threadsPerBlock - 1) / threadsPerBlock;
        run_turing_machine<<<blocksPerGrid, threadsPerBlock>>>(currentMachineNum, d_machineEnded, d_nbIteration, d_printedTape, d_tapeSize, tapeLength, maxNbelements);
        
        cudaError_t err = cudaGetLastError();

        if (err != cudaSuccess)
        {
			// Erreur Cuda, peut être les paramètres ont des valeurs trop élevée pour la carte graphique utilisée...
            fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        
		// Rappatrier les données de la carte graphique dans les structures processeur afin de pouvoir les traiter.
        cudaMemcpy(nbIteration, d_nbIteration, maxSimultaneously*sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(machineEnded, d_machineEnded, maxSimultaneously*sizeof(bool), cudaMemcpyDeviceToHost);
        cudaMemcpy(tapeSize, d_tapeSize, maxSimultaneously*sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(printedTape, d_printedTape, tapeLength*maxSimultaneously*sizeof(int*), cudaMemcpyDeviceToHost);
        
		// Parcour de tous les éléments.
        for (int k = 0; k < maxNbelements; k++) {
			
			// Test la pertinence des données pour la machine k (machine s'est arrêtée et à écrit quelque chose?)
            if (machineEnded[k] == true && tapeSize[k] >= 0) {
                std::vector<int> infosToStore;
                infosToStore.reserve(tapeSize[k]+1);

				// Mettre l'impression de la bande dans un vecteur afin de pouvoir faire une recherche dans la table de hash.
                int* pointerBegin = &printedTape[k*tapeLength];
                int* pointerEnd = &printedTape[k*tapeLength]+tapeSize[k]+1;
                for (;pointerBegin != pointerEnd; ++pointerBegin) {
                    infosToStore.push_back(*pointerBegin);
                }

                long long curMachine = currentMachineNum+k;
                std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator mapIter = map.find(infosToStore);
                if (mapIter == map.end()) {
					// Résulat non trouvé, on l'insère donc.
                    map[infosToStore] = std::make_pair(curMachine, nbIteration[k]);
                }
                else {
					// Résultat déjà existant, on insère seulement si le numéro de la machine est plus petit que celui déjà stocké.
                    if (curMachine < mapIter->second.first) {
                        map[infosToStore] = std::make_pair(curMachine, nbIteration[k]);
                    }
                }

                numberMachinesStopped++;
            }
        }

        currentMachineNum += maxSimultaneously;
        
    }

	// Libèration de la mémoire sur GPU
    cudaFree(d_nbIteration);
    cudaFree(d_machineEnded);
    cudaFree(d_tapeSize);
    cudaFree(d_printedTape);

	// Libèration de la mémoire sur CPU
    free(nbIteration);
    free(machineEnded);
    free(tapeSize);
    free(printedTape);


	// Voir explication projet 3statesCpu...

    FILE* f = fopen("MT4_Cuda1.txt", "w");

    fprintf(f, "Nombre de machines executees: %lld\n", limit);
    fprintf(f, "Nombre de machines qui se sont arretee: %lld\n", numberMachinesStopped);
    
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




