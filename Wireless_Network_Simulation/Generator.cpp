#include "Generator.h"



Generator::Generator(int kernel) :kernel_(kernel)
{
}

Generator::~Generator()
{
}

double Generator::Rand()
{
	int h = kernel_ / kQ;
	kernel_ = kA * (kernel_ - kQ * h) - kR * h;
	if (kernel_ < 0)
		kernel_ = kernel_ + static_cast<int>(kM);
	return kernel_ / kM;
}

double Generator::Rand(int min, int max)
{
	return Rand()*((max+1) - min) + min; 
}

double Generator::RndExp(double lambda)
{
	double k = Rand();
	return -(1.0 / lambda)*log(k);
}

int Generator::RndZeroOne(double p)
{
	double k = Rand();
	if (k < p)
		return 1;
	return 0;
}

int Generator::get_kernel()
{
	return kernel_;
}

void Generator::set_seed(int seed)
{
	kernel_ = seed;
}
