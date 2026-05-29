#include <stddef.h>
#include <stdint.h>

extern int __atomic_compare_exchange_1(void *ptr, void *expected, void *desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_2(void *ptr, void *expected, void *desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_4(void *ptr, void *expected, void *desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_8(void *ptr, void *expected, void *desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_16(void *ptr, void *expected, void *desired, int weak, int success, int failure);

int __atomic_compare_exchange(size_t size, void *ptr, void *expected, void *desired, int weak, int success, int failure)
{
    switch (size)
    {
        case 1:  return __atomic_compare_exchange_1(ptr, expected, desired, weak, success, failure);
        case 2:  return __atomic_compare_exchange_2(ptr, expected, desired, weak, success, failure);
        case 4:  return __atomic_compare_exchange_4(ptr, expected, desired, weak, success, failure);
        case 8:  return __atomic_compare_exchange_8(ptr, expected, desired, weak, success, failure);
        case 16: return __atomic_compare_exchange_16(ptr, expected, desired, weak, success, failure);
        default: __builtin_unreachable();
    }
    return 0;
}
