#include <stdint.h>

// Macros de Invocação usando as Hashes Oficiais da NativeDB para v1.32
#define INVOKE(h, T, ...) ((T(*)(...))h)(__VA_ARGS__)

// Hashes Extraídas da NativeDB (RDR2Mods)
#define N_PLAYER_PED_ID          0x2190E381D3930472
#define N_GET_PLAYER_PED         0x2190E381D3930472
#define N_CREATE_PED             0xD49F340656402010
#define N_REQUEST_MODEL          0x125FD99564264624
#define N_HAS_MODEL_LOADED       0x125FD99564264625
#define N_SET_PED_AS_GROUP_MEMBER 0x10113106
#define N_GET_PLAYER_GROUP       0x256B38647614A69D
#define N_IS_CONTROL_PRESSED     0x0E0313B0
#define N_TASK_LOOT_PED          0xE13262B50A07C5
#define N_GET_CLOSEST_PED        0x14E56BC5B50A07C5
#define N_IS_PED_ON_MOUNT        0x4600003E
#define N_GET_MOUNT              0xE7E11B8E
#define N_TASK_MOUNT_ANIMAL      0x92DB31E0

int johnPed = 0;
int timerBotao = 0;

void GerenciarIA(int john, int player) {
    // Se o jogador estiver montado e o John não, ele sobe na garupa ou no dele
    if (INVOKE(N_IS_PED_ON_MOUNT, bool, player) && !INVOKE(N_IS_PED_ON_MOUNT, bool, john)) {
        int cavalo = INVOKE(N_GET_MOUNT, int, player);
        if (cavalo != 0) {
            INVOKE(N_TASK_MOUNT_ANIMAL, void, john, cavalo, -1, -1, 2.0f, 1, 0, 0);
        }
    }
    // Lógica de Saque (quando fora de combate e a pé)
    if (!INVOKE(0x4600003E, bool, player)) { // Jogador a pé
        int corpo = INVOKE(N_GET_CLOSEST_PED, int, john, 10.0f);
        if (corpo != 0) {
            INVOKE(N_TASK_LOOT_PED, void, john, corpo);
        }
    }
}

extern "C" __attribute__((visibility("default"))) int module_start() {
    while (true) {
        // Comando: L2 (0x07CEABE9) + SETA CIMA (0x911CB91D)
        if (INVOKE(N_IS_CONTROL_PRESSED, bool, 0, 0x07CEABE9) && 
            INVOKE(N_IS_CONTROL_PRESSED, bool, 0, 0x911CB91D)) {
            
            timerBotao++;
            if (timerBotao > 150) { // ~2 Segundos
                uint32_t johnHash = 0xD8093262; // Hash do John Marston
                
                INVOKE(N_REQUEST_MODEL, void, johnHash, false);
                // Aguarda o modelo carregar
                while (!INVOKE(N_HAS_MODEL_LOADED, bool, johnHash)) {
                    for(volatile int i=0; i<10000; i++);
                }

                int pPed = INVOKE(N_PLAYER_PED_ID, int);
                // Criar John (PLAYER_THREE)
                johnPed = INVOKE(N_CREATE_PED, int, johnHash, 0, 0, 0, 0.0f, false, false, false, false);
                
                // Definir como membro do grupo (Guarda-costa)
                int group = INVOKE(N_GET_PLAYER_GROUP, int, pPed);
                INVOKE(N_SET_PED_AS_GROUP_MEMBER, void, johnPed, group);
                
                timerBotao = 0;
            }
        } else { timerBotao = 0; }

        if (johnPed != 0) {
            GerenciarIA(johnPed, INVOKE(N_PLAYER_PED_ID, int));
        }

        // Delay para estabilidade na FW 12.00
        for(volatile int i=0; i<400000; i++); 
    }
    return 0;
}

extern "C" int module_stop() { return 0; }
