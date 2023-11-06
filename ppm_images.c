#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

RGB make_color (unsigned char r, unsigned char g, unsigned char b) {
    RGB color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color; 
}


const RGB BLACK_COLOR = {0,0,0};
const RGB RED_COLOR =  {255,0,0};
const RGB BLUE_COLOR = {0,0,255};
const RGB GREEN_COLOR = {0,255,0};

RGB** create_image (RGB color, int width, int height) {
    RGB** image = (RGB**) malloc(height * sizeof(RGB*));
    for (int i = 0; i < height; i++) {
        image[i] = (RGB*) malloc (width * sizeof(RGB));
    }
    printf("general color %u, %u, %u\n", color.r, color.g, color.b);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j].r = color.r;
            image[i][j].g = color.g;
            image[i][j].b = color.b;
        }
    }
    printf("image created\n");

    return image;
}

void save_image(char* version, int width, int height, int color, RGB** image, char* file_path) {
    


    if (file_path == NULL) printf("Error path name");

    FILE* output = fopen(file_path, "w");

    printf("file_opened\n");

    if (!output) {
        printf("error open file");
        return;
    }

    fprintf(output, "%s\n%u %u\n%u\n", version, width, height, color);

    for (int i = 0; i < height; i++)
            for ( int j = 0; j < width; j++) {
                fprintf(output, "%u %u %u\n", image[i][j].r, image[i][j].g, image[i][j].b);
            }

    fclose(output);
}

/*void read_image(char* file_path) {

    if (file_path == NULL) printf("Error path name");

    ifstream input(file_path);

    if (input.isopen()) {
        
    }

}*/


int main(int argc, char* argv[]) {
    
    RGB** image = create_image(RED_COLOR, 800, 600);
    save_image("P3", 800, 600, 255, image, "./test2.ppm");
    return 0;
}