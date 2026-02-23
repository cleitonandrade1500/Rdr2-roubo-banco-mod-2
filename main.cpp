#include <stdint.h>

// Macros de Invocação para RDR2 v1.32
#define INVOKE(h, T, ...) ((T(*)(...))h)(__VA_ARGS__)

// Variáveis de Estado
int johnPed = 0;
int timer = 0;

void LoopIA(int john, int player) {
    // Lógica de Montaria
    if (INVOKE(0x4600003E, bool, player) && !INVOKE(0x4600003E, bool, john)) {
        int cav = INVOKE(0xE7E11B8E, int, player);
        if (cav) INVOKE(0x92DB31E0, void, john, cav, -1, -1, 2.0f, 1, 0, 0);
    }
    // Lógica de Saque
    if (!INVOKE(0x8434317BF5E90C05, bool, john) && !INVOKE(0x4600003E, bool, player)) {
        int corpo = INVOKE(0x14E56BC5B50A07C5, int, john, 15.0f);
        if (corpo) INVOKE(0xE13262B50A07C5, void, john, corpo);
    }
}

// Função que o GoldHEN executa continuamente
extern "C" __attribute__((visibility("default"))) int module_start() {
    // Loop infinito seguro para Plugins GoldHEN
    while (true) {
        // L2 + Seta Cima por 2 Segundos
        if (INVOKE(0x0E0313B0, bool, 0, 0x07CEABE9) && INVOKE(0x0E0313B0, bool, 0, 0x911CB91D)) {
            if (++timer > 200) { // Ajustado para FW 12.00
                INVOKE(0x125FD99564264624, void, 0xD8093262, false); // John Hash
                johnPed = INVOKE(0xD49F340656402010, int, 0xD8093262, 0,0,0,0,0,0,0,0);
                int grp = INVOKE(0x256B38647614A69D, int, INVOKE(0x2190E381D3930472, int));
                INVOKE(0x10113106, void, johnPed, grp);
                timer = 0;
            }
        } else { timer = 0; }

        if (johnPed) LoopIA(johnPed, INVOKE(0x2190E381D3930472, int));

        // Delay manual para não causar Kernel Panic na FW 12.00
        for(volatile int i=0; i<500000; i++); 
    }
    return 0;
}

extern "C" __attribute__((visibility("default"))) int module_stop() { return 0; }
