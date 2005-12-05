#ifndef LIBUCD_COMPILER_H
#define LIBUCD_COMPILER_H

#if defined(__GNUC__)

#define ALIGNED(x) __attribute__((aligned(x)))
#define noreturn void __attribute__((noreturn))

#else

#define ALIGNED(x)
#define noreturn void

#endif

#endif /* LIBUCD_COMPILER_H */
