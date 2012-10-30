#ifndef BETANOISE_H
#define BETANOISE_H

#include <vector>
#include <stdlib.h>
#include <math.h>

struct Complex
{
	double real;
	double imaginary;
};

class BetaNoise
{
	public:
		static std::vector<Complex> noise(int,double);

	private:
		static double gauss();
		static void fastfourierinverse(int, std::vector<struct Complex>);
};

#endif