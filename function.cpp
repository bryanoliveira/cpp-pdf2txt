int ** recebeImagem(){
	FILE *arq = fopen("Treino.txt","r");
	int i,j;
	int qtd,**imagem;

	fscanf(arq,"%d",&qtd);
	imagem = (int **)malloc(sizeof(int)*qtd);

	for (i=0;i<qtd;i++){
		imagem[i] = (int *)malloc(sizeof(int)*601);
	}

	for (i=0;i<qtd;i++){
		for (j=0;j<600;j++){
			fscanf(arq,"%d",&imagem[i][j]);
		}
		desvet(imagem[i]);
	}
	return imagem;
}
