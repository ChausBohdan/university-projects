#include <stdio.h>
#include <math.h>

extern "C" double calc(double, float, float);

double calc_c(double a, float c, float d) {
	if (c > d)
	{
		return ((2 * c - log(a + d) * c) / (c / 4 - 1));
	}
	else {
		return ((41 - d / 4 - 1) / ((c / tan(d + a)) - d));
	}
}

int main()
{
	double a = 0;
	float c = 0;
	float d = 0;

	printf("((2 * c - ln(a + d) * c) / (c / 4 - 1)  c > d\n");
	printf("((41 - d / 4 - 1) / (c / tan(d + a) - d))  c <= d\n");

	printf("\nEnter numbers:\n");

	printf("a = ");
	scanf_s("%lf", &a);
	printf("c = ");
	scanf_s("%f", &c);
	printf("d = ");
	scanf_s("%f", &d);

	double res_c = calc_c(a, c, d);
	double res_asm = calc(double(a), float(c), float(d));

	printf("\nResult of c calc is: %lf\n", res_c);
	printf("\nResult of procedure calc is: %lf\n", res_asm);
	return 0;
}