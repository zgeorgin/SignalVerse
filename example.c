#include "Signalverse.c"

int main()
{
   /* SV_Signal example = SV_meander_signal(0.01);
    SV_save_signal_to_file(SV_Mult(SV_expand_signal(example, 4*PI), 10.0), "./test.txt");*/

    SV_Signal example = SV_sin_signal(0.01);
    SV_Complex* F = SV_forward_Fourier_transform(example);
    size_t size = (size_t)(example.period/example.dt);
    SV_Signal transformed_example = SV_reverse_Fourier_transform(F, size, 2*PI);
    SV_save_signal_to_file(transformed_example, "./test_Fourier.txt");
    SV_save_signal_to_file(example, "./test_sin.txt");
    return 0;
}