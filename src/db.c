/**
 * @file db.c
 * @brief Implementación de acceso a la base de datos de sensores.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 */

#include "db.h"
#include "utils.h"
#include <string.h>


int getSensorConfig(sqlite3 *db, int sensorId, SensorConfig *config) {
    const char *sql = "SELECT firmware_version, frequency, scale, threshold_stdv, enable FROM sensors WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        logError("SQLite prepare", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_bind_int(stmt, 1, sensorId) != SQLITE_OK) {
        logError("SQLite bind", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *fw = sqlite3_column_text(stmt, 0);
        if (fw == NULL) {
            logError("SQLite column", "firmware_version es NULL");
            sqlite3_finalize(stmt);
            return -1;
        }
        strncpy(config->firmwareVersion, (const char *)fw, VERSION_SIZE - 1);
        config->firmwareVersion[VERSION_SIZE - 1] = '\0';
        config->frequency = (uint8_t)sqlite3_column_int(stmt, 1);
        config->scale = (uint8_t)sqlite3_column_int(stmt, 2);
        config->thresholdStdv = (float)sqlite3_column_double(stmt, 3);
        config->enable = (uint8_t)sqlite3_column_int(stmt, 4);

        sqlite3_finalize(stmt);
        return 0;
        
    } else {
        logError("SQLite step", "No config found");
        sqlite3_finalize(stmt);
        return -1;
    }
}
