#include <stdint.h>

// Endereços (Hashes) da versão 1.32
#define INVOKE(h, ...) ((T(*)(...))h)(__VA_ARGS__)

int john = 0;
int timer = 0;

void LógicaIA(int ped, int player) {
    // 1. Montar se o player montar
    if (INVOKE<bool>(0x4600003E, player) && !INVOKE<bool>(0x4600003E, ped)) {
        int cavalo = INVOKE<int>(0xE7E11B8E, player);
        if (cavalo) INVOKE<void>(0x92DB31E0, ped, cavalo, -1, -1, 2.0f, 1, 0, 0);
    }
    // 2. Saquear se estiver livre
    if (!INVOKE<bool>(0x8434317BF5E90C05, ped) && !INVOKE<bool>(0x4600003E, player)) {
        int corpo = INVOKE<int>(0x14E56BC5B50A07C5, ped, 15.0f);
        if (corpo) INVOKE<void>(0xE13262B50A07C5, ped, corpo);
    }
}

extern "C" int module_start() {
    while (true) {
        // Comando: L2 + Seta Cima por 2 seg
        if (INVOKE<bool>(0x0E0313B0, 0, 0x07CEABE9) && INVOKE<bool>(0x0E0313B0, 0, 0x911CB91D)) {
            if (++timer > 100) {
                INVOKE<void>(0x125FD99564264624, 0xD8093262, false);
                john = INVOKE<int>(0xD49F340656402010, 0xD8093262, 0,0,0,0,0,0,0,0);
                INVOKE<void>(0x10113106, john, INVOKE<int>(0x256B38647614A69D, INVOKE<int>(0x2190E381D3930472)));
                timer = 0;
            }
        } else timer = 0;

        if (john) LógicaIA(john, INVOKE<int>(0x2190E381D3930472));
    }
    return 0;
}
