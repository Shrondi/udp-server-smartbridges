/**
 * @file udp_handler.c
 * @brief Implementación del manejo de peticiones UDP para el servidor SmartBridges.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 */

#include "udp_handler.h"
#include "db.h"
#include "utils.h"
#include "udp_config.h"
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>


void handleRequest(Request *request) {
    if (request->dataLen < 35) {
        logError("UDP", "Packet too short");
        return;
    }

    char version[VERSION_SIZE] = {0};
    memcpy(version, request->data, VERSION_SIZE - 1);
    int sensorId = *(uint16_t *)(request->data + 32);
    uint8_t statusCode = *(uint8_t *)(request->data + 34);
    const char *statusStr = (statusCode == 1) ? "CHECK" : "AWAKE";

    char logbuf[128];
    snprintf(logbuf, sizeof(logbuf), "REQ: dev.%d; ver.%s; sta: %s", sensorId, version, statusStr);
    logMessage(logbuf);

    char db_path_expanded[512];
    expand_home(SQLITE_DB_PATH, db_path_expanded, sizeof(db_path_expanded));
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(db_path_expanded, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        logError("SQLite open_v2", sqlite3_errmsg(db));
        return;
    }

    SensorConfig config;
    if (getSensorConfig(db, sensorId, &config) != 0) {
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t secs = ts.tv_sec;
    uint64_t usecs = ts.tv_nsec / 1000;

    uint8_t response[64] = {0};
    memcpy(response, config.firmwareVersion, VERSION_SIZE);
    response[32] = config.frequency;
    response[33] = config.scale;
    response[34] = config.enable;
    memcpy(response + 35, &config.thresholdStdv, sizeof(float));
    memcpy(response + 39, &secs, sizeof(uint64_t));
    memcpy(response + 47, &usecs, sizeof(uint64_t));

    sendto(request->socketFd, response, 55, 0,
           (struct sockaddr *)&request->clientAddr, request->addrLen);
}
