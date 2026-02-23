#include <stdint.h>

// --- NATIVES RDR2 1.32 CUSA03041 ---
namespace Native {
    template <typename T, typename... Args>
    static T Call(uint64_t hash, Args... args) {
        typedef T (*Func)(Args...);
        return ((Func)hash)(args...);
    }
}

// Hashes Fixas v1.32
#define N_PLAYER_PED_ID          0x2190E381D3930472
#define N_GET_MOUNT              0xE7E11B8E  // GET_MOUNT (Pega o cavalo do ped)
#define N_IS_PED_ON_MOUNT        0x4600003E  // IS_PED_ON_MOUNT
#define N_TASK_MOUNT_ANIMAL      0x92DB31E0  // TASK_MOUNT_ANIMAL
#define N_GET_CLOSEST_PED        0x14E56BC5B50A07C5
#define N_TASK_LOOT_PED          0xE13262B50A07C5
#define N_IS_PED_IN_COMBAT       0x8434317BF5E90C05
#define N_IS_CONTROL_PRESSED     0x0E0313B0

// Variáveis de Estado
int johnPed = 0;
int64_t timerBotao = 0;

void LogicaIAJohn(int john, int player) {
    bool emCombate = Native::Call<bool>(N_IS_PED_IN_COMBAT, john);
    bool playerMontado = Native::Call<bool>(N_IS_PED_ON_MOUNT, player);
    bool johnMontado = Native::Call<bool>(N_IS_PED_ON_MOUNT, john);

    // 1. LÓGICA DE MONTARIA AUTOMÁTICA
    if (playerMontado && !johnMontado) {
        int cavaloPlayer = Native::Call<int>(N_GET_MOUNT, player);
        if (cavaloPlayer != 0) {
            // John monta no seu cavalo (na garupa) ou no dele se estiver perto
            Native::Call<void>(N_TASK_MOUNT_ANIMAL, john, cavaloPlayer, -1, -1, 2.0f, 1, 0, 0);
        }
    }

    // 2. LÓGICA DE SAQUE (FORA DE COMBATE E DESMONTADO)
    if (!emCombate && !playerMontado && !johnMontado) {
        int corpo = Native::Call<int>(N_GET_CLOSEST_PED, john, 15.0f);
        if (corpo != 0) {
            Native::Call<void>(N_TASK_LOOT_PED, john, corpo);
        }
    }
}

void ProcessarComando() {
    // L2 (0x07CEABE9) + SETA CIMA (0x911CB91D)
    if (Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x07CEABE9) && 
        Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x911CB91D)) {
        
        timerBotao++;
        if (timerBotao > 120) { // ~2 segundos em 60fps
            uint32_t johnHash = 0xD8093262; // PLAYER_THREE
            Native::Call<void>(0x125FD99564264624, johnHash, false); // REQUEST_MODEL
            
            int pPed = Native::Call<int>(N_PLAYER_PED_ID);
            johnPed = Native::Call<int>(0xD49F340656402010, johnHash, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, false);
            
            // Setup Guarda-Costa
            int group = Native::Call<int>(0x256B38647614A69D, pPed);
            Native::Call<void>(0x10113106, johnPed, group);
            Native::Call<void>(0x67C546AC, johnPed, true); // Invencível
            
            timerBotao = 0;
        }
    } else {
        timerBotao = 0;
    }
}

extern "C" int module_start() {
    while (true) {
        ProcessarComando();
        if (johnPed != 0) {
            LogicaIAJohn(johnPed, Native::Call<int>(N_PLAYER_PED_ID));
        }
        // O sistema de build do GitHub cuidará da espera de frame via Makefile
    }
    return 0;
}

extern "C" int module_stop() { return 0; }
