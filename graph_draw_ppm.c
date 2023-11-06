#include <stdio.h>
#include <stdlib.h>
#include "ppm_images.c"
#include <math.h>
#include "Signalverse.c"



double max (double* array, int size) {
    
    double max = array[0];
    
    for (int i = 0; i < size; i++) {
        if (max < array[i]) max = array[i];
    }

    return max;
}

double min (double* array, int size) {
    
    double min = array[0];

    for (int i = 0; i < size; i++) {
        if (min > array[i]) min = array[i];
    }

    return min;
}

double max_abs(double x1, double x2) {

    if (fabs(x1) > fabs(x2)) return fabs(x1);
    else return fabs(x2);

}

double lerp(double x1, double x2, double t) {
    return (x1 + (x2 - x1) * t);
}


/*double discr_freq(double* x) {
    return (max(x) - min(x)) / (sizeof(x) / sizeof(double) - 1);
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
    
    int k_y = (int) (height / 2 / max_abs(max(signal.amps, x_size), min(signal.amps, x_size)));          //how many times do we extend the signal in amplitude
    printf("max y = %f, min y = %f, max_abs = %f", max(signal.amps, x_size), min(signal.amps, x_size), max_abs(max(signal.amps, x_size), min(signal.amps, x_size)));
    for (int i = 0; i < y_size; i++) {
        y_new[i] = (int) k_y * signal.amps[i];
    }
    
    int k_x = (int) (width/x_size);                                                  // how many times do we expand the signal in width
    if (k_x < 2) {                                                                   //(filling in intermediate points with lerp)
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
                y_new[i] = (int) lerp( (double) y_new[i - i % k_x], (double) y_new[i - i % k_x + k_x], (double) 1 / k_x * (i % k_x));
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

    for (int i = 0; i < k_x * x_size; i++) {
        printf("%d, %d\n",x_new[i], y_new[i]);
    }

    
    for (int i = 0; i < x_size * k_x; i++) {
        image[height/2 + y_new[i]][x_new[i]].r = 255;
        image[height/2 + y_new[i]][x_new[i]].g = 0;
        image[height/2 + y_new[i]][x_new[i]].b = 0;
    }
    printf("%d", x_size * k_x);
    return image;
}

int main(int argc, char* argv[]) {
    double dt = 0.1;
    SV_Signal sin = SV_sin_signal(dt);
    RGB** image = SV_draw_graph (sin, 800, 600);
    SV_save_image("P3", 800, 600, 255, image, "./sin.ppm");
    return 0;
}

