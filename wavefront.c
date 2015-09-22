// compile in ubuntu 14.04 
// using command gcc -o shareMemory -fopenmp shared.c

// add the opemMP model
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
// Specification of f, f(a,b) returns a+b+1;
int foo(int a, int b){
	return (a+b+1);
}

// function to compute value  in  matrix

int compute (int wavefront[][10], int x, int y){
	if(x==0 && y==0) return 0;
	else if ( x > 0 && y ==0) return foo(wavefront[x-1][0],0);
	else if ( x == 0 && y > 0) return foo(wavefront[0][y-1],0);
	else return foo(wavefront[x-1][y],wavefront[x][y-1]);
}



int main (){
	// share memory solution for wavefront computing
    // configuration of share memory
    const int M = 10;
    int wavefront [M][M];
    const int ZERO = 0;   // constant value for initials
	wavefront[0][0] = ZERO;


	for(int i =0 ; i < M ; i++){
		for(int j = 0 ; j < M; j++){
			wavefront[i][j] = 0;
		}
	}
	
	int n = 0; // n is number of elements that can be computed in parallel
	int m = 0;
    for(int level = 0; level < 2*M-1; level++){
    	n++;
		if(n < M){ // in upper triangle
			
			
			#pragma omp parallel 
			for (int i = 0 ;i<=n ; i++){
				
				wavefront[i][n-i] = compute(wavefront,i, n-i);
				
			}
		}
		
		else{ // in lower triangle
			m++;
			#pragma omp parallel 
			for (int i = m ;i<=n-m ; i++){
				wavefront[i][n-i] = compute(wavefront ,i, n-i);
				
			}
		}	
		
	}
	
	for(int i =0 ; i < M ; i++){
		for(int j = 0 ; j < M; j++)
			printf(" %d ",wavefront[i][j]);
		printf("\n");
	}

	return 0;
}



