#include <stdint.h>

// Macro para invocar comandos do jogo de forma limpa
#define INVOKE(h, T, ...) ((T(*)(...))h)(__VA_ARGS__)

// Variáveis Globais
int johnPed = 0;
int timerBotao = 0;

void LogicaIA(int ped, int player) {
    // 1. Montaria: Se o player montar, John monta na garupa ou no dele
    if (INVOKE(0x4600003E, bool, player) && !INVOKE(0x4600003E, bool, ped)) {
        int cavalo = INVOKE(0xE7E11B8E, int, player);
        if (cavalo) INVOKE(0x92DB31E0, void, ped, cavalo, -1, -1, 2.0f, 1, 0, 0);
    }
    // 2. Saque: Se não estiver em combate e player estiver a pé, John saqueia
    if (!INVOKE(0x8434317BF5E90C05, bool, ped) && !INVOKE(0x4600003E, bool, player)) {
        int corpo = INVOKE(0x14E56BC5B50A07C5, int, ped, 15.0f);
        if (corpo) INVOKE(0xE13262B50A07C5, void, ped, corpo);
    }
}

extern "C" int module_start() {
    while (true) {
        // Comando: Segurar L2 + Seta Cima por ~2 seg
        if (INVOKE(0x0E0313B0, bool, 0, 0x07CEABE9) && INVOKE(0x0E0313B0, bool, 0, 0x911CB91D)) {
            if (++timerBotao > 100) {
                // Spawn John Marston (PLAYER_THREE)
                INVOKE(0x125FD99564264624, void, 0xD8093262, false); // Request Model
                johnPed = INVOKE(0xD49F340656402010, int, 0xD8093262, 0, 0, 0, 0.0f, false, false, false, false);
                
                // Set as Bodyguard
                int group = INVOKE(0x256B38647614A69D, int, INVOKE(0x2190E381D3930472, int));
                INVOKE(0x10113106, void, johnPed, group);
                
                timerBotao = 0;
            }
        } else { timerBotao = 0; }

        if (johnPed) LogicaIA(johnPed, INVOKE(0x2190E381D3930472, int));
    }
    return 0;
}

extern "C" int module_stop() { return 0; }
