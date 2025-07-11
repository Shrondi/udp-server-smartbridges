<div align="center">
  <a href="https://www.ciencia.gob.es/portal/site/MICINN/">
    <img src="https://otri.ugr.es/sites/webugr/otri/public/inline-images/PLEC_2021_Logos_micin-financiadoUEnextgeneration-prtr-aei.jpg" alt="MCIN, AEI y NextGenerationEU"/>
  </a>
</div>

# UDP Server SmartBridges <!-- omit from toc -->

Servidor UDP concurrente y no bloqueante para la recepción y gestión de los parámtros de configuración de sensores en el marco del proyecto SMART-BRIDGES.


## Índice <!-- omit from toc -->

- [Información del proyecto](#información-del-proyecto)
- [Financiación](#financiación)
- [Licencia](#licencia)
- [Dependencias](#dependencias)
- [Compilación](#compilación)
- [Ejecución](#ejecución)
- [Funcionamiento e información](#funcionamiento-e-información)
- [Formato de las tramas UDP](#formato-de-las-tramas-udp)
  - [Request frame](#request-frame)
  - [Response frame](#response-frame)
- [Base de datos](#base-de-datos)
  - [Esquema](#esquema)
    - [Creación](#creación)
  - [Gestión de la base de datos](#gestión-de-la-base-de-datos)


## Información del proyecto

- **Título:** `SMART-BRIDGES`  
  Monitorización inteligente del estado estructural de puentes ferroviarios de alta velocidad
- **Acrónimo:** `SMART-BRIDGES`
- **Referencia:** `PLEC2021-007798`
- **Entidades participantes:**
  - [UNIVERSIDAD DE CÓRDOBA](https://www.uco.es/)
  - [UNIVERSIDAD DE GRANADA](https://www.ugr.es/)
  - [INECO](https://www.ineco.com/)

## Financiación

**“SMART-BRIDGES – Monitorización inteligente del estado estructural de puentes ferroviarios de alta velocidad”**  
(Ref. `PLEC2021-007798`)

Proyecto financiado por:
- **Ministerio de Ciencia e Innovación (MCIN)**
- **Agencia Estatal de Investigación (AEI)**
- **Unión Europea** a través de **NextGenerationEU / Plan de Recuperación, Transformación y Resiliencia (PRTR)**

Convocatoria de proyectos de I+D+I en Líneas Estratégicas 2021 (PLEC2021),
procedimiento de concesión de ayudas a proyectos de I+D+i en colaboración público-privada,
del Programa Estatal de I+D+i Orientada a los Retos de la Sociedad,
en el marco del Plan Estatal de Investigación Científica y Técnica y de Innovación 2017-2020.  
(BOE Núm. 98, de 24 de abril de 2021).

## Licencia

Consulta el archivo [`LICENSE`](LICENSE) para más detalles.


## Dependencias

- [CMake](https://cmake.org/) >= 3.10
- [GCC](https://gcc.gnu.org/) o [Clang](https://clang.llvm.org/) (soporte C99)
- [SQLite3](https://www.sqlite.org/index.html) y su desarrollo (`libsqlite3-dev` en Debian/Ubuntu)
- [Python 3](https://www.python.org/) (opcional, solo para el script de gestión `db/databaseManager.py`)


## Compilación

Desde la raíz del proyecto ejecuta:

```bash
cmake -B build
cmake --build build
```

Esto generará el ejecutable en `build/UDPServer`.

## Ejecución

Ejecuta el servidor UDP con:

```bash
./build/UDPServer
```

El servidor escuchará por defecto en el puerto `24243` (puedes modificarlo en `config/udp_config.c`).

>  **Ubicación del archivo de la base de datos:**  
> El archivo `devices.db` debe encontrarse en la ruta especificada en `config/udp_config.c` (por defecto, `~/database-sensors/devices.db`).  
> 
> - Si usas `~`, asegúrate de que corresponde al directorio *home* del usuario que ejecuta el servidor.  
> - Si la ruta es absoluta, verifica que el archivo exista en esa ubicación y que el usuario del servidor tenga permisos de lectura y escritura.



## Funcionamiento e información

El servidor espera recibir paquetes UDP de sensores, procesa la petición y responde con la configuración correspondiente.

- El formato de los paquetes y la estructura de la respuesta están definidos en el código fuente (`include/udp_handler.h`, `src/udp_handler.c`).
- Puedes consultar y modificar la lógica de gestión de sensores en `src/db.c` y la configuración global en `config/udp_config.c`.
- La documentación técnica se genera automáticamente con Doxygen en la carpeta `docs/html` tras compilar.

Para más detalles sobre el protocolo o ejemplos de uso, consulta el código fuente y la documentación generada.


## Formato de las tramas UDP

### Request frame

<table>
  <tr><th>Offset</th><th>Tamaño</th><th>Campo</th><th>Descripción</th></tr>
  <tr><td align="center">0</td><td align="center">31</td><td><code>version</code></td><td>Versión de firmware (string, null-terminated)</td></tr>
  <tr><td align="center">32</td><td align="center">2</td><td><code>sensorId</code></td><td>ID del sensor (<code>uint16_t</code>)</td></tr>
  <tr><td align="center">34</td><td align="center">1</td><td><code>statusCode</code></td><td>Código de estado (<code>uint8_t</code>)</td></tr>
</table>

### Response frame

<table>
  <tr><th>Offset</th><th>Tamaño</th><th>Campo</th><th>Descripción</th></tr>
  <tr><td align="center">0</td><td align="center">31</td><td><code>firmwareVersion</code></td><td>Versión de firmware (string, null-terminated)</td></tr>
  <tr><td align="center">32</td><td align="center">1</td><td><code>frequency</code></td><td>Frecuencia (<code>uint8_t</code>)</td></tr>
  <tr><td align="center">33</td><td align="center">1</td><td><code>scale</code></td><td>Escala (<code>uint8_t</code>)</td></tr>
  <tr><td align="center">34</td><td align="center">1</td><td><code>enable</code></td><td>Habilitado (<code>uint8_t</code>)</td></tr>
  <tr><td align="center">35</td><td align="center">4</td><td><code>thresholdStdv</code></td><td>Umbral (<code>float</code>)</td></tr>
  <tr><td align="center">39</td><td align="center">8</td><td><code>secs</code></td><td>Timestamp (segundos, <code>uint64_t</code>)</td></tr>
  <tr><td align="center">47</td><td align="center">8</td><td><code>usecs</code></td><td>Timestamp (microsegundos, <code>uint64_t</code>)</td></tr>
</table>

## Base de datos

### Esquema

A continuación se muestra el diagrama de la tabla `sensors`:

```
+-------------------+-------------------+----------+-------------------------------+
|      Campo        |      Tipo         | Clave    |        Descripción            |
+-------------------+-------------------+----------+-------------------------------+
| id                | INTEGER           | PK       | Identificador único           |
| name              | TEXT              |          | Nombre del sensor             |
| firmware_version  | TEXT              |          | Versión del firmware          |
| scale             | INTEGER           |          | Escala configurada            |
| frequency         | INTEGER           |          | Frecuencia de muestreo        |
| threshold_stdv    | REAL              |          | Umbral de desviación estándar |
| enable            | BOOLEAN (0 o 1)   |          | Habilitado (1) o no (0)       |
+-------------------+-------------------+----------+-------------------------------+
```

#### Creación

Si necesitas crear la base de datos `devices.db` desde cero, puedes hacerlo fácilmente usando SQLite3 junto a `db/scheme-database.sql`

```bash
cd db
sqlite3 devices.db < scheme-database.sql
```

Esto generará el archivo `devices.db` con la tabla `sensors` lista para ser utilizada por el servidor y las utilidades de gestión.

- <b>id</b>: Identificador único del sensor.
- <b>name</b>: Nombre del sensor.
- <b>firmware_version</b>: Versión del firmware del sensor.
- <b>scale</b>: Escala configurada para el sensor.
- <b>frequency</b>: Frecuencia de muestreo/configuración.
- <b>threshold_stdv</b>: Umbral de desviación estándar.
- <b>enable</b>: Indica si el sensor está habilitado (1) o no (0).


### Gestión de la base de datos

En la carpeta `db/` se proporciona el script `databaseManager.py` para la gestión directa de la base de datos:
  - Insertar, borrar, listar y modificar sensores.
  - Habilitar/deshabilitar todos los sensores.
  - Cambiar la versión de firmware de todos los sensores.

Para usar el gestor desde terminal:

```bash
cd db
python3 databaseManager.py
```


<p align="right"><sub><em>Este repositorio corresponde a un módulo/parte del proyecto principal <b>SMART-BRIDGES</b>.</em></sub></p>
