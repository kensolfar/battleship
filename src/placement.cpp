/**
 * @file placement.cpp
 * 
 */
#include "placement.h"

using namespace battleship;

bool Placement::canPlace(const Ship& proto, const Board& b, const Fleet& f) 
{
    if (!Ship::isValidPrototype(proto)) return false;

    // verifica que la posición inicial está dentro del tablero
    if (!Board::isInside(proto.start)) return false;

    // verifica que la posición final está dentro del tablero
    if (proto.orientation == Orientation::Horizontal) {
        if (proto.start.col + proto.size > Board::kSize) return false;
    } else {
        if (proto.start.row + proto.size > Board::kSize) return false;
    }

    // construir las celdas que ocuparía el barco
    std::vector<Position> shipCells;
    shipCells.reserve(proto.size);
    for (int i = 0; i < proto.size; ++i) {
        Position cell = proto.orientation == Orientation::Horizontal
            ? Position{proto.start.row, proto.start.col + i}
            : Position{proto.start.row + i, proto.start.col};
        // doble verificación de límites
        if (!Board::isInside(cell)) return false;
        // verificación de superposición: las celdas objetivo deben estar vacías (sin id de barco)
        if (b.getShipId(cell) != Board::kNoShipId) return false;
        shipCells.push_back(cell);
    }

    // verificar la regla de "no contigüidad": todas las celdas del anillo deben estar libres de barcos
    const std::vector<Position> ring = computeRing(proto.start, proto.orientation, proto.size);
    for (const Position& r : ring) {
        if (b.getShipId(r) != Board::kNoShipId) return false;
    }

    return true;
}

int Placement::place(const Ship& proto, Position start, Orientation orient,
              Board& b, Fleet& f)
{
    if (!Ship::isValidPrototype(proto) || !canPlace(proto, b, f)) return -1;

    // construir las celdas que ocuparía el barco y validar límites/superposición
    Ship s;
    s.name = proto.name;
    s.size = proto.size;
    s.start = start;
    s.orientation = orient;
    s.hits = 0;
    s.sunk = false;

    for (int i = 0; i < s.size; ++i) {
        Position cell = orient == Orientation::Horizontal
            ? Position{start.row, start.col + i}
            : Position{start.row + i, start.col};
        if (!Board::isInside(cell)) return -1;
        if (b.getShipId(cell) != Board::kNoShipId) return -1; // verificación de superposición
        s.cells.push_back(cell);
    }

    // add to fleet
    f.ships.push_back(s);
    int shipId = static_cast<int>(f.ships.size()) - 1;

    // mark board
    for (const Position& cell : s.cells) {
        b.set(cell, CellState::Ship);
        b.setShipId(cell, shipId);
    }

    return shipId;
}

std::vector<Position> Placement::n8(Position pos) 
{
    std::vector<Position> neighbors;
    if (Board::isInside(pos)) {
        // Iterate over the 8 possible neighbors
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                // Skip the center position
                if (dr == 0 && dc == 0) continue;
                const Position nb{pos.row + dr, pos.col + dc};
                if (Board::isInside(nb)) {
                    neighbors.push_back(nb);
                }
            }
        }
    }
    return neighbors;
}

std::vector<Position> Placement::computeRing(Position pos, Orientation orient, int size) 
{
    std::vector<Position> ring;

    if (size <= 0) return ring;

    // build ship cells
    std::vector<Position> shipCells;
    shipCells.reserve(size);
    for (int i = 0; i < size; ++i) {
        Position cell = orient == Orientation::Horizontal
            ? Position{pos.row, pos.col + i}
            : Position{pos.row + i, pos.col};
        shipCells.push_back(cell);
    }

    // helper lambdas for comparisons
    auto samePos = [](const Position& a, const Position& b){ return a.row == b.row && a.col == b.col; };
    auto contains = [&](const std::vector<Position>& vec, const Position& p){
        return std::any_of(vec.begin(), vec.end(), [&](const Position& q){ return samePos(q, p); });
    };

    // collect neighbors of each ship cell, excluding the ship cells themselves
    for (const Position& cell : shipCells) {
        for (const Position& nb : n8(cell)) {
            // respect board bounds
            if (!Board::isInside(nb)) continue;
            // skip the ship cells
            if (contains(shipCells, nb)) continue;
            // deduplicate
            if (!contains(ring, nb)) ring.push_back(nb);
        }
    }

    return ring;
} 