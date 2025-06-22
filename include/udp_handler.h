/**
 * @file udp_handler.h
 * @brief Declaraciones para el manejo de peticiones UDP.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 *
 * @section formato_trama_entrada Formato de trama de entrada (UDP)
 *
 * La trama recibida debe tener el siguiente formato (mínimo 35 bytes):
 *
 * | Offset | Tamaño | Campo              | Descripción                       |
 * |--------|--------|--------------------|-----------------------------------|
 * | 0      | 31     | version            | Versión de firmware (string)      |
 * | 32     | 2      | sensorId           | ID del sensor (uint16_t)          |
 * | 34     | 1      | statusCode         | Código de estado (uint8_t)        |
 *
 * @section formato_trama_salida Formato de trama de salida (UDP)
 *
 * La respuesta enviada tiene el siguiente formato (55 bytes):
 *
 * | Offset | Tamaño | Campo              | Descripción                       |
 * |--------|--------|--------------------|-----------------------------------|
 * | 0      | 31     | firmwareVersion    | Versión de firmware (string)      |
 * | 32     | 1      | frequency          | Frecuencia (uint8_t)              |
 * | 33     | 1      | scale              | Escala (uint8_t)                  |
 * | 34     | 1      | enable             | Habilitado (uint8_t)              |
 * | 35     | 4      | thresholdStdv      | Umbral (float)                    |
 * | 39     | 8      | secs               | Timestamp (segundos, uint64_t)    |
 * | 47     | 8      | usecs              | Timestamp (microsegundos, uint64_t)|
 *
 * Ver detalles y uso en udp_handler.c
 */

#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

#include "udp_config.h"
#include <stdint.h>
#include <netinet/in.h>

/**
 * @struct Request
 * @brief Estructura que representa una petición UDP recibida.
 */
typedef struct {
    struct sockaddr_in clientAddr; /**< Dirección del cliente */
    socklen_t addrLen;             /**< Longitud de la dirección */
    uint8_t *data;                 /**< Datos de la petición */
    int dataLen;                   /**< Longitud de los datos */
    int socketFd;                  /**< Descriptor de archivo del socket */
} Request;

/**
 * @brief Procesa una petición UDP recibida y responde al cliente.
 * @param request Puntero a la estructura con los datos de la petición.
 */
void handleRequest(Request *request);

#endif // UDP_HANDLER_H
