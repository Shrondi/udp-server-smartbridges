/**
 * @file db.h
 * @brief Declaraciones para el acceso a la base de datos de sensores.
 * @author Carlos Lucena Robles
 * @date 2025-06-21
 */

#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdint.h>

#define VERSION_SIZE 32

/**
 * @struct SensorConfig
 * @brief Estructura que almacena la configuración de un sensor.
 */
typedef struct {
    char firmwareVersion[VERSION_SIZE]; ///< Versión del firmware.
    uint8_t frequency;                  ///< Frecuencia del sensor.
    uint8_t scale;                     ///< Escala del sensor.
    float thresholdStdv;               ///< Umbral en desviaciones estándar.
    uint8_t enable;                   ///< Habilita o deshabilita el sensor.
} SensorConfig;

/**
 * @brief Obtiene la configuración de un sensor desde la base de datos.
 * @param db Puntero a la conexión SQLite.
 * @param sensorId ID del sensor a consultar.
 * @param config Puntero a la estructura donde se almacena la configuración.
 * @return 0 si tiene éxito, -1 si ocurre un error o no se encuentra el sensor.
 */
int getSensorConfig(sqlite3 *db, int sensorId, SensorConfig *config);

#endif // DB_H
