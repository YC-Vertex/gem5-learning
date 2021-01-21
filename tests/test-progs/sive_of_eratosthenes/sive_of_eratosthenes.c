#include <stdio.h>

#define false 0
#define true 1

int main() {
    const int MAX = 100000;
    const int SEARCH_MAX = 100;

    int is_prime[MAX+5];
    for (int i = 2; i <= MAX; ++i)
        is_prime[i] = true;

    for (int i = 2; i <= SEARCH_MAX; ++i) {
        if (!is_prime[i])
            continue;
        for (int num = i * i; num <= MAX; num += i)
            is_prime[num] = false;
    }

    int prime_count = 0;
    for (int i = 2; i <= MAX; ++i) {
        if (is_prime[i])
            ++prime_count;
    }

    printf("%d\n", prime_count);

    return 0;
}
