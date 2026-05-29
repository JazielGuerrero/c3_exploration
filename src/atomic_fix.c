#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern int __atomic_compare_exchange_1(void *ptr, void *expected, uint8_t desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_2(void *ptr, void *expected, uint16_t desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_4(void *ptr, void *expected, uint32_t desired, int weak, int success, int failure);
extern int __atomic_compare_exchange_8(void *ptr, void *expected, uint64_t desired, int weak, int success, int failure);

extern int __atomic_compare_exchange_16(void *ptr, void *expected, uint64_t desired_lo, uint64_t desired_hi, int success, int failure);

int __atomic_compare_exchange(size_t size, void *ptr, void *expected, void *desired, int weak, int success, int failure)
{
    switch (size)
    {
        case 1: {
            uint8_t val;
            memcpy(&val, desired, 1);
            return __atomic_compare_exchange_1(ptr, expected, val, weak, success, failure);
        }
        case 2: {
            uint16_t val;
            memcpy(&val, desired, 2);
            return __atomic_compare_exchange_2(ptr, expected, val, weak, success, failure);
        }
        case 4: {
            uint32_t val;
            memcpy(&val, desired, 4);
            return __atomic_compare_exchange_4(ptr, expected, val, weak, success, failure);
        }
        case 8: {
            uint64_t val;
            memcpy(&val, desired, 8);
            return __atomic_compare_exchange_8(ptr, expected, val, weak, success, failure);
        }
        case 16: {
            uint64_t lo, hi;
            memcpy(&lo, desired, 8);
            memcpy(&hi, (const char*)desired + 8, 8);
            return __atomic_compare_exchange_16(ptr, expected, lo, hi, success, failure);
        }
        default: __builtin_unreachable();
    }
    return 0;
}
