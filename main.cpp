#include <stdint.h>
#define RDR2_132_CUSA03041 // Define a versão para as offsets

// Simulação de headers necessários para o exemplo
typedef uint32_t Hash;
struct Vector3 { float x, y, z; };

// Função Principal de Spawn
void SpawnJohnBodyguard() {
    Hash johnHash = 0xD8093262; // PLAYER_THREE (John Marston)

    // Solicitar Modelo
    if (Native::STREAMING::IS_MODEL_IN_CDIMAGE(johnHash)) {
        Native::STREAMING::REQUEST_MODEL(johnHash, false);
        
        // Esperar carregar (simplificado para o post-build)
        while (!Native::STREAMING::HAS_MODEL_LOADED(johnHash)) {
            // Loop de espera nativo do PS4
        }

        // Pegar Coordenadas do Jogador
        int playerPed = Native::PLAYER::PLAYER_PED_ID();
        Vector3 pos = Native::ENTITY::GET_ENTITY_COORDS(playerPed, true, true);

        // Criar o Ped
        int john = Native::PED::CREATE_PED(johnHash, pos.x + 3.0f, pos.y, pos.z, 0.0f, false, false, false, false);
        
        // Configurar Guarda-costas
        int group = Native::PLAYER::GET_PLAYER_GROUP(Native::PLAYER::PLAYER_ID());
        Native::PED::SET_PED_AS_GROUP_MEMBER(john, group);
        Native::ENTITY::SET_ENTITY_INVINCIBLE(john, true);
        
        Native::STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(johnHash);
    }
}

// Ponto de entrada do Mod
extern "C" int module_start() {
    // Lógica para detectar botão (ex: L1 + R1) e chamar SpawnJohnBodyguard()
    return 0;
}
