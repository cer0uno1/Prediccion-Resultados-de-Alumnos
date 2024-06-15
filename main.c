#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    char nombre[50];
    int dni;
    int legajo;
    char materia[50];
    int faltas;
    bool libre;
    int parcial1;
    int parcial2;
    int notaFinal;
    bool materiaAprobada;
} alumno;

void limpiar() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    FILE *arch;
    int opcion = 0;
    int i = 0;
    int j = 0;
    int n = 0;
    int indice;
    int datoscargados = 0;
    int continuar;
    int numAlumnos;

    printf("Bienvenido al programa de predicción de resultados de alumno\n");
    printf("Ingrese el numero de alumnos a cargar: \n");
    scanf("%i", &numAlumnos);
    limpiar();

    while (numAlumnos <= 0) {
        printf("Numero incorrecto, ingrese de nuevo el numero de alumnos a cargar \n");
        scanf("%i", &numAlumnos);
        limpiar();
    }

    alumno alumnos[numAlumnos];

    arch = fopen("alumnos.dat", "wb+");
    do {
        printf("Ingrese la opcion que desea usar.\n 1 para cargar datos. \n 2 para borrar datos cargados. \n 3 para modificar datos cargados. \n 4 para enlistar todos los datos cargados. \n 5 para enlistar los datos de un solo alumno solicitado.\n 6 para cerrar el programa.\n");
        scanf("%i", &opcion);
        limpiar();

        switch (opcion) {
            case 1:
                arch = fopen("alumnos.dat", "ab");
                for (i = 0; i < numAlumnos; i++) {
                    printf("Ingrese el nombre completo del alumno %i: \n", i + 1);
                    fgets(alumnos[i].nombre, 50, stdin);
                    alumnos[i].nombre[strcspn(alumnos[i].nombre, "\n")] = 0;
                    while (strlen(alumnos[i].nombre) > 50) {
                        printf("Supero los caracteres maximos. Introduzca de nuevo el nombre: \n");
                        fgets(alumnos[i].nombre, 50, stdin);
                        alumnos[i].nombre[strcspn(alumnos[i].nombre, "\n")] = 0;
                    }

                    printf("Ingrese el DNI del alumno %i: \n", i + 1);
                    scanf("%i", &alumnos[i].dni);
                    limpiar();

                    printf("Ingrese el legajo del alumno %i: \n", i + 1);
                    scanf("%i", &alumnos[i].legajo);
                    limpiar();

                    printf("Ingrese el nombre de la materia:\n");
                    fgets(alumnos[i].materia, 50, stdin);
                    alumnos[i].materia[strcspn(alumnos[i].materia, "\n")] = 0;

                    printf("Ingrese las faltas del alumno %i: \n", i + 1);
                    scanf("%i", &alumnos[i].faltas);
                    limpiar();
                    while (alumnos[i].faltas < 0) {
                        printf("No se puede tener faltas negativas. Ingrese nuevamente: \n");
                        scanf("%i", &alumnos[i].faltas);
                        limpiar();
                    }
                    if (alumnos[i].faltas >= 4) {
                        printf("ADVERTENCIA!\n El alumno ha quedado libre. Sin importar si ha aprobado o no, debera recursar la materia.\n");
                        alumnos[i].libre = true;
                    } else {
                        alumnos[i].libre = false;
                    }

                    printf("Ingrese la calificacion del primer parcial: \n");
                    scanf("%i", &alumnos[i].parcial1);
                    limpiar();

                    printf("Ingrese la calificacion del segundo parcial: \n");
                    scanf("%i", &alumnos[i].parcial2);
                    limpiar();

                    if ((alumnos[i].parcial1 >= 6) && (alumnos[i].parcial2 >= 6)) {
                        printf("El alumno %s promociono la materia, no debera rendir final.\n", alumnos[i].nombre);
                        alumnos[i].notaFinal = ((alumnos[i].parcial1 + alumnos[i].parcial2) / 2);
                        printf("La nota final del alumno %s es: %i \n", alumnos[i].nombre, alumnos[i].notaFinal);
                        alumnos[i].materiaAprobada = true;
                    } else if ((alumnos[i].parcial1 >= 4 && alumnos[i].parcial1 <= 5) || (alumnos[i].parcial2 >= 4 && alumnos[i].parcial2 <= 5)) {
                        printf("El alumno aprobo ambos parciales, pero debera rendir el examen final. \n ");
                        printf("Ingrese la nota obtenida en el examen final: \n");
                        scanf("%i", &alumnos[i].notaFinal);
                        limpiar();
                    } else {
                        printf("El alumno no alcanzó la nota necesaria para aprobar la materia, debera recursarla\n");
                        alumnos[i].materiaAprobada = false;
                    }

                    if (alumnos[i].notaFinal >= 6) {
                        printf("El alumno %s ha aprobado la cursada de %s, con una nota final de %i \n", alumnos[i].nombre, alumnos[i].materia, alumnos[i].notaFinal);
                        alumnos[i].materiaAprobada = true;
                    } else {
                        printf("El alumno %s no ha alcanzado la nota necesaria para aprobar la materia %s. (Su nota final fue: %i) \n", alumnos[i].nombre, alumnos[i].materia, alumnos[i].notaFinal);
                        alumnos[i].materiaAprobada = false;
                    }

                    if ((alumnos[i].materiaAprobada == true) && (alumnos[i].libre == false)) {
                        printf("El alumno %s ha cumplido todos los requisitos para aprobar la materia. \n", alumnos[i].nombre);
                    } else {
                        printf("El alumno %s no ha cumplido con uno o mas de los requisitos para aprobar la materia.\nDebera recursarla.\n", alumnos[i].nombre);
                    }

                    fwrite(&alumnos[i], sizeof(alumno), 1, arch);
                    printf("\n");
                }
                datoscargados = 1;
                fclose(arch);
                break;

            case 2:
                if (datoscargados != 1) {
                    printf("No hay datos cargados, debe cargar datos primero.\n");
                    break;
                } else {
                    arch = fopen("alumnos.dat", "rb+");
                    if (arch == NULL) {
                        printf("El archivo no existe/no se ha encontrado\n");
                        break;
                    }
                }

                printf("Ingrese el numero del alumno al que desea borrar sus datos: \n");
                scanf("%i", &indice);
                limpiar();
                indice--;

                if (indice >= 0 && indice < numAlumnos) {
                    alumno temp;
                    int pos = 0;

                    fseek(arch, 0, SEEK_SET);
                    for (int i = 0; i < numAlumnos; i++) {
                        fread(&temp, sizeof(alumno), 1, arch);
                        if (i != indice) {
                            fseek(arch, pos * sizeof(alumno), SEEK_SET);
                            fwrite(&temp, sizeof(alumno), 1, arch);
                            pos++;
                        }
                    }

                    ftruncate(fileno(arch), pos * sizeof(alumno));

                    for (int i = indice; i < numAlumnos - 1; i++) {
                        alumnos[i] = alumnos[i + 1];
                    }
                    numAlumnos--;

                    printf("Alumno borrado correctamente. \n");
                } else {
                    printf("Numero de alumno incorrecto. \n");
                }
                fclose(arch);
                break;

            case 3:
                if (datoscargados != 1) {
                    printf("No hay datos cargados, debe ingresar datos primero. \n");
                    break;
                } else {
                    arch = fopen("alumnos.dat", "rb+");
                    if (arch == NULL) {
                        printf("El archivo no existe/no se ha encontrado");
                        break;
                    }
                }
                printf("Ingrese el numero del alumno cuyos datos desea modificar: \n");
                scanf("%i", &indice);
                limpiar();
                indice--;

                if (indice < 0 || indice >= numAlumnos) {
                    printf("Numero de alumno incorrecto.\n");
                    fclose(arch);
                    break;
                } else {
                    do {
                        fseek(arch, indice * sizeof(alumno), SEEK_SET);
                        fread(&alumnos[indice], sizeof(alumno), 1, arch);

                        int modificacion;
                        printf("Seleccione el dato que desea modificar:\n");
                        printf("1 para modificar nombre.\n");
                        printf("2 para modificar DNI.\n");
                        printf("3 para modificar legajo.\n");
                        printf("4 para modificar el nombre de la materia.\n");
                        printf("5 para modificar las faltas.\n");
                        printf("6 para modificar la calificacion del primer parcial y segundo parcial (y por ende, la calificacion final).\n");
                        printf("7 para salir.\n");
                        scanf("%i", &modificacion);
                        limpiar();

                        switch (modificacion) {
                            case 1:
                                printf("Ingrese el nuevo nombre: \n");
                                fgets(alumnos[indice].nombre, 50, stdin);
                                alumnos[indice].nombre[strcspn(alumnos[indice].nombre, "\n")] = 0;
                                break;
                            case 2:
                                printf("Ingrese el nuevo DNI: \n");
                                scanf("%i", &alumnos[indice].dni);
                                limpiar();
                                break;
                            case 3:
                                printf("Ingrese el nuevo legajo: \n");
                                scanf("%i", &alumnos[indice].legajo);
                                limpiar();
                                break;
                            case 4:
                                printf("Ingrese el nuevo nombre de la materia: \n");
                                fgets(alumnos[indice].materia, 50, stdin);
                                alumnos[indice].materia[strcspn(alumnos[indice].materia, "\n")] = 0;
                                break;
                            case 5:
                                printf("Ingrese la nueva cantidad de faltas: \n");
                                scanf("%i", &alumnos[indice].faltas);
                                limpiar();
                                alumnos[indice].libre = alumnos[indice].faltas >= 4;
                                break;
                            case 6:
                                printf("Ingrese la nueva calificacion del primer parcial: \n");
                                scanf("%i", &alumnos[indice].parcial1);
                                limpiar();
                                printf("Ingrese la nueva calificacion del segundo parcial: \n");
                                scanf("%i", &alumnos[indice].parcial2);
                                limpiar();
                                if ((alumnos[indice].parcial1 >= 6) && (alumnos[indice].parcial2 >= 6)) {
                                    alumnos[indice].notaFinal = (alumnos[indice].parcial1 + alumnos[indice].parcial2) / 2;
                                    alumnos[indice].materiaAprobada = true;
                                } else if ((alumnos[indice].parcial1 >= 4 && alumnos[indice].parcial1 <= 5) || (alumnos[indice].parcial2 >= 4 && alumnos[indice].parcial2 <= 5)) {
                                    printf("El alumno debe rendir examen final. Ingrese la calificacion del examen final: \n");
                                    scanf("%i", &alumnos[indice].notaFinal);
                                    limpiar();
                                    alumnos[indice].materiaAprobada = alumnos[indice].notaFinal >= 4;
                                } else {
                                    alumnos[indice].materiaAprobada = false;
                                }
                                break;
                            case 7:
                                printf("Saliendo de la modificacion.\n");
                                break;
                            default:
                                printf("Opcion invalida.\n");
                                break;
                        }

                        fseek(arch, indice * sizeof(alumno), SEEK_SET);
                        fwrite(&alumnos[indice], sizeof(alumno), 1, arch);

                        if (modificacion != 7) {
                            printf("Desea continuar modificando los datos del alumno? (1 para si, 0 para no) \n");
                            scanf("%i", &continuar);
                            limpiar();
                        } else {
                            continuar = 0;
                        }
                    } while (continuar == 1);

                    fclose(arch);
                }
                break;

            case 4:
                if (datoscargados != 1) {
                    printf("No hay datos cargados, debe ingresar datos primero.\n");
                    break;
                } else {
                    arch = fopen("alumnos.dat", "rb");
                    if (arch == NULL) {
                        printf("El archivo no existe/no se pudo abrir.\n");
                        break;
                    }

                    alumno temp;
                    while (fread(&temp, sizeof(alumno), 1, arch) == 1) {
                        printf("Nombre: %s\n", temp.nombre);
                        printf("DNI: %d\n", temp.dni);
                        printf("Legajo: %d\n", temp.legajo);
                        printf("Materia: %s\n", temp.materia);
                        printf("Faltas: %d\n", temp.faltas);
                        printf("Quedó libre: %s\n", temp.libre ? "Sí" : "No");
                        printf("Nota del primer parcial: %d\n", temp.parcial1);
                        printf("Nota del segundo parcial: %d\n", temp.parcial2);
                        printf("Nota final: %d\n", temp.notaFinal);
                        printf("Materia aprobada: %s\n", temp.materiaAprobada ? "Sí" : "No");
                        printf("\n");
                    }

                    fclose(arch);
                }
                break;

            case 5:
                if (datoscargados != 1) {
                    printf("No hay datos cargados, debe ingresar datos primero. \n");
                    break;
                } else {
                    arch = fopen("alumnos.dat", "rb");
                    if (arch == NULL) {
                        printf("El archivo no existe/no se pudo abrir.\n");
                        break;
                    }

                    printf("Ingrese el numero del alumno cuyos datos desea consultar: \n");
                    scanf("%i", &indice);
                    limpiar();
                    indice--;

                    if (indice >= 0 && indice < numAlumnos) {
                        fseek(arch, indice * sizeof(alumno), SEEK_SET);
                        alumno temp;
                        if (fread(&temp, sizeof(alumno), 1, arch) == 1) {
                            printf("Nombre: %s\n", temp.nombre);
                            printf("DNI: %d\n", temp.dni);
                            printf("Legajo: %d\n", temp.legajo);
                            printf("Materia: %s\n", temp.materia);
                            printf("Faltas: %d\n", temp.faltas);
                            printf("Quedó libre: %s\n", temp.libre ? "Sí" : "No");
                            printf("Nota del primer parcial: %d\n", temp.parcial1);
                            printf("Nota del segundo parcial: %d\n", temp.parcial2);
                            printf("Nota final: %d\n", temp.notaFinal);
                            printf("Materia aprobada: %s\n", temp.materiaAprobada ? "Sí" : "No");
                            printf("\n");
                        } else {
                            printf("No se pudo leer los datos del alumno.\n");
                        }
                    } else {
                        printf("Numero de alumno incorrecto.\n");
                    }

                    fclose(arch);
                }
                break;

            case 6:
                printf("Hasta luego.\n");
                break;

            default:
                printf("Opción inválida.\n");
                break;
        }

        if (opcion != 6) {
            printf("Desea continuar?\n 1 para continuar, 0 para cerrar\n");
            scanf("%i", &continuar);
            if (continuar != 1) {
                printf("Hasta luego.\n");
                break;
            }
        }
    } while (opcion != 6);

    return 0;
}
