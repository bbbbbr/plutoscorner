//
// PlayerHealth.c
//

#include "ZGBMain.h"
#include "PlayerHealth.h"


extern UINT8 playerhealth;
extern UINT8 playerlives;


void PlayerHealth_Reset(UINT8 new_health) {
    // Clear bits and set to default
    playerhealth = new_health;
}

void PlayerLives_Reset(void) {
    // Clear bits and set to default
    playerlives = PLAYER_LIVES_MAX;
}
