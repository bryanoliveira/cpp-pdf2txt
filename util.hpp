#include <stdlib.h>
#include <string.h>
#include <vector>

#define DIST_THRES 3

long Pot(long base, long exp) {
	if(exp < 1) {
		return 1;
	}
	return base * pot(base, exp-1);
}

char * Int2string(int n) {
	int digits, d = n;
	char * str;
	// conta os digitos
	for(digits = 0; d != 0; d /= 10, digits++);
	// aloca a string do tamanho certo
	str = (char *) malloc((digits + 1) * sizeof(char));
	// como é uma string, deve ter o marcador de fim
	str[digits] = '\0';
	// itera de digitos até 1
	for(int i = digits; i > 0; i--) {
		// pra cada digito de n, converte pra char e armazena
		// o correspondente em str é o inverso do correspondente da potência em n 
		str[digits - i] = (char) (48 + (int)((n % Pot(10, i)) / Pot(10, i - 1)));
	}
	return str;
}

int FileLen(char * fileName) {
	FileLen(filename, false);
}

int FileLen(char * fileName, bool ignoreLineBreaks) {
	int len = 0;
	FILE * file = fopen(filename, "r");
	
	if(ignoreLineBreaks) {
		while((c = fgetc(file)) != EOF && c != '\n') {
			len++;
		}
	}
	else {
		while((c = fgetc(file)) != EOF) {
			len++;
		}
	}
	
	fclose(file);
	return len;
}

int LineLen(FILE * file) {
	int len = 0;
	while((c = fgetc(file)) != EOF && c != '\n') {
		len++;
	}
	fseek(file, -len, SEEK_CUR); // !TODO: O VALOR NEGATIVO EM LEN PODE DAR ERRADO
	return len;	
}

int LineCount(char * fileName) {
	int len = 0;
	FILE * file = fopen(filename, "r");
	while((c = fgetc(file)) != EOF && c == '\n') {
		len++;
	}
	fclose(file);
	return len;
}

int HammingDistance(char * str1, char * str2) {
	int dist = 0, l1 = strlen(str1), l2 = strlen(str2);
	
	if(l1 == l2) {
		for(int i = 0; i < l1; i++) {
			if(str1[i] != str2[i]) {
				dist++;
			}
		}
	}
	else {
		return sizeof(long);
	}
	return dist;
}

// s: start index; e: end index;
char * Substr(char * original, int s, int e) {
	char * substr = malloc((s - e) * sizeof(char));
	for(int i = s; i < e; i++) {
		substr[i - s] = original[i];	
	}
	return substr;
}

// copia a palavra src para a string dest entre os indices 's' e 'e'
void Copy(char * dest, char * src, int s, int e) {
	for(int i = s; i < e; i++) {
		dest[i] = src[i-s];
	}
}

// Big Endian
void Char2Bin(char c, int * bin, int bits) {
	for(int i = 0; i < bits; i++) {
		bit[bits - i - 1] = c % 2;
		c /= 2;
	}
}

void AutoCorrect(char * fileName, char * dic) {
	// dicionário
	FILE * dicFile = fopen(dic, "r");
	// arquivo de saída
	FILE * outputFile = fopen(fileName, "rw");
	// quantidade de linhas no dicionário
	int dicLines = LineCount(dic);
	// tamanho da saída
	int fileLen = FileLen(fileName);
	// matriz do dicionário
	char ** dicMat = (char **) malloc(dicLines * sizeof(char *));
	// o vetor de conteúdos do output
	char * fileContent = (char *) malloc(fileLen * sizeof(char));
	// vetor de palavras empatadas // !TODO: talvez precise reservar espaço antes
	vector<char *> words;
	
	// le o dicionario numa matriz de chars, alocando dinamicamente cada linha usando o linelen
	for(int i = 0; i < dicLines; i++) {
		// conta o tamanho da linha do dicionario
		int lineLen = LineLen(dicFile);
		// aloca com 1 espaço a mais pra incluir o terminador de string
		dicMat[i] = (char *) malloc((lineLen+1) * sizeof(char));
		// inclui o terminador
		dicMat[i][lineLen] = '\0';
		// copia o conteúdo da linha do arquivo pra matriz
		for(int j = 0; j < lineLen; j++) {
			dicMat[i][j] = fgetc(dicFile);
		}
		// pega o \n
		fgetc(outputFile);
	}
	
	// marca onde pode começar a primeira palavra
	int lastWord = 0;
	// procura palavras por todo o arquivo de saída
	for(int i = 0; i < fileLen; i++) {
		// faz a leitura do arquivo armazenando em fileContent
		fileContent[i] = fgetc(outputFile);
		// se o caractere lido for um espaço, provavelmente já lemos uma palavra
		if(fileContent[i] == ' ' && i > lastword) {
			// calcula a palavra do dicionário que tem a menor distancia
			int lowerDistance = sizeof(long); // sim eu sei que o sizeof(long) nem é infinito mas já serve
			// guarda a palavra atual
			char * word = Substr(fileContent, lastWord, i - 1);
			// fixou uma palavra, agora varre o vetor de palavras do dicionario
			for(int j = 0; j < dicLines; j++) {
				// pega a palavra lida que começa em lastWord e termina em i-1 (não queremos comparar o espaço também)
				int dist;
				// se a distancia de for menor que o threshold
				if((dist = HammingDistance(word, dicMat[j])) < DIST_THRES) {
					// e se for menor que a menor
					if(dist < lowerDistance) {
						// reseta o vetor de palavras empatadas
						words.erase(words.begin(), words.end());
						words.push_back(dicMat[j]);
						// atualiza a lowerDistance
						lowerDistance = dist;
					}
					// se há empate
					else if(dist == lowerDistance) {
						// adiciona palavra às palavras empatadas
						words.push_back(dicMat[j]);
					}
				}
			}
			// encontrou as palavras que podem substituir, faz a substituição
			// se o vetor de palavras empatadas tem tamanho 1, substitui
			if(words.size() == 1) {
				Copy(fileContent, dicMat[j], lastWord, i - 1);
			}
			else if(words.size() > 1) {
				// se não, pergunta qual palavra deve-se substituir por
				// calcula o contexto
				int contextS, contextE;
				for(contextS = lastWord; contextS > 0 && lastWord - contextS < 10; contextS--);
				for(contextE = i - 1; contextE < fileLen && contextE - i - 1 < 10; contextE++);
				//
				cout << "Empate na correção: A palavra \"" << word << "\" no contexto \"..." << Substr(fileContent, contextS, contextE) << "...\" significa qual das opções abaixo?\n";
				for(int j = 0; j < words.size(); j++) {
					cout << j+1 << ". " << words[j] << endl;
				}
				cout << "0. Nenhuma das opções\n";
				char opt;
				cin >> opt;
				if(opt == '0') {
					cout << "A palavra não será substituída.\n";
				}
				else if(opt > '0' && opt < '0' + words.size()) {
					Copy(fileContent, words[(opt - '0')], lastWord, i - 1);
				}
				else {
					cout << "Opção inválida. A palavra não será substituída.\n";
				}
			}
			// se for 0, a palavra provavelmente está certa
			
			// atualiza a lastWord pra próxima iteração		
			lastWord = i + 1;
		} // endif
	} // endfor
}
