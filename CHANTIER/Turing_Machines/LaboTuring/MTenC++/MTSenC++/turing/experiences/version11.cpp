
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream> 
#include <hash_map>
using namespace std;




/*Ce programme calcule les machines à S états. La bande est limitée à 100 cases. Les MT qui se sont arrêtées sont imprimées dans test. 
Celles qui travaillent toujours après 2500 pas sont notées dans nest. Celles qui ont débordé sont rejetées dans overtape*/

int main()
{long long int i, S=5, quo, nb, nx, halt=0, loop=0, nest=0; 
int j=0,j2=0,k,k1,kk,ipas,jj=0,j1,nbfig[11] /*2S+1*/,state,startstate,starttape[101],pos,u,posmax,posmin,posleft,posright,tripl,tape[101],return1,coupure[7]={4,16,48,96,300,1000,2500}; 
bool return2, ok;
std::vector<unsigned char> res(2147483648, '0');   //Stocke les suites imprimées jusqu'à 30 bits (2147483648=2^31)

 ofstream fichier("halting.txt", ios::out | ios::trunc);  //ouverture en écriture avec effacement du fichier ouvert
 ofstream remise("nested.txt", ios::out | ios::trunc);
 ofstream overtape("overtape.txt", ios::out | ios::trunc);

for (nb = 10000000000000; nb <10000000000000+10240000000; nb+=2) { 


	quo=nb; for (i=2*S; i>0; i--) {nbfig[i] = quo%(4*S); quo=quo/(4*S);}   //décomposition de nb en base 4S


	for (k1=0;k1<101;k1++) tape[k1]=0; pos=1; state=tripl=0; ipas=1; posmax=1;    //initialisation de la machine nb


	for (k=0; k<7;k++)   {posmin=pos;posright=pos;posleft=posmax;startstate=state; for (kk=posleft; kk>0;kk--) {starttape[kk]=tape[kk];}    
						 //on explore son évolution par fenêtres successives de longueurs coupure[k]



while (pos>0  && ipas<coupure[k]) { tripl=nbfig[2*S-2*state-tape[pos]]; state=tripl/4; tape[pos]=(tripl/2) % 2; pos=pos+1-2*(tripl % 2);
if (pos>100) {overtape <<nb  <<" , "; if (j2%10==9) overtape <<"\n"; j2++;goto cpt;}    //on a débordé d'une bande à 100 cases et on imprime par paquets de 10
       if (pos>posmax) posmax=pos; if (pos<posmin) posmin=pos; 
	   ok=true; if ((state != startstate) || ((posleft-posright) != (posmax-pos))) {ok=false; goto next;}

	   for (kk=0;kk<posleft-posmin+1;kk++) { if (tape[posmax-kk]!=starttape[posleft-kk]) {ok=false; goto next;};} 
								   if (ok==true) {loop=loop+1; goto suivante;}
                   next : ipas++; }
 /*while est terminé : soit pos=0 (=halt) soit ipas a atteint le bas de la fenêtre sans trouver de périodicité (= on passe à la fenêtre suivante)*/


								   if (pos==0) {halt=halt+1; if (posmax<31) {nx=1;for (i=posmax; i>0; i--) {nx=2*nx + tape[i];} ;}
             if ((posmax<31 && res[nx]=='0')||posmax>30)  { 
			 fichier <<"{ {" ;   for (i=posmax; i>1; i--) {fichier << tape[i]    <<" , ";}   fichier << tape[1] <<" }, "  << nb  <<" , " << ipas/2  <<" }, "  <<"\n";  res[nx]='1';}    goto suivante;}  
			 /*Le if intérieur regarde si la suite codée par nx est imprimée pour la première fois. 
			 nx est l'entier binaire obtenu en préfixant par 1 la suite calculée.*/  	

				
					    ;}  //fin du for précédent le while


		;   //fin du else


suivante : if (ok==false && pos != 0) {nest=nest+return1; remise <<nb <<"," ; if (j%10==9) remise <<"\n"; j++;} //on comptabilise dans nest les machines qui ne sont ni dans halt ni dans loop et on imprime par paquets de 10

     cpt:;}  //fin du for nb

fichier << "Nombre de machines qui se sont certainement arrêtées : "  << halt  <<"\n";
fichier << "Nombre de machines qui ont certainement bouclé : "  << loop  <<"\n";
fichier << "Nombre de machines qui s'arrêteront ou qui boucleront ou qui ont nesté : "  << nest  <<"\n";
fichier.close(); remise.close(); overtape.close();
return(0)                               
	
;}







 /*Lorsque S=5, il est possible que le container res ne suffise plus. On peut imaginer prévoir un fichier d'exceptions plus longues qu'un certain seuil.
long long exceptions[1000]={0}; permet de stocker un maximum de 1000 exceptions consommant 13 bits et plus
if (nx>4095) {except=false; for(j1=0;j1<jj;j1++) if(nx==exceptions[j1]) except=true; 
if (except==false)   {j1++; resu[j]=nx; fichier << nb  <<" : " << resu[j] <<"\n"; exceptions[j1]=nx;};}

if ((nx<4096) && (res[nx]==false))  {j++; resu[j]=nx; fichier << nb  <<" : " << resu[j] <<"\n";  res[nx]=true;}*/


/*
 

        ofstream fichier("test.txt", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
 
        if(fichier)
        {
                string nom = "Xav57";
                int age = 19;
                fichier << "Date de naissance : " << 24 << '/' << 3 << '/' << 1988 << endl;
                fichier << "Bonjour, " << nom << ". Vous avez " << age << " ans.";
 
                fichier.close();
        }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl;
 
        */
  
/*int main()
{

    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

	long long i,limit=4294967296;

    FILE* f = fopen("MT4Cpu.txt", "w");
    for (i = 1000200; i < limit; i+=2) {if (sym4(i) % 2 == 0) continue;
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
}*/