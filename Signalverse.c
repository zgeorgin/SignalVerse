#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define PI 3.14159

double SV_LINE(double x)
{
	return x;
}

double SV_MEANDER(double x)
{
	if (x < 0)
		return 0;
	if (x < 0.5)
		return 1;
	return 0;
}
typedef struct 
{
	double* amps;
	double period;
	double dt;
} SV_Signal;


typedef struct
{
	double real;
	double imag;
} SV_Complex;

SV_Complex SV_create_complex(double real, double imag)
{
	SV_Complex res;
	res.real = real;
	res.imag = imag;
	return res;
}

SV_Signal SV_create_signal_from_amps(double* amps, double period, double dt)
{
	SV_Signal res;
	res.amps = amps;
	res.period = period;
	res.dt = dt;
	return res;
}


SV_Signal SV_create_signal_from_function(double (*f)(double), double period, double dt)
{
	double* amps = (double*) malloc(((int)(period/ dt) + 1) * sizeof(double));
	for (int i = 0; i < (int)(period / dt); i ++)
	{
		amps[i] = (*f)(i * dt);
	}
	SV_Signal res = SV_create_signal_from_amps(amps, period, dt);
	return res;
}

SV_Signal SV_copy_signal(SV_Signal s)
{
	SV_Signal res;
	res.dt = s.dt;
	res.period = s.period;
	res.amps = (double*) malloc(((int)(s.period/ res.dt) + 1) * sizeof(double));
	for (int i = 0; i < (int)(s.period / s.dt); i++)
	{
		res.amps[i] = s.amps[i];
	}
	return res;
}

SV_Complex SV_MULT_COMPLEX_COMPLEX(SV_Complex z1, SV_Complex z2)
{
	SV_Complex res = SV_create_complex( z1.real * z2.real - z1.imag * z2.imag, z1.real * z2.imag + z2.real * z1.imag);
	return res;
}

SV_Complex SV_MULT_DOUBLE_COMPLEX(double z1, SV_Complex z2)
{
	SV_Complex res = SV_create_complex( z1 * z2.real, z1 * z2.imag);
	return res;
}

SV_Complex SV_MULT_COMPLEX_DOUBLE(SV_Complex z1, double z2)
{
	SV_Complex res = SV_create_complex( z2 * z1.real, z2 * z1.imag);
	return res;

}

SV_Signal SV_MULT_SIGNAL_DOUBLE(SV_Signal s, double k)
{
	SV_Signal res = SV_copy_signal(s);
	for (int i = 0; i <= (int)(s.period / s.dt); i++)
		res.amps[i] *= k;
	return res;
}

SV_Signal SV_MULT_DOUBLE_SIGNAL(double k, SV_Signal s)
{
	SV_Signal res = SV_copy_signal(s);
	for (int i = 0; i <= (int)(s.period / s.dt); i++)
		res.amps[i] *= k;
	return res;
}

#define SV_Mult(X, Y) _Generic((X),\
    SV_Complex:   _Generic((Y),\
        SV_Complex:   SV_MULT_COMPLEX_COMPLEX,\
        double: SV_MULT_COMPLEX_DOUBLE\
    ),\
    SV_Signal: _Generic((Y),\
        double:   SV_MULT_SIGNAL_DOUBLE\
    )\
)(X, Y)

SV_Complex SV_Sum(SV_Complex z1, SV_Complex z2)
{
	SV_Complex res = SV_create_complex(z1.real + z2.real, z1.imag + z2.imag);
	return res;
}

double SV_Abs(SV_Complex z1)
{
	return z1.real * z1.real + z1.imag * z1.imag;
}

SV_Signal SV_expand_signal(SV_Signal s, double time)
{
    SV_Signal res;
    res.dt = s.dt;
    res.period = time;
    int new_size = (int)(time / res.dt);
    res.amps = (double*) malloc((new_size + 1) * sizeof(double));
    for (int i = 0; i < new_size; i++)
        res.amps[i] = s.amps[i % (int)(s.period / res.dt)];
    
    return res;
}

void SV_print_signal(SV_Signal s)
{
	for (int i = 0; i < (int)(s.period / s.dt); i++)
        printf("%.4f\n", s.amps[i]);
}

SV_Signal SV_file_to_signal(char* path)
{
	SV_Signal res;
	return res; // TODO
}

bool SV_save_signal_to_file(SV_Signal s, char* path)
{
	char dt[10], period[10];
	sprintf(dt, "%f", s.dt);
	sprintf(period, "%f", s.period);
	FILE *fp = fopen(path, "w");
    if(fp)
    {
        fputs(dt, fp);
		fputs("\n", fp);
		fputs(period, fp);
		fputs("\n", fp);

		for (int i = 0; i < (int)(s.period/s.dt); i++)
		{
			char value[10];
			sprintf(value, "%f", s.amps[i]);
			fputs(value, fp);
			fputs(", ", fp);
		}

        fclose(fp);
        printf("File has been written\n");
		return 1;
    }
	else
		printf("NO\n");
	return 0;
}

SV_Signal SV_sin_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(sin, 2*PI, dt);
	return res;
}

SV_Signal SV_cos_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(cos, 2*PI, dt);
	return res;
}

SV_Signal SV_triangle_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(SV_LINE, 1, dt);
	return res;
}

SV_Signal SV_meander_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(SV_MEANDER, 1, dt);
	return res;
}

SV_Complex SV_exp (SV_Complex z)
{
	SV_Complex res = SV_create_complex(exp(z.real) * cos(z.imag), exp(z.real) * sin(z.imag));
	return res;
}

SV_Complex* SV_forward_Fourier_transform (SV_Signal s)
{
	int size = (int)(s.period/s.dt);
	SV_Complex* res = (SV_Complex*) malloc(size * sizeof(SV_Complex));
	for (int i = 0; i < size; i++)
	{
		SV_Complex sum = SV_create_complex(0.0, 0.0);
		for (int j = 0; j < size; j++)
		{
			sum = SV_Sum(sum, SV_Mult(SV_exp(SV_create_complex(0.0, -2*PI*j*i / size)), s.amps[j]));
		}
		res[i] = sum;
 	}
	return res;
}

SV_Signal SV_reverse_Fourier_transform (SV_Complex* F, size_t size, double period)
{
	SV_Signal res;
	res.amps = (double*) malloc (size * sizeof(double));
	for (int i = 0; i < size; i++)
	{
		double sum = 0;
		for (int j = 0; j < size; j++)
		{
			res.amps[i] = 1.0/size * SV_MULT_COMPLEX_COMPLEX(F[j], SV_exp(SV_create_complex(0, 2*PI/size * i * j))).real;
		}
	}
	res.dt = period/size;
	res.period = period;
	return res;
}
/* 
	TODO:
	1.) Error handling
	2.) Signal visualisation
	3.) A lot more...
*/ 
