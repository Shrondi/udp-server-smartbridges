cmake_minimum_required(VERSION 3.10)
project(udp_smartbridges C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

# Archivos fuente
set(SOURCES
    src/main.c
    src/udp_handler.c
    src/db.c
    src/utils.c
    config/udp_config.c
)

add_executable(UDPServer ${SOURCES})
target_link_libraries(UDPServer sqlite3)
target_compile_options(UDPServer PRIVATE -Wall -Wextra -Wpedantic)

# --- Generación automática de documentación con Doxygen usando Doxyfile existente ---
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    # Crear carpeta docs si no existe y generar doc en cada build
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMENT "Creando carpeta docs para documentación"
    )
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/docs/html/index.html
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${SOURCES}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generando documentación con Doxygen (Doxyfile existente)"
        VERBATIM
    )
    add_custom_target(doc ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docs/html/index.html
    )
    add_dependencies(UDPServer doc)
else()
    message(STATUS "Doxygen no encontrado. El objetivo 'doc' no estará disponible.")
endif()

