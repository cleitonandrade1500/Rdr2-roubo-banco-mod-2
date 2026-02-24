#include "natives.h"
#include "script.h"
#include <stdint.h>

// Variáveis de Controle
Ped johnPed = 0;
int timerSegurando = 0;

void AtualizarIAJohn() {
    Ped player = PLAYER::PLAYER_PED_ID();
    // 1. IA DE MONTARIA: John sobe no cavalo se você montar
    if (PED::IS_PED_ON_MOUNT(player) && !PED::IS_PED_ON_MOUNT(johnPed)) {
        Ped cavalo = PED::GET_MOUNT(player);
        if (ENTITY::DOES_ENTITY_EXIST(cavalo)) {
            TASK::TASK_MOUNT_ANIMAL(johnPed, cavalo, -1, -1, 2.0f, 1, 0, 0);
        }
    }
    // 2. IA DE SAQUE: John saqueia corpos se não houver combate
    if (!PED::IS_PED_IN_COMBAT(johnPed, 0) && !PED::IS_PED_ON_MOUNT(player)) {
        Ped corpo = PED::GET_CLOSEST_PED(johnPed, 10.0f, 1, 0, 0, 0);
        if (ENTITY::DOES_ENTITY_EXIST(corpo) && PED::IS_PED_DEAD_OR_DYING(corpo, true)) {
            TASK::TASK_LOOT_PED(johnPed, corpo);
        }
    }
}

extern "C" __attribute__((visibility("default"))) int module_start(size_t argc, const void* argv) {
    while (true) {
        // Comando: L2 + SETA CIMA por 2 segundos
        if (PAD::IS_CONTROL_PRESSED(0, 0x07CEABE9) && PAD::IS_CONTROL_PRESSED(0, 0x911CB91D)) {
            if (++timerSegurando > 150) {
                Hash johnHash = MISC::GET_HASH_KEY("PLAYER_THREE");
                STREAMING::REQUEST_MODEL(johnHash, false);
                while (!STREAMING::HAS_MODEL_LOADED(johnHash)) WAIT(0);

                Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
                johnPed = PED::CREATE_PED(johnHash, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);
                
                PED::SET_PED_AS_GROUP_MEMBER(johnPed, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID()));
                ENTITY::SET_ENTITY_INVINCIBLE(johnPed, true);
                timerSegurando = 0;
            }
        } else timerSegurando = 0;

        if (ENTITY::DOES_ENTITY_EXIST(johnPed)) AtualizarIAJohn();
        WAIT(0);
    }
    return 0;
}
