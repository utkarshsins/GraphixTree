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
		static std::vector<std::vector<double>> noises;
		static double gauss();
		static void generatenoises();

	private:
		static void fastfourierinverse(int, std::vector<struct Complex>);
		static std::vector<double> librarynoise(int,double);
};

#endif