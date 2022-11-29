#include <cstdio>
#include <ctime>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <cstring>

typedef unsigned long long ull;

void print_factors(std::vector<std::vector<int>> &compositeFactors, std::vector<ull> composites) {
    int it = 0;
    for(auto fac: compositeFactors) {
        it++;
        printf("%d) ", it);
        for (int i = fac.size() - 1; i >= 0; i--) {
            printf("%d", fac[i]);
        }
        printf(" = %llu\n", composites[it - 1]);
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
                printf("%d", fac[i]);
            }

            printf(" = %llu\n", composites[it - 1]);

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


std::vector<bool> primeNumber(int range, std::vector<int> &rangePrimes) {
    std::vector<bool> primes(range + 1, true);

    for(int p = 2; p <= range; p++) {
        if(primes[p]) {
            for(int i = p * p; i < range; i += p) {
                primes[i] = false;
            }
            rangePrimes.emplace_back(p);
        }
    }

    return primes;
}

bool isPrime(ull n) {
    for(int i = 2; i <= n / i; i++)
        if(n % i == 0)
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
            compositeFactors.push_back(tmp);
            composites.push_back(primes[j] * primes[i]);
            if(primes[i] == 2) {
                candidates.emplace_back(primes[j] * primes[i]);
            }
        }
    }

    int oldSize(0);
    while (true){
        bool add(false);
        int size(compositeFactors.size());
        for (int k = 0; k < primes.size(); k++) {
            bool append(false);
            int lastIndex(compositeFactors[oldSize].size() - 1);
            
            for (int i = oldSize; i < size; i++) {
                if (primes[k] == compositeFactors[i][lastIndex]) {
                    append = true;
                }

                if (append) {
                    auto tmp(compositeFactors[i]);
                    ull mul = composites[i] * primes[k];
                    if(mul <= Mmax) {
                        add = true;
                        tmp.push_back(primes[k]);
                        compositeFactors.push_back(tmp);
                        composites.push_back(mul);
                        if(mul % 2 == 0) {
                            if(isPrime(mul + 1))
                                candidates.push_back(mul + 1);
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


void solution() {
    ull Mmax;
    int D;
    scanf("%llu %d", &Mmax, &D);

    std::vector<int> rangePrimes;
    std::vector<bool> primes(primeNumber(D, rangePrimes));

    auto candidates (compositesGenerator(rangePrimes, Mmax));
    //print_candidates(candidates);



    //printf("%d", candidates.size());

}

int main() {
    clock_t start, end;
    start = clock();

    solution();

    end = clock();

    printf("\nExecution time: %f", (end - start) / 1000.);

    return 0;
}
