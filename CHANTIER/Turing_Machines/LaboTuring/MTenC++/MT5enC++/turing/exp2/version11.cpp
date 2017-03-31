
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream> 
#include <hash_map>
using namespace std;


inline void sym5 (long long int nb, int& return1, bool& return2)   /*Cette fonction calcule les (S-1)! machines symétriques qui impriment la même suite que nb*/
	{
	long long int quo, nx, min; 
	short i,j,k,nb20[24][10]={};

	quo=min=nb;
	
	for (i=9; quo>0; i--) {nb20[0][i] = quo%20; quo=quo/20;}

	nb20[1][0]=nb20[2][0]=nb20[3][0]=nb20[4][0]=nb20[5][0]=nb20[6][2]=nb20[7][2]=nb20[8][4]=nb20[9][6]=nb20[10][4]=nb20[11][6]=nb20[12][2]=nb20[13][2]=nb20[14][4]=nb20[15][6]=nb20[16][4]=nb20[17][6]=nb20[18][2]=nb20[19][2]=nb20[20][4]=nb20[21][6]=nb20[22][4]=nb20[23][6]=nb20[0][0];
	nb20[1][1]=nb20[2][1]=nb20[3][1]=nb20[4][1]=nb20[5][1]=nb20[6][3]=nb20[7][3]=nb20[8][5]=nb20[9][7]=nb20[10][5]=nb20[11][7]=nb20[12][3]=nb20[13][3]=nb20[14][5]=nb20[15][7]=nb20[16][5]=nb20[17][7]=nb20[18][3]=nb20[19][3]=nb20[20][5]=nb20[21][7]=nb20[22][5]=nb20[23][7]=nb20[0][1];
	nb20[1][2]=nb20[2][4]=nb20[3][6]=nb20[4][4]=nb20[5][6]=nb20[6][0]=nb20[7][0]=nb20[8][0]=nb20[9][0]=nb20[10][0]=nb20[11][0]=nb20[12][4]=nb20[13][6]=nb20[14][2]=nb20[15][2]=nb20[16][6]=nb20[17][4]=nb20[18][4]=nb20[19][6]=nb20[20][2]=nb20[21][2]=nb20[22][6]=nb20[23][4]=nb20[0][2];
	nb20[1][3]=nb20[2][5]=nb20[3][7]=nb20[4][5]=nb20[5][7]=nb20[6][1]=nb20[7][1]=nb20[8][1]=nb20[9][1]=nb20[10][1]=nb20[11][1]=nb20[12][5]=nb20[13][7]=nb20[14][3]=nb20[15][3]=nb20[16][7]=nb20[17][5]=nb20[18][5]=nb20[19][7]=nb20[20][3]=nb20[21][3]=nb20[22][7]=nb20[23][5]=nb20[0][3];
	nb20[1][6]=nb20[2][2]=nb20[3][2]=nb20[4][6]=nb20[5][4]=nb20[6][4]=nb20[7][6]=nb20[8][2]=nb20[9][2]=nb20[10][6]=nb20[11][4]=nb20[12][0]=nb20[13][0]=nb20[14][0]=nb20[15][0]=nb20[16][0]=nb20[17][0]=nb20[18][6]=nb20[19][4]=nb20[20][6]=nb20[21][4]=nb20[22][2]=nb20[23][2]=nb20[0][4];
	nb20[1][7]=nb20[2][3]=nb20[3][3]=nb20[4][7]=nb20[5][5]=nb20[6][5]=nb20[7][7]=nb20[8][3]=nb20[9][3]=nb20[10][7]=nb20[11][5]=nb20[12][1]=nb20[13][1]=nb20[14][1]=nb20[15][1]=nb20[16][1]=nb20[17][1]=nb20[18][7]=nb20[19][5]=nb20[20][7]=nb20[21][5]=nb20[22][3]=nb20[23][3]=nb20[0][5];
	nb20[1][4]=nb20[2][6]=nb20[3][4]=nb20[4][2]=nb20[5][2]=nb20[6][6]=nb20[7][4]=nb20[8][6]=nb20[9][4]=nb20[10][2]=nb20[11][2]=nb20[12][6]=nb20[13][4]=nb20[14][6]=nb20[15][4]=nb20[16][2]=nb20[17][2]=nb20[18][0]=nb20[19][0]=nb20[20][0]=nb20[21][0]=nb20[22][0]=nb20[23][0]=nb20[0][6];
	nb20[1][5]=nb20[2][7]=nb20[3][5]=nb20[4][3]=nb20[5][3]=nb20[6][7]=nb20[7][5]=nb20[8][7]=nb20[9][5]=nb20[10][3]=nb20[11][3]=nb20[12][7]=nb20[13][5]=nb20[14][7]=nb20[15][5]=nb20[16][3]=nb20[17][3]=nb20[18][1]=nb20[19][1]=nb20[20][1]=nb20[21][1]=nb20[22][1]=nb20[23][1]=nb20[0][7];
	nb20[1][8]=nb20[2][8]=nb20[3][8]=nb20[4][8]=nb20[5][8]=nb20[6][8]=nb20[7][8]=nb20[8][8]=nb20[9][8]=nb20[10][8]=nb20[11][8]=nb20[12][8]=nb20[13][8]=nb20[14][8]=nb20[15][8]=nb20[16][8]=nb20[17][8]=nb20[18][8]=nb20[19][8]=nb20[20][8]=nb20[21][8]=nb20[22][8]=nb20[23][8]=nb20[0][8];
	nb20[1][9]=nb20[2][9]=nb20[3][9]=nb20[4][9]=nb20[5][9]=nb20[6][9]=nb20[7][9]=nb20[8][9]=nb20[9][9]=nb20[10][9]=nb20[11][9]=nb20[12][9]=nb20[13][9]=nb20[14][9]=nb20[15][9]=nb20[16][9]=nb20[17][9]=nb20[18][9]=nb20[19][9]=nb20[20][9]=nb20[21][9]=nb20[22][9]=nb20[23][9]=nb20[0][9];
	
	for (i=0; i<10; i++) 
	{
		
		switch (nb20[1][i]/4)
	{case 0 : break;
	 case 1 : nb20[1][i]=nb20[1][i]+4; break;
	 case 2 : nb20[1][i]=nb20[1][i]-4; break;
	 case 3 : break;
	 case 4 : break;}

		switch (nb20[2][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[2][i]=nb20[2][i]+4; break;
	 case 3 : nb20[2][i]=nb20[2][i]-4; break;
	 case 4 : break;}
	
		switch (nb20[3][i]/4)
	{case 0 : break;
	 case 1 : nb20[3][i]=nb20[3][i]+4; break;
	 case 2 : nb20[3][i]=nb20[3][i]+4; break;
	 case 3 : nb20[3][i]=nb20[3][i]-8; break;
	 case 4 : break;}

		switch (nb20[4][i]/4)
	{case 0 : break;
	 case 1 : nb20[4][i]=nb20[4][i]+8; break;
	 case 2 : nb20[4][i]=nb20[4][i]-4; break;
	 case 3 : nb20[4][i]=nb20[4][i]-4; break;
	 case 4 : break;}

		switch (nb20[5][i]/4)
	{case 0 : break;
	 case 1 : nb20[5][i]=nb20[5][i]+8; break;
	 case 2 : break;
	 case 3 : nb20[5][i]=nb20[5][i]-8; break;
	 case 4 : break;}

		switch (nb20[6][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : break;
	 case 3 : nb20[6][i]=nb20[6][i]+4; break;
	 case 4 : nb20[6][i]=nb20[6][i]-4; break;}

		switch (nb20[7][i]/4)
	{case 0 : break;
	 case 1 : nb20[7][i]=nb20[7][i]+4; break;
	 case 2 : nb20[7][i]=nb20[7][i]-4; break;
	 case 3 : nb20[7][i]=nb20[7][i]+4; break;
	 case 4 : nb20[7][i]=nb20[7][i]-4; break;}

		switch (nb20[8][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[8][i]=nb20[8][i]+4; break;
	 case 3 : nb20[8][i]=nb20[8][i]+4; break;
	 case 4 : nb20[8][i]=nb20[8][i]-8; break;}

		switch (nb20[9][i]/4)
	{case 0 : break;
	 case 1 : nb20[9][i]=nb20[9][i]+4; break;
	 case 2 : nb20[9][i]=nb20[9][i]+4; break;
	 case 3 : nb20[9][i]=nb20[9][i]+4; break;
     case 4 : nb20[9][i]=nb20[9][i]-12; break;}

		switch (nb20[10][i]/4)
	{case 0 : break;
	 case 1 : nb20[10][i]=nb20[10][i]+8; break;
	 case 2 : nb20[10][i]=nb20[10][i]-4; break;
	 case 3 : nb20[10][i]=nb20[10][i]+4; break;
	 case 4 : nb20[10][i]=nb20[10][i]-8; break;}

		switch (nb20[11][i]/4)
	{case 0 : break;
	 case 1 : nb20[11][i]=nb20[11][i]+8; break;
	 case 2 : break;
	 case 3 : nb20[11][i]=nb20[11][i]+4; break;
	 case 4 : nb20[11][i]=nb20[11][i]-12; break;}

		switch (nb20[12][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[12][i]=nb20[12][i]+8; break;
	 case 3 : nb20[12][i]=nb20[12][i]-4; break;
	 case 4 : nb20[12][i]=nb20[12][i]-4; break;}
	
		switch (nb20[13][i]/4)
	{case 0 : break;
	 case 1 : nb20[13][i]=nb20[13][i]+4; break;
	 case 2 : nb20[13][i]=nb20[13][i]+8; break;
	 case 3 : nb20[13][i]=nb20[13][i]-8; break;
	 case 4 : nb20[13][i]=nb20[13][i]-4; break;}

		switch (nb20[14][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[14][i]=nb20[14][i]+8; break;
	 case 3 : break;
	 case 4 : nb20[14][i]=nb20[14][i]-8; break;}

		switch (nb20[15][i]/4)
	{case 0 : break;
	 case 1 : nb20[15][i]=nb20[15][i]+4; break;
	 case 2 : nb20[15][i]=nb20[15][i]+8; break;
	 case 3 : break;
	 case 4 : nb20[15][i]=nb20[15][i]-12; break;}
	
		switch (nb20[16][i]/4)
	{case 0 : break;
	 case 1 : nb20[16][i]=nb20[16][i]+8; break;
	 case 2 : nb20[16][i]=nb20[16][i]+8; break;
	 case 3 : nb20[16][i]=nb20[16][i]-8; break;
	 case 4 : nb20[16][i]=nb20[16][i]-8; break;}

		switch (nb20[17][i]/4)
	{case 0 : break;
	 case 1 : nb20[17][i]=nb20[17][i]+8; break;
	 case 2 : nb20[17][i]=nb20[17][i]+8; break;
	 case 3 : nb20[17][i]=nb20[17][i]-4; break;
	 case 4 : nb20[17][i]=nb20[17][i]-12; break;}

		switch (nb20[18][i]/4)
	{case 0 : break;
	 case 1 : nb20[18][i]=nb20[18][i]+12; break;
	 case 2 : nb20[18][i]=nb20[18][i]-4; break;
	 case 3 : nb20[18][i]=nb20[18][i]-4; break;
	 case 4 : nb20[18][i]=nb20[18][i]-4; break;}
	
		switch (nb20[19][i]/4)
	{case 0 : break;
	 case 1 : nb20[19][i]=nb20[19][i]+12; break;
	 case 2 : break;
	 case 3 : nb20[19][i]=nb20[19][i]-8; break;
	 case 4 : nb20[19][i]=nb20[19][i]-4; break;}

		switch (nb20[20][i]/4)
	{case 0 : break;
	 case 1 : nb20[20][i]=nb20[20][i]+12; break;
	 case 2 : nb20[20][i]=nb20[20][i]-4; break;
	 case 3 : break;
	 case 4 : nb20[20][i]=nb20[20][i]-8; break;}

		switch (nb20[21][i]/4)
	{case 0 : break;
	 case 1 : nb20[21][i]=nb20[21][i]+12; break;
	 case 2 : break;
	 case 3 : break;
	 case 4 : nb20[21][i]=nb20[21][i]-12; break;}
	
		switch (nb20[22][i]/4)
	{case 0 : break;
	 case 1 : nb20[22][i]=nb20[22][i]+12; break;
	 case 2 : nb20[22][i]=nb20[22][i]+4; break;
	 case 3 : nb20[22][i]=nb20[22][i]-8; break;
	 case 4 : nb20[22][i]=nb20[22][i]-8; break;}

		switch (nb20[23][i]/4)
	{case 0 : break;
	 case 1 : nb20[23][i]=nb20[23][i]+12; break;
	 case 2 : nb20[23][i]=nb20[23][i]+4; break;
	 case 3 : nb20[23][i]=nb20[23][i]-4; break;
	 case 4 : nb20[23][i]=nb20[23][i]-12; break;}

	
	}
	
	k=1;
	for (j=1; j<24; j++) {nx=0;for (i=0; i<10; i++) {nx=20*nx + nb20[j][i];}   if (nx<min) min=nx; if (nx==nb) k++;}
	return1=24/k; if (nb==min) return2=true; else return2=false;  /*Renvoie return1 et return2 au main; return1 est la multiplicité (1, 2, 3, 4, 6, 12 ou 24)
														  et return2 vaut 0 (skipper le calcul) ou 1 (faire le calcul complet)*/
}

/*Ce programme calcule les machines à 5 états les répartissant entre halt, loop et nest*/

int main()
{long long int i,quo, nb, nx, halt=0, loop=0, nest=0; 
int j=0,k,k1,kk,ipas,jj=0,j1,nbfig[2*5+1],state,startstate,starttape[101],pos,u,posmax,posmin,posleft,posright,tripl,tape[101],return1,coupure[7]={4,16,48,96,300,1000,2500}; 
bool return2, ok;
std::vector<unsigned char> res(2147483648, '0');   //Stocke les suites imprimées jusqu'à 30 bits

 ofstream fichier("test.txt", ios::out | ios::trunc);  //ouverture en écriture avec effacement du fichier ouvert
 ofstream remise("nest.txt", ios::out | ios::trunc);
 ofstream overtape("overtape.txt", ios::out | ios::trunc);

for (nb = 0; nb <1024000000/*0000*/; nb+=2) { 

	sym5(nb,return1,return2); if (return2 == false)  continue;   //on ignore les machines symétriques

	else {quo=nb; for (i=10; i>0; i--) {nbfig[i] = quo%20; quo=quo/20;}   //décomposition de nb en base 4S


	for (k1=0;k1<100;k1++) tape[k1]=0; pos=1; state=tripl=0; ipas=1; posmax=1;    //initialisation de la machine nb


	for (k=0; k<7;k++)   {posmin=pos;posright=pos;posleft=posmax;startstate=state; for (kk=posleft; kk>0;kk--) {starttape[kk]=tape[kk];}    
						 //on explore son évolution par fenêtres successives de longueurs coupure[k]



while (pos>0  && ipas<coupure[k]) { tripl=nbfig[10-2*state-tape[pos]]; state=tripl/4; tape[pos]=(tripl/2) % 2; pos=pos+1-2*(tripl % 2);
if (pos>100) {overtape <<nb  <<" , "; goto cpt;}
       if (pos>posmax) posmax=pos; if (pos<posmin) posmin=pos; 
	   ok=true; if ((state != startstate) || ((posleft-posright) != (posmax-pos))) {ok=false; goto next;}

	   for (kk=0;kk<posleft-posmin+1;kk++) { if (tape[posmax-kk]!=starttape[posleft-kk]) {ok=false; goto next;};} 
								   if (ok==true) {loop=loop+return1; goto suivante;}
                   next : ipas++; }
 /*while est terminé : soit pos=0 (=halt) soit ipas a atteint le bas de la fenêtre sans trouver de périodicité (= on passe à la fenêtre suivante)*/


								   if (pos==0) {halt=halt+return1; if (posmax<25) {nx=1;for (i=posmax; i>0; i--) {nx=2*nx + tape[i];} ;}
             if ((posmax<25 && res[nx]=='0')||posmax>24)  { 
			 fichier <<"{ {" ;   for (i=posmax; i>1; i--) {fichier << tape[i]    <<" , ";}   fichier << tape[1] <<" }, "  << nb  <<" , " << ipas/2  <<" }, "  <<"\n";  res[nx]='1';}    goto suivante;}  
			 /*Le if intérieur regarde si la suite codée par nx est imprimée pour la première fois. 
			 nx est l'entier binaire obtenu en préfixant par 1 la suite calculée.*/  	

				
					    ;}  //fin du for précédent le while


		;}   //fin du else


suivante : if (ok==false && pos != 0) {nest=nest+return1; remise <<nb <<"," ; if (j%10==9) remise <<"\n"; j++;} //on comptabilise dans nest les machines qui ne sont ni dans halt ni dans loop

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