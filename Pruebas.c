#include <stdio.h> 
#include <stdlib.h> 
#include "rand.h"
#include <math.h>
#include <unistd.h>

int main(){
	int n = 20;
	int *alpha1 = calloc(n, sizeof(int));
	int *alpha2 = calloc(n, sizeof(int));
	int *x = calloc(n, sizeof(int));
	
	for (int i = 0; i < n; i++){
		if(i%2 == 0) alpha1[i] = 1;
		else alpha1[i] = 0;
	}

	int con = 0;
	int v = 1;
	for (int i = 0; i < n; i++){
		if(con < 3){ alpha2[i] = v; con++; }
		else if(con == 3 && v == 1){ v = 0; con = 0; i--;}
		else if(con == 3 && v == 0){ v = 1; con = 0; i--;}
		
	}

	for (int i = 0; i < n; i++){
		printf("%i\n",alpha2[i]);
	}
/*
	

	//Prueba xor con la norma

	double norm = 0;
	x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[4] = 1; x[5] = 1; x[6] = 1; x[7] = 1;
	

	for (int i = 0; i < n; i++){alpha1[i] = x[i] ^ alpha1[i];} //XOR
    for (int i = 0; i < n; i++) norm+= alpha1[i]; //Calculo norma
    norm = sqrt(norm); 
        
    

	for (int i = 0; i < n; i++){
		printf("%i\n",alpha1[i]);
	}
	printf("Norma %lf\n",norm);


 
	//Probando shuffle ME QUEDE AQUI ANTES DE PROBARLO

	int *A = calloc(n, sizeof(int));
	for (int i = 0; i < n; i++){A[i] = i;}
	for (int i = 0; i < n; ++i){printf("%d ",A[i]);}
	
	myshuffle(A,n);
	printf("\n");
	for (int i = 0; i < n; i++){printf("%d ",A[i]); }

*/
	
	return 0;
}