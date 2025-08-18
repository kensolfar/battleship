/**
 * @file fleet.h
 * @brief Contratos y tipos para la flota de barcos.
 *
 * La `Fleet` representa la colección de barcos del jugador.
 * Sus responsabilidades principales son:
 * - Mantener la lista de `Ship` que forman la flota.
 * - Proveer utilidades para consultar el estado agregado de la flota
 *   (p. ej. si todos los barcos están hundidos).
 * - Buscar y contar barcos por propiedades (nombre, id).
 *
 * Invariantes y convenciones:
 * - Cada elemento de `ships` representa un barco distinto; el identificador
 *   único del barco se consulta mediante la interfaz de `Ship`.
 * - Los métodos no modifican la colección salvo que se indique explícitamente.
 */
#pragma once
#include <string>
#include <vector>
#include "types.h"
#include "ship.h"

namespace battleship
{

/**
 * @brief Colección de barcos y utilidades de consulta.
 *
 * Contrato resumido:
 * - `allSunk()` devuelve true cuando todos los barcos de `ships` están hundidos.
 * - `countByName(name)` devuelve cuántos barcos en la flota tienen el nombre dado.
 * - `get(shipId)` devuelve un puntero constante al `Ship` con el id indicado o `nullptr`.
 */
struct Fleet
{
    /**
     * @brief Contenedor de barcos que forman la flota.
     *
     * Cada elemento es un `Ship` con su estado (intacto, tocado, hundido, etc.).
     */
    std::vector<Ship> ships;

    /**
     * @brief Indica si todos los barcos de la flota están hundidos.
     * @return true si la flota no contiene ningún barco no hundido.
     */
    bool allSunk() const;

    /**
     * @brief Cuenta los barcos cuyo nombre coincide con `name`.
     * @param name Nombre a buscar (case-sensitive según `Ship::name()` o equivalente).
     * @return Número de barcos con ese nombre en la flota.
     */
    int countByName(const std::string& name) const;

    /**
     * @brief Busca un barco por su identificador único.
     * @param shipId Identificador del barco a buscar.
     * @return Puntero constante al `Ship` si se encuentra, o `nullptr` si no existe.
     * @post No modifica la flota.
     */
    const Ship* get(int shipId) const;
};

} // namespace battleship
