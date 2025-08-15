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

    // Post: devuelve un puntero al barco con el id dado, o nullptr si no existe.
    const Ship* get(int shipId) const;
};

} // namespace battleship
