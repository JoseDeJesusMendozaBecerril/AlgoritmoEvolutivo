#include <stdio.h> 
#include <stdlib.h> 
#include "rand.h"
#include <math.h>



typedef struct{
    int *x; // cadena binaria (genotipo)
    double f; // fenotipo
}INDIVIDUO;

void imprimirPoblacion(INDIVIDUO *P, int N, int n);

double f1(int *x, int n){ //XOR con la norma
    
    //printf("Este es n%i\n",n);
    int *alpha1 = calloc(n, sizeof(int));
    double norm = 0;
    for (int i = 0; i < n; i++){ // 1 0 1 0 1 0 1 0 1 0 1 0
        if(i%2 == 0) alpha1[i] = 1;
        else alpha1[i] = 0;
    }

    for (int i = 0; i < n; i++){alpha1[i] = x[i] ^ alpha1[i];} //XOR
    for (int i = 0; i < n; i++) norm+= alpha1[i]; //Calculo norma
    
    norm = sqrt(norm); 
        
    return norm;
}
double f2(int *x, int n){ //XOR con la norma
    
    //printf("Este es n%i\n",n);
    int *alpha2 = calloc(n, sizeof(int));
    double norm = 0;
    int con = 0;
	int v = 1;
	for (int i = 0; i < n; i++){ // 0 0 0 1 1 1 0 0 0 1 1 1
		if(con < 3){ alpha2[i] = v; con++; }
		else if(con == 3 && v == 1){ v = 0; con = 0; i--;}
		else if(con == 3 && v == 0){ v = 1; con = 0; i--;}
		
	}

    for (int i = 0; i < n; i++){alpha2[i] = x[i] ^ alpha2[i];} //XOR
    for (int i = 0; i < n; i++) norm+= alpha2[i]; //Calculo norma
    
    norm = sqrt(norm); 
        
    return norm;
}

void funcion_objetivo(INDIVIDUO *ind, int n)
{
    ind->f = f1(ind->x,n); //cambie x f2
}

void alloc_inds(INDIVIDUO *P, int N, int n)
{
    for(int i=0; i<N; i++)
    {
        P[i].x = calloc(n, sizeof(int));
    }
}

void free_inds(INDIVIDUO *P, int N)
{
    for(int i=0; i<N; i++)
    {
        free(P[i].x);
    }
}


void inicializa_pop(INDIVIDUO *P, int N,int n)
{
    for(int i=0; i<N; i++) // recorre los individuos
    {
        for(int j=0; j<n; j++) // recorre los bits
        {
            P[i].x[j] = (randomperc()<0.5)? 1: 0;
        }
    }
}


void evaluar_pop(INDIVIDUO *P, int N, int n)
{
    for(int i=0; i<N; i++) // recorre los individuos
    {
        funcion_objetivo(&P[i],n);
    }
}


INDIVIDUO *torneo(INDIVIDUO *ind1, INDIVIDUO *ind2)
{
    if(ind1->f <= ind2->f ) return ind1;    
    else return ind2;
    
}
//Cruza 2 Individuos
INDIVIDUO crossover_Ind(INDIVIDUO *ind1, INDIVIDUO *ind2, int n){
	int c = n/2; //cuantos de cada papa
	INDIVIDUO nuevo;
	nuevo.x = calloc(n, sizeof(int));
    nuevo.f = 0;

	for (int i = 0; i < n; i++){
		if(i < c){ //Papa1
			nuevo.x[i] = ind1->x[i];
		}
		else{ //Papa2
			nuevo.x[i] = ind2->x[i];
		}	
	}
return nuevo;
}


// Cruza todos los individuos de P y los deja en Q (crea una población hijo)
void crossover_pop(INDIVIDUO *P, INDIVIDUO *Q, int N, int n){
        int barajeo1[N];
        int barajeo2[N];
        int ganadores1[N/2];
        int ganadores2[N/2];
        for (int i = 0; i < N; i++){barajeo1[i] = i;}
        for (int i = 0; i < N; i++){barajeo2[i] = i;}  
        for (int i = 0; i < N; i++){ganadores1[i] = 0; ganadores2[i]=0;}  
        
        myshuffle(barajeo1,N);//Barajeo 1
        myshuffle(barajeo2,N); //Barajeo 2

       // printf("Positions 1\n");
       // for (int i = 0; i < N; i++){ printf("%i ",barajeo1[i]);}printf("\n");
       // printf("Positions 2\n");
       // for (int i = 0; i < N; i++){ printf("%i ",barajeo2[i]);}

        //printf("\nPares 1 \n");
        for (int i = 0; i < N; i+=2){
         //   printf("%i %i\n",barajeo1[i],barajeo1[i+1]);
        }

       // printf("\nPares 2 \n");
        for (int i = 0; i < N; i+=2){
           // printf("%i %i\n",barajeo2[i],barajeo2[i+1]);
        }
        int j=0;
        
        for (int i = 0; i < N; i+=2){
            if(P[barajeo1[i]].f  <= P[barajeo1[i+1]].f ){ ganadores1[j] = barajeo1[i];}
            else { ganadores1[j] = barajeo1[i+1];}
            j++;
        }
        j=0;
        for (int i = 0; i < N; i+=2){
            if(P[barajeo2[i]].f  <= P[barajeo2[i+1]].f ){ ganadores2[j] = barajeo2[i];}
            else { ganadores2[j] = barajeo2[i+1];}
            j++;
        }
        
        
      //  printf("\nGanadores 1\n");
        for (int i = 0; i < N/2; i++){
      //      printf("%i ",ganadores1[i]);    
        }
      //  printf("\nGanadores 2\n");
        for (int i = 0; i < N/2; i++){
       //     printf("%i ",ganadores2[i]); 
        }

      //  printf("Valor de n %d\n",n);
      //  printf("Valor de N %d\n",N);
       
        //Cruza
        j=0; //meto a Q iniciando desde 0
        for (int i = 0; i < N/2; i+=2){ //Cruzo ganadores1
        	INDIVIDUO h1 = crossover_Ind(&P[ganadores1[i]],&P[ganadores1[i+1]],n); 
        	INDIVIDUO h2 = crossover_Ind(&P[ganadores1[i+1]],&P[ganadores1[i]],n); 	
        	Q[j] = h1;
        	Q[j+1] = h2;
        	j+=2; //Muevo en Q
        }
        j= N/2; //Meto a Q iniciando desde mitad para adelante
        for (int i = 0; i < N/2; i+=2){ //Cruzo ganadores2
        	 INDIVIDUO h1 = crossover_Ind(&P[ganadores2[i]],&P[ganadores2[i+1]],n); 
        	 INDIVIDUO h2 = crossover_Ind(&P[ganadores2[i+1]],&P[ganadores2[i]],n); 	
        	Q[j] = h1; //Meto en Q 
        	Q[j+1] = h2;
        	j+=2; //Muevo en Q	
        }

    	printf("\n");

        
            
}

// Muta todos los individuos de Q
void mutacion_pop(INDIVIDUO *Q, int N, int n) //cambie N por n
{
     
    double Pm = (double) 1 / n;
    //printf("Pm %lf\n",Pm);
    for (int i = 0; i < N; i++){
        for (int j = 0; j < n; j++){
            double a = randomperc();
            //printf("%lf\n",a);
            if(a < Pm){
                if(Q[i].x[j] == 0) Q[i].x[j] = 1;
                else Q[i].x[j] = 0;    
            }  
        }
        
    }
}


void copy_pop(INDIVIDUO *P, INDIVIDUO *Q, int N) // P = Q agregue N
{
    // P=Q
    for (int i = 0; i < N; i++){
        P[i] = Q[i];
    }
}   

void union_pop(INDIVIDUO *M, INDIVIDUO *P, INDIVIDUO *Q, int N){
	for (int i = 0; i < N; i++){
		M[i] = P[i];
	}
	for (int i = 0; i < N; i++){
		M[i+(N)] = Q[i];
	}
}


void seleccionar(INDIVIDUO *P, INDIVIDUO *M, int N) 
{
	for (int i = 0; i < N; i++){
		P[i] = M[i];
	}
    // P =  {a los N mejores individuos de M}
}
void imprimirIndividuo(INDIVIDUO *ind1 , int n){
	printf("[");	
	for (int i = 0; i < n; i++){
		printf("%d",ind1->x[i]); //Imprime Alelos o cadena de bits
	}
	printf("] ");
    printf("Aptitud %lf\n",ind1->f);	
}
void imprimirPoblacion(INDIVIDUO *P, int N, int n){ //N numero de Agentes Poblacion  n numero de Bits Cadena o alelos 
	for (int i = 0; i < N; i++){
		imprimirIndividuo(&P[i],n);
	}
}

/* qsort struct comparision function (aptitud f field) */ 
int compareByFitness(const void *a, const void *b) 
{ 
    INDIVIDUO *ia = (INDIVIDUO *)a;
    INDIVIDUO *ib = (INDIVIDUO *)b;
    return (double)(1000 *ia->f -1000 *ib->f);
	/* float comparison: returns negative if b > a 
	and positive if a > b. We multiplied result by 100.0
	to preserve decimal fraction */ 
 
} 

int main()
{
      //inicializo manual standar 
 /*   P[0].x[0] = 0; P[0].x[1] = 1; P[0].x[2] = 1; P[0].x[3] = 0;// 0110   
    P[1].x[0] = 0; P[1].x[1] = 1; P[1].x[2] = 1; P[1].x[3] = 1;// 0111   
    P[2].x[0] = 0; P[2].x[1] = 1; P[2].x[2] = 0; P[2].x[3] = 1;// 0101   
    P[3].x[0] = 1; P[3].x[1] = 1; P[3].x[2] = 0; P[3].x[3] = 0;// 1100   
    P[4].x[0] = 0; P[4].x[1] = 0; P[4].x[2] = 0; P[4].x[3] = 0;// 0000   
    P[5].x[0] = 1; P[5].x[1] = 1; P[5].x[2] = 1; P[5].x[3] = 1;// 1111   
    P[6].x[0] = 0; P[6].x[1] = 1; P[6].x[2] = 0; P[6].x[3] = 1;// 0101   
    P[7].x[0] = 1; P[7].x[1] = 1; P[7].x[2] = 1; P[7].x[3] = 0;// 1110
*/
	int N, n;
	double seed = 0.5;
    int max_gen; // maximo numero de iteracion
    warmup_random(seed);

	printf("Pop size: "); 
    scanf("%d", &N);
    printf("Numero bits: "); 
    scanf("%d", &n);
    printf("Seed: "); 
    scanf("%lf", &seed);
    printf("Number of generations: "); 
    scanf("%d", &max_gen);

    warmup_random(seed);
    INDIVIDUO *P,*Q,*M;
    

    P = calloc(N, sizeof(INDIVIDUO)); //vector
    alloc_inds(P, N, n); //matrix    
    inicializa_pop(P,N,n);
    

     Q = calloc(N, sizeof(INDIVIDUO));
     M = calloc(2*N, sizeof(INDIVIDUO));

     alloc_inds(Q, N, n);
     alloc_inds(M, 2*N, n);

    

     int op=0;
     printf("Seleccione 1(miu,lambda) 2(miu + lambda) \n");
     scanf("%d",&op);
     switch(op){
     	case 1:
     				printf("Este es P Inicial \n");
     				evaluar_pop(P, N,n);
     				imprimirPoblacion(P,N,n);

     				for(int gen=1; gen<=max_gen; gen++){ //Miu , lambda LISTA
				        printf("\nGeneration:%d",gen);
				        crossover_pop(P, Q, N,n);
				        mutacion_pop(Q, N,n);
				        evaluar_pop(Q, N,n);
				        // (mu, lambda)-GA
				        imprimirPoblacion(Q,N,n);
				        copy_pop(P, Q,N);
				    }
     	break;

     	case 2: 
     				printf("Este es P Inicial \n");
     				evaluar_pop(P, N,n);
     				imprimirPoblacion(P,N,n);

     				for (int gen=1; gen < max_gen; gen++){ //Miu + lambda    	
				     	printf("\nGeneration:%d",gen);
				     	crossover_pop(P, Q, N,n);
				        mutacion_pop(Q, N,n);
				        evaluar_pop(Q, N,n);
						union_pop(M, P, Q, N);
						qsort(M, 2*N, sizeof(INDIVIDUO), &compareByFitness);
						seleccionar(P,M,N);
						imprimirPoblacion(P,N,n);
					}

     	break;
     }

	//free_inds(P, N);
    //free_inds(Q, N);
    //free_inds(M, 2*N);   

    free(P);
    free(Q);
    free(M);

//     printf("Este es P Inicial \n");
//     evaluar_pop(P, N,n);
//     imprimirPoblacion(P,N,n);

/* PARTE CHIDA 1 YA 
     for(int gen=1; gen<=100; gen++){ //Miu , lambda LISTA
        printf("\nGeneration:%d",gen);
        crossover_pop(P, Q, N,n);
        mutacion_pop(Q, N,n);
        evaluar_pop(Q, N,n);
        // (mu, lambda)-GA
        imprimirPoblacion(Q,N,n);
        copy_pop(P, Q,N);
    }
*/
/*
     for (int gen=1; gen < 100; gen++){ //Miu + lambda    	
     	printf("\nGeneration:%d",gen);
     	crossover_pop(P, Q, N,n);
        mutacion_pop(Q, N,n);
        evaluar_pop(Q, N,n);
		union_pop(M, P, Q, N);
		qsort(M, 2*N, sizeof(INDIVIDUO), &compareByFitness);
		seleccionar(P,M,N);
		imprimirPoblacion(P,N,n);
	 
	}
*/
/*    
    
    
    
    printf("Pop size: "); 
    scanf("%d", &N);
    printf("Numero bits: "); 
    scanf("%d", &n);
    printf("Seed: "); 
    scanf("%lf", &seed);
    
    N=100; 
    n=50;
    max_gen=100;

    warmup_random(seed);
    P = calloc(N, sizeof(INDIVIDUO));
    Q = calloc(N, sizeof(INDIVIDUO));
    M = calloc(2*N, sizeof(INDIVIDUO));

    alloc_inds(P, N, n);
    alloc_inds(Q, N, n);
    alloc_inds(M, N, n);

    inicializa_pop(P, N);
    evaluar_pop(P, N);

    for(int gen=1; gen<=max_gen; gen++)
    {
        crossover_pop(P, Q, N);
        mutacion_pop(Q, N);
        evaluar_pop(Q, N);

        // (mu, lambda)-GA
        copy_pop(P, Q);

        // (mu+lambda)-GA
        union_pop(M, P, Q); // Union de P con Q (M=PUQ)
        //qsort(M, 2*N, sizeof(INDIVIDUO), &compara);
        seleccionar(P, M, N);
    }
    free_inds(P, N);
    free_inds(Q, N);
    free_inds(M, 2*N);   

    free(P);
    free(Q);
    free(M);
*/    
    return 0;
}
