## types.h
```c++
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
    int shipId;      // kNoShipId si no hay barco
    Position pos;    // Posición afectada
};


} // namespace battleship

```

## board.h
```c++
/**
 * @file board.h
 * @brief Board — Definición de tipos y contratos para el tablero de Batalla Naval. Dueño de celdas e índice inverso
 *
 * Invariantes:
 *   - Tamaño fijo kSize x kSize.
 *   - 0 ≤ row, col < kSize en cualquier acceso.
 *   - shipIdAt == kNoShipId si no hay barco en esa celda.
 *   - Si shipIdAt >= 0 entonces grid == Ship o Hit.
 *   - Transiciones válidas en disparos:
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

```

## coords.h
```c++
/**
 * @file coords.h
 * @brief Funciones para normalizar y parsear coordenadas.
 * 
 * Proporciona utilidades para trabajar con coordenadas del juego Battleship.
 * 
 * - La interfaz externa acepta coordenadas en el formato "A..J" + "1..10" (por ejemplo, "A1", "J10").
 * - La función normalize convierte la entrada a mayúsculas y elimina espacios.
 * - La función parseCoord mapea coordenadas alfanuméricas normalizadas (por ejemplo, "A1"…"J10") a objetos Position{0..9, 0..9}.
 *   Si la coordenada es inválida, retorna std::nullopt.
 */
#pragma once
#include <string>
#include <optional>
#include "types.h"

namespace battleship {

struct Coords {
    // Normaliza las coordenadas de entrada.
    // Post: mayúsculas y sin espacios (trim).
    std::string normalize(std::string in);

    // Mapea coordenadas alfanumericas 
    // Post: mapea, p. ej., "A0"…"J9" → Position{0..9, 0..9}; si inválido, std::nullopt.
    static std::optional<Position> parseCoord(const std::string& normalized);
};

} // namespace battleship

```

## fleet.h
```c++
/**
 * @file fleet.h
 * @brief Represents a fleet of ships in the Battleship game.
 * 
 * The Fleet struct manages a collection of Ship objects, providing
 * utility functions to check if all ships are sunk, count ships by name,
 * and retrieve ships by their unique identifier.
 */
#pragma once
#include <string>
#include <vector>
#include "types.h"
#include "ship.h"

namespace battleship
{

struct Fleet 
{
    std::vector<Ship> ships;

    bool allSunk() const;
    int countByName(const std::string& name) const;
    const Ship* get(int shipId) const;
};

} // namespace battleship

```

## placement.h
```c++
/**
 * @file placement.h
 * @brief Header file for the Placement class.
 * 
 * Contratos para validación y colocación de barcos en el tablero.
 * Invariantes: 
 * - Los barcos solo se pueden colocar Horizontal o Verticalmente
 * - Los barcos no pueden sobresalir del tablero.
 * - Ningun barco pisa al otro (no hay solapamiento de celdas). shipIdAt == kNoShipId
 */
#pragma once
#include <string>
#include <vector>
#include "board.h"
#include "fleet.h"

namespace battleship
{

struct Placement 
{
    // Comprueba si un barco puede colocarse en el tablero sin salirse ni solaparse.
    // Post: true si el barco cabe (límites) y no hay solape.
    bool canPlace(const Ship& proto, Position start, Orientation orient, const Board& b, Fleet& f);

    // Coloca un barco en el tablero y lo añade a la flota.
    // Devuelve: El ID del barco colocado o -1 si no se pudo colocar.
    // Pre: canPlace(proto, start, orient, b, f) == true.
    // Post: agrega Ship a f y marca grid/shipIdAt en b
    int place(const Ship& proto, Position start, Orientation orient, Board& b, Fleet& f);
};

} // namespace battleship
```

## ship.h
```c++
#pragma once
/**
 * @file ship.h
 * @brief Definición de tipos y contratos para los barcos en Batalla Naval.
 *
 * Invariantes:
 * - Los barcos tienen una posición y una orientación (vertical u horizontal).
 * - Los barcos están compuestos por múltiples celdas en el tablero.
 * - 0 <= hits <= size
 * - sunk == (hits == size)
 */
#include <string>
#include <vector>
#include "types.h" // Para Position

namespace battleship
{

struct Ship 
{
    std::string name;               // Nombre del barco
    int size;                       // Tamaño del barco
    std::vector<Position> cells;    // Celdas ocupadas por el barco
    int hits;                       // Cantidad de impactos recibidos
    bool sunk;                      // Indica si el barco ha sido hundido

    // Evalua si el prototipo de barco es válido.
    // Pre: name != "" && size > 0
    static bool isValidPrototype(const Ship& proto); 

    // Evalua si el barco está hundido.
    // Pre: s.hits == s.size
    static bool isSunk(const Ship& s);           
};

} // namespace battleship
```

## shooting.h
```c++
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
```