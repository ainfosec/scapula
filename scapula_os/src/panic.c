#include "print.h"
#include "panic.h"

void panic(void)
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
    while(1);
}
