#include "Signalverse.c"

int main()
{
    SV_Signal example = SV_sin_signal(0.001);
    SV_save_signal_to_file(SV_MULT(SV_expand_signal(example, 4*PI), 10.0), "./test.txt");
    return 0;
}