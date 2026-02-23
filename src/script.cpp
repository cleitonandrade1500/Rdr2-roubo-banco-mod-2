#include "script.h"
#include "natives.h"

/* 
   MOD: John Marston Bodyguard (v1.32 CUSA03041)
   COMANDO: Segurar L2 + SETA CIMA por 2 segundos
*/

Ped johnPed = 0;
int timerSegurando = 0;

void GerenciarIAJohn() {
    Ped player = PLAYER::PLAYER_PED_ID();

    // 1. MONTARIA: John sobe no cavalo se você montar
    if (PED::IS_PED_ON_MOUNT(player) && !PED::IS_PED_ON_MOUNT(johnPed)) {
        Ped cavalo = PED::GET_MOUNT(player);
        if (ENTITY::DOES_ENTITY_EXIST(cavalo)) {
            TASK::TASK_MOUNT_ANIMAL(johnPed, cavalo, -1, -1, 2.0f, 1, 0, 0);
        }
    }

    // 2. SAQUE: John saqueia corpos se não houver combate por perto
    if (!PED::IS_PED_IN_COMBAT(johnPed, 0) && !PED::IS_PED_ON_MOUNT(player)) {
        Ped corpo = PED::GET_CLOSEST_PED(johnPed, 10.0f, 1, 0, 0, 0);
        if (ENTITY::DOES_ENTITY_EXIST(corpo) && PED::IS_PED_DEAD_OR_DYING(corpo, true)) {
            TASK::TASK_LOOT_PED(johnPed, corpo);
        }
    }
}

void scriptMain() {
    while (true) {
        // Comando: L2 (INPUT_CONTEXT) + SETA CIMA (INPUT_FRONTEND_UP)
        if (PAD::IS_CONTROL_PRESSED(0, INPUT_CONTEXT) && PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_UP)) {
            timerSegurando++;
            
            if (timerSegurando > 150) { // ~2 Segundos
                Hash johnHash = MISC::GET_HASH_KEY("PLAYER_THREE"); // John Marston

                STREAMING::REQUEST_MODEL(johnHash, false);
                while (!STREAMING::HAS_MODEL_LOADED(johnHash)) {
                    WAIT(0); // Essencial no Havana para não travar o console
                }

                Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
                johnPed = PED::CREATE_PED(johnHash, pos.x + 2.0f, pos.y, pos.z, 0.0f, false, false, false, false);

                // Configuração de Guarda-Costa Oficial
                int group = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID());
                PED::SET_PED_AS_GROUP_MEMBER(johnPed, group);
                ENTITY::SET_ENTITY_INVINCIBLE(johnPed, true); // John Imortal
                
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(johnHash);
                timerSegurando = 0;
            }
        } else {
            timerSegurando = 0;
        }

        if (ENTITY::DOES_ENTITY_EXIST(johnPed)) {
            GerenciarIAJohn();
        }

        WAIT(0); // Sincroniza o mod com o motor do RDR2
    }
}
