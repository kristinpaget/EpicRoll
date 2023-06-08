#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// STRATEGY
#define MINLIFE		4
#define NUMGAMES	1000000

// DICE
#define FACE_BLANK		0
#define FACE_HIT		1
#define FACE_HIT2		2
#define FACE_BLOCK		3
#define FACE_STAR		4

const uint8_t WizardDie[6]	 	= {0,0,1,1,2,2};
const uint8_t WarriorDie[6] 	= {0,1,1,1,2,3};
const uint8_t ElfDie[6] 		= {1,1,1,1,3,3};
const uint8_t MonsterDie[6] 	= {0,0,1,1,1,1};
const uint8_t AdvMonsterDie[6]	= {0,0,1,1,1,2};
const uint8_t PowerDie[6]		= {0,0,0,0,4,4};
const uint8_t NullDie[6]		= {0,0,0,0,0,0};

// ENCOUNTER DICE	 
// NB: Monster starting health is their ID plus 3.
#define MONSTER_MUMMY		0
#define MONSTER_GHOUL		1
#define MONSTER_SKELETON	2
#define MONSTER_GHOST		3
#define MONSTER_ZOMBIE		4
#define MONSTER_BANSHEE		5
#define MONSTER_LICH		6
const uint8_t EncounterDie[6] 		= {0,0,1,1,2,2};
const uint8_t AdvEncounterDie[6] 	= {3,3,4,4,5,5};

// CARD DECK
#define CARD_HIT 		0
#define CARD_BLOCK 		1
#define CARD_COUNTER 	2
#define CARD_HEAL 		3
const uint8_t Deck[20] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3};	

// PLAYER CLASSES
#define PLAYER_WIZARD	0
#define PLAYER_WARRIOR	1
#define PLAYER_ELF		2
#define PLAYER_NONE		3

// BOARD
#define SPACE_START		0
#define SPACE_BATTLE	1
#define SPACE_BANNER	2
#define SPACE_ADVBATTLE	3
#define SPACE_LICH		4
const uint8_t Board[9] = {0,1,1,1,2,3,3,3,4};


typedef struct struct_PlayerState {
	uint8_t PlayerType;
	int8_t Life;
	uint8_t BoardPosition;
	uint8_t Winner;
} PlayerState;

typedef struct struct_GameState {
	uint8_t NumPlayers;
	PlayerState Players[3];
	uint8_t ActivePlayer;
} GameState;

uint32_t Wins[3];

