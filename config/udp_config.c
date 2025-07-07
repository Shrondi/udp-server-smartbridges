/**
 * @file udp_config.c
 * @brief Definiciones de variables de configuración global para el servidor UDP SmartBridges.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 */

#include "udp_config.h"

const int UDP_PORT = 24243;
const char *SQLITE_DB_PATH = "sqlite://~/database-sensors/devices.db";
const int MAX_BUFFER_SIZE = 1024;
const int MAX_EVENTS = 8;
