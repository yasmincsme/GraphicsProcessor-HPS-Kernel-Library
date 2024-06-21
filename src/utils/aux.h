#ifndef AUX_H_
#define AUX_H_

#include "types.h"  // Include necessary for u32_t type definition

/**
 * @brief Reverses a string.
 *
 * This function reverses the characters in the given string `str` of length `length`.
 *
 * @param str The string to be reversed.
 * @param length The length of the string.
 */
void reverse(char str[], u32_t length);

/**
 * @brief Converts an unsigned integer to a string representation in a specified base.
 *
 * This function converts the unsigned integer `num` to a string `str` in the given `base`.
 * The base must be between 2 and 36 (inclusive). The function returns a pointer to `str`.
 *
 * @param num The unsigned integer to be converted.
 * @param str The buffer to store the resulting string.
 * @param base The base for conversion (must be between 2 and 36).
 * @return A pointer to the resulting string `str`.
 */
char* itoa(u32_t num, char* str, u32_t base);

/**
 * @brief Clamps a value within a specified range.
 *
 * This function ensures that the given `value` is within the inclusive range specified by `min` and `max`.
 * If `value` is less than `min`, it returns `min`. If `value` is greater than `max`, it returns `max`.
 * Otherwise, it returns `value` unchanged.
 *
 * @param value The value to be clamped.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @return The clamped value.
 */
int clamp(int value, int min, int max);

#endif  // !AUX_H_
