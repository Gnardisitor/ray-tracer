#include "main.h"

int main(void) {
    // Create world
    hittable_list world;
    world.count = 0;
    add_sphere(&world, 0.0, 0.0, -1.0, 0.5);
    add_sphere(&world, 0.0, -100.5, -1.0, 100.0);

    // Create camera
    camera cam;
    double aspect_ratio = 16.0 / 9.0;
    int samples_per_pixel = 10;
    int image_width = 400;
    camera_create(&cam, 0.0, 0.0, 0.0, samples_per_pixel, aspect_ratio, image_width);

    // Create image file
    FILE *image = fopen("image.ppm", "wt");
    if (image == NULL) {
        printf("Could not create image\n");
        return EXIT_FAILURE;
    }

    // Render the scene
    camera_render(&cam, &world, image);
    
    fclose(image);
    return EXIT_SUCCESS;
}
