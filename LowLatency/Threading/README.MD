# GCC - COMPILER OPTIMIZATIONS USED

**#pragma once**
The #pragma once directive has a very simple concept. The header file containing this directive is included only once even if the programmer includes it multiple times during a compilation. This directive works similar to the #include guard idiom. Use of #pragma once saves the program from multiple inclusion optimisation.


**Built-in Function: long __builtin_expect (long exp, long c)**
You may use __builtin_expect to provide the compiler with branch prediction information. In general, you should prefer to use actual profile feedback for this (-fprofile-arcs), as programmers are notoriously bad at predicting how their programs actually perform. 

For the purposes of branch prediction optimizations, the probability that a __builtin_expect expression is true is controlled by GCC’s builtin-expect-probability parameter, which defaults to 90%.

You can also use __builtin_expect_with_probability to explicitly assign a probability value to individual expressions. If the built-in is used in a loop construct, the provided probability will influence the expected number of iterations made by loop optimizations.

[Read More](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)