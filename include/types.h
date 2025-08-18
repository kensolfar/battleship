#pragma once
/**
 * @file types.h
 * @brief Tipos básicos y primitivas para el núcleo del juego Battleship.
 *
 * Este fichero define los tipos de datos usados por el resto del motor:
 * - Estados de celda (`CellState`).
 * - Orientación de barcos (`Orientation`).
 * - Modo de visualización (`ViewMode`).
 * - Posiciones internas (`Position`).
 * - Resultado de un disparo (`ShotResult`).
 *
 * Invariantes generales:
 * - Las `Position` internas usan índices 0..9 (filas y columnas).
 * - Cuando un campo documentado exige `Position` válida, se asume
 *   `0 <= row < 10` y `0 <= col < 10`.
 */

namespace battleship {

/**
 * @brief Estado posible de una celda del tablero.
 *
 * Semántica:
 * - `Empty`: celda sin barco ni disparo.
 * - `Ship`: celda ocupada por parte de un barco (aún no impactada).
 * - `Hit`: celda con parte de barco que ha recibido impacto.
 * - `Miss`: celda donde se ha disparado y no había barco.
 */
enum class CellState 
{
    Empty, // Celda vacía
    Ship,  // Ocupada por parte de un barco (no impactada)
    Hit,   // Barco impactado en esta celda
    Miss   // Disparo fallido (agua)
};

/**
 * @brief Orientación de un barco al colocarlo en el tablero.
 *
 * - `Vertical`: el barco ocupa celdas con la misma columna y filas consecutivas.
 * - `Horizontal`: el barco ocupa celdas con la misma fila y columnas consecutivas.
 */
enum class Orientation 
{
    Vertical,    // Orientación vertical
    Horizontal   // Orientación horizontal
};

/**
 * @brief Modo de visualización del tablero.
 *
 * - `Own`: vista del tablero propio (muestra barcos propios).
 * - `Enemy`: vista del tablero enemigo (oculta barcos enemigos).
 */
enum class ViewMode 
{
    Own,    // Mi tablero
    Enemy   // Tablero enemigo
};

/**
 * @brief Posición interna en el tablero (índices 0-based).
 *
 * Invariante: 0 ≤ row < 10, 0 ≤ col < 10.
 */
struct Position 
{
    int row;  ///< Fila (0..9)
    int col;  ///< Columna (0..9)
    bool operator==(const Position& o) const noexcept
    {
        return row == o.row && col == o.col;
    }
};

/**
 * @brief Resultado semántico de un disparo realizado en el tablero.
 *
 * Contiene:
 * - `kind`: tipo de resultado (Miss, Hit o Sunk).
 * - `shipId`: identificador del barco afectado, o `Board::kNoShipId` si no aplica.
 * - `pos`: la `Position` objetivo del disparo.
 */
struct ShotResult 
{
    enum class Kind 
    {
        Miss, /**< Disparo al agua */
        Hit,  /**< Disparo que impactó un barco pero no lo hundió */
        Sunk  /**< Disparo que hundió un barco (última célula) */
    };

    Kind kind;       ///< Tipo de impacto
    int shipId;      ///< Id del barco afectado, `Board::kNoShipId` si no aplica
    Position pos;    ///< Posición afectada
};

} // namespace battleship