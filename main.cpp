#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "util.hpp"
#include "preprocess.hpp"
#include "visao.hpp"
#include "mlp.hpp"

#define BITS 8

using namespace std;

/* error codes:
	-1: argumentos não informados
	-2: não foi possível abrir o arquivo para escrita da saída
	-3: não foi possível abrir o arquivo de saída do treinamento
	-4: não foi possível abrir a imagem de treinamento
*/
int main(int argc, char ** argv) {
	char cmd[500], opt, dic[500];
	int page = 1;
	FILE * output;
	Mat src;
	
	// verifica se foi passado o pdf como argumento
	if(argc < 2) {
		cout << "O arquivo .pdf de entrada ou uma opção deve ser informada!\nOpções:\n\t-h : ajuda\n\t-t : treinar rede\n";
		return -1;
	}
	if(argv[1][0] == '-' && argv[1][1] == 't') {
		int textLen, ** textBin;
		char outputName[200], inputName[200], charRead;
		FILE * desiredOutput;
		
		cout << "Insira a imagem de treinamento";
		cin >> inputName;
		if(!Preprocess(inputName, &src)) {
			cout << "Não foi possível abrir a imagem de treinamento.\n";
			return -4; // joga um erro se não conseguir abrir a imagem
		}
		cout << "Insira a saída esperada para o treinamento: ";
		cin >> outputName;
		
		if((desiredOutput = fopen(outputName, "r")) == NULL) {
			cout << "Não foi possível abrir a saída do treinamento.\n";
			return -3;
		}
	
		// lê o texto da saída
		textLen = FileLen(outputName, true);
		textBin = (char **) malloc(textLen * sizeof(char *));
		for(int i = 0; i < textLen;) {
			// aloca os vetores binários
			textBin[i] = (int *) malloc(BITS * sizeof(int));
		
			charRead = fgetc(desiredOutput);
			if(charRead == '\n')
				continue;
			
			// converte o caractere e armazena no vetor
			Char2Bin(charRead, textBin[i], BITS);
			
			// aqui embaixo pq se o char for uma quebra de linha ele não é contabilizado
			i++;
		}
		
		// pre-processar imagem
		// processar
			// pra cada letra, chamar a mlp passando a matriz da letra e a letra lida corresopndente
	}
	else {
		// tenta abrir o output
		if((output = fopen("outputTemp1.txt", "w")) == NULL) {
			cout << "Não foi possível inicializar o arquivo de saída. Verifique se temos permissão para escrita neste diretório.\n";
			return -2;
		}
		cout << "Convertendo .pdf para .png...\n";
		// cria uma pasta temporária
		system("mkdir temp");
		// cria o comando
		strcpy(cmd, "pdftoppm -png ");
		strcat(cmd, argv[1]);
		strcat(cmd, " temp/tdn");
		// converte o pdf para páginas em png
		system(cmd);
		// as páginas são armazenandas em "temp/tdn-X.png", onde X é o número da página
		cout << "Arquivo convertido.\n";
		// para cada página executa a detecção, concatenando o resultado
		strcpy(cmd, "temp/tdn-1.png");
		while(fopen(cmd, "r") != NULL) {
			cout << "Processando " << cmd << "\n";
	
			if(!Preprocess(cmd, &src)) {
				cout << "Não foi possível abrir a imagem de treinamento.\n";
				return -4; // joga um erro se não conseguir abrir a imagem
			}
			// detecção
				// Encontrar linhas (limites em y)
				// Encontra caracteres (limites em x)
				// Converte a submatriz do caractere pra escala padrão de entrada do MLP
			// Reconhecimento (para cada caractere)
				// Envia a matriz para o MLP
				// Salva o caractere reconhecido na string de saída temporária
				// (Fase de aprendizado) Compara com o resultado esperado e propaga os erros
			for(int ch = 0; ch < chcount; ch++) {
				outChar = MLP.Process(window[ch]);
				fputc(outChar, output);
			}
	
			// fim do processamento
		
			page++;	
			// recria a string só pra saber se existe a próxima página
			strcpy(cmd, "temp/tdn-");
			strcat(cmd, Int2string(page));
			strcat(cmd, ".png");
		}
		// fecha o arquivo, não precisamos mais escrever nele
		fclose(output);
		// apaga a pasta temporária
		system("rm -R temp");
	
		// módulo de correção
		cout << "Deseja rodar uma autocorreção de erros (s/n)? ";
		cin >> opt;
		if(opt == 's') {
			cout << "Entre com o dicionário correspondente ao idioma do arquivo: ";
			cin >> dic;
			AutoCorrect("outputTemp1.txt", dic);
		}
	
		system("mv outputTemp1.txt output.txt");
	
		// mostra resultados
		cout << "Deseja exibir os resultados (s/n)?";
		cin >> opt;
		if(opt == 's') {
			system("cat output.txt");
		}
	} // endif
	return 0;
}
