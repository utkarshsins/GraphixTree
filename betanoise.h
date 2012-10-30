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
		static double gauss();

	private:
		static void fastfourierinverse(int, std::vector<struct Complex>);
};

#endif