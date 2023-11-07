#include <stdio.h>
#include <stdlib.h>
#include "ppm_images.c"
#include <math.h>
#include "Signalverse.c"



double SV_MAX (double* array, int size) {
    
    double SV_MAX = array[0];
    
    for (int i = 0; i < size; i++) {
        if (SV_MAX < array[i]) SV_MAX = array[i];
    }

    return SV_MAX;
}

double SV_MIN (double* array, int size) {
    
    double SV_MIN = array[0];

    for (int i = 0; i < size; i++) {
        if (SV_MIN > array[i]) SV_MIN = array[i];
    }

    return SV_MIN;
}

double SV_MAX_ABS(double x1, double x2) {

    if (fabs(x1) > fabs(x2)) return fabs(x1);
    else return fabs(x2);

}

double SV_LERP(double x1, double x2, double t) {
    return (x1 + (x2 - x1) * t);
}


/*double discr_freq(double* x) {
    return (SV_MAX(x) - SV_MIN(x)) / (sizeof(x) / sizeof(double) - 1);
}*/

void SV_prepare_to_ppm_out (SV_Signal signal, int width, int height, int x_new[], int y_new[]) {
    
    int x_size = (int) (signal.period / signal.dt);
    printf("size of x = %d\n", x_size);
    printf("signal period =  = %f\n", signal.period);
    printf("signal dt =  = %f\n", signal.dt);
    int y_size = x_size;

    if (y_size > height || x_size > width) {
        printf("too large size of array y\n");
        return;
    }
    
    int k_y = (int) (height / 2 / SV_MAX_ABS(SV_MAX(signal.amps, x_size), SV_MIN(signal.amps, x_size)));          //how many times do we extend the signal in amplitude
    printf("SV_MAX y = %f, SV_MIN y = %f, SV_MAX_ABS = %f", SV_MAX(signal.amps, x_size), SV_MIN(signal.amps, x_size), SV_MAX_ABS(SV_MAX(signal.amps, x_size), SV_MIN(signal.amps, x_size)));
    for (int i = 0; i < y_size; i++) {
        y_new[i] = (int) k_y * signal.amps[i];
    }
    
    int k_x = (int) (width/x_size);                                                  // how many times do we expand the signal in width
    if (k_x < 2) {                                                                   //(filling in intermediate points with SV_LERP)
        for (int i = 0; i < x_size; i++) {
            x_new[i] = i;
        }
    }
    else {
        for (int i = x_size * k_x - 1; i >=0 ; i--) {
            x_new[i] = i;
            if (i % k_x == 0) {
                y_new[i] = y_new[i/k_x];
            }
        }
        for (int i = 0; i < x_size * k_x; i++) {
            if (i % k_x != 0) {
                y_new[i] = (int) SV_LERP( (double) y_new[i - i % k_x], (double) y_new[i - i % k_x + k_x], (double) 1 / k_x * (i % k_x));
            }
        }
    }

    printf("x_new, y_new - created, k_x = %d, k_y = %d\n", k_x, k_y);

    return;
}

void SV_draw_axis(RGB** image, int width, int height) {
    for (int i = 0; i < height; i++) {
        image[i][0].r = 0;
        image[i][0].g = 255;
        image[i][0].b = 0;
    }

    int i = height / 2;

    for (int j = 0; j < width; j++) {
        image[i][j].r = 0;
        image[i][j].g = 0;
        image[i][j].b = 0;
    }

}

RGB** SV_draw_graph (SV_Signal signal, int width, int height) {

    RGB** image = SV_create_image(WHITE_COLOR, width, height);

    SV_draw_axis(image, width, height);

    int x_size = (int) (signal.period / signal.dt);

    int k_x = (int) (width/ x_size);

    int* x_new = (int*) malloc (x_size * k_x * sizeof(int));
    int* y_new = (int*) malloc (x_size * k_x * sizeof(int));

    SV_prepare_to_ppm_out(signal, width, height, x_new, y_new);


    
    for (int i = 0; i < x_size * k_x; i++) {
        image[height/2 + y_new[i]][x_new[i]].r = 255;
        image[height/2 + y_new[i]][x_new[i]].g = 0;
        image[height/2 + y_new[i]][x_new[i]].b = 0;
    }
    return image;
}

int main(int argc, char* argv[]) {
    double dt = 0.01;
    SV_Signal sin = SV_sin_signal(dt);
    RGB** image = SV_draw_graph (sin, 1280, 720);
    SV_save_image("P3",  1280, 720, 255, image, "./sin.ppm");
    return 0;
}

