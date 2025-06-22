/**
 * @file utils.c
 * @brief Implementación de utilidades de logging para el servidor UDP SmartBridges.
 */

#include "utils.h"
#include <stdio.h>
#include <time.h>

/**
 * @brief Imprime un mensaje informativo con timestamp en stdout.
 * @param message Mensaje a imprimir.
 */
void logMessage(const char *message) {
    time_t now = time(NULL);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("%s [%s] %s\n", TAG_UDP, timebuf, message);
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
