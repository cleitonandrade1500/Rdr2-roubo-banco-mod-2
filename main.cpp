#include <stdint.h>

// --- NATIVES RDR2 1.32 CUSA03041 ---
namespace Native {
    template <typename T, typename... Args>
    static T Call(uint64_t hash, Args... args) {
        typedef T (*Func)(Args...);
        return ((Func)hash)(args...);
    }
}

// Definições de Hashes 1.32
#define N_PLAYER_PED_ID          0x2190E381D3930472
#define N_IS_CONTROL_PRESSED     0x0E0313B0
#define N_REQUEST_MODEL          0x125FD99564264624
#define N_CREATE_PED             0xD49F340656402010
#define N_SET_PED_AS_GROUP_MEMBER 0x10113106
#define N_IS_PED_IN_COMBAT       0x8434317BF5E90C05
#define N_GET_CLOSEST_PED        0x14E56BC5B50A07C5
#define N_TASK_LOOT_PED          0xE13262B50A07C5
#define N_IS_PED_ON_MOUNT        0x4600003E
#define N_GET_MOUNT              0xE7E11B8E
#define N_TASK_MOUNT_ANIMAL      0x92DB31E0

int johnPed = 0;
int64_t timerBotao = 0;

void AtualizarIA(int john, int player) {
    bool emCombate = Native::Call<bool>(N_IS_PED_IN_COMBAT, john);
    bool playerMontado = Native::Call<bool>(N_IS_PED_ON_MOUNT, player);
    bool johnMontado = Native::Call<bool>(N_IS_PED_ON_MOUNT, john);

    // Lógica Montaria
    if (playerMontado && !johnMontado) {
        int cavalo = Native::Call<int>(N_GET_MOUNT, player);
        if (cavalo != 0) Native::Call<void>(N_TASK_MOUNT_ANIMAL, john, cavalo, -1, -1, 2.0f, 1, 0, 0);
    }

    // Lógica Saque
    if (!emCombate && !playerMontado && !johnMontado) {
        int corpo = Native::Call<int>(N_GET_CLOSEST_PED, john, 15.0f);
        if (corpo != 0) Native::Call<void>(N_TASK_LOOT_PED, john, corpo);
    }
}

void ChecarInput() {
    // L2 + Seta para Cima
    if (Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x07CEABE9) && 
        Native::Call<bool>(N_IS_CONTROL_PRESSED, 0, 0x911CB91D)) {
        timerBotao++;
        if (timerBotao > 100) { // ~2 segundos
            uint32_t johnHash = 0xD8093262;
            Native::Call<void>(N_REQUEST_MODEL, johnHash, false);
            johnPed = Native::Call<int>(N_CREATE_PED, johnHash, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, false);
            Native::Call<void>(N_SET_PED_AS_GROUP_MEMBER, johnPed, Native::Call<int>(0x256B38647614A69D, Native::Call<int>(N_PLAYER_PED_ID)));
            timerBotao = 0;
        }
    } else { timerBotao = 0; }
}

extern "C" int module_start() {
    while (true) {
        ChecarInput();
        if (johnPed != 0) AtualizarIA(johnPed, Native::Call<int>(N_PLAYER_PED_ID));
    }
    return 0;
}
extern "C" int module_stop() { return 0; }
