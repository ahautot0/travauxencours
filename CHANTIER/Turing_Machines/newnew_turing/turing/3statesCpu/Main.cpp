// turing.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream> 
#include <hash_map>


class MyClass_Hasher {
    public:
     static const size_t bucket_size = 10; // mean bucket size that the container should try not to exceed
     static const size_t min_buckets = (1 << 10); // minimum number of buckets, power 
     MyClass_Hasher() {
          // should be default-constructible
     }

	 // Fonction qui attribue une clé pour un vecteur donné.
     size_t operator()(const std::vector<int>& v) const {
             size_t hash = 5381;

            std::vector<int>::const_iterator iter = v.begin();
            std::vector<int>::const_iterator end  = v.end();
            for(; iter != end; ++iter) {
                hash = ((hash << 5) + hash) + *iter;
            }

            return hash;
     }

	 // Cette fonction défini normalement un ordre et doit répondre true si le premier element est plus petit que le deuxième.
	 // Dans notre cas, seul l'égalité nous intéresse. Tout ce qu'il nous faut est de répondre false en cas d'égalité.
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

    Machine(int machineNumber)
        :transitionsTable_(),
        currentStateIndex_(0),
        tape_(),
        currentIndexInTape_(0),
        iterationNumber_(0),
        machineNumber_(machineNumber),
        maxIndexWriteInTape_(0)
    {
		// Ici on initialise toutes les variable que l'on utilisera.
		// (table de transitions, la bande d'impression, ...)

        int nb = machineNumber;
        transitionsTable_.resize(6);

        {
            // inverse order!!!!!
            std::vector<std::vector<int> >::iterator iter = transitionsTable_.begin();
            std::vector<std::vector<int> >::iterator end = transitionsTable_.end();
            for (; iter != end; ++iter) {
                int rest = nb % 12;

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


                nb = (nb - rest)/12;
            }

        }

        tape_.resize(35, 0);            

    }

    void process(FILE* f, std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >& map)
    {
		// Boucle sur le nombre d'itération. Ici 32 (qui peut être changé).
        for (int iterationCounter = 0; iterationCounter < 32; ++iterationCounter) {
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
            else {
                if (charRead == 0) {
                    pointerOnInstructionToFollow = &transitionsTable_[4];
                }
                else {
                    pointerOnInstructionToFollow = &transitionsTable_[5];
                }
            }

            // écriture sur la bande
            tape_[currentIndexInTape_] = (*pointerOnInstructionToFollow)[1];

            
            // Met à jour l'index où on se trouve sur la bande.
            if ((*pointerOnInstructionToFollow)[0] == 0) {
                currentIndexInTape_++;
                if (currentIndexInTape_ > maxIndexWriteInTape_) {
                    maxIndexWriteInTape_ = currentIndexInTape_;
                }
            }
            else {
                currentIndexInTape_--;
				// Test si on a franchit la ligne rouge.
                if (currentIndexInTape_ < 0) {
                    
					// Vecteur qui contiendra ce qui a été écrit sur le ruban. 
                    std::vector<int> infosToStore;
                    infosToStore.reserve(maxIndexWriteInTape_);

                    if (maxIndexWriteInTape_ >= 0) {
                        for (int i = 0; i <= maxIndexWriteInTape_; ++i) {
                            infosToStore.push_back(tape_[i]);
                        }
                    }

					// On recherche dans la table de hash si ce que la machine à écrit a déjà été écrit avant.
                    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator mapIter = map.find(infosToStore);
                    if (mapIter == map.end()) {
						// C'est la première fois.
                        map[infosToStore] = std::make_pair(machineNumber_, (iterationCounter+1));
                    }
                    else {
						// Ce n'est pas la première fois.

						// Si jamais l'index de la machine est plus petit que celui déjà dans la structure, on le remplace.
                        if (machineNumber_ < mapIter->second.first) {
                            map[infosToStore] = std::make_pair(machineNumber_, (iterationCounter+1));
                        }
                    }

                    return;
                }
            }

            // On met à jour l'état où on doit aller à l'itération suivante.
            currentStateIndex_ = (*pointerOnInstructionToFollow)[2];
        }
    }

    int machineNumber_;

    int currentStateIndex_;
	
    int currentIndexInTape_;

    int iterationNumber_;

    std::vector<int> tape_;

    int maxIndexWriteInTape_;

    std::vector<std::vector<int> > transitionsTable_; 

};

int main(int argc, char* argv[])
{
	// Déclaration de la hash map.
    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

	// Fichier de sortie. Par defaut, il sera "turing\3statesCpu\test.txt"
    FILE* f = fopen("MT3.txt", "w");

	// Boucle sur toutes les machines.
    for (int i = 0; i < 2985984; i+=2) {
	    Machine mTest(i);
        mTest.process(f, map);
    }

	// On met tous les résultats dans un vecteur afin de pouvoir les trier par ordre de machine croissante.
    std::vector<std::pair<long long, std::pair<int, std::vector<int> > > > allMachines;
    {
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator iter = map.begin();
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator end = map.end();
        for (; iter != end; ++iter) {
            allMachines.push_back(std::make_pair(iter->second.first, std::make_pair(iter->second.second, iter->first)));
        }
    }

	// Tri
    std::sort(allMachines.begin(), allMachines.end());

	// Parcours des machines à afficher et impression dans le fichier.
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

