#include "placement.h"
#include "board.h"
#include "fleet.h"
#include "types.h"

namespace battleship
{

bool Placement::canPlace(const Ship& proto, Position start, Orientation orient, const Board& b, Fleet& f)
{
    // Comprobar límites
    if (!Board::isInside(start)) return false;

    // Comprobar solapamientos
    for (const auto& cell : proto.cells) {
        Position p = start;
        if (orient == Orientation::Horizontal) p.col += cell.col;
        else p.row += cell.row;

        if (!Board::isInside(p) || b.get(p) != CellState::Empty) return false;
    }

    return true;
}

int Placement::place(const Ship& proto, Position start, Orientation orient, Board& b, Fleet& f)
{
    if (!canPlace(proto, start, orient, b, f)) return -1;

    // Colocar barco en el tablero
    for (const auto& cell : proto.cells) {
        Position p = start;
        if (orient == Orientation::Horizontal) p.col += cell.col;
        else p.row += cell.row;

        b.set(p, CellState::Ship);
        b.setShipId(p, f.ships.size());
    }

    // Añadir barco a la flota
    f.ships.push_back(proto);
    return f.ships.size() - 1;
}

} // namespace battleship
