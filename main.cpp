#include <cstdio>
#include <ctime>
#include <vector>
#include <cstring>
#include <climits>
#include <limits>
#include <cmath>


typedef unsigned long long ull;

void print_factors(std::vector<std::vector<int>> &compositeFactors, std::vector<ull> composites) {
    int it = 0;
    for(auto fac: compositeFactors) {
        it++;
        printf("%d) ", it);
        for (int i = fac.size() - 1; i >= 0; i--) {
            printf(" %d,", fac[i]);
        }
        printf(" = %llu \n", composites[it - 1]);
    }
}
void print_factors2(std::vector<std::vector<int>> &compositeFactors, std::vector<ull> composites) {
    int it = 0;
    int c = 0;
    for(auto fac: compositeFactors) {
        it++;

        if(composites[it] % 2 == 0){
            c++;
            printf("%d) ", c);
            for (int i = fac.size() - 1; i >= 0; i--) {
                printf(" %d,", fac[i]);
            }

            printf(" = %llu \n", composites[it - 1]);

        }
    }
}
void print_candidates(std::vector<ull> candidates) {
    int it = 0;
    for(auto c: candidates) {
        it++;
        printf("%d) %llu\n", it, c);
    }
}

void print_candidates_rs(std::vector<ull> candidates, std::vector<int> rs) {
    int it = 0;
    for(auto c: candidates) {

        printf("%d) %llu, r = %d\n", it + 1, c, rs[it]);
        it++;
    }
}

void primeNumber(int range, std::vector<int> &rangePrimes) {
    std::vector<bool> primes(range + 1, true);

    for(int p = 2; p <= range; p++) {
        if(primes[p]) {
            for(int i = p * p; i <= range; i += p) {
                primes[i] = false;
            }
            rangePrimes.emplace_back(p);
        }
    }
}

bool isPrime(const ull n) {
    if(n == 2) return true;
    if(n == 3) return true;

    if(n % 2 == 0 || n % 3 == 0)
        return false;


    double root(std::sqrt(n));
    for(int i = 5; i <= root; i += 6)
        if(n % i == 0 || n % (i + 2) == 0)
            return false;


    return true;
}

std::vector<ull> compositesGenerator(const std::vector<int> &primes, const ull Mmax) {
    std::vector<std::vector<int>> compositeFactors;
    std::vector<ull> composites;
    std::vector<ull> candidates{2};

    for(int i = 0; i < primes.size(); i++) {
        for(int j = i; j < primes.size(); j++) {

            std::vector<int>tmp{primes[j], primes[i]};
            compositeFactors.emplace_back(tmp);
            composites.push_back(primes[j] * primes[i]);

            if(primes[i] == 2) {
                if(primes[j] * primes[i] < Mmax && isPrime(primes[j] * primes[i] + 1)) {
                    candidates.emplace_back(primes[j] * primes[i]);
                }
            }
        }
    }

    int oldSize(0);
    while (true){
        bool add(false);
        int size(compositeFactors.size());

        for (int prime : primes) {
            bool append(false);
            int lastIndex(compositeFactors[oldSize].size() - 1);

            for (int i = oldSize; i < size; i++) {
                if (prime == compositeFactors[i][lastIndex]) {
                    append = true;
                }

                if (append) {
                    auto tmp(compositeFactors[i]);
                    ull mul = composites[i] * prime;
                    if(mul < Mmax) {
                        add = true;
                        tmp.push_back(prime);
                        compositeFactors.push_back(tmp);
                        composites.push_back(mul);
                        if(mul % 2 == 0) {
                            if(isPrime(mul + 1))
                                candidates.push_back(mul);
                        }
                    }
                }
            }
        }

        if(!add)
            break;
        oldSize = size;
    }

    //print_factors(compositeFactors, composites);
    //print_factors2(compositeFactors, composites);
    //print_candidates(candidates);

    return candidates;
}

ull overflowSafeMultiplication(ull x, ull y, const ull modulo){
    unsigned long long res = 0;
    while (y) {
        if (y & 1)
            res = (res + x) % modulo;

        x = (2 * x) % modulo;

        y >>= 1;
    }
    return res;
}

ull modExp(ull base, ull power, ull modulo) {
    if (base == 0) return 0;

    ull result = 1;

    base %= modulo;

    while (power > 0) {
        if (power & 1) {
            if(base < modulo) {
                if(base > std::numeric_limits<ull>::max() / result) {
                    //overflow
                    result = overflowSafeMultiplication(result, base, modulo);
                }
                else
                    result = (result * base) % modulo;

            }
            else
                result = ((result % modulo) * (base % modulo)) % modulo;
        }

        power = power >> 1;

        if(base < modulo) {
            if(base > std::numeric_limits<ull>::max() / base) {
                //overflow
                base = overflowSafeMultiplication(base, base, modulo);
            }
            else
                base = (base * base) % modulo;
        }
        else
            base = ((base % modulo) * (base % modulo)) % modulo;

    }

    return result;
}

int primitiveRootTest(const std::vector<ull> &candidates, const std::vector<int> &primes) {
    int maxR(0);
    for(ull m_1: candidates) {

        for (int r = 2; ; r++) {
            bool found = false;

            for (int prime: primes) {
                if (m_1 % prime == 0) {

                    if (modExp(r, m_1 / prime, m_1 + 1) == 1) {
                        found = true;
                        break;
                    }

                }
            }
            if(!found) {
                if(r > maxR)
                    maxR = r;

                break;
            }
        }
    }

    return maxR;
}


void solution() {
    ull Mmax;
    int D;
    scanf("%llu %d", &Mmax, &D);

    std::vector<int> primes;
    primeNumber(D, primes);

    auto candidates (compositesGenerator(primes, Mmax));

    int maxR(primitiveRootTest(candidates, primes));

    printf("%zu %d", candidates.size(), maxR);

}

int main() {
    clock_t start, end;
    start = clock();

    solution();

    end = clock();

    //printf("\nExecution time: %f", (end - start) / 1000.);

    return 0;
}
