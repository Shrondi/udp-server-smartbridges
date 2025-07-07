/**
 * @file utils.h
 * @brief Declaraciones de utilidades de logging para el servidor UDP SmartBridges.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

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

/**
 * @brief Expande el símbolo ~ al directorio home del usuario si está presente al inicio de la ruta.
 * @param input Ruta de entrada (puede empezar por ~)
 * @param output Buffer de salida donde se almacena la ruta expandida
 * @param out_size Tamaño del buffer de salida
 */
void expand_home(const char *input, char *output, size_t out_size);

#endif // UTILS_H
