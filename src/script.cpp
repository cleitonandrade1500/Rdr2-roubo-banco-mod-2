#include <stdint.h>

// --- DEFINIÇÕES INTERNAS (Substitui o script.h e natives.h) ---
typedef uint32_t Hash;
struct Vector3 { float x, y, z; };
typedef int Ped;

#define INVOKE(h, T, ...) ((T(*)(...))h)(__VA_ARGS__)

// Variáveis de Controle
Ped johnPed = 0;
int timer = 0;

// Função principal de lógica
void scriptMain() {
    while (true) {
        // Comando: L2 (0x07CEABE9) + SETA CIMA (0x911CB91D)
        if (INVOKE(0x0E0313B0, bool, 0, 0x07CEABE9) && INVOKE(0x0E0313B0, bool, 0, 0x911CB91D)) {
            if (++timer > 150) {
                Hash johnHash = 0xD8093262; // PLAYER_THREE
                
                // Request Model
                INVOKE(0x125FD99564264624, void, johnHash, false);
                
                // Spawn John
                johnPed = INVOKE(0xD49F340656402010, Ped, johnHash, 0, 0, 0, 0.0f, false, false, false, false);
                
                // Set Bodyguard (Grupo do Player)
                int pPed = INVOKE(0x2190E381D3930472, Ped);
                int group = INVOKE(0x256B38647614A69D, int, pPed);
                INVOKE(0x10113106, void, johnPed, group);
                
                timer = 0;
            }
        } else { timer = 0; }

        if (johnPed != 0) {
            // IA de Montaria
            if (INVOKE(0x4600003E, bool, INVOKE(0x2190E381D3930472, Ped))) {
                int cav = INVOKE(0xE7E11B8E, int, INVOKE(0x2190E381D3930472, Ped));
                if (cav) INVOKE(0x92DB31E0, void, johnPed, cav, -1, -1, 2.0f, 1, 0, 0);
            }
        }
        // Pequena pausa para o PS4 respirar (FW 12.00)
        for(volatile int i=0; i<500000; i++);
    }
}
