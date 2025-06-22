/**
 * @file main.c
 * @brief Punto de entrada del servidor UDP SmartBridges.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 */

#include "udp_handler.h"
#include "udp_config.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

/**
 * @brief Función principal. Inicializa y ejecuta el servidor UDP.
 * @return 0 en caso de éxito, otro valor en caso de error.
 */
int main() {

    logMessage("Starting UDP server...");

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        logError("Socket", "Could not create socket");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) flags = 0;
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        logError("Bind", "Could not bind socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        logError("epoll_create1", "Could not create epoll");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        logError("epoll_ctl", "Could not register socket in epoll");
        close(sockfd);
        close(epollFd);
        exit(EXIT_FAILURE);
    }

    logMessage("UDP server ready to receive...");
    char portMsg[64];
    snprintf(portMsg, sizeof(portMsg), "Listening on port %d...", UDP_PORT);
    logMessage(portMsg);

    struct epoll_event *events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
    if (!events) {
        logError("malloc", "No se pudo asignar memoria para eventos epoll");
        close(epollFd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    while (1) {
        int n = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (n < 0) {
            logError("epoll_wait", "Error in epoll_wait");
            break;
        }
        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == sockfd) {
                Request request;
                request.data = malloc(MAX_BUFFER_SIZE);
                if (!request.data) {
                    logError("malloc", "No se pudo asignar memoria para el buffer UDP");
                    continue;
                }
                request.addrLen = sizeof(request.clientAddr);
                request.dataLen = recvfrom(sockfd, request.data, MAX_BUFFER_SIZE, 0,
                                            (struct sockaddr *)&request.clientAddr,
                                            &request.addrLen);
                request.socketFd = sockfd;
                if (request.dataLen > 0) {
                    handleRequest(&request);
                }
                free(request.data);
            }
        }
    }
    free(events);
    
    close(epollFd);
    close(sockfd);
    
    return 0;
}
