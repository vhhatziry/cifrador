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
void nombreSalida(const char* entrada_temporal, char* salida, size_t tam_salida, char OP);

// =================== main (sin cambios) ===================
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <alpha> <b> <E/D> <archivo_entrada>\n", argv[0]);
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    char opcion = argv[3][0];
    char *archivoEntrada = argv[4];
    char archivoSalida[256];
    if (MCD(a, 26) != 1) {
        printf("Error: alpha=%d no es coprimo con 26.\n", a);
        return 1;
    }
    nombreSalida(archivoEntrada, archivoSalida, sizeof(archivoSalida), opcion);
    cargarArchivo(archivoEntrada, archivoSalida, a, b, opcion);
    printf("%s", archivoSalida);
    return 0;
}

// =================== Funciones matemáticas (sin cambios) ===================
int MCD(int alpha, int n){
    int temp;
    while(n!=0){ temp = n; n = alpha % n; alpha = temp; }
    return alpha;
}

int inversoMultiplicativo(int a, int m) {
    a %= m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

// =================== Cargar archivo (sin cambios) ===================
void cargarArchivo(const char *entrada, const char *salida, int a, int b, char opcion){
    FILE *in = fopen(entrada, "r");
    if (!in) { printf("Error: no se pudo abrir %s\n", entrada); exit(1); }
    FILE *out = fopen(salida, "w");
    if (!out) { printf("Error: no se pudo crear %s\n", salida); fclose(in); exit(1); }
    if (opcion == 'E' || opcion == 'e')
        cifrarAfin(in, out, a, b);
    else
        descifrarAfin(in, out, a, b);
    fclose(in);
    fclose(out);
}

// =================== FUNCIÓN CIFRAR CORREGIDA ===================
void cifrarAfin(FILE *entrada, FILE *salida, int a, int b){
    char c;
    while ((c = fgetc(entrada)) != EOF) {
        if (isalpha(c)) {
            c = toupper(c);
            c = ((a * (c - 'A') + b) % 26) + 'A';
            fputc(c, salida); // Movido aquí adentro
        }
        // Los caracteres que no son letras simplemente se ignoran
    }
}

// =================== FUNCIÓN DESCIFRAR CORREGIDA ===================
void descifrarAfin(FILE *entrada, FILE *salida, int a, int b){
    int invA = inversoMultiplicativo(a, 26);
    if (invA == -1) { printf("Error: no existe inverso multiplicativo para a=%d\n", a); exit(1); }
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        if (isalpha(c)) {
            c = toupper(c);
            int y = c - 'A';
            int x = (invA * (y - b + 26)) % 26;
            c = x + 'a'; // Salida en minúsculas
            fputc(c, salida); // Movido aquí adentro
        }
        // Los caracteres que no son letras simplemente se ignoran
    }
}

// =================== NUEVA FUNCIÓN NOMBRESALIDA ===================
void nombreSalida(const char* entrada_temporal, char* salida, size_t tam_salida, char OP) {
    const char *nombre_base = strrchr(entrada_temporal, '/');
    if (nombre_base == NULL) {
        nombre_base = entrada_temporal;
    } else {
        nombre_base++;
    }

    const char *nombre_original = strchr(nombre_base, '_');
    if (nombre_original == NULL) {
        nombre_original = nombre_base;
    } else {
        nombre_original++;
    }

    const char *punto = strrchr(nombre_original, '.');
    const char *sufijo = (OP == 'E' || OP == 'e') ? "_e" : "_d";
    
    // El archivo de salida también debe estar en la carpeta uploads para que PHP lo encuentre
    char nombre_final[256] = "uploads/";

    if (punto) {
        size_t base_len = punto - nombre_original;
        strncat(nombre_final, nombre_original, base_len);
        strcat(nombre_final, sufijo);
        strcat(nombre_final, punto);
    } else {
        strcat(nombre_final, nombre_original);
        strcat(nombre_final, sufijo);
    }
    strncpy(salida, nombre_final, tam_salida - 1);
    salida[tam_salida - 1] = '\0';
}