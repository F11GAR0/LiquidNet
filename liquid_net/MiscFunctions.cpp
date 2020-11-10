#include "main.h"
#include "MiscFunctions.h"

bool is_prime(unsigned long long p) {
	for (int i = 2; i < p; i++) {
		if (p % i == 0) return false;
	}
	return true;
}

unsigned long long gen_prime(unsigned long long start, unsigned long long end) {
	int jump = rand() % (end - start) / 5;
	for (unsigned long long i = start + jump; i < end; i++) {
		if (is_prime(i)) return i;
	}
	return 17;
}

unsigned long long long_pow(unsigned long long n, unsigned long long e) {
	unsigned long long ret = n;
	for (int i = 1; i < e; i++) {
		ret *= n;
	}
	return ret;
}