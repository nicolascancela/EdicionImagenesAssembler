#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void leer_rgb(char *archivo, unsigned char *buffer, int filas, int columnas);
void escribir_rgb(char *archivo, unsigned char *buffer, int filas, int columnas);
void separarRGB(unsigned char*rgb, unsigned char *r, unsigned char *g, unsigned char *b,int ancho, int alto);
void combinarRGB(unsigned char*rgb, unsigned char *r, unsigned char *g, unsigned char *b,int ancho, int alto);
void invertirRGB(unsigned char*rgb,unsigned char*r, int alto, int ancho);
void gen_mascara(unsigned char *r, unsigned char *g, unsigned char *b,int cantidad, int rt, int gt, int bt, float t, unsigned char *mascara);
void aplica_mascara(unsigned char *mascara, unsigned char *canal1, unsigned char *canal2, int cantidad, unsigned char *resultado);

int main(int argc, char *argv[])
{
	//LEO PARAMETROS: 1) ARCHIVO1-RGB 2) ARCHIVO2-RGB 3) FILAS 4) COLUMNAS 5) R 6) G 7) B 8) T 9) RESULTADO.RGB
	char *archivo1 = argv[1];
	char *archivo2 = argv[2];
	int filas = atoi(argv[3]);
	int columnas= atoi(argv[4]);
	int R= atoi(argv[5]);
	int G= atoi(argv[6]);
	int B= atoi(argv[7]);
	float t = atof(argv[8]);
	char *resultado = argv[9];
	
	//RESERVO ESPACIO EN MEMORIA
	int cantB= columnas*filas;
	int cantA= cantB*3;
	if(cantA%4!=0)
	{
		cantA = cantA + (4 - cantA%4); //EL ESPACIO RESERVADO SERA MULTIPLO DE 4!
	}
	
	unsigned char * bufferArchivo1;
	bufferArchivo1 = malloc(cantA);
	unsigned char * bufferArchivo2;
	bufferArchivo2 = malloc(cantA);
	unsigned char * bufferResultado;
	bufferResultado = malloc(cantA);
	
	if(bufferArchivo1==NULL || bufferArchivo2==NULL || bufferResultado==NULL)
	{
		//Retorno con error
		return 1;
	}
	
	if(cantB%8!=0)
	{
		cantB = cantB + (8 - cantB%8); //EL ESPACIO RESERVADO SERA MULTIPLO DE 8!
	}
	
	unsigned char * R1;
	R1 = malloc(cantB);
	unsigned char * G1;
	G1 = malloc(cantB);
	unsigned char * B1;
	B1 = malloc(cantB);
	
	//CHEQUEO SI FALLO LA ALOCACIÓN DE MEMORIA
	if(R1==NULL || G1==NULL || B1==NULL)
	{
		//Retorno con error
		return 1;
	}
	
	unsigned char * R2;
	R2 = malloc(cantB);
	unsigned char * G2;
	G2 = malloc(cantB);
	unsigned char * B2;
	B2 = malloc(cantB);
	
	//CHEQUEO SI FALLO LA ALOCACIÓN DE MEMORIA
	if(R2==NULL || G2==NULL || B2==NULL)
	{
		//Retorno con error
		return 1;
	}

	unsigned char * R3;
	R3 = malloc(cantB);
	unsigned char * G3;
	G3 = malloc(cantB);
	unsigned char * B3;
	B3 = malloc(cantB);

	//CHEQUEO SI FALLO LA ALOCACIÓN DE MEMORIA
	if(R3==NULL || G3==NULL || B3==NULL)
	{
		//Retorno con error
		return 1;
	}	
		
	unsigned char * mascara;
	mascara = malloc(cantB);

	//CHEQUEO SI FALLO LA ALOCACIÓN DE MEMORIA
	if(mascara==NULL)
	{
		//Retorno con error
		return 1;
	}	
	
	//LEO ARCHIVO 1 Y ARCHIVO 2
	leer_rgb(archivo1, bufferArchivo1, filas, columnas);
	leer_rgb(archivo2, bufferArchivo2, filas, columnas);

	separarRGB(bufferArchivo1,R1,G1,B1, columnas, filas);
	separarRGB(bufferArchivo2,R2,G2,B2, columnas, filas);
	
	gen_mascara(R1, G1, B1, cantB, R, G, B, t, mascara);
	
	aplica_mascara(mascara, R1, R2, cantB, R3);
	aplica_mascara(mascara, G1, G2, cantB, G3);
	aplica_mascara(mascara, B1, B2, cantB, B3);

	combinarRGB(bufferResultado, R3, G3, B3,columnas, filas);
	
	//GUARDAMOS EL RESULTADO
	escribir_rgb(resultado, bufferResultado, filas, columnas);

	//LIBERO LOS BUFFERS!
	free(bufferArchivo1);
	free(bufferArchivo2);
	free(bufferResultado);

	free(R1);
	free(G1);
	free(B1);
	
	free(R2);
	free(G2);
	free(B2);
	
	free(R3);
	free(G3);
	free(B3);

	free(mascara);

	return 0;
}


void leer_rgb(char *archivo, unsigned char *buffer, int filas, int columnas)
{
	FILE *ArchivoL = NULL;
	ArchivoL = fopen(archivo, "rb");	//rb ES EL MODO DE LECTURA DEL ARCHIVO: SOLO LECTURA, O ESCRITURA!
	fread(buffer,columnas*filas*3,1,ArchivoL); //LLENO EL BUFFER
	fclose(ArchivoL); //CIERRO EL ARCHIVO
}

void escribir_rgb(char *archivo, unsigned char *buffer, int filas, int columnas)
{
	FILE *ArchivoE = NULL;
	ArchivoE = fopen(archivo, "wb"); //MODO DE LECTRUA: W ESCRITURA!
	fwrite(buffer, columnas*filas*3, 1,ArchivoE); //ESCRIBO EL ARCHIVO
	fclose(ArchivoE); //CIERRO EL ARCHIVO
	printf("El nombre del archivo es %s", archivo);
}

void separarRGB(unsigned char*rgb, unsigned char *r, unsigned char *g, unsigned char *b,int ancho, int alto)
{
    int i;
    int cont = 0;
    for(i=0; i< ancho*alto*3; i+=3)
    {
        r[cont]=rgb[i+0];
        g[cont]=rgb[i+1];
        b[cont]=rgb[i+2];
        cont++;
    }
}

void combinarRGB(unsigned char*rgb, unsigned char *r, unsigned char *g, unsigned char *b,int ancho, int alto)
{
    int i;
    for(i=0; i< ancho*alto; i++)
    {
        rgb[(i*3)+0]=r[i];
        rgb[(i*3)+1]=g[i];
        rgb[(i*3)+2]=b[i];
    }
}

void invertirRGB(unsigned char*rgb,unsigned char*r, int alto, int ancho)
{
    int i;
    for(i=0; i< ancho*alto*3; i+=3)
    {
        r[i+0] = 255- rgb[i+0];
        r[i+1] = 255- rgb[i+1];
        r[i+2] = 255 -rgb[i+2];
    }
}

