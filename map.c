#include "map.h"

int map(float val, float minx, float maxx, float miny, float maxy) {
    // Asegurarse de que el valor de entrada (val) esté dentro del rango dado
    if (val < minx) {
        val = minx;
    } else if (val > maxx) {
        val = maxx;
    }
  
    // Calcular la escala del valor desde el rango de entrada al rango de salida
    // y devolver el valor mapeado
    return ((val - minx) * (maxy - miny) / (maxx - minx) + miny);
}
