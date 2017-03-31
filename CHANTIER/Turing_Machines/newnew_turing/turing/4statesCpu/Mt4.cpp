// turing.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream> 
#include <hash_map>



long long int sym4 (long long int nb)
	{
	long long int quo, nx, min; 
	short i,j,k,nb16[6][8]={};

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
	
	k=1;
	for (j=1; j<6; j++) {nx=0;for (i=0; i<8; i++) {nx=16*nx + nb16[j][i];} if (nx<min) min=nx; if (nx==nb) k++;}
	if (nb==min) return (12/k+1); else return(12/k);
}



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

   	long long int halt=0;

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

        tape_.resize(170, 0);            

    }

    void process(FILE* f, std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >& map)
    {
        for (int iterationCounter = 0; iterationCounter < 170; ++iterationCounter) {
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
					halt=halt+sym4(machineNumber_)/2;
                    
                    std::vector<int> infosToStore;
                    infosToStore.reserve(maxIndexWriteInTape_);

                    if (maxIndexWriteInTape_ >= 0) {
                        for (int i = 0; i <= maxIndexWriteInTape_; ++i) {
                            infosToStore.push_back(tape_[i]);
                        }
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



int main(int argc, char* argv[])
{

    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

	long long i,limit=4294967296;

    FILE* f = fopen("MT4Cpu.txt", "w");
    for (i = 0; i < limit; i+=2) {if (sym4(i) % 2 == 0) continue;
	    Machine mTest(i);
        mTest.process(f, map);
    }


    std::vector<std::pair<long long, std::pair<int, std::vector<int> > > > allMachines;

    {
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator iter = map.begin();
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator end = map.end();
        for (; iter != end; ++iter) {
            allMachines.push_back(std::make_pair(iter->second.first, std::make_pair(iter->second.second, iter->first)));
        }
    }

    std::sort(allMachines.begin(), allMachines.end());

    {
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator iter = allMachines.begin();
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator end  = allMachines.end();

		fprintf(f, "Nombre de MT exécutées : %lld\n",limit);
		fprintf(f, "Nombre de MT paires qui se sont arrêtées : %lld\n",halt); 

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

