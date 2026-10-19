#ifndef XD_FUNCTIONS
#define XD_FUNCTIONS

typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct MoveData
{
    unsigned short moveIndex;
    unsigned short movePP;
} MoveData;

typedef struct ItemInstance
{
    unsigned short itemIndex;
    unsigned short itemQuantity;
} ItemInstance;

typedef struct Pokemon
{ /* PlaceHolder Structure */
    ushort Species;
    ushort HeldItem;
    ushort CurrentHp;
    ushort Happiness;
    ushort unknown;
    char field5_0xa;
    char field6_0xb;
    char field7_0xc;
    char field8_0xd;
    char LevelObtained;
    char PokeballType;
    ushort CurrentLevel;
    char field12_0x12;
    char field13_0x13;
    char field14_0x14;
    char field15_0x15;
    char field16_0x16;
    char field17_0x17;
    char field18_0x18;
    char field19_0x19;
    char field20_0x1a;
    char field21_0x1b;
    char field22_0x1c;
    char Ability;
    char field24_0x1e;
    char field25_0x1f;
    uint Unkown;
    ushort SecretID;
    ushort TrainerID;
    uint PID;
    char OTName[22];
    char Name1[22];
    char Name2[22];
    char field33_0x6e[14];
    ushort Ribbons;
    ushort field48_0x7e;
    struct MoveData moves[4];
    ushort HP;
    ushort Attack;
    ushort Defense;
    ushort SpecialAttack;
    ushort SpecialDefense;
    ushort Speed;
    ushort HPEV;
    ushort AttackEV;
    ushort DefenseEV;
    ushort SpecialAttackEV;
    ushort SpecialDefenseEV;
    ushort SpeedEV;
    char HPIV;
    char AttackIV;
    char DefenseIV;
    char SpecialAttackIV;
    char SpecialDefenseIV;
    char SpeedIV;
    uint ContestMedals;
    char field70_0xb2;
    char field71_0xb3;
    char field72_0xb4;
    char field73_0xb5;
    char field74_0xb6;
    char field75_0xb7;
    char field76_0xb8;
    char field77_0xb9;
    ushort ShadowId;
    char field79_0xbc;
    char field80_0xbd;
    char field81_0xbe;
    char field82_0xbf;
    char field83_0xc0;
    char field84_0xc1;
    char field85_0xc2;
    char field86_0xc3;
} Pokemon;

typedef struct HeroPokemonGetParam
{
    char Gender;
    char CurrentLevel;
    unsigned short Species;
    unsigned short IsShadow;
    unsigned short Friendship;
    unsigned short MoveIds[4];
    unsigned int TrainerID;
    short *NameId;
    short *NicknamePtr;
    unsigned int CatchFloorID;
    unsigned int Experience;
} HeroPokemonGetParam;

typedef struct DarkPokemon
{ /* PlaceHolder Structure */
    uint Flags;
    uint AccumulatedExp;
    char field2_0x8[16];
    ushort Happiness;
    ushort Species;
    uint field20_0x1c;
    uint StepCounter;
    uint HeartGauge;
    char SnaggedFrom[22];
    ushort ShadowId;
    char Aggression;
    char Unknown[7];
} DarkPokemon;

typedef struct Hero
{
    char Name[25];
    char field1_0x19[19];
    uint TrainerID;
    Pokemon Party[6];
    ItemInstance NormalItems[30];
    ItemInstance KeyItems[43];
    ItemInstance BallItems[16];
    ItemInstance TMItems[64];
    ItemInstance Berries[46];
    ItemInstance Cologne[3];
    ItemInstance BattleCDs[60];
    char SexDataId;
    char HomePlace;
} Hero;

typedef unsigned int size_t;
typedef char bool;
void *memcpy(void *__dest, void *__src, size_t __n);

extern unsigned char zz_item_get_bag_slot(uint itemIndex);
extern unsigned int *heroItemAddItemDataId(ItemInstance *itemArray, int itemIndex, int itemQuantity, unsigned short atIndex, int trainerId, int, char);
extern unsigned int *heroCatchPokemon(unsigned int *player, Pokemon *pokemon, short battleRoomId, char ballType, bool putInPC);
extern unsigned int pokemonGet(unsigned int *player, HeroPokemonGetParam *pokemon, bool unknown);
extern bool fightSeqItemSeq_AddPoke(long param_1, long param_2, long param_3, long param_4,
                                    long param_5, long param_6, long param_7, long param_8,
                                    Pokemon *pokemon, char ballType, short *param_11, int *player, int *param_13,
                                    int *param_14, int *param_15, int *param_16);

extern int *zz_get_trainer_data_start_offset(int *offset, ushort slot);
extern ushort zz_battle_pokemon_get_shadow_id_safely(Pokemon *pokemon);
extern ushort zz_battle_pokemon_data_get_species(Pokemon *pokemon);
extern char zz_battle_pokemon_data_get_level_a(Pokemon *pokemon);
extern int heroBiosGetSexDataId(unsigned int *player);
extern int zz_trainer_get_tid(unsigned int *player);
extern int gamedataBiosGetGamedataAtttestPtr(unsigned int *player);

extern void fightMain(double param_1, double param_2, double param_3, double param_4, double param_5, double param_6, double param_7, double param_8);

extern void darkPokemonSetStatus(DarkPokemon *darkPokemon, uint param_1);
extern void darkPokemonBiosSetReliveFlag(ushort shadowId);
extern int darkPokemonBiosGetReliveFlag(ushort shadowId);
extern DarkPokemon *zz_shadow_id_get_start_of_stored_shadow_data(ushort id);

extern uint fightFloorSetFightResultId(uint *param_1, short param_2);
extern void zz_set_battle_result(ushort battleResult);

extern int *cmpHero(double param_1_00, double param_2_00, double param_3_00, double param_4, double param_5, double param_6, double param_7, double param_8, int *this, int param_10, int param_11, short *param_12, float *param_13, uint *param_14, int *param_15, int *param_16);
#endif