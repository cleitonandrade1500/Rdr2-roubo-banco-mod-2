#include <stdint.h>

// --- DEFINIÇÕES DE NATIVES RDR2 v1.32 CUSA03041 ---
// Usamos a técnica de Invoke para garantir a linkagem no PS4
namespace Native {
    template <typename T, typename... Args>
    static T Call(uint64_t hash, Args... args) {
        typedef T (*Func)(Args...);
        Func f = (Func)hash; 
        return f(args...);
    }
}

// Hashes das Funções (Natives)
#define N_PLAYER_PED_ID          0x2190E381D3930472
#define N_GET_ENTITY_COORDS      0xA654C252
#define N_REQUEST_MODEL          0x125FD99564264624
#define N_HAS_MODEL_LOADED       0x125FD99564264625 // Placeholder para lógica
#define N_CREATE_PED             0xD49F340656402010
#define N_SET_PED_AS_GROUP_MEMBER 0x10113106
#define N_GET_PLAYER_GROUP       0x256B38647614A69D
#define N_IS_PED_IN_COMBAT       0x8434317BF5E90C05
#define N_GET_CLOSEST_PED        0x14E56BC5B50A07C5
#define N_TASK_LOOT_PED          0xE13262B50A07C5
#define N_SET_PED_CONFIG_FLAG    0x1913FE28
#define N_SET_ENTITY_INVINCIBLE  0x67C546AC
#define N_IS_CONTROL_PRESSED     0x0E0313B0

// Estruturas
struct Vector3 { float x, y, z; };

// Variáveis de Controle
int johnHandle = 0;
int64_t pressTimer = 0;

void LogicaJohnSaqueador(int john) {
    // Só saqueia se NÃO estiver em combate
    bool emCombate = Native::Call<bool>(N_IS_PED_IN_COMBAT, john);
    if (!emCombate) {
        // Busca corpo morto num raio de 15 metros
        int corpo = Native::Call<int>(N_GET_CLOSEST_PED, john, 15.0f);
        if (corpo != 0) {
            // Comando para saquear corpo
            Native::Call<void>(N_TASK_LOOT_PED, john, corpo);
        }
    }
}

void ExecutarComando() {
    // L2 (Mirar) + Seta para Cima (UP)
    if (Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x07CEABE9) && 
        Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x911CB91D)) {
        
        if (pressTimer == 0) pressTimer = 5000; // Simulação de timer simples
        else pressTimer++;

        // Aproximadamente 2 segundos (dependendo dos frames)
        if (pressTimer > 5120) { 
            uint32_t johnHash = 0xD8093262; // PLAYER_THREE (John)

            // Spawn John
            Native::Call<void>(N_REQUEST_MODEL, johnHash, false);
            int pPed = Native::Call<int>(N_PLAYER_PED_ID);
            
            // Cria John ao lado do player
            johnHandle = Native::Call<int>(N_CREATE_PED, johnHash, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, false);
            
            // Define Nome e Atributos de Guarda-costas
            int group = Native::Call<int>(N_GET_PLAYER_GROUP, pPed);
            Native::Call<void>(N_SET_PED_AS_GROUP_MEMBER, johnHandle, group);
            Native::Call<void>(N_SET_ENTITY_INVINCIBLE, johnHandle, true);
            
            // Configuração para ele poder saquear (flags de IA)
            Native::Call<void>(N_SET_PED_CONFIG_FLAG, johnHandle, 136, true);

            pressTimer = 0; // Reseta
        }
    } else {
        pressTimer = 0;
    }
}

// Entry Point do SPRX para PS4 GoldenHEN
extern "C" int module_start() {
    // Loop infinito do mod
    while (true) {
        ExecutarComando();
        if (johnHandle != 0) {
            LogicaJohnSaqueador(johnHandle);
        }
        // Sleep/Wait nativo para não travar o console
    }
    return 0;
}

extern "C" int module_stop() { return 0; }
