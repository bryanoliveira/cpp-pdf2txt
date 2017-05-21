#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
using namespace std;

class MLP {
public:	
	double ** w_ji;
    double ** w_kj;
    int N, H, M;
    
    
    MLP(const char *directory) {
    	this->Open(directory);
	}
    
    MLP(int N,int H, int M) {
    	this->H = H;
    	this->M = M;
    	this->N = N;
    	w_ji = (double**)malloc(sizeof(double*)*H);
    	w_kj = (double**)malloc(sizeof(double*)*M);
    	for(int i = 0; i < H; i++)
    		w_ji[i] = (double*)malloc(sizeof(double)*(N+1));
    	for(int i = 0; i < M; i++)
    		w_kj[i] = (double*)malloc(sizeof(double)*(H+1));
    	
    }
	
    void Train(int eta, int Es, int **X, int **D, int dataLenght, int bias) {
        double E  = 100;    // iterador de aprendizado
        int count = 0;      // contador de épocas
        

		//sortei pesos para neuronios da Hidden layer
        srand((unsigned)time(NULL));
        for (int j = 0; j < H; j++) {
            for (int i = 0; i < N + 1; i++) {
                w_ji[j][i] = (rand()%2) - 0.5;
            }
        }
        
        //sortei pesos para neuronios da camada de saida
        for (int k = 0; k < M; k++) {
            for (int j = 0; j < H + 1; j++) {
                w_kj[k][j] = (rand()%2) - 0.5;
            }
        }

        double *net_pj   = (double*)malloc(sizeof(double)*H);
        double *o_pj     = (double*)malloc(sizeof(double)*(H+1));
        double *net_pk   = (double*)malloc(sizeof(double)*M);
        double *o_pk     = (double*)malloc(sizeof(double)*M);
        double *E_p      = (double*)malloc(sizeof(double)*dataLenght);
        double *delta_pk = (double*)malloc(sizeof(double)*M);
        double *delta_pj = (double*)malloc(sizeof(double)*H);

        while (E > Es) {

            count += 1;
            E = 0; // começa sem erros

            for (int p = 0; p < dataLenght; p++) {
                for (int j = 0; j < H; j++) {
                    net_pj[j] = 0;
                    for (int i = 0; i < N + 1; i++) {
                        // soma dos pesos multiplciados pelas entradas do neuronio J da camada do meio
                        net_pj[j] += (w_ji[j][i] * X[p][i]);
                    }
                    // Função sigmoid 
                    o_pj[j] = 1 / (1 + exp(-net_pj[j])); // o_pj[j] contém a saida do neuronio j da camada do meio
                }
                o_pj[H] = bias;        // bias

                for (int k = 0; k < M; k++) {
                    net_pk[k] = 0;
                    for (int j = 0; j < H + 1; j++) {
                        // soma dos pesos multiplicados pelas entradas do neuronio J da camada do meio
                        net_pk[k] += w_kj[k][j] * o_pj[j];
                    }
                    // Função simoid
                    o_pk[k] = 1 / (1 + exp(-net_pk[k])); // o_pk[j] contém a saida do neuronio j da camada de saida
                }
                // esse conjunto de dados tem erro 0
                E_p[p] = 0;
                // calcula delta_pk para cada neuronio da camada de saida
                for (int k = 0; k < M; k++) {
                    delta_pk[k] = (D[p][k] - o_pk[k]) * o_pk[k] * (1 - o_pk[k]);
                    E_p[p] = E_p[p] + 0.5 * pow((D[p][k] - o_pk[k]), 2); // calcula erro de cada elemento do conjunto de treinamento
                }

                // atribui erro do elemento p do conjunto de treinamento ao erro geral
                E = E + E_p[p];
                
				//calcula delta_pj para cada neuronio da camada do meio
                for (int j = 0; j < H; j++) {
                    delta_pj[j] = 0;
                    for (int k = 0; k < M; k++) {
                        delta_pj[j] += delta_pk[k] * w_kj[k][j] * o_pj[j] * (1 - o_pj[j]);
                    }
                }
                
                //ajusta pesos dos neuronios da camada de saida
                for (int k = 0; k < M; k++) {
                    for (int j = 0; j < H + 1; j++) {
                        w_kj[k][j] += eta * delta_pk[k] * o_pj[j];
                    }
                }
                
                //ajusta pesos dos neuronios da camada do meio
                for (int j = 0; j < H; j++) {
                    for (int i = 0; i < N + 1; i++) {
                        w_ji[j][i] += eta * delta_pj[j] * X[p][i];
                    }
                }
            }
        }
    }
    
    
    int Process(int *X) {
    	int result = 0;
    	double net_pj;
        double net_pk;
        double *o_pj = (double*)malloc(sizeof(double)*(H+1));
		double *o_pk = (double*)malloc(sizeof(double)*M);
    	
    	for (int j = 0; j < H; j++) {
            net_pj = 0;
            for (int i = 0; i < N; i++) {
                // soma dos pesos multiplciados pelas entradas do neuronio J da camada do meio
                net_pj += (w_ji[j][i] * X[i]);
            }
            net_pj -= w_ji[j][N];
            // Função sigmoid 
            o_pj[j] = 1 / (1 + exp(-net_pj)); // o_pj[j] contém a saida do neuronio j da camada do meio
        }

        for (int k = 0; k < M; k++) {
            net_pk = 0;
            for (int j = 0; j < H; j++) {
                // soma dos pesos multiplicados pelas entradas do neuronio J da camada do meio
                net_pk += w_kj[k][j] * o_pj[j];
            }
            net_pk -= w_kj[k][H];
            // Função simoid
            o_pk[k] = 1 / (1 + exp(-net_pk)); // o_pk[j] contém a saida do neuronio j da camada de saida
        }
        
        for(int l = M-1; l >= 0; l--) {
        	if(o_pk[l] > 0.5)
        		result += pow(2, l);
		}
		return result;
	}
	
	void Open(const char *directory){
		
		double *vet1, *vet2;
		
		FILE *arq;
		arq = fopen(directory, "rb");
		if(arq == NULL) {
			printf("Falha ao abrir arquibo '%s'", directory);
			exit(1);
		}
		
		fscanf(arq, "%d %d %d\n", &N, &H, &M);
		
		vet1 = (double*)malloc(sizeof(double) * H * (N+1));
		vet2 = (double*)malloc(sizeof(double) * M * (H+1));
		
		if(fread(vet1, sizeof(double) * H, N + 1, arq) != N + 1) {
			printf("Erro na leitura dos pesos da ANN(hidden layer)!\n");
			exit(1);
		}
		if(fread(vet2, sizeof(double) * M, H + 1, arq) != H + 1) {
			printf("Erro na leitura dos pesos da ANN(output layer)!\n");
			exit(1);
		}
		
		w_ji = (double**)malloc(sizeof(double*)*H);
		w_kj = (double**)malloc(sizeof(double*)*M);
		for(int j = 0; j<H; j++) {
			w_ji[j] = (double*)malloc(sizeof(double)*(N+1));
			for(int i = 0; i<N+1; i++) {
				w_ji[j][i] = vet1[j*(N+1) + i];
			}
		}
		for(int k = 0; k<H; k++) {
			w_kj[k] = (double*)malloc(sizeof(double)*(H+1));
			for(int j = 0; j<H+1; j++) {
				w_kj[k][j] = vet2[k*(H+1) + j];
			}
		}
		
		free(vet1);
		free(vet2);
	}
	
	void Save(const char *directory) {
		
		double vet1[H*(N+1)];
		double vet2[M*(H+1)];
		
		for(int j = 0; j<H; j++) {
			for(int i = 0; i<N+1; i++) {
				vet1[j*(N+1) + i] = w_ji[j][i];
			}
		}
		
		for(int k = 0; k<M; k++) {
			for(int j = 0; j<H+1; j++) {
				vet2[k*(H+1) + j] = w_kj[k][j];
			}
		}
		
		FILE *arq;
		arq = fopen(directory, "wb");
		if(arq == NULL) {
			printf("Falha ao criar arquivo '%s'", directory);
			exit(1);
		}
		
		fprintf(arq, "%d %d %d\n", N, H, M);
		fwrite(vet1, sizeof(double) * H, N+1, arq);
		fwrite(vet2, sizeof(double) * M, H+1, arq);
		
		fclose(arq);
	}
};
