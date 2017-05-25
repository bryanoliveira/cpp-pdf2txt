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
	if (*colunaF == p.cols){
		*colunaF-=1;
	}
}

//Função para ajustar a ultima linha com a linha do maior caractere (Exemplo: g)
void ajusta(Mat p, int *linhaF, int colunaI, int colunaF){
	int i,mm,achou,cond;
	unsigned char vix;
	
	achou = 1;

	while (achou){
		achou = 0;
		cond = 1;
		for (i=colunaI;i<=colunaF && cond == 1 && *linhaF+1 < p.rows;i++){
			vix = p.at<uchar>(*linhaF+1,i);			
			mm = (int)vix;
			if (mm == 0){
				*linhaF = *linhaF+1;
				achou = 1;
				cond = 0;
			}
		}
	}

	if (*linhaF == p.rows){
		*linhaF -= 1;
	}
}


//Função que encontra os caracteres
//Retorno: Matriz com somente 1 caractere
Mat janelaCaractere(Mat p, int linhaI, int linhaF, int *colunaiC, int *colunafC, int colunaF){
	int i,op,cond,mm;
	unsigned char vix;
	
	op = 1;	

	while (op){
		op = 0;
		cond = 0;
		for (i=linhaI;i<=linhaF && cond == 0 && *colunaiC < colunaF;i++){
			vix = p.at<uchar>(i,*colunaiC);			
			mm = (int)vix;
			if (mm==0){
				cond = 1;
			}
		
		}
		if (cond==0 && *colunaiC<colunaF){
			*colunaiC = *colunaiC + 1;
			op = 1;
		}
	}
	
	*colunafC = *colunaiC;

	op = 1;

	while (op){
		op = 0;
		cond = 0;
		for (i=linhaI;i<=linhaF && cond == 0 && *colunafC+1<=colunaF;i++){
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

//Função para vetorizar a janela do caractere
void vetoriza(Mat p, int *vet){
	int i,j,k;
	unsigned char l;

	k=0;
	for (i=0;i<20;i++){
		for (j=0;j<30;j++){
			l = p.at<uchar>(i,j);
			if ((int)l == 255){			
				vet[k] = 1;
			} else vet[k] = 0;
			k++;
		}
	}


}

//Função para desvetorizar(usada para teste apenas)
void desvet(int *Vet){
	Mat p(20,30,DataType<uchar>::type);
	int i,j,k;
	k=0;
	for (i=0;i<20;i++){
		for (j=0;j<30;j++){
			p.at<uchar>(i,j) = (unsigned char)Vet[k];
			k++;
		}
	}
	imshow("aaa",p);
	waitKey(0);

}

void achaCaracteres (Mat p){
	int i,j,pp,op,imagem[601];
	int ultimal,primeiral,ultimac,primeirac,ultimacC,primeiracC;
	Mat ver;
	Mat red(30,20,DataType<uchar>::type);
	
	//Inicializa a primeira linha e a primeira coluna com 0, ou seja, ira começar a procurar a partir do pixel (0,0)
	primeiral = 0;
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
		if (op==0){
			return;
		}
		//Agora temos a janela do caractere, que no caso é: A[primeiral - ultimal][primeirac - ultimac]
		//Vamos então achar a janela da linha
		acharJL(p,primeiral,ultimal,&primeirac,&ultimac);

	

		/*Neste momento temos a janela da linha, porém, letras como p e q estao cortadas ao meio, pois suas "pernas" ficam
		abaixo da linha final */
		//Iremos então ajustar a ultima linha
		ajusta(p,&ultimal,primeirac,ultimac);
		if (primeiral<ultimal && primeirac<ultimac){
			primeiracC = primeirac;
			ultimacC = primeiracC;
			//Faz o reconhecimento das janelas de cada caractere
			while (primeiracC<ultimac){
				//A matriz ver é a matriz de cada caractere
				ver = janelaCaractere(p, primeiral,ultimal,&primeiracC,&ultimacC,ultimac);
				if (primeiracC<ultimacC && ultimacC){
					imshow("aa",ver);
					//Redimensiona a janela para ficar 30x20
					resize(ver, red, Size(30,20), 0, 0, INTER_LINEAR);
					imshow("resize",red);
					waitKey(0);
					threshold(red, red, 240, 255,0);
					//Vetoriza a imagem
					vetoriza(red,imagem);
					//Aqui entra o algoritmo do Marcos usando o vetor imagem como parametro
				}
				//Obs.: As duas proximas linhas SÃO NECESSARIAS
				ultimacC++;
				primeiracC = ultimacC;
			}
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
	//thresh = teste(Range(837,903),Range::all());
	//imshow("tr",thresh);
	//waitKey(0);
	achaCaracteres(teste);

	return (0);
}
