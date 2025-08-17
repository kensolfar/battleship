#pragma once
/**
 * @file placement.h
 * @brief Contratos para validar y colocar barcos en el tablero.
 *
 * Reglas e invariantes que debe cumplir la colocación:
 * - Los barcos solo pueden colocarse en orientación Horizontal o Vertical.
 * - Ninguna parte del barco puede salir de los límites del tablero.
 * - Ninguna celda del barco puede solaparse con celdas ya ocupadas por otros barcos.
 * - Regla de no contigüidad (8 vecinos): los barcos no pueden tocarse ni por lados ni por diagonales.
 *   Es decir, todas las celdas adyacentes (ortogonales y diagonales) alrededor de cada celda del barco
 *   deben estar libres de barcos.
 *
 * Nota sobre "no contiguos":
 * - Al validar, se deben considerar las 8 celdas adyacentes a cada celda del barco.
 * - Si alguna de esas celdas adyacentes contiene parte de otro barco, la colocación es inválida.
 */

#include <string>
#include <vector>
#include "board.h"
#include "fleet.h"
#include "types.h"

namespace battleship {

struct Placement {

    /**
     * @brief Comprueba si un barco puede colocarse en el tablero.
     *
     * @param proto   Prototipo del barco (nombre y tamaño válidos).
     * @param start   Celda inicial (ancla) desde donde se traza el barco.
     * @param orient  Orientación (Horizontal o Vertical).
     * @param b       Tablero donde se intenta colocar.
     * @param f       Flota (se usa solo para coherencia/consultas; no se modifica).
     * @return true   Si y solo si:
     *   - Todas las celdas necesarias para el barco quedan dentro del tablero.
     *   - Ninguna de esas celdas está ocupada por otro barco.
     *   - Todas las celdas adyacentes (8 vecinos) alrededor de cada celda del barco
     *     están libres de barcos (regla de no contigüidad).
     * @return false  En cualquier otro caso. No modifica ni el tablero ni la flota.
     *
     * Precondiciones:
     * - Ship::isValidPrototype(proto) == true
     * - Board::isInside(start) == true
     */
    bool canPlace(const Ship& proto, Position start, Orientation orient,
                  const Board& b, const Fleet& f);

    /**
     * @brief Coloca un barco en el tablero y lo añade a la flota.
     *
     * @param proto   Prototipo del barco (se toman name y size).
     * @param start   Celda inicial (ancla).
     * @param orient  Orientación.
     * @param b       Tablero (se marcan las celdas del barco).
     * @param f       Flota (se agrega el barco ya materializado).
     * @return int    Identificador del barco agregado (índice en f.ships).
     *
     * Pre:
     * - canPlace(proto, start, orient, b, f) == true.
     *
     * Post:
     * - Se agrega un Ship a f.ships con:
     *     name = proto.name,
     *     size = proto.size,
     *     start = start,
     *     orientation = orient,
     *     cells = tramo recto derivado de (start, orient, size) en orden,
     *     hits = 0, sunk = false.
     * - Para cada celda del barco:
     *     b.grid[row][col]    = CellState::Ship
     *     b.shipIdAt[row][col]= shipId (índice del barco recién agregado)
     */
    int place(const Ship& proto, Position start, Orientation orient,
              Board& b, Fleet& f);

    /**
     * @brief Devuelve las celdas adyacentes (8 direcciones) de una posición, 
     *        excluyendo las posiciones que estén fuera de los límites del tablero.
     *
     * @param pos      Posición central.
     * @return std::vector<Position>  Lista de posiciones adyacentes válidas (dentro del tablero).
     *
     * Nota:
     * - Esta función descarta las posiciones que quedan fuera del tablero.
     */
    static std::vector<Position> neighborhood(Position pos);

    /**
     * @brief Calcula el “anillo” (conjunto de celdas adyacentes) que rodea al tramo recto
     *        que ocuparía un barco de cierto tamaño, partiendo de start y con orientación orient.
     *
     * @param start    Celda inicial (ancla) del tramo.
     * @param orient   Orientación del tramo.
     * @param size     Longitud del barco.
     * @return std::vector<Position>  Conjunto de posiciones adyacentes válidas que rodean al tramo,
     *                                sin incluir las celdas del propio tramo y descartando posiciones
     *                                fuera del tablero.
     *
     * Detalles:
     * - Para cada celda del tramo, se toman sus 8 vecinos válidos y se unen en un conjunto (sin duplicados).
     * - Las celdas del propio tramo no se incluyen en el resultado.
     */
    static std::vector<Position> computeRing(Position start, Orientation orient, int size);
};

} // namespace battleship
