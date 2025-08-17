/**
 * @file board.h
 * @brief Board — Definición de tipos y contratos para el tablero de Batalla Naval. Dueño de celdas e índice inverso
 *
 * Invariantes por celda (para todo p dentro del tablero):
 * 1) shipIdAt[p] == kNoShipId  <=>  no hay barco en p
 * 2) shipIdAt[p] >= 0          =>   grid[p] es Ship o Hit
 * 3) grid[p] == Ship o Hit     =>   shipIdAt[p] >= 0
 * 
 * - Transiciones válidas en disparos:
 *       Empty -> Miss
 *       Ship  -> Hit
 *   (controladas fuera de Board)
 */
#pragma once
#include <array>
#include "types.h" // Para CellState, Position

namespace battleship 
{

struct Board final 
{
    static constexpr int kSize     = 10;
    static constexpr int kNoShipId = -1;

    using CellRow  = std::array<CellState, kSize>;
    using CellGrid = std::array<CellRow,   kSize>;

    using IdRow    = std::array<int, kSize>;
    using IdGrid   = std::array<IdRow, kSize>;

    CellGrid grid;        ///< Estado de cada celda.
    IdGrid   shipIdAt;    ///< Índice inverso: id de barco por celda (-1 si vacío).

    // --- Contratos de métodos ---

    /// Restaura el tablero a estado vacío.
    /// Post: todas las celdas = Empty, shipIdAt = kNoShipId.
    void reset(); // TODO: implementar en .cpp

    /// Comprueba si la posición está dentro del tablero.
    /// Post: true si 0 ≤ row,col < kSize.
    static bool isInside(Position p); // TODO: implementar

    /// Obtiene el estado de una celda.
    /// Pre: isInside(p) == true.
    CellState get(Position p) const; // TODO: implementar

    /// Establece el estado de una celda.
    /// Pre: isInside(p) == true.
    void set(Position p, CellState s); // TODO: implementar

    /// Obtiene el id de barco en una celda, o kNoShipId si no hay barco.
    /// Pre: isInside(p) == true.
    int getShipId(Position p) const; // TODO: implementar

    /// Asigna el id de barco en una celda.
    /// Pre: isInside(p) == true.
    void setShipId(Position p, int shipId); // TODO: implementar
};

} // namespace battleship
