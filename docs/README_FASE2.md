# Decisiones Pendientes

## Regla de Adyacencia
Los barcos no pueden tocarse ni lateral ni diagonalmente. Esto significa que si un barco está colocado en una posición determinada, no puede haber otros barcos en las celdas adyacentes (incluyendo las diagonales). 

## Regla de no Contiguidad
Los barcos deben estar completamente separados por al menos una celda vacía en todas las direcciones y en las esquinas.

## Ancla de colocacion. 
Agregado `Position start;` a `Ship`

## Orientacion
Agregado `Orientation orientation;` a `Ship`