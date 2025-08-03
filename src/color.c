#include "color.h"

void write_color(FILE *image, color *color) {
    // Clamp color values to the range [0, 1] and convert to 8-bit integers
    interval intensity = {0.000, 0.999};
    int rbyte = (int)(256 * clamp(&intensity, (*color)[0]));
    int gbyte = (int)(256 * clamp(&intensity, (*color)[1]));
    int bbyte = (int)(256 * clamp(&intensity, (*color)[2]));
    fprintf(image, "%d %d %d\n", rbyte, gbyte, bbyte);
}
