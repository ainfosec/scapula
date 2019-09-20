#ifndef SCAPULA_OS_PRINT_H
#define SCAPULA_OS_PRINT_H

extern void printf_(char * fmt, ...);

#define SCAPULA_PRINT(X, ...) printf_((X"\n"), ##__VA_ARGS__)

#if LOG_LEVEL>=1
#define SCAPULA_ERROR(X, ...) printf_(("[SCAPULA ERROR] " X"\n"), ##__VA_ARGS__)
#else
#define SCAPULA_ERROR(X, ...)
#endif

#if LOG_LEVEL>=2
#define SCAPULA_ALERT(X, ...) printf_(("[SCAPULA ALERT] " X"\n"), ##__VA_ARGS__)
#else
#define SCAPULA_ALERT(X, ...)
#endif

#if LOG_LEVEL>=3
#define SCAPULA_INFO(X, ...) printf_(("[SCAPULA] " X"\n"), ##__VA_ARGS__)
#define SCAPULA_SUBINFO(X, ...) printf_(("[SCAPULA]     " X"\n"), ##__VA_ARGS__)
#else
#define SCAPULA_INFO(X, ...)
#define SCAPULA_SUBINFO(X, ...)
#endif

#if LOG_LEVEL>=4
#define SCAPULA_DEBUG(X, ...) printf_(("[SCAPULA] " X"\n"), ##__VA_ARGS__)
#define VECTOR_PRINT(X, ...) printf_(("[INTERRUPT_VECTOR] --> " X"\n"), ##__VA_ARGS__)
#else
#define SCAPULA_DEBUG(X, ...)
#define VECTOR_PRINT(X, ...)
#endif

void print_banner(void);
void print_success(void);

#endif
