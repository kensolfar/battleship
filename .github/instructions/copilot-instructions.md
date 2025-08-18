# Copilot Instructions — Proyecto Batalla Naval en C++

## Contexto general
Estamos desarrollando un juego de **Batalla Naval (Battleship)** en C++ de forma incremental, con un enfoque **didáctico y modular**:

- Objetivo: aprender C++ intermedio con un proyecto práctico.
- Enfoque: dividir en fases, introducir un concepto nuevo en cada una (OOP, estructuras de datos, persistencia, etc.).
- El código se está organizando en un proyecto **CMake multiplataforma** (Windows, Linux, macOS), compilado en **Visual Studio 2022** y también con `cmake` en CLI.

---

## Estructura del proyecto

battleship/
├─ CMakeLists.txt
├─ CMakePresets.json
├─ include/
│ ├─ types.h
│ ├─ board.h
│ ├─ ship.h
│ ├─ fleet.h
│ ├─ placement.h
│ ├─ shooting.h
│ └─ coords.h
├─ src/
│ ├─ board.cpp
│ ├─ fleet.cpp
│ ├─ placement.cpp
│ ├─ shooting.cpp
│ ├─ coords.cpp
│ └─ main.cpp
└─ tests/
├─ test_geometry.cpp
├─ test_shooting.cpp
└─ ...


---

## Reglas del juego y decisiones tomadas

### Tablero
- Tamaño: **10 × 10**.
- Coordenadas externas: filas `A..J`, columnas `1..10`.
- Internamente: índices `0..9`.

### Flota
- Portaaviones (5)
- Acorazado (4)
- Crucero (3)
- 2 × Destructores (2)
- 2 × Submarinos (1)

### Reglas
- **Regla de adyacencia / no contiguidad**: los barcos no pueden tocarse **ni lateral ni diagonalmente**. Siempre debe haber al menos 1 celda vacía alrededor de cada barco.
- **Orientación**: Horizontal o Vertical.
- **Ancla de colocación**: la celda `start` es el inicio del tramo recto del barco.
- **Turnos**:
  - Si fallas → cambia turno.
  - Si aciertas y **no hundes** → repites turno.
  - Si hundes → cambia turno.
- **Fog of war**: al mostrar el tablero enemigo, solo se ven tus disparos (`Hit/Miss/Sunk`). Nunca se muestran barcos intactos.
- **Disparo repetido**: rechazado, se pide nueva coordenada sin perder turno.

---

## Diseño de datos

### `types.h`
- `enum class CellState { Empty, Ship, Hit, Miss };`
- `struct Position { int row; int col; };`
- `enum class Orientation { Horizontal, Vertical };`
- `struct ShotResult { enum class Kind { Miss, Hit, Sunk }; Kind kind; int shipId; Position pos; };`

### `board.h`
- `struct Board`
  - `CellGrid grid` (estado visible de celdas).
  - `IdGrid shipIdAt` (índice inverso celda→shipId, `-1` si vacío).
- Invariantes:
  - `shipIdAt[p] == -1` ⇔ celda vacía.
  - `shipIdAt[p] >= 0` ⇒ `grid[p]` es Ship o Hit.

### `ship.h`
- `struct Ship`
  - `name, size, start, orientation, cells, hits, sunk`.
- Invariantes:
  - `0 ≤ hits ≤ size`.
  - `sunk == (hits == size)`.
  - `cells.size() == size`.
  - `cells` coherente con `start` y `orientation`.

### `fleet.h`
- `struct Fleet`
  - `std::vector<Ship> ships`.
  - Métodos: `allSunk()`, `countByName()`, `get(id)`.

### `placement.h`
- `struct Placement`
  - `bool canPlace(proto,start,orient,b,f);`
  - `int place(proto,start,orient,b,f);`
  - `static std::vector<Position> neighborhood(Position p);`
  - `static std::vector<Position> computeRing(start,orient,size);`
- Regla de no contiguos: `computeRing` calcula las posiciones vecinas (8-direcciones) de todas las celdas de un barco y comprueba que estén libres.

### `shooting.h`
- `struct Shooting`
  - `ShotResult shoot(Board& b, Fleet& f, Position p);`
- Reglas:
  - `Empty → Miss`.
  - `Ship → Hit` (incrementa `hits`; si `hits == size` → `Sunk`).

### `coords.h`
- `struct Coords`
  - `static std::string normalize(std::string in);`
  - `static std::optional<Position> parseCoord(const std::string& normalized);`
- Reglas:
  - Entrada `"A1"…"J10"`.
  - Interno: `(row,col)` en 0..9.

---

## Pruebas

### Framework
- Inicialmente con `assert` (sin dependencias).
- Se integran con **CTest** (`add_test`) en CMake.

### Ejemplos de pruebas unitarias
- `neighborhood`: centro → 8 vecinos; esquina (0,0) → 3 vecinos.
- `computeRing`: no incluye las celdas del propio barco, descarta fuera de tablero.
- `canPlace`: falso si fuera de rango / solape / adyacencia.
- `place`: marca grid y shipIdAt en todas las celdas, inicializa barco.
- `shoot`: Empty → Miss, Ship → Hit, último impacto → Sunk.
- `game loop`: verifica regla de turnos (Hit repite, Sunk cambia).

---

## CMake y multiplataforma

### Requisitos
- CMake ≥ 3.20.
- Ninja o MSBuild como generador.
- Compiladores:
  - Windows → MSVC (cl.exe).
  - Linux → gcc/clang (WSL o nativo).
  - macOS → clang.

### `CMakePresets.json`
- Configurados presets para:
  - `win-msvc-debug`
  - `linux-wsl-debug`
  - `macos-clang-debug`
- Permite cambiar entre entornos desde Visual Studio 2022 o CLI.

### Build CLI
```bash
cmake -S . -B out/build/win-msvc-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build out/build/win-msvc-debug
ctest --test-dir out/build/win-msvc-debug --output-on-failure