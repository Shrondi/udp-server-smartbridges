/**
 * @file utils.h
 * @brief Declaraciones de utilidades de logging para el servidor UDP SmartBridges.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

#define TAG_UDP "[UDP]"

/**
 * @brief Imprime un mensaje informativo con timestamp en stdout.
 * @param message Mensaje a imprimir.
 */
void logMessage(const char *message);

/**
 * @brief Imprime un mensaje de error con contexto en stderr.
 * @param context Contexto o módulo donde ocurrió el error.
 * @param error Descripción del error.
 */
void logError(const char *context, const char *error);

#endif // UTILS_H
