#ifndef _BOOTLOADER_PRINT_H_
#define _BOOTLOADER_PRINT_H_

extern void printf_(char * fmt, ...);

#define BOOTLOADER_PRINT(X, ...) printf_((X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_INFO(X, ...) printf_(("[BOOTLOADER] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_SUBINFO(X, ...) printf_(("[BOOTLOADER]     " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_DEBUG(X, ...) printf_(("[BOOTLOADER DEBUG] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_ALERT(X, ...) printf_(("[BOOTLOADER ALERT] " X"\n"), ##__VA_ARGS__)
#define BOOTLOADER_ERROR(X, ...) printf_(("[BOOTLOADER ERROR] " X"\n"), ##__VA_ARGS__)

#define VECTOR_PRINT(X, ...) printf_(("[INTERRUPT_VECTOR] --> " X"\n"), ##__VA_ARGS__)

#endif
