#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//Função para achar em qual linha e em qual coluna tem algo escrito
//Retorno: 1 para bem sucedido, 0 para mal sucedido
int acharLI (Mat p,int *linhaI, int *colunaI){
	int i,j,mm;
	unsigned char vix;

	
	for (i=*linhaI;i<=p.rows;i++){
		for (j=0;j<=p.cols;j++){
			vix = p.at<uchar>(i,j);			
			mm = (int)vix;		
			if (mm == 0){
				*linhaI = i;
				*colunaI = j;
				return 1;
			}		
		}
	}
	return 0;
}

//Função para achar a janela do caractere encontrado
//Retorno: 1 para bem sucedido, 0 para mal sucedido
int acharJC(Mat p, int *linhaI, int *linhaF, int *colunaI, int *colunaF){
	int pp,a,b,i,mm;
	unsigned char vix;
	
	pp=1;	
	while (pp){
		a = 1;
		b = 1;
		while (a){
			vix = p.at<uchar>(*linhaF,*colunaI-1);			
			mm = (int)vix;
			if (mm == 0 && *colunaI > 0){
				*colunaI = *colunaI-1;
			} else a=0;
		}
			
		while (b){
			vix = p.at<uchar>(*linhaF,*colunaF+1);			
			mm = (int)vix;
			if (mm == 0 && *colunaF < p.cols){
				*colunaF= *colunaF + 1;
			} else b=0;
		}

		pp = 0;
		for (i=*colunaI;i<=*colunaF && pp==0 && *linhaF < p.rows;i++){
			vix = p.at<uchar>(*linhaF+1,i);			
			mm = (int)vix;
			if(mm==0){
				pp=1;
				*linhaF = *linhaF + 1;
			}
		}
	}
}

//Função para achar a janela da linha
void acharJL(Mat p, int linhaI, int linhaF, int *colunaI, int *colunaF){
	int i,j,mm,achou;
	unsigned char vix;

	achou = 0;

	//Acha o limite da esquerda
	for (i=0;i<=*colunaI && achou == 0;i++){
		for (j=linhaI;j<=linhaF && achou == 0;j++){
			vix = p.at<uchar>(j,i);			
			mm = (int)vix;
			if (mm == 0){
				*colunaI = i;
				achou = 1;
			}
		}
		
	}
	
	achou = 0;
	//Acha o limite da direita
	for (i=p.cols;i>=*colunaF && achou == 0;i--){
		for (j=linhaI;j<=linhaF && achou == 0;j++){
			vix = p.at<uchar>(j,i);			
			mm = (int)vix;
			if (mm == 0){
				*colunaF = i;
				achou = 1;
			}
		}
	}
}

void ajusta(Mat p, int *linhaF, int colunaI, int colunaF){
	int i,mm,achou,cond;
	unsigned char vix;
	
	achou = 1;

	while (achou){
		achou = 0;
		cond = 1;
		for (i=colunaI;i<=colunaF && cond == 1;i++){
			vix = p.at<uchar>(*linhaF+1,i);			
			mm = (int)vix;
			if (mm == 0){
				*linhaF = *linhaF+1;
				achou = 1;
				cond = 0;
			}
			
		}
	}
}

Mat janelaCaractere(Mat p, int linhaI, int linhaF, int *colunaiC, int *colunafC){
	int i,op,cond,mm;
	unsigned char vix;
	
	op = 1;	

	while (op){
		op = 0;
		cond = 0;
		for (i=linhaI;i<=linhaF && cond == 0;i++){
			vix = p.at<uchar>(i,*colunaiC);			
			mm = (int)vix;
			if (mm==0){
				cond = 1;
			}
		
		}
		if (cond==0){
			*colunaiC = *colunaiC + 1;
			op = 1;
		}
	}
	
	*colunafC = *colunaiC;

	op = 1;

	while (op){
		op = 0;
		cond = 0;
		for (i=linhaI;i<=linhaF && cond == 0;i++){
			vix = p.at<uchar>(i,*colunafC+1);			
			mm = (int)vix;
			if (mm == 0){
				*colunafC = *colunafC+1;
				op = 1;
				cond = 1;
			}
			
		}	
	}
	p = p(Range(linhaI,linhaF),Range(*colunaiC,*colunafC));
	return p;
}

void achaL (Mat p){
	int i,j,op;
	int ultimal,primeiral,ultimac,primeirac,ultimacC,primeiracC;
	Mat ver;
	
	//Inicializa a primeira linha e a primeira coluna com 0, ou seja, ira começar a procurar a partir do pixel (0,0)
	primeiral = 0;
	primeirac = 0;
	while(primeiral<p.rows){
		primeirac = 0;
		ultimac = 0;
		//Acha onde tem um pixel de cor preta (letra)
		op = acharLI(p,&primeiral,&primeirac);
		if (op == 0){
			return;
		}
	
		//Coloca valores na linha final e na coluna final
		ultimal = primeiral;
		ultimac = primeirac;

		//Nesse momento temos a seguinte janela: A[primeiral - primeiral][primeirac - primeirac]
		//Devemos agora definir até onde o caractere esta escrito, ou seja, a ultima linha e a ultima coluna (conectividade)
		op = acharJC(p,&primeiral,&ultimal,&primeirac,&ultimac);

		if (op == 0){
			return;
		}

		//Agora temos a janela do caractere, que no caso é: A[primeiral - ultimal][primeirac - ultimac]
		//Vamos então achar a janela da linha
		acharJL(p,primeiral,ultimal,&primeirac,&ultimac);


		/*Neste momento temos a janela da linha, porém, letras como p e q estao cortadas ao meio, pois suas "pernas" ficam
		abaixo da linha final */
		//Iremos então ajustar a ultima linha
		ajusta(p,&ultimal,primeirac,ultimac);
	
		primeiracC = primeirac;
		ultimacC = primeiracC;
		//Faz o reconhecimento das janelas de cada caractere
		while (primeiracC<ultimac){
			//A matriz ver é a matriz de cada caractere
			ver = janelaCaractere(p, primeiral,ultimal,&primeiracC,&ultimacC);
			//Aqui entra o algoritmo do Marcos pegando a matriz ver como parametro
			imshow("ca",ver);
			waitKey(0);
			//Obs.: As duas proximas linhas SÃO NECESSARIAS
			ultimacC++;
			primeiracC = ultimacC;
		}

		ultimal++;
		primeiral = ultimal;
	}
}


int main (int argc, char *argv[]){
	Mat teste = imread(argv[1]);
	Mat thresh;
	unsigned char caractere;
	int i,j,l,m,k,deu=0;


	cvtColor(teste,teste,CV_BGR2GRAY);

	
	
	threshold(teste, teste, 220, 255,0);
	//erosao(teste);
	achaL(teste);
	//Mat ver = teste(Range(110,140), Range(114,205));
	//caractere = teste.at<uchar>(Point(77,186));

	//cout << "AKI E: "<<(int)caractere;

	//imshow("aa",teste);
	//waitKey(0);
	return (0);
}
