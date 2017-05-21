#include <opencv2/opencv.hpp>

using namespace std;

#define LIM_THRESH 200
#define FILTER_WINDOW 9
#define SIGMA_COLOR 75
#define SIGMA_SPACE 75
#define MAX_INTENSITY 255
#define THRESH_METHOD 0

bool Preprocess(char * srcName, Mat * src) {
	Mat img = imread(srcName);
	if(img.empty()) {
		return false;	
	}
	
	// transforma em grayscale
	cvtColor(img, img, COLOR_BGR2GRAY);
	
	// remove ruídos
	//medianBlur(img, img, FILTER_WINDOW); // imagem fonte, destino, tamanho da janela
	bilateralFilter(img, img, FILTER_WINDOW, SIGMA_COLOR, SIGMA_SPACE); // imagem fonte, destino, tamanho da janela, sigma de cor (distancia de cores consideradas correspondentes), sigma de espaço (distancia de pixels considerados correspondentes);
	
	// faz a limiarização
	threshold(img, img, LIM_THRESH, MAX_INTENSITY, THRESH_METHOD); // fonte, destino, limiar, max-intensity, método
	
	// !TODO: processar pose
	
	*src = img;
	return true
}
