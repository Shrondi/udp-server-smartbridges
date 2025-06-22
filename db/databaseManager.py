# databaseManager.py
# Módulo para gestionar la base de datos de sensores

import sqlite3
import csv
import os
from contextlib import contextmanager

DB_NAME = 'devices.db'

@contextmanager
def db_connection(db_name=DB_NAME):
    conn = sqlite3.connect(db_name)
    try:
        yield conn
        conn.commit()
    except Exception as e:
        conn.rollback()
        print(f"Error en la base de datos: {e}")
        raise
    finally:
        conn.close()

def sensor_exists(sensor_id):
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT 1 FROM sensors WHERE id = ?", (sensor_id,))
        return cursor.fetchone() is not None
        
def insertar_sensores_desde_archivo(ruta_archivo):
    if not os.path.exists(ruta_archivo):
        print(f"[!] El archivo '{ruta_archivo}' no existe.")
        return

    with open(ruta_archivo, newline='', encoding='utf-8') as csvfile:
        lector = csv.DictReader(csvfile)
        insertados = 0
        fallidos = 0
        for fila in lector:
            try:
                sensor_data = {
                    "id": int(fila["id"]),
                    "name": fila["name"],
                    "firmware_version": fila["firmware_version"],
                    "scale": int(fila["scale"]),
                    "frequency": int(fila["frequency"]),
                    "threshold_stdv": float(fila["threshold_stdv"]),
                    "enable": int(fila["enable"])
                }
                if insertar_sensor(sensor_data):
                    insertados += 1
                else:
                    fallidos += 1
            except Exception as e:
                print(f"[!] Error al procesar fila: {fila} — {e}")
                fallidos += 1

    print(f"\nSe insertaron correctamente {insertados} sensores. {fallidos} fallaron.\n")

def insertar_sensor(sensor_data):
    if sensor_exists(sensor_data['id']):
        print(f"Error: El ID {sensor_data['id']} ya existe en la base de datos.")
        return False
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute('''
            INSERT INTO sensors (id, name, firmware_version, scale, frequency, threshold_stdv, enable)
            VALUES (:id, :name, :firmware_version, :scale, :frequency, :threshold_stdv, :enable)
        ''', sensor_data)
    print("Sensor insertado correctamente.")
    return True

def borrar_sensor_por_id(sensor_id):
    if not sensor_exists(sensor_id):
        print(f"No se encontró ningún sensor con ID {sensor_id}.")
        return False
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("DELETE FROM sensors WHERE id = ?", (sensor_id,))
    print(f"Sensor con ID {sensor_id} eliminado correctamente.")
    return True

def listar_sensores():
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM sensors")
        sensores = cursor.fetchall()
    if sensores:
        print("\nLista de sensores:")
        for sensor in sensores:
            print(f"ID: {sensor[0]}, Name: {sensor[1]}, Firmware: {sensor[2]}, Scale: {sensor[3]}, "
                  f"Frequency: {sensor[4]}, Threshold stdv: {sensor[5]}, Enable: {sensor[6]}")
    else:
        print("No hay sensores en la base de datos.")
    return sensores

def obtener_sensor(sensor_id):
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM sensors WHERE id = ?", (sensor_id,))
        return cursor.fetchone()

def modificar_sensor(sensor_id, nuevos_valores=None):
    sensor = obtener_sensor(sensor_id)
    if not sensor:
        print(f"No se encontró ningún sensor con ID {sensor_id}.")
        return False
    if nuevos_valores is None:
        print("\nConfiguración actual del sensor:")
        print(f"ID: {sensor[0]}")
        print(f"Name: {sensor[1]}")
        print(f"Firmware version: {sensor[2]}")
        print(f"Scale: {sensor[3]}")
        print(f"Frequency: {sensor[4]}")
        print(f"Threshold stdv: {sensor[5]}")
        print(f"Enable: {sensor[6]}")
        print("\nIntroduce nuevos valores (deja vacío para mantener el valor actual):")
        name = input(f"Nombre [{sensor[1]}]: ") or sensor[1]
        firmware_version = input(f"Firmware version [{sensor[2]}]: ") or sensor[2]
        try:
            scale_input = input(f"Scale [{sensor[3]}]: ")
            scale = int(scale_input) if scale_input else sensor[3]
            frequency_input = input(f"Frequency [{sensor[4]}]: ")
            frequency = int(frequency_input) if frequency_input else sensor[4]
            threshold_input = input(f"Threshold stdv [{sensor[5]}]: ")
            threshold_stdv = float(threshold_input) if threshold_input else sensor[5]
            enable_input = input(f"Enable (0 o 1) [{sensor[6]}]: ")
            enable = int(enable_input) if enable_input else sensor[6]
            if enable not in (0, 1):
                raise ValueError("Enable debe ser 0 o 1.")
        except ValueError as e:
            print(f"Valor inválido: {e}")
            return False
        nuevos_valores = {
            'name': name,
            'firmware_version': firmware_version,
            'scale': scale,
            'frequency': frequency,
            'threshold_stdv': threshold_stdv,
            'enable': enable
        }
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute('''
            UPDATE sensors
            SET name = :name, firmware_version = :firmware_version, scale = :scale, frequency = :frequency, threshold_stdv = :threshold_stdv, enable = :enable
            WHERE id = :id
        ''', {**nuevos_valores, 'id': sensor_id})
    print("Sensor actualizado correctamente.")
    return True

def habilitar_todos():
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("UPDATE sensors SET enable = 1")
    print("Todos los sensores han sido habilitados.")

def deshabilitar_todos():
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("UPDATE sensors SET enable = 0")
    print("Todos los sensores han sido deshabilitados.")

def modificar_firmware_todos(nueva_version=None):
    if nueva_version is None:
        nueva_version = input("Introduce la nueva versión de firmware para todos los sensores: ")
        if not nueva_version.strip():
            print("La versión no puede estar vacía.")
            return False
    with db_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("UPDATE sensors SET firmware_version = ?", (nueva_version,))
        filas_afectadas = cursor.rowcount
    print(f"Versión de firmware actualizada a '{nueva_version}' para {filas_afectadas} sensores.")
    return True

def mostrar_menu():
    print("""
========= MENÚ DE GESTIÓN DE SENSORES =========
  1. Insertar un sensor
  2. Borrar un sensor por ID
  3. Listar todos los sensores
  4. Modificar un sensor por ID
-----------------------------------------------
  5. Habilitar todos los sensores
  6. Deshabilitar todos los sensores
  7. Modificar versión de firmware de todos
-----------------------------------------------
  8. Insertar sensores desde archivo CSV
  0. Salir
===============================================
""")

def menu():
    while True:
        mostrar_menu()
        opcion = input("Elige una opción (0-7): ").strip()
        if opcion == '1':
            try:
                id_sensor = int(input("  ID del sensor (entero único): "))
                name = input("  Nombre del sensor: ")
                firmware_version = input("  Versión firmware: ")
                scale = int(input("  Scale (entero): "))
                frequency = int(input("  Frecuencia (entero): "))
                threshold_stdv = float(input("  Threshold stdv (decimal): "))
                enable = int(input("  Enable (0 o 1): "))
                if enable not in (0, 1):
                    raise ValueError
            except ValueError:
                print("\n[!] Valores inválidos. Intenta de nuevo.\n")
                continue
            sensor_data = {
                "id": id_sensor,
                "name": name,
                "firmware_version": firmware_version,
                "scale": scale,
                "frequency": frequency,
                "threshold_stdv": threshold_stdv,
                "enable": enable
            }
            insertar_sensor(sensor_data)
        elif opcion == '2':
            id_borrar = input("  Introduce el ID del sensor que quieres borrar: ")
            if id_borrar.isdigit():
                borrar_sensor_por_id(int(id_borrar))
            else:
                print("\n[!] ID inválido. Debe ser un número entero.\n")
        elif opcion == '3':
            listar_sensores()
        elif opcion == '4':
            id_modificar = input("  Introduce el ID del sensor que quieres modificar: ")
            if id_modificar.isdigit():
                modificar_sensor(int(id_modificar))
            else:
                print("\n[!] ID inválido. Debe ser un número entero.\n")
        elif opcion == '5':
            habilitar_todos()
        elif opcion == '6':
            deshabilitar_todos()
        elif opcion == '7':
            modificar_firmware_todos()
        elif opcion == '8':
            ruta = input("  Introduce la ruta del archivo CSV: ").strip()
            insertar_sensores_desde_archivo(ruta)
        elif opcion == '0':
            print("\nSaliendo... ¡Hasta luego!\n")
            break
        else:
            print("\n[!] Opción no válida. Intenta otra vez.\n")

if __name__ == "__main__":
    menu()
