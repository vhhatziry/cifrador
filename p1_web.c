// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Prototipos de funciones 
int MCD(int alpha, int n);
int inversoMultiplicativo(int a, int m);
void cargarArchivo(const char *entrada, const char *salida, int a, int b, char opcion);
void cifrarAfin(FILE *entrada, FILE *salida, int a, int b);
void descifrarAfin(FILE *entrada, FILE *salida, int a, int b);
void nombreSalida(const char* entrada, char* salida, size_t tam_salida, char OP);

int main(int argc, char *argv[]) {
    // argc es el número de argumentos, argv[] son los argumentos.
    // Ejemplo de ejecución: ./programa 7 10 E archivo.txt
    if (argc != 5) {
        printf("Uso: %s <alpha> <b> <E/D> <archivo_entrada>\n", argv[0]);
        return 1; // Termina el programa si no hay suficientes argumentos
    }

    // Convertir los argumentos de texto a números y caracteres
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    char opcion = argv[3][0];
    char *archivoEntrada = argv[4];
    
    char archivoSalida[100];

    // Validar 'a' (alpha)
    if (MCD(a, 26) != 1) {
        printf("Error: alpha=%d no es coprimo con 26.\n", a);
        return 1;
    }

    // Generar el nombre del archivo de salida
    nombreSalida(archivoEntrada, archivoSalida, sizeof(archivoSalida), opcion);

    // Procesar el archivo
    cargarArchivo(archivoEntrada, archivoSalida, a, b, opcion);

    // Imprimimos el nombre del archivo de salida para que el script PHP lo sepa
    printf("%s", archivoSalida);

    return 0;
}

// Función MCD (sin cambios)
int MCD(int alpha, int n){
    int temp;
    while(n!=0){
        temp = n;
        n = alpha % n;
        alpha = temp;
    }
    return alpha;
}

// Inverso multiplicativo módulo m (sin cambios)
int inversoMultiplicativo(int a, int m) {
    a %= m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1; // no existe
}

// Función cargarArchivo (sin cambios)
void cargarArchivo(const char *entrada, const char *salida, int a, int b, char opcion){
    FILE *in = fopen(entrada, "r");
    if (!in) {
        printf("Error: no se pudo abrir %s\n", entrada);
        exit(1);
    }
    FILE *out = fopen(salida, "w");
    if (!out) {
        printf("Error: no se pudo crear %s\n", salida);
        fclose(in);
        exit(1);
    }
    if (opcion == 'E' || opcion == 'e')
        cifrarAfin(in, out, a, b);
    else
        descifrarAfin(in, out, a, b);
    fclose(in);
    fclose(out);
}

// Función cifrarAfin (sin cambios)
void cifrarAfin(FILE *entrada, FILE *salida, int a, int b){
    char c;
    while ((c = fgetc(entrada)) != EOF) {
        if (isalpha(c)) {
            c = toupper(c);
            c = ((a * (c - 'A') + b) % 26) + 'A';
        }
        fputc(c, salida);
    }
}

// Función descifrarAfin (sin cambios)
void descifrarAfin(FILE *entrada, FILE *salida, int a, int b){
    int invA = inversoMultiplicativo(a, 26);
    if (invA == -1) {
        printf("Error: no existe inverso multiplicativo para a=%d\n", a);
        exit(1);
    }
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        if (isalpha(c)) {
            c = toupper(c); // Para consistencia, trabajamos con mayúsculas
            int y = c - 'A';
            int x = (invA * (y - b + 26)) % 26;
            c = x + 'A';
            c = tolower(c); // Se pasa a minúscula como en tu código original
        }
        fputc(c, salida);
    }
}

// Función nombreSalida (sin cambios, pero corregí un detalle)
void nombreSalida(const char* entrada, char* salida, size_t tam_salida, char OP) {
    const char *punto = strrchr(entrada, '.');
    if (punto != NULL) {
        int longitud_base = punto - entrada;
        strncpy(salida, entrada, longitud_base);
        salida[longitud_base] = '\0';
        if (OP == 'E' || OP == 'e') {
            strcat(salida, "_cifrado");
        } else {
            strcat(salida, "_descifrado");
        }
        strcat(salida, punto);
    } else {
        // Si no hay extensión, solo añade el sufijo
        strcpy(salida, entrada);
        if (OP == 'E' || OP == 'e') {
            strcat(salida, "_cifrado.txt");
        } else {
            strcat(salida, "_descifrado.txt");
        }
    }
}