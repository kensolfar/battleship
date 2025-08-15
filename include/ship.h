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
    Position start;                 // Posición de inicio del barco
    
    // Horizontal: todas las celdas tienen row == start.row y col consecutivos.
    // Vertical: todas las celdas tienen col == start.col y row consecutivos.
    Orientation orientation;        // Orientación del barco
    int hits;                       // Cantidad de impactos recibidos
    bool sunk;                      // Indica si el barco ha sido hundido

    // Evalua si el prototipo de barco es válido.
    // Pre: name != "" && size > 0
    static bool isValidPrototype(const Ship& proto); 

    // Evalua si el barco está hundido.
    // Pre: s.hits == s.size
    static bool isSunk(const Ship& s);  

    // Coherencia estructural con start/orientation/size
    static bool isConsistent(const Ship& s /*, const Board& b */);      
};

} // namespace battleship