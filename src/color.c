#include "color.h"

void write_color(FILE *image, color *color) {
    int ir = (int)(255.999 * (*color)[0]);
    int ig = (int)(255.999 * (*color)[1]);
    int ib = (int)(255.999 * (*color)[2]);
    fprintf(image, "%d %d %d\n", ir, ig, ib);
}
