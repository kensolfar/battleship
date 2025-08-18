#pragma once
/**
 * @file board.h
 * @brief Contratos y tipos para el tablero de Batalla Naval.
 *
 * El tablero es responsable de:
 * - Mantener el estado de cada celda (vacía, barco, disparo, etc.).
 * - Llevar el índice inverso de barcos por celda (shipIdAt).
 * - Garantizar invariantes de consistencia entre grid y shipIdAt.
 *
 * Invariantes por celda (para todo p dentro del tablero):
 * 1) shipIdAt[p] == kNoShipId  <=>  no hay barco en p
 * 2) shipIdAt[p] >= 0          =>   grid[p] es Ship o Hit
 * 3) grid[p] == Ship o Hit     =>   shipIdAt[p] >= 0
 *
 * Transiciones válidas en disparos:
 * - Empty -> Miss
 * - Ship  -> Hit
 * (controladas fuera de Board)
 *
 * Convenciones:
 * - El tablero es cuadrado de tamaño kSize x kSize (por defecto 10x10).
 * - Las posiciones se representan con el tipo Position (ver types.h).
 * - El estado de cada celda se representa con CellState.
 * - El identificador de barco por celda es un entero (kNoShipId si no hay barco).
 */

#include <array>
#include "types.h" // Para CellState, Position

namespace battleship
{

/**
 * @brief Representa el tablero de juego y sus celdas.
 *
 * Métodos principales:
 * - reset(): Restaura el tablero a estado vacío.
 * - isInside(p): Comprueba si una posición está dentro de los límites.
 * - get(p): Obtiene el estado de una celda.
 * - set(p, s): Establece el estado de una celda.
 * - getShipId(p): Obtiene el id de barco en una celda.
 * - setShipId(p, id): Asigna el id de barco en una celda.
 *
 * Invariantes:
 * - Todas las celdas fuera de los límites no son accesibles.
 * - El tablero mantiene la coherencia entre grid y shipIdAt.
 */
struct Board final
{
    static constexpr int kSize     = 10;    ///< Tamaño del tablero (filas y columnas).
    static constexpr int kNoShipId = -1;    ///< Valor especial para indicar ausencia de barco.

    using CellRow  = std::array<CellState, kSize>; ///< Fila de estados de celda.
    using CellGrid = std::array<CellRow,   kSize>; ///< Matriz de estados de celda.

    using IdRow    = std::array<int, kSize>;       ///< Fila de identificadores de barco.
    using IdGrid   = std::array<IdRow, kSize>;     ///< Matriz de identificadores de barco.

    CellGrid grid;        ///< Estado de cada celda (Empty, Ship, Hit, Miss, etc.).
    IdGrid   shipIdAt;    ///< Índice inverso: id de barco por celda (-1 si vacío).

    /**
     * @brief Restaura el tablero a estado vacío.
     * @post Todas las celdas = Empty, shipIdAt = kNoShipId.
     */
    void reset();

    /**
     * @brief Comprueba si la posición está dentro del tablero.
     * @param p Posición a verificar.
     * @return true si 0 ≤ row,col < kSize.
     */
    static bool isInside(Position p);

    /**
     * @brief Obtiene el estado de una celda.
     * @param p Posición de la celda.
     * @pre isInside(p) == true.
     * @return Estado actual de la celda.
     */
    CellState get(Position p) const;

    /**
     * @brief Establece el estado de una celda.
     * @param p Posición de la celda.
     * @param s Nuevo estado a asignar.
     * @pre isInside(p) == true.
     */
    void set(Position p, CellState s);

    /**
     * @brief Obtiene el id de barco en una celda, o kNoShipId si no hay barco.
     * @param p Posición de la celda.
     * @pre isInside(p) == true.
     * @return Identificador de barco o kNoShipId.
     */
    int getShipId(Position p) const;

    /**
     * @brief Asigna el id de barco en una celda.
     * @param p Posición de la celda.
     * @param shipId Identificador de barco a asignar.
     * @pre isInside(p) == true.
     */
    void setShipId(Position p, int shipId);
};

} //