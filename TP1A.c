#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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
        rgb[i+0]=r[i];
        rgb[i+1]=g[i];
        rgb[i+2]=b[i];
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

void interpolar(unsigned char *img1, unsigned char *img2, unsigned char *resultado, float p, int cantbits);

int main(int argc, char *argv[])
{
	//PARAMETROS: 1) ARCHIVO1-RGB 2) ARCHIVO2-RGB 3) FILAS 4) COLUMNAS 5) NIVELP 6) RESULTADO-RGB
	//AGARRO LOS PARAMETROS!
	char *archivo1 = argv[1];
	char *archivo2 = argv[2];
	int filas = atoi(argv[3]);
	int columnas= atoi(argv[4]);
	float p = atof(argv[5]);
	char *resultado = argv[6];

	//RESERVO EL ESPACIO EN MALLOC
	int r= columnas*filas*3;
	if(r%4!=0)
	{
		r = r+ (4 - r%4); //AHORA EL ESPACIO RESERVADO SERA MULTIPLO DE 4!


	}
	printf("El espacio a reservar sera de %d bytes\n", r);

	unsigned char * bufferArchivo1;
	bufferArchivo1 = malloc(r);

	unsigned char * bufferArchivo2;
	bufferArchivo2 = malloc(r);

	unsigned char * bufferResultado;
	bufferResultado = malloc(r);

	//CHEQUEO SINO FALLO LA ALOCACIÓN DE MEMORIA
	if(bufferArchivo1!=NULL && bufferArchivo2!=NULL && bufferResultado!=NULL)
	{

		//LEO ARCHIVO 1 Y ARCHIVO 2
		leer_rgb(archivo1, bufferArchivo1, filas, columnas);
		leer_rgb(archivo2, bufferArchivo2, filas, columnas);

		//invertirRGB(bufferArchivo1, bufferResultado, filas, columnas); //ANDA!
		interpolar(bufferArchivo1, bufferArchivo2, bufferResultado, p, r); //NO ANDA!

		//GUARDAMOS EL RESULTADO
		escribir_rgb(resultado, bufferResultado, filas, columnas);

		//LIBERO LOS BUFFERS!
		free(bufferArchivo1);
		free(bufferArchivo2);
		free(bufferResultado);

//SI DEVUELVO 0 ERA PORQUE TODO HABIA ANDANDO BIEN?
		return 0;
	}
	//SI DEVOLVÍA 1 ERA PORQUE SE HABIA PRODUCIDO UN ERROR?
    return 1;
}


