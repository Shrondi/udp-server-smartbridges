/**
 * @file utils.c
 * @brief Implementación de utilidades de logging para el servidor UDP SmartBridges.
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Imprime un mensaje informativo con timestamp en stdout.
 * @param message Mensaje a imprimir.
 */
void logMessage(const char *message) {
    printf("[UDP] %s\n", message);
    fflush(stdout);
}

/**
 * @brief Imprime un mensaje de error con contexto en stderr.
 * @param context Contexto o módulo donde ocurrió el error.
 * @param error Descripción del error.
 */
void logError(const char *context, const char *error) {
    fprintf(stderr, "[UDP] ERROR: %s - %s\n", context, error);
    fflush(stderr);
}

/**
 * @brief Expande el símbolo de tilde (~) en una ruta al directorio home del usuario.
 * @param input Ruta de entrada que puede contener el símbolo ~.
 * @param output Buffer donde se almacenará la ruta de salida expandida.
 * @param out_size Tamaño del buffer de salida.
 */
void expand_home(const char *input, char *output, size_t out_size) {
    if (input[0] == '~') {
        const char *home = getenv("HOME");
        if (home) {
            snprintf(output, out_size, "%s%s", home, input + 1);
            return;
        }
    }
    snprintf(output, out_size, "%s", input);
}
