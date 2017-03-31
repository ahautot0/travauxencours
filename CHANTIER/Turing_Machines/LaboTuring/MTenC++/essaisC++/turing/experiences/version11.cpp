
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream> 
#include <hash_map>
using namespace std;


inline void sym4 (long long int nb, int& return1, bool& return2)   /*Cette fonction calcule les (S-1)! machines symétriques qui impriment la même suite que nb*/
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
	return1=6/k; if (nb==min) return2=true; else return2=false;  /*Renvoie return1 et return2 au main; return1 est la multiplicité (1, 2, 3 ou 6)
														  et return2 vaut 0 (skipper le calcul) ou 1 (faire le calcul complet)*/
}

/*Ce programme calcule les machines à 4 états les répartissant entre halt, loop et nest*/

int main()
{long long int i,quo, nb, nx, halt=0, loop=0, nest=0, resu[321]={0}; 
int S=4,j=0,k,k1,kk,ipas,jj=0,j1,nbfig[2*4+1],state,startstate,starttape[170],pos,u,posmax,posmin,posleft,posright,tripl,tape[170],return1,coupure[6]={4,16,48,96,300}; 
bool return2, ok;
std::vector<unsigned char> res(16777216, '0');

 ofstream fichier("test.txt", ios::out | ios::trunc);  //ouverture en écriture avec effacement du fichier ouvert
 ofstream remise("nest.txt", ios::out | ios::trunc);

for (nb = 0; nb <4294967296; nb+=2) { 

	sym4(nb,return1,return2); if (return2 == false)  continue;   //on ignore les machines symétriques

	else {quo=nb; for (i=2*S; i>0; i--) {nbfig[i] = quo%(4*S); quo=quo/(4*S);}   //décomposition de nb en base 4S


	for (k1=0;k1<170;k1++) tape[k1]=0; pos=1; state=tripl=0; ipas=1; posmax=1;    //initialisation de la machine nb


	for (k=0; k<5;k++)   {posmin=pos;posright=pos;posleft=posmax;startstate=state; for (kk=posleft; kk>0;kk--) {starttape[kk]=tape[kk];}    
						 //on explore son évolution par fenêtres successives de longueurs coupure[k]



while (pos>0  && ipas<coupure[k]) { tripl=nbfig[2*S-2*state-tape[pos]]; state=tripl/4; tape[pos]=(tripl/2) % 2; pos=pos+1-2*(tripl % 2); 
       if (pos>posmax) posmax=pos; if (pos<posmin) posmin=pos; 
	   ok=true; if ((state != startstate) || ((posleft-posright) != (posmax-pos))) {ok=false; goto next;}

	   for (kk=0;kk<posleft-posmin+1;kk++) { if (tape[posmax-kk]!=starttape[posleft-kk]) {ok=false; goto next;};} 
								   if (ok==true) {loop=loop+return1; goto suivante;}
                   next : ipas++; }
 /*while est terminé : soit pos=0 (=halt) soit ipas a atteint le bas de la fenêtre sans trouver de périodicité (= on passe à la fenêtre suivante)*/


if (pos==0) {halt=halt+return1; nx=1;for (i=posmax; i>0; i--) {nx=2*nx + tape[i];} 
             if (res[nx]=='0')  {j++; resu[j]=nx;  
			 fichier << nb  <<" : " << ipas-1  <<" : "  << resu[j] <<"\n";  res[nx]='1';}    goto suivante;}  
			 /*Le if intérieur regarde si la suite codée par nx est imprimée pour la première fois. 
			 nx est l'entier binaire obtenu en préfixant la suite calculée par 1.*/  	

				
					    ;}  //fin du for précédent le while


		;}   //fin du else


suivante : if (ok==false && pos != 0) {nest=nest+return1;remise <<nb <<"," ;} //on comptabilise dans nest les machines qui ne sont ni dans halt ni dans loop

     }  //fin du for nb

fichier << "Nombre de machines qui se sont arrêtées : "  << halt  <<"\n";
fichier << "Nombre de machines qui ont bouclé : "  << loop  <<"\n";
fichier << "Nombre de machines qui ont nesté : "  << nest  <<"\n";
fichier.close(); remise.close();
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