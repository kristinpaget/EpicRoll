#include "epicroll.h"

int StartGame(GameState *Game, int PlayerA, int PlayerB, int PlayerC)
{
	switch(PlayerA)
	{
		case PLAYER_WIZARD:
			Game->Players[0].PlayerType = PLAYER_WIZARD;
			break;
		case PLAYER_WARRIOR:
			Game->Players[0].PlayerType = PLAYER_WARRIOR;
			break;
		case PLAYER_ELF:
		case PLAYER_NONE:
		default:
			printf("ERROR: First player must be either Wizard or Warrior!\n");
			return 1;
			break;
	}

	switch(PlayerB)
	{
		case PLAYER_WARRIOR:
			if (Game->Players[0].PlayerType == PLAYER_WARRIOR)
			{
				printf("ERROR: Players A and B cannot both be warrior!\n");
				return 1;
			}
			Game->Players[1].PlayerType = PLAYER_WARRIOR;
			break;
		case PLAYER_ELF:
			Game->Players[1].PlayerType = PLAYER_ELF;
			break;
		case PLAYER_WIZARD:
		case PLAYER_NONE:
		default:
			printf("ERROR: Second player must be either Warrior or Elf!\n");
			return 1;
			break;
	}
	
	switch(PlayerC)
	{
		case PLAYER_ELF:
			Game->Players[2].PlayerType = PLAYER_ELF;
			Game->NumPlayers = 3;
			break;
		case PLAYER_NONE:
			Game->Players[2].PlayerType = PLAYER_NONE;
			Game->NumPlayers = 2;
			break;
		case PLAYER_WIZARD:
		case PLAYER_WARRIOR:
		default:
			printf("ERROR: Third player must be either Elf or None!\n");
			return 1;
			break;
	}

	for (int i = 0; i < 3; i++)
	{
		Game->Players[i].BoardPosition = 0;
		Game->Players[i].Winner = 0;
	}
	Game->ActivePlayer = 0;

	return 0;
}

uint8_t RollDie (const uint8_t Die[6])
{
	int Face = rand() % 6;
	return Die[Face];
}

void DoBattle(PlayerState *Player, uint8_t MonsterType)
{
	const uint8_t *PlayerDie;
	const uint8_t *MyMonsterDie;	
	const uint8_t *PlayerPowerDie;
	const uint8_t *MonsterPowerDie;	
	uint32_t Turns = 0;
	
//	printf("Player picked a fight!\n");
	
	// Set up the dice.
	// First the player die
	switch(Player->PlayerType)
	{
		case PLAYER_WIZARD:
			PlayerDie = WizardDie;
			break;
		case PLAYER_WARRIOR:
			PlayerDie = WarriorDie;
			break;
		case PLAYER_ELF:
			PlayerDie = ElfDie;
			break;
		default:
			printf("ERROR: Invalid player type calling DoBattle()!\n");
			exit(1);
			break;
	}
	
	// Then all the dice for the battle.
	switch(MonsterType)
	{
		case 0:
			MyMonsterDie = MonsterDie;
			PlayerPowerDie = NullDie;
			MonsterPowerDie = NullDie;
			break;
		case 1:
			MyMonsterDie = AdvMonsterDie;
			PlayerPowerDie = PowerDie;
			MonsterPowerDie = NullDie;
			break;
		case 2:
			MyMonsterDie = AdvMonsterDie;
			PlayerPowerDie = PowerDie;
			MonsterPowerDie = PowerDie;
			break;
		default:
			printf("ERROR: Invalid encounter type!\n");
			exit(1);
			break;
	}
	
	// Now set up the battle parameters
	// Choose a monster
	int8_t MonsterLife;
	if (MonsterType == 0)
	{
		MonsterLife = RollDie(EncounterDie) + 3;
	}
	else if (MonsterType == 1)
	{
		MonsterLife = RollDie(AdvEncounterDie) + 3;
	}
	else if (MonsterType == 2)
	{
		MonsterLife = 9;
	}
	
	while(1)
	{
		// Roll and process all the dice.
		int MonsterBlocked = 0;
		int PlayerHeal = 0;
		int MonsterHeal = 0;
		Turns++;

//		printf("P: %u, M: %u, T: %u\n", Player->Life, MonsterLife, Turns);
				
		switch(RollDie(PlayerDie))
		{
			case FACE_BLANK:
//				printf("MISS ");
				break;
			case FACE_HIT:
//				printf("HIT ");
				MonsterLife--;
				break;
			case FACE_HIT2:
//				printf("HIT2 ");
				MonsterLife -= 2;
				break;
			case FACE_BLOCK:
//				printf("BLOCK ");
				MonsterBlocked = 1;
				break;
			default:
				printf("ERROR: Unknown player die result!\n");
				exit(1);
				break;
		}
		switch(RollDie(PlayerPowerDie))
		{
			case FACE_BLANK:
				break;
			case FACE_STAR:
//				printf("STAR ");
				switch(Player->PlayerType)
				{
					case PLAYER_WIZARD:
						MonsterBlocked = 1;
						break;
					case PLAYER_WARRIOR:
						PlayerHeal = 1;
						break;
					case PLAYER_ELF:
						MonsterLife--;
						break;
				}
				break;
			default:
				printf("ERROR: Invalid player power die!\n");
				exit(1);
				break;				
		}
		switch(RollDie(MonsterPowerDie))
		{
			case FACE_BLANK:
				break;
			case FACE_STAR:
//				printf("MSTAR ");
				if (MonsterLife < 9)
				{
					MonsterLife++;
				}
				break;
			default:
				printf("ERROR: Invalid player power die!\n");
				exit(1);
				break;				
		}
		if (MonsterBlocked == 0)
		{
			switch(RollDie(MyMonsterDie))
			{
				case FACE_BLANK:
//					printf("MMISS ");
					break;
				case FACE_HIT:
//					printf("MHIT ");
					Player->Life--;
					break;
				case FACE_HIT2:
//					printf("MHIT2 ");
					Player->Life -= 2;
					break;
				default:
					printf("ERROR: Unknown monster die result!\n");
					exit(1);
					break;
			}
		}
		
//		printf("\n");
		
		// Process healing
		if (MonsterHeal && (MonsterLife < 9))
		{
			MonsterLife++;
		}
		if (PlayerHeal && (Player->Life < 6))
		{
			Player->Life++;
		}

		// Did anyone die?
		if (Player->Life <= 0)
		{
			// Oops.
			if (Player->BoardPosition == 8)
			{
				Player->BoardPosition = 4;
			}
			else
			{
				Player->BoardPosition = 0;
			}
//			printf("Player died after %u turns :(\n", Turns);
			return;			
		}
		if (MonsterLife <= 0)
		{
			// Yay!
//			printf("Player won the fight after %u turns!\n", Turns);
			if (Player->BoardPosition == 8)
			{
				// Woop woop!
				Player->Winner = 1;
//				printf("Player won the game!\n");
			}
			return;
		}
	}
}
			
void TakeTurn (GameState *Game)
{
	PlayerState *Player = &Game->Players[Game->ActivePlayer];
	Player->Life = 6;
	
	while(Player->Life >= MINLIFE)
	{
		// Start a fight!
//		printf("Player %u's turn.\n", Game->ActivePlayer);		
		Player->BoardPosition++;
		switch(Board[Player->BoardPosition])
		{
			case SPACE_BATTLE:
				DoBattle(Player, 0);
				break;
			case SPACE_ADVBATTLE:
				DoBattle(Player, 1);
				break;
			case SPACE_LICH:
				DoBattle(Player, 2);
				break;
			case SPACE_BANNER:
				break;
			default:
				printf("ERROR: Invalid player position, space %u type %u\n", Player->BoardPosition, Board[Player->BoardPosition]);
				exit(1);
				break;
		}
		
		if (Player->Winner) 
		{
			return;
		}
	}
}		

int main (int argc, char *argv[])
{
	for (int i = 0; i < 3; i++)
	{
		Wins[i] = 0;
	}

	time_t t;
	srand((unsigned) time(&t));

	for (int i = 0; i < NUMGAMES; i++)
	{
		GameState Game;
		if (StartGame(&Game, PLAYER_WIZARD, PLAYER_WARRIOR, PLAYER_ELF))
		{
			printf("ERROR: StartGame() failed!\n");
			exit(1);
		}
		
		while(1)
		{
			TakeTurn(&Game);
			if (Game.Players[Game.ActivePlayer].Winner)
			{
				Wins[Game.Players[Game.ActivePlayer].PlayerType]++;
				if (i && (i % (NUMGAMES / 100) == 0))
				{
					printf(".");
					fflush(stdout);
				}
				break;
			}
			else
			{
				Game.ActivePlayer++;
				if (Game.ActivePlayer == Game.NumPlayers)
				{
					Game.ActivePlayer = 0;
				}
			}
		}
	}
	
	printf("\n");
	
	float WizPC = (float)Wins[0] * 100.0 / (float)NUMGAMES;
	float WarPC = (float)Wins[1] * 100.0 / (float)NUMGAMES;
	float ElfPC = (float)Wins[2] * 100.0 / (float)NUMGAMES;
	
	printf("%u games played\nWizard won  %04u(%02.2f%%)\nWarrior won %04u(%02.2f%%)\nElf won     %04u(%02.2f%%)\n", \
		NUMGAMES, \
		Wins[0], WizPC, \
		Wins[1], WarPC, \
		Wins[2], ElfPC);
		
	return 0;	
}

