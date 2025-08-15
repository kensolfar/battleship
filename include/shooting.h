/**
 * @file shooting.h
 * @brief Header file for the Shooting class.
 * 
 * Contratos para realizar disparos en el tablero y actualizar el estado de la flota.
 * Invariantes:
 * - Los disparos solo se pueden realizar en celdas dentro de los límites del tablero.
 * - No se pueden realizar disparos en celdas que ya han sido disparadas (Hit o Miss).
 */
#pragma once
#include <string>
#include <vector>
#include "types.h"
#include "board.h"
#include "fleet.h"

namespace battleship
{

struct Shooting 
{
    // Realiza un disparo en las coordenadas especificadas.
    // Pre: Board::isInside(p) y la celda no está ya en Hit o Miss (la politica de disparo repetido se valida fuera)
    // Post: 
    // - Empty -> Miss (no cambia fleet)
    // - Ship -> Hit (hits++; si hits == size => Sunk)
    // Devuelve: ShotResult{Kind, shipId, p}
    ShotResult shoot(Board& defenderBoard, Fleet& defenderFleet, Position p);
};

} // namespace battleship