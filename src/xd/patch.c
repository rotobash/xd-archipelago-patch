#ifdef REGION_EU
#define ROOM_FLAG_MEMORY 0
#define START_DATA_MEMORY 0
#elif REGION_JP
#define ROOM_FLAG_MEMORY 0
#define START_DATA_MEMORY 0
#else REGION_US
#define BATTLE_FLAG_MEMORY 0x804eb910
#define BATTLE_RESULT_MEMORY 0x804eb90e
#define ROOM_FLAG_MEMORY 0x80814ab6
#define START_DATA_MEMORY 0x81700000
#endif

#define AP_POKEBALL_ITEM_INDEX 4
#define AP_REPLACEMENT_ITEM_INDEX 86

#define AP_TREASURE_ITEM 0
#define AP_SPARKLE_ITEM 1
#define AP_SCRIPT_ITEM 2

#define AP_ENABLE_ITEMCHECKS 0
#define AP_ENABLE_POKEMONCHECKS 1
#define AP_ENABLE_PURIFYCHECKS 2
#define AP_ENABLE_DEATHLINK 3

#define NUMBER_OF_ITEMS 300
#define NUMBER_OF_SHADOW_POKEMON 0x80
#define NUMBER_OF_TRAINERS 450

#include "xd_function_defs.h"

typedef struct ClientPokemonGetParam
{
    char CurrentLevel;
    unsigned short Species;
    unsigned short IsShadow;
    short *NameId;
    unsigned int Experience;
} ClientPokemonGetParam;

typedef struct CaughtPokemon
{
	char CurrentLevel;
	unsigned short Species;
	unsigned short ShadowIndex;
} CaughtPokemon;

typedef struct GiveAPItems
{
	unsigned short pokemonCounter;
	ClientPokemonGetParam pokemon[NUMBER_OF_SHADOW_POKEMON];

	unsigned short purifyCounter;
	unsigned short purified[NUMBER_OF_SHADOW_POKEMON];
} GiveAPItems;

typedef struct CheckedAPItems
{
	unsigned short beatTrainerCounter;
	unsigned short beatTrainerBattleIndex[NUMBER_OF_TRAINERS];

	unsigned short purifyCounter;
	unsigned short purified[NUMBER_OF_SHADOW_POKEMON];

	unsigned short pokemonCounter;
	CaughtPokemon pokemon[NUMBER_OF_SHADOW_POKEMON];

} CheckedAPItems;

typedef struct AP
{
	unsigned short flags;
	unsigned char deathLink;
	unsigned char escape;
	CheckedAPItems checks;
	GiveAPItems give;
} AP;

typedef enum BagSlots
{
	None,
	Pokeballs,
	Items,
	Berries,
	KeyItems,
	TMItems,
	Colognes,
	BattleCDs,
} BagSlots;

typedef enum HeroSections
{
	NONE,
	GAMEDATA_ATTEST,
	TRAINER_ID,
	POKEMON_DATA,
	ITEMS_NORMAL,
	ITEMS_EXTRA,
	ITEMS_BALL,
	ITEMS_KEY,
	ITEMS_SEED,
	ITEMS_KORON,
	ITEMS_HIZUKI,
	SEX_DATA_ID,
	POKEDORU,
	POKECOUPON,
	POKECOUPON_ALL,
	BADGE_1,
	BADGE_2,
	BADGE_3,
	BADGE_4,
	BADGE_5,
	BADGE_6,
	BADGE_7,
	BADGE_8,
	NAME,
	HIZUKI,
	HOME,
} HeroSections;

static AP *ap = (AP *)(START_DATA_MEMORY);
static char getflag(flagType) { return (ap->flags >> flagType) & 1; }
HeroPokemonGetParam emptyGetParam = {0};
CaughtPokemon emptyPokeCheck = {0};

int catchPokemon(Pokemon *pokemon)
{
	ap->checks.pokemon[ap->checks.pokemonCounter].ShadowIndex = zz_battle_pokemon_get_shadow_id_safely(pokemon);
	ap->checks.pokemon[ap->checks.pokemonCounter].Species = zz_battle_pokemon_data_get_species(pokemon);
	ap->checks.pokemon[ap->checks.pokemonCounter].CurrentLevel = zz_battle_pokemon_data_get_level_a(pokemon);

	ap->checks.pokemonCounter += 1;
}

int apPatchPokemonCapture(int *param_1, Pokemon *pokemon, ushort battleRoomId, char ballType, bool putInPC)
{
	catchPokemon(pokemon);
	if (getflag(AP_ENABLE_POKEMONCHECKS))
	{
		return -2;
	}
	else
	{
		return heroCatchPokemon(param_1, pokemon, battleRoomId, ballType, putInPC);
	}
}

void apPatchPurify(ushort shadowIndex)
{
	DarkPokemon *pokemon = zz_shadow_id_get_start_of_stored_shadow_data(shadowIndex);
	ap->checks.purified[ap->checks.purifyCounter] = pokemon->ShadowId;
	ap->checks.purifyCounter += 1;
	return darkPokemonBiosSetReliveFlag(shadowIndex);
}

int apPatchAbleToPurify(int shadowID)
{
	if (!getflag(AP_ENABLE_PURIFYCHECKS))
	{
		return darkPokemonBiosGetReliveFlag(shadowID);
	}

	for (int i = 0; i < ap->give.purifyCounter; i++)
	{
		// have we received the ability to purify the pokemon? if yes, return original funtion call
		if (ap->give.purified[i] == shadowID)
		{
			return darkPokemonBiosGetReliveFlag(shadowID);
		}
	}
	// else none for you
	return 0;
}

int apCheckBattleResult(uint *param_1, short fightResult)
{
	if (fightResult == 2)
	{
		ap->checks.beatTrainerBattleIndex[ap->checks.beatTrainerCounter] = *(unsigned short *)BATTLE_FLAG_MEMORY;
		ap->checks.beatTrainerCounter += 1;
	}

	return fightFloorSetFightResultId(param_1, fightResult);
}

unsigned short apQueueDeathLink()
{
	if (ap->deathLink > 0)
	{
		return 1;
	}
	else
	{
		return *(unsigned short *)BATTLE_RESULT_MEMORY;
	}
}

unsigned short apClearDeathLink()
{
	if (ap->deathLink > 0)
	{
		ap->deathLink = 0;
		return 1;
	}
	else
	{
		return *(unsigned short *)BATTLE_RESULT_MEMORY;
	}
}

void apGiveItems(short *param_12)
{
	Hero *heroPointer = zz_get_trainer_data_start_offset(0, 2);
	int trainerId = zz_trainer_get_tid(heroPointer);

	ClientPokemonGetParam *getPokemonParam = &ap->give.pokemon[ap->give.pokemonCounter];
	while (getPokemonParam->Species != 0)
	{
		emptyGetParam.Species = getPokemonParam->Species;
		emptyGetParam.CurrentLevel = getPokemonParam->CurrentLevel;
		emptyGetParam.IsShadow = getPokemonParam->IsShadow;
		emptyGetParam.Experience = getPokemonParam->Experience;

		emptyGetParam.CatchFloorID = *(unsigned int *)ROOM_FLAG_MEMORY;
		emptyGetParam.Gender = heroBiosGetSexDataId(heroPointer);
		emptyGetParam.TrainerID = trainerId;
		emptyGetParam.NameId = (short *)heroPointer;

		int partyIndex = pokemonGet(heroPointer, &emptyGetParam, 1);
		*param_12 = 1;
		*(int *)(param_12 + 2) = partyIndex;

		ap->give.pokemonCounter += 1;
		getPokemonParam = &ap->give.pokemon[ap->give.pokemonCounter];
	}
}

int *apMain(double param_1_00, double param_2_00, double param_3_00, double param_4, double param_5, double param_6, double param_7, double param_8, int *this, int param_10, int param_11, short *param_12, float *param_13, unsigned int *param_14, int *param_15, int *param_16)
{
	apGiveItems(param_12);
	return cmpHero(param_1_00, param_2_00, param_3_00, param_4, param_5, param_6, param_7, param_8, this, param_10, param_11, param_12, param_13, param_14, param_15, param_16);
}

int apHeroInit(Hero *heroPointer)
{
	int tid = zz_trainer_get_tid(heroPointer);
	heroItemAddItemDataId(heroPointer, AP_POKEBALL_ITEM_INDEX, 1, -1, tid, 0, 1);
	return tid;
}