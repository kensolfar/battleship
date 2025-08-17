#pragma once
/**
 * @file coords.h
 * @brief Utilidades para normalizar y convertir coordenadas de entrada a índices internos.
 *
 * Formato de entrada (interfaz para el usuario):
 * - Coordenadas tipo "A..J" + "1..10" (por ejemplo, "A1", "J10"), con o sin espacios.
 *
 * Convenciones:
 * - Internamente se trabaja con índices 0..9 para filas (A..J) y columnas (1..10 → 0..9).
 * - La función normalize convierte el texto a mayúsculas y elimina espacios en blanco.
 * - La función parseCoord convierte una cadena ya normalizada a una Position interna.
 */

#include <string>
#include <optional>
#include "types.h"

namespace battleship {

struct Coords {

    /**
     * @brief Normaliza una cadena de coordenadas.
     *
     * @param in   Texto introducido por el usuario (puede contener espacios y minúsculas).
     * @return std::string  Cadena transformada a mayúsculas y sin espacios (trim + remove inner spaces si decides).
     *
     * Detalle lingüístico:
     * - “Normalizar” aquí significa estandarizar la forma del texto (p. ej., "  a 10 " → "A10").
     */
    static std::string normalize(std::string in);

    /**
     * @brief Convierte una coordenada alfanumérica normalizada a índices internos.
     *
     * @param normalized  Cadena ya normalizada, p. ej., "A1", "C7", "J10".
     * @return std::optional<Position>
     *         - Contiene Position{row,col} si la coordenada es válida y está dentro del tablero.
     *         - std::nullopt si el formato o el rango son inválidos.
     *
     * Detalles:
     * - “Descartar las posiciones que están fuera del tablero” significa que si el texto mapea a
     *   una fila o columna fuera de 0..9, se devuelve std::nullopt.
     */
    static std::optional<Position> parseCoord(const std::string& normalized);
};

} // namespace battleship
