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

bool SV_MAKE_RANGE(double begin, double end, int N, double* array)
{
	if (end - begin < 0 || N < 0)
		return 0;

	array = (double*)malloc(N * sizeof(double));
	double step = (end - begin)/N;
	for (int i = 0; i < N; i+=1)
	{
		array[i] = begin + step*i;
	}
	return 1;
}

typedef struct
{
	double *amps;
	double period;
	double dt;
} SV_Signal;

typedef struct
{
	double real;
	double imag;
} SV_Complex;

SV_Complex SV_create_complex(double real, double imag) //Useless, but if you want to do it this way, you can
{
	SV_Complex res;
	res.real = real;
	res.imag = imag;
	return res;
}

SV_Signal SV_create_signal_from_amps(double *amps, double period, double dt) //Useless, but if you want to do it this way, you can
{
	SV_Signal res;
	res.amps = amps;
	res.period = period;
	res.dt = dt;
	return res;
}

SV_Signal SV_create_signal_from_function(double (*f)(double), double period, double dt)
{
	double *amps = (double *)malloc(((int)(period / dt) + 1) * sizeof(double));
	for (int i = 0; i < (int)(period / dt); i++)
	{
		amps[i] = (*f)(i * dt);
	}
	SV_Signal res = {amps, period, dt};
	return res;
}

SV_Signal SV_copy_signal(SV_Signal s)
{
	SV_Signal res;
	res.dt = s.dt;
	res.period = s.period;
	res.amps = (double *)malloc(((int)(s.period / res.dt) + 1) * sizeof(double));
	for (int i = 0; i < (int)(s.period / s.dt); i++)
	{
		res.amps[i] = s.amps[i];
	}
	return res;
}

SV_Complex SV_MULT_COMPLEX_COMPLEX(SV_Complex z1, SV_Complex z2)
{
	SV_Complex res = {z1.real * z2.real - z1.imag * z2.imag, z1.real * z2.imag + z2.real * z1.imag};
	return res;
}

SV_Complex SV_MULT_DOUBLE_COMPLEX(double z1, SV_Complex z2)
{
	SV_Complex res = {z1 * z2.real, z1 * z2.imag};
	return res;
}

SV_Complex SV_MULT_COMPLEX_DOUBLE(SV_Complex z1, double z2)
{
	SV_Complex res = {z2 * z1.real, z2 * z1.imag};
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

/*#define SV_Mult(X, Y) _Generic((X),      \
	SV_Complex: _Generic((Y),            \
	SV_Complex: SV_MULT_COMPLEX_COMPLEX, \
	double: SV_MULT_COMPLEX_DOUBLE),     \
	SV_Signal: _Generic((Y),             \
	double: SV_MULT_SIGNAL_DOUBLE),\
	double: _Generic((Y),             \
	SV_Complex: SV_MULT_DOUBLE_COMPLEX, \
	SV_Signal: SV_MULT_DOUBLE_SIGNAL))(X, Y)
*/
SV_Complex SV_Sum(SV_Complex z1, SV_Complex z2)
{
	SV_Complex res = {z1.real + z2.real, z1.imag + z2.imag};
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
	res.amps = (double *)malloc((new_size + 1) * sizeof(double));
	for (int i = 0; i < new_size; i++)
		res.amps[i] = s.amps[i % (int)(s.period / res.dt)];

	return res;
}

void SV_print_signal(SV_Signal s)
{
	for (int i = 0; i < (int)(s.period / s.dt); i++)
		printf("%.4f\n", s.amps[i]);
}

bool SV_save_signal_to_file(SV_Signal s, char *path)
{
	FILE *fp = fopen(path, "w");
	if (!fp)
	{
		printf("Failed to save signal in file %s!", path);
		return 1;
	}
	fprintf(fp, "%f\n%f\n", s.dt, s.period);

	for (int i = 0; i < (int)(s.period / s.dt) - 1; i++)
		fprintf(fp, "%f, ", s.amps[i]);

	fprintf(fp, "%f", s.amps[(int)(s.period / s.dt) - 1]);
	fclose(fp);
	printf("File has been written\n");
	return 1;
}

bool SV_file_to_signal(char *path, SV_Signal *s)
{
	SV_Signal res;
	FILE *mf = fopen(path, "r");
	if (!mf)
	{
		printf("Error opening file %s! Maybe it does not exist", path);
		return 1;
	}

	return 0; // TODO
}

SV_Signal SV_sin_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(sin, 2 * PI, dt);
	return res;
}

SV_Signal SV_cos_signal(double dt)
{
	SV_Signal res = SV_create_signal_from_function(cos, 2 * PI, dt);
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

SV_Complex SV_exp(SV_Complex z)
{
	SV_Complex res = {exp(z.real) * cos(z.imag), exp(z.real) * sin(z.imag)};
	return res;
}

SV_Complex *SV_forward_Fourier_transform(SV_Signal s)
{
	int size = (int)(s.period / s.dt);
	SV_Complex *res = (SV_Complex *)malloc(size * sizeof(SV_Complex));
	for (int i = 0; i < size; i++)
	{
		SV_Complex sum = SV_create_complex(0.0, 0.0);
		for (int j = 0; j < size; j++)
		{
			sum = SV_Sum(sum, SV_MULT_DOUBLE_COMPLEX(s.amps[j], SV_exp(SV_create_complex(0.0, -2 * PI / size * j * i))));
		}
		res[i] = sum;
	}
	return res;
}

SV_Signal SV_reverse_Fourier_transform(SV_Complex *F, size_t size, double period)
{
	SV_Signal res;
	res.amps = (double *)malloc(size * sizeof(double));
	for (int i = 0; i < size; i++)
	{
		double sum = 0;
		for (int j = 0; j < size; j++)
		{
			sum += 1.0 / size * SV_MULT_COMPLEX_COMPLEX(F[j], SV_exp(SV_create_complex(0, 2 * PI / size * i * j))).real;
		}
		res.amps[i] = sum;
	}
	res.dt = period / size;
	res.period = period;
	return res;
}

double* SV_AFR (SV_Signal s) // TODO
{
	double* res = (double *)malloc((int)(s.period / s.dt) * sizeof(double));
	return res;
}

//TODO:
//Error handling
//Signal visualisation
//A lot more...

