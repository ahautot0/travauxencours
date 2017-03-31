#include <iostream>
using namespace std;

void fonction (int& n1, int& n2, int& n3, int chiffre)
	{n1=chiffre+1;n2=chiffre+2;n3=chiffre+3;}

int main()
	{int a,b,c;

	fonction(a,b,c,10);
	
	cout << "a:"  <<  a   <<"\n"; 
	cout << "b:"  <<  b   <<"\n"; 
	cout << "c:"  <<  c   <<"\n"; 
	return(0);}
