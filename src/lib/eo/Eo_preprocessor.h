#ifndef EO_PREPROCESSOR_H
#define EO_PREPROCESSOR_H

#include "Eo_foreach.h"
#include "Eo_foreach_inner.h"

#define EO_PREPROCESSOR_VARIADIC_SIZE(...) EO_PREPROCESSOR_VARIADIC_SIZE_I(__VA_ARGS__, 100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,)
#define EO_PREPROCESSOR_VARIADIC_SIZE_I(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, A38, A39, A40, A41, A42, A43, A44, A45, A46, A47, A48, A49, A50, A51, A52, A53, A54, A55, A56, A57, A58, A59, A60, A61, A62, A63, A64, A65, A66, A67, A68, A69, A70, A71, A72, A73, A74, A75, A76, A77, A78, A79, A80, A81, A82, A83, A84, A85, A86, A87, A88, A89, A90, A91, A92, A93, A94, A95, A96, A97, A98, A99, size, ...) size

//#define EO_PREPROCESSOR_VARIADIC_SIZE(...) EO_PREPROCESSOR_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
//#define EO_PREPROCESSOR_VARIADIC_SIZE_I(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, A38, A39, A40, A41, A42, A43, A44, A45, A46, A47, A48, A49, A50, A51, A52, A53, A54, A55, A56, A57, A58, A59, A60, A61, A62, A63, size, ...) size

#define EO_PREPROCESSOR_CONCAT(a, b) EO_PREPROCESSOR_CONCAT_I(a, b)
#define EO_PREPROCESSOR_CONCAT_I(a, b) a ## b

#define EO_PREPROCESSOR_DEC(I) EO_PREPROCESSOR_CONCAT(EO_PREPROCESSOR_DEC_, I)

#define EO_PREPROCESSOR_DEC_0 0
#define EO_PREPROCESSOR_DEC_1 0
#define EO_PREPROCESSOR_DEC_2 1
#define EO_PREPROCESSOR_DEC_3 2
#define EO_PREPROCESSOR_DEC_4 3
#define EO_PREPROCESSOR_DEC_5 4
#define EO_PREPROCESSOR_DEC_6 5
#define EO_PREPROCESSOR_DEC_7 6
#define EO_PREPROCESSOR_DEC_8 7
#define EO_PREPROCESSOR_DEC_9 8
#define EO_PREPROCESSOR_DEC_10 9
#define EO_PREPROCESSOR_DEC_11 10
#define EO_PREPROCESSOR_DEC_12 11
#define EO_PREPROCESSOR_DEC_13 12
#define EO_PREPROCESSOR_DEC_14 13
#define EO_PREPROCESSOR_DEC_15 14
#define EO_PREPROCESSOR_DEC_16 15

#define EO_PREPROCESSOR_IF(I, A, B) EO_PREPROCESSOR_CONCAT(EO_PREPROCESSOR_IF_, I)(A, B)
#define EO_PREPROCESSOR_IF_0(A, B) B
#define EO_PREPROCESSOR_IF_1(A, B) A
#define EO_PREPROCESSOR_IF_2(A, B) A
#define EO_PREPROCESSOR_IF_3(A, B) A
#define EO_PREPROCESSOR_IF_4(A, B) A
#define EO_PREPROCESSOR_IF_5(A, B) A
#define EO_PREPROCESSOR_IF_6(A, B) A
#define EO_PREPROCESSOR_IF_7(A, B) A
#define EO_PREPROCESSOR_IF_8(A, B) A
#define EO_PREPROCESSOR_IF_9(A, B) A
#define EO_PREPROCESSOR_IF_10(A, B) A
#define EO_PREPROCESSOR_IF_11(A, B) A
#define EO_PREPROCESSOR_IF_12(A, B) A

#define EO_PREPROCESSOR_EMPTY()
#define EO_PREPROCESSOR_COMMA() ,
#define EO_PREPROCESSOR_COMMA_IF(I) EO_PREPROCESSOR_IF(I, EO_PREPROCESSOR_COMMA, EO_PREPROCESSOR_EMPTY)()

#define EO_PREPROCESSOR_POP_FRONT_1(A0) ()
#define EO_PREPROCESSOR_POP_FRONT_2(A0, A1) (A1)
#define EO_PREPROCESSOR_POP_FRONT_3(A0, A1, A2) (A1, A2)
#define EO_PREPROCESSOR_POP_FRONT_4(A0, A1, A2, A3) (A1, A2, A3)
#define EO_PREPROCESSOR_POP_FRONT_5(A0, A1, A2, A3, A4) (A1, A2, A3, A4)
#define EO_PREPROCESSOR_POP_FRONT_6(A0, A1, A2, A3, A4, A5) (A1, A2, A3, A4, A5)
#define EO_PREPROCESSOR_POP_FRONT_7(A0, A1, A2, A3, A4, A5, A6) (A1, A2, A3, A4, A5, A6)
#define EO_PREPROCESSOR_POP_FRONT_8(A0, A1, A2, A3, A4, A5, A6, A7) (A1, A2, A3, A4, A5, A6, A7)
#define EO_PREPROCESSOR_POP_FRONT_9(A0, A1, A2, A3, A4, A5, A6, A7, A8) (A1, A2, A3, A4, A5, A6, A7, A8)
#define EO_PREPROCESSOR_POP_FRONT_10(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) (A1, A2, A3, A4, A5, A6, A7, A8, A9)
#define EO_PREPROCESSOR_POP_FRONT_11(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)
#define EO_PREPROCESSOR_POP_FRONT_12(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11)
#define EO_PREPROCESSOR_POP_FRONT_13(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12)
#define EO_PREPROCESSOR_POP_FRONT(I, ...) EO_PREPROCESSOR_CONCAT(EO_PREPROCESSOR_POP_FRONT_, EO_PREPROCESSOR_VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)

// #define EO_PREPROCESSOR_FOR_EACH_0(MACRO, X)
// #define EO_PREPROCESSOR_FOR_EACH_1(MACRO, A0) MACRO(0, A0)
// #define EO_PREPROCESSOR_FOR_EACH_2(MACRO, A0, A1) MACRO(0, A0) MACRO(1, A1)
// #define EO_PREPROCESSOR_FOR_EACH_3(MACRO, A0, A1, A2) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2)
// #define EO_PREPROCESSOR_FOR_EACH_4(MACRO, A0, A1, A2, A3) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3)
// #define EO_PREPROCESSOR_FOR_EACH_5(MACRO, A0, A1, A2, A3, A4) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4)
// #define EO_PREPROCESSOR_FOR_EACH_6(MACRO, A0, A1, A2, A3, A4, A5) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5)
// #define EO_PREPROCESSOR_FOR_EACH_7(MACRO, A0, A1, A2, A3, A4, A5, A6) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6)
// #define EO_PREPROCESSOR_FOR_EACH_8(MACRO, A0, A1, A2, A3, A4, A5, A6, A7) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7)
// #define EO_PREPROCESSOR_FOR_EACH_9(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8)
// #define EO_PREPROCESSOR_FOR_EACH_10(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8) MACRO(9, A9)
// #define EO_PREPROCESSOR_FOR_EACH_11(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8) MACRO(9, A9) MACRO(10, A10)


#define EO_PREPROCESSOR_FOR_EACH(MACRO, ...) EO_PREPROCESSOR_CONCAT(EO_PREPROCESSOR_FOR_EACH_, EO_PREPROCESSOR_VARIADIC_SIZE(__VA_ARGS__)) (MACRO, __VA_ARGS__)

// #define EO_PREPROCESSOR_FOR_EACH_INNER_0(MACRO, X)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_1(MACRO, A0) MACRO(0, A0)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_2(MACRO, A0, A1) MACRO(0, A0) MACRO(1, A1)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_3(MACRO, A0, A1, A2) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_4(MACRO, A0, A1, A2, A3) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_5(MACRO, A0, A1, A2, A3, A4) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_6(MACRO, A0, A1, A2, A3, A4, A5) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_7(MACRO, A0, A1, A2, A3, A4, A5, A6) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_8(MACRO, A0, A1, A2, A3, A4, A5, A6, A7) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_9(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_10(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8) MACRO(9, A9)
// #define EO_PREPROCESSOR_FOR_EACH_INNER_11(MACRO, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) MACRO(0, A0) MACRO(1, A1) MACRO(2, A2) MACRO(3, A3) MACRO(4, A4) MACRO(5, A5) MACRO(6, A6) MACRO(7, A7) MACRO(8, A8) MACRO(9, A9) MACRO(10, A10)

#define EO_PREPROCESSOR_FOR_EACH_INNER(MACRO, ...) EO_PREPROCESSOR_CONCAT(EO_PREPROCESSOR_FOR_EACH_INNER_, EO_PREPROCESSOR_VARIADIC_SIZE(__VA_ARGS__)) (MACRO, __VA_ARGS__)

#define EO_PREPROCESSOR_LP() (
#define EO_PREPROCESSOR_RP() )

#endif
