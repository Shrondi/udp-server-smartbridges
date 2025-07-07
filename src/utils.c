/**
 * @file utils.c
 * @brief Implementación de utilidades de logging para el servidor UDP SmartBridges.
 */

#include "utils.h"
#include <stdio.h>

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
    fprintf(stderr, "%s ERROR: %s - %s\n", TAG_UDP, context, error);
    fflush(stderr);
}
