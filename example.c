#include "Signalverse.c"

double test_func(double x)
{
    return sin(x)*x*x;
}

int main()
{
   /* SV_Signal example = SV_meander_signal(0.01);
    SV_save_signal_to_file(SV_Mult(SV_expand_signal(example, 4*PI), 10.0), "./test.txt");*/

    SV_Signal example = SV_create_signal_from_function(test_func, 2*PI, 0.001);
    SV_Complex* F = SV_forward_Fourier_transform(example);
    size_t size = (size_t)(example.period/example.dt);
    SV_Signal transformed_example = SV_reverse_Fourier_transform(F, size, 2.0);
    SV_save_signal_to_file(transformed_example, "./test_Fourier.txt");
    SV_save_signal_to_file(example, "./test_func.txt");
    SV_file_to_signal("./test_error.txt");
    return 0;
}