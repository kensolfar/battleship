#pragma once
/**
 * @file types.h
 * @brief Tipos básicos para el juego Battleship.
 *
 * Invariantes:
 * - Las coordenadas internas (Position) cumplen: 0 ≤ row < 10, 0 ≤ col < 10.
 * Tipos:
 * - CellState representa el estado de una celda del tablero.
 * - Orientation indica la orientación de un barco (vertical u horizontal).
 * - ViewMode define el modo de visualización del tablero (propio o enemigo).
 * - ShotResult encapsula el resultado de un disparo, incluyendo el tipo de impacto,
 *   el identificador del barco (si corresponde) y la posición afectada.
 */
namespace battleship {

enum class CellState {
    Empty, // Celda vacía
    Ship,  // Ocupada por parte de un barco (no impactada)
    Hit,   // Barco impactado en esta celda
    Miss   // Disparo fallido (agua)
};

enum class Orientation {
    Vertical,    // Orientación vertical
    Horizontal   // Orientación horizontal
};

enum class ViewMode { 
    Own,    // Mi tablero
    Enemy   // Tablero enemigo
};

// El parser hace el mapeo a indices internos.
struct Position {
    int row;  // Fila (0..9)
    int col;  // Columna (0..9)
};

struct ShotResult {
    enum class Kind { 
        Miss, 
        Hit, 
        Sunk 
    };
    Kind kind;       // Tipo de impacto
    int shipId;      // Board::kNoShipId si no hay barco
    Position pos;    // Posición afectada
};


} // namespace battleship