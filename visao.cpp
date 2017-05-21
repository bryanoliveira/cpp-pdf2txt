#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


void erosao(Mat p){
	unsigned char a;
	int v,b,i,j,c;
	Mat k(p.rows, p.cols, DataType<uchar>::type);

	for (i=0;i<p.rows;i++){
		for (j=0;j<p.cols;j++){
			k.at<uchar>(i,j) = p.at<uchar>(i,j);
		}
	}

	for (i=0;i<p.rows;i++){
		for (j=1;j<p.cols;j++){
			a = p.at<uchar>(i,j-1);
			v = (int)a;
			a = p.at<uchar>(i,j);
			b = (int)a;
			if (v-b != 0){
				c = 255;
				a = (unsigned char)c;
				k.at<uchar>(i,j) = a;
			}
		}
	}
	imshow("k",k);
	waitKey(0);

}

void achaL (Mat p, int l, int c){
	int i,j,achou = 0,temp,pp=1,a,b,k;
	int ultimal,primeiral,ultimac,primeirac,primeiracL,ultimacC,ultimacL,primeiracC;
	unsigned char vix;
	int mm;
	
	
	for (i=l;i<=p.rows && achou==0;i++){
		for (j=c;j<=p.cols && achou==0;j++){
			vix = p.at<uchar>(i,j);			
			mm = (int)vix;		
			if (mm == 0){
				achou = 1;
			}		
		}
	}
	primeiral = i;
	ultimal = i;
	primeirac = j;
	ultimac = j;
	


	//Verifica a conectividade

	while (pp){
		a = 1;
		b = 1;
		while (a){
			vix = p.at<uchar>(ultimal,primeirac-1);			
			mm = (int)vix;
			if (mm == 0 && primeirac > 0){
				primeirac--;
			} else a=0;
		}
			
		while (b){
			vix = p.at<uchar>(ultimal,ultimac+1);			
			mm = (int)vix;
			if (mm == 0 && ultimac < p.cols){
				ultimac++;
			} else b=0;
		}

		pp = 0;
		for (k=primeirac;k<=ultimac && pp==0 && ultimal<p.rows;k++){
			vix = p.at<uchar>(ultimal+1,k);			
			mm = (int)vix;
			if(mm==0){
				pp=1;
				ultimal++;
			}
		}
	}


	k = (ultimal+primeiral)/2;

	primeiracC = primeirac;

	achou = 0;

	for (i=0;i<primeirac && achou==0;i++){
		for (j=primeiral;j<=ultimal && achou==0;j++){
			vix = p.at<uchar>(j,i);
			mm = (int)vix;
			if (mm==0){
				achou = 1;
				primeiracC = i;
			}
		}

	}

	ultimacC = ultimac;
	achou = 0;

	for (i=p.cols;i>ultimac && achou==0;i--){
		for (j=primeiral;j<=ultimal && achou==0;j++){
			vix = p.at<uchar>(j,i);
			mm = (int)vix;
			if (mm==0){
				achou = 1;
				ultimacC = i;
			}
		}

	}

	primeirac = primeiracC;
	ultimac = ultimacC;


	//Ajusta a ultima linha
	achou = 1;
	while (achou==1){
		achou = 0;
		for (i=primeirac;i<=ultimac && achou == 0;i++){
			vix = p.at<uchar>(ultimal+1,i);
			mm = (int)vix;
			if (mm==0){
				achou = 1;
				ultimal++;
			}
			
			
		}
	}
	Mat ver;
	primeiracC = primeirac;
	ultimacC = primeirac;

	achou = 1;
	temp = 1;
	while (ultimacC<ultimac){
		achou = 0;

		if (temp==0){
			for (i=primeiral;i<=ultimal && temp==0;i++){
				vix = p.at<uchar>(i,primeiracC);
				mm = (int)vix;
				if (mm==0){
					temp = 1;
				}
			}
		}
		if (temp==0){
			primeiracC++;
			ultimacC++;
		}

		for (i=primeiral;i<=ultimal && achou==0 && ultimacC<ultimac;i++){
			vix = p.at<uchar>(i,ultimacC);
			mm = (int)vix;
			if (mm==0){
				ultimacC++;
				achou=1;
			}	
		}

		if (achou==0 && temp==1 || (achou==1 && ultimacC==ultimac)){
			ver = p(Range(primeiral,ultimal),Range(primeiracC,ultimacC));
			imshow("aaa",ver);
			waitKey(0);
			ultimacC;
			primeiracC = ultimacC;
			temp = 0;
		}
	}
	cout <<ultimal;

}


int main (int argc, char *argv[]){
	Mat teste = imread(argv[1]);
	Mat thresh;
	unsigned char caractere;
	int i,j,l,m,k,deu=0;


	cvtColor(teste,teste,CV_BGR2GRAY);

	
	
	threshold(teste, teste, 220, 255,0);
	erosao(teste);
	//achaL(teste,83,0);
	//Mat ver = teste(Range(110,140), Range(114,205));
	//caractere = teste.at<uchar>(Point(77,186));

	//cout << "AKI E: "<<(int)caractere;

	//imshow("aa",teste);
	//waitKey(0);
	return (0);
}
