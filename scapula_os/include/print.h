#ifndef SCAPULA_OS_PRINT_H
#define SCAPULA_OS_PRINT_H

extern void printf_(char * fmt, ...);

#define SCAPULA_PRINT(X, ...) printf_((X"\n"), ##__VA_ARGS__)
#define SCAPULA_INFO(X, ...) printf_(("[SCAPULA] " X"\n"), ##__VA_ARGS__)
#define SCAPULA_SUBINFO(X, ...) printf_(("[SCAPULA]     " X"\n"), ##__VA_ARGS__)
#define SCAPULA_DEBUG(X, ...) printf_(("[SCAPULA DEBUG] " X"\n"), ##__VA_ARGS__)
#define SCAPULA_ALERT(X, ...) printf_(("[SCAPULA ALERT] " X"\n"), ##__VA_ARGS__)
#define SCAPULA_ERROR(X, ...) printf_(("[SCAPULA ERROR] " X"\n"), ##__VA_ARGS__)

#define VECTOR_PRINT(X, ...) printf_(("[INTERRUPT_VECTOR] --> " X"\n"), ##__VA_ARGS__)

inline void print_banner(void)
{
    SCAPULA_PRINT("---------------------------------------------------------------");
    SCAPULA_PRINT("   _____                       _          ____   _____ ");
    SCAPULA_PRINT("  / ____|                     | |        / __ \\ / ____|");
    SCAPULA_PRINT(" | (___   ___ __ _ _ __  _   _| | __ _  | |  | | (___  ");
    SCAPULA_PRINT("  \\___ \\ / __/ _` | '_ \\| | | | |/ _` | | |  | |\\___ \\ ");
    SCAPULA_PRINT("  ____) | (_| (_| | |_) | |_| | | (_| | | |__| |____) |");
    SCAPULA_PRINT(" |_____/ \\___\\__,_| .__/ \\__,_|_|\\__,_|  \\____/|_____/ ");
    SCAPULA_PRINT("                  | |                                  ");
    SCAPULA_PRINT("                  |_|                                  ");
    SCAPULA_PRINT("              ARM Testcase Runtime Environment");
    SCAPULA_PRINT("---------------------------------------------------------------");
}

inline void print_success(void)
{
    SCAPULA_PRINT("---------------------------------------------------------------");
    SCAPULA_PRINT(" _______           _______  _______  _______  _______  _______");
    SCAPULA_PRINT("(  ____ \\|\\     /|(  ____ \\(  ____ \\(  ____ \\(  ____ \\(  ____ \\");
    SCAPULA_PRINT("| (    \\/| )   ( || (    \\/| (    \\/| (    \\/| (    \\/| (    \\/");
    SCAPULA_PRINT("| (_____ | |   | || |      | |      | (__    | (_____ | (_____");
    SCAPULA_PRINT("(_____  )| |   | || |      | |      |  __)   (_____  )(_____  )");
    SCAPULA_PRINT("      ) || |   | || |      | |      | (            ) |      ) |");
    SCAPULA_PRINT("/\\____) || (___) || (____/\\| (____/\\| (____/\\/\\____) |/\\____) |");
    SCAPULA_PRINT("\\_______)(_______)(_______/(_______/(_______/\\_______)\\_______)");
    SCAPULA_PRINT("              Scapula OS exited successfully");
    SCAPULA_PRINT("---------------------------------------------------------------");
}

inline void print_panic(void)
{
    SCAPULA_PRINT("--------------------------------------------------------");
    SCAPULA_PRINT("|\\     /||\\     /|       (  ___  )|\\     /|       / )");
    SCAPULA_PRINT("| )   ( || )   ( |       | (   ) || )   ( |   _  / /");
    SCAPULA_PRINT("| |   | || (___) | _____ | |   | || (___) |  (_)( (");
    SCAPULA_PRINT("| |   | ||  ___  |(_____)| |   | ||  ___  |     | |");
    SCAPULA_PRINT("| |   | || (   ) |       | |   | || (   ) |   _ ( (");
    SCAPULA_PRINT("| (___) || )   ( |       | (___) || )   ( |  (_) \\ \\");
    SCAPULA_PRINT("(_______)|/     \\|       (_______)|/     \\|       \\_)");
    SCAPULA_PRINT("          Scapula OS terminated unexpectedly");
    SCAPULA_PRINT("--------------------------------------------------------");
}

#endif
