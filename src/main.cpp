#include <stdint.h>
#include <stddef.h>

// --- MOTOR DE NATIVES RDR2 v1.32 CUSA03041 ---
#define INVOKE(h, T, ...) ((T(*)(...))h)(__VA_ARGS__)

// Variáveis de Controle
int johnPed = 0;
int timerBotao = 0;

void AtualizarIA(int john, int player) {
    // 1. Lógica de Montaria: John sobe no cavalo se você montar
    if (INVOKE(0x4600003E, bool, player) && !INVOKE(0x4600003E, bool, john)) {
        int cavalo = INVOKE(0xE7E11B8E, int, player);
        if (cavalo != 0) INVOKE(0x92DB31E0, void, john, cavalo, -1, -1, 2.0f, 1, 0, 0);
    }
    // 2. Lógica de Saque: Fora de combate, John saqueia corpos próximos
    if (!INVOKE(0x8434317BF5E90C05, bool, john) && !INVOKE(0x4600003E, bool, player)) {
        int corpo = INVOKE(0x14E56BC5B50A07C5, int, john, 10.0f);
        if (corpo != 0) INVOKE(0xE13262B50A07C5, void, john, corpo);
    }
}

// Ponto de entrada oficial para GoldHEN 2.4 / FW 12.00
extern "C" __attribute__((visibility("default"))) int module_start(size_t argc, const void* argv) {
    while (true) {
        // Comando: L2 (0x07CEABE9) + SETA CIMA (0x911CB91D) por 2 segundos
        if (INVOKE(0x0E0313B0, bool, 0, 0x07CEABE9) && INVOKE(0x0E0313B0, bool, 0, 0x911CB91D)) {
            if (++timerBotao > 150) { 
                uint32_t johnHash = 0xD8093262; // PLAYER_THREE (John)

                // Request e Spawn
                INVOKE(0x125FD99564264624, void, johnHash, false);
                int pPed = INVOKE(0x2190E381D3930472, int);
                johnPed = INVOKE(0xD49F340656402010, int, johnHash, 0, 0, 0, 0.0f, false, false, false, false);
                
                // Configurar como Guarda-costas
                int group = INVOKE(0x256B38647614A69D, int, pPed);
                INVOKE(0x10113106, void, johnPed, group);
                INVOKE(0x67C546AC, void, johnPed, true); // Invencível
                
                timerBotao = 0;
            }
        } else { timerBotao = 0; }

        if (johnPed != 0) {
            AtualizarIA(johnPed, INVOKE(0x2190E381D3930472, int));
        }

        // Delay para estabilidade na FW 12.00
        for(volatile int i=0; i<500000; i++); 
    }
    return 0;
}

extern "C" int module_stop(size_t argc, const void* argv) { return 0; }
