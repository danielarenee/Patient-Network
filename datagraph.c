
//vertices = los puntos del conjunto de datos 
//aristas = se forman entre los puntos que estan a una distancia menor de r
//dot -Tpng grafo_pacientes.dot -o grafo_pacientes.png

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_PATIENTS 145

// funcion calcular la distancia entre dos puntos
double distancia(double *punto1, double *punto2) { //distancia euclidiana
    
    double suma = 0.0; //variable para la suma de los cuadrados de las diferencias
    for (int i = 0; i < 5; i++) {
        suma += (punto1[i] - punto2[i]) * (punto1[i] - punto2[i]); //se suma el cuadrado de la diferencia entre las coordenadas
    }
    return sqrt(suma); //retorna la raiz cuadrada de la suma
}

//..............................................................
int main() {
   
    FILE *file = fopen("Diabetes.csv", "r");//abrir archivo
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo.\n"); //o no o nada
        return 1;
    }

   // saltar encabezado
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);

    double data[NUM_PATIENTS][5]; //array
    for (int i = 0; i < NUM_PATIENTS; i++) {
        fscanf(file, "%*d,%lf,%lf,%lf,%lf,%lf,%*s", //no lee patient number ni clinical classification
               &data[i][0], &data[i][1], &data[i][2], &data[i][3], &data[i][4]);
    }
    fclose(file);

    double r;

    printf("Da el numero para r: ");
    scanf("%lf", &r);


    double matriz_distancias[NUM_PATIENTS][NUM_PATIENTS] = {0}; //matriz de distancias
    int matriz_adyacencia[NUM_PATIENTS][NUM_PATIENTS] = {0}; //matriz de adyacencias

    // calcular las distancias y llenar la matriz de distancias y la matriz de adyacencia
    for (int i = 0; i < NUM_PATIENTS; i++) {
        for (int j = i + 1; j < NUM_PATIENTS; j++) {
            double distancia_actual = distancia(data[i], data[j]); //calcular la distancia entre paciente i y paciente j
            matriz_distancias[i][j] = distancia_actual; //almacenar en matriz de distancias 
            matriz_distancias[j][i] = distancia_actual;
            if (distancia_actual > 0 && distancia_actual < r) { // si la distancia es menor a r se conectan los nodos en el  grafo
                matriz_adyacencia[i][j] = 1;
                matriz_adyacencia[j][i] = 1;
            }
        }
    }

    // guardar las conexiones en un txt
    FILE *output_file = fopen("matriz_adyacencia.txt", "w"); //nuevo txt con matriz de adyacencia 
    if (output_file == NULL) {
        printf("no se pudo abrir el archivo de salida");
        return 1;
    }

    fprintf(output_file, "Matriz de adyacencia:\n\n");
    for (int i = 0; i < NUM_PATIENTS; i++) { // imprime las conexiones de todos los pacientes
        int hay_conexiones = 0; 
        for (int j = 0; j < NUM_PATIENTS; j++) {
            if (matriz_adyacencia[i][j] == 1) { // verificar si hay una conexión con otro paciente
                hay_conexiones = 1;
                break; // si hay al menos una sale del for
            }
        }
        if (hay_conexiones) { 
            fprintf(output_file, "Paciente %d: ", i + 1);
            for (int j = 0; j < NUM_PATIENTS; j++) {
                if (matriz_adyacencia[i][j] == 1) { // imprimir todos los pacientes conectados
                    fprintf(output_file, "%d, ", j + 1);
                }
            }
            fprintf(output_file, "\n");
        } else { //o no o nada
            fprintf(output_file, "Paciente %d: No tiene conexiones\n", i + 1);
        }
    }

    fclose(output_file);
    printf("Matriz de adyacencia guardada en 'matriz_adyacencia.txt'.\n");

   // Guardar la matriz binaria de adyacencia en un archivo de texto
    FILE *binary_output_file = fopen("matriz_binaria_adyacencia.txt", "w");
    if (binary_output_file == NULL) {
        printf("Error: no se pudo abrir el archivo binario de salida.\n");
        return  1;
    }

    for (int i = 0; i < NUM_PATIENTS; i++) {
        for (int j = 0; j < NUM_PATIENTS; j++) {
            fprintf(binary_output_file, "%2d", matriz_adyacencia[i][j]); // Escribir la matriz de adyacencia completa como binaria (0 o 1), con alineación para mejor lectura
        }
        fprintf(binary_output_file, "\n");
    }

    fclose(binary_output_file); // Cerrar el archivo de salida binario
    printf("Matriz binaria de adyacencia guardada en 'matriz_binaria_adyacencia.txt'.\n");

    // generar el  DOT para visualizar con Graphviz
    FILE *dot_file = fopen("grafo_pacientes.dot", "w");
    if (dot_file == NULL) {
        printf("no se pudo abrir el archivo DOT de salida\n");
        return 1;
    }

    fprintf(dot_file, "graph G {\n");
    for (int i = 0; i < NUM_PATIENTS; i++) {
        for (int j = i + 1; j < NUM_PATIENTS; j++) {
            if (matriz_adyacencia[i][j] == 1) {
                fprintf(dot_file, "  %d -- %d;\n", i + 1, j + 1);
            }
        }
    }
    fprintf(dot_file, "}\n");
    fclose(dot_file);
    printf("Archivo DOT para Graphviz guardado en 'grafo_pacientes.dot'.\n");

    return 0;
}
