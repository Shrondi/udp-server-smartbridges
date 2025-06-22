/**
 * @file udp_handler.h
 * @brief Declaraciones para el manejo de peticiones UDP.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
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
