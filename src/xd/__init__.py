from dol_c_kit import Project

# called when a map sparkle item is collected and about to be added to the inventory
SPARKLE_ITEM_GIVE_FUNCTION_ADD_ITEM = 0x80121598
# called when a map chest item is collected and about to be added to the inventory
CHEST_ITEM_GIVE_FUNCTION_ADD_ITEM = 0x801217b8
# called when a NPC gift item is collected and about to be added to the inventory
SCRIPT_ITEM_GIVE_FUNCTION_ADD_ITEM = 0x801c82f0

# called when a pokemon is gifted to the player and about to be added to the party
POKEMON_GET_FUNCTION_ADD_POKEMON = 0x802190a0
# called when a pokemon is captured and about to be added to the party
POKEMON_FIGHT_ADD_POKEMON = 0x802097a8
# called when the purify ceremony is about to set the purify flag
POKEMON_PURIFY_CEREMONY_SET_PURIFY_FLAG = 0x80233268

# called every loop in the battle main function
BATTLE_MAIN_FUNCTION = 0x801ef3b8
# called when the battle ends and about to check the result
# used to check if the player has lost or won the battle for that check
BATTLE_END_FUNCTION = 0x80212634
BATTLE_END_FUNCTION2 = 0x802126dc
BATTLE_END_FUNCTION3 = 0x80221b80

# all of these are called when the battle ends and about to check the result
# used to queue whiteouts for death links
BATTLE_LOOP_GET_RESULT_CHECK_1 = 0x801ef1ac
BATTLE_LOOP_GET_RESULT_CHECK_2 = 0x801ef3f4
BATTLE_LOOP_GET_RESULT_CHECK_3 = 0x801ef44c

# called every loop in the overworld main function
HERO_MAIN = 0x801be06c
# called when new save data is created and the hero is initialized
# used to give the player 1 pokeball
HERO_INIT = 0x801ced40


def hook(proj: Project):
    proj.hook_branchlink(POKEMON_GET_FUNCTION_ADD_POKEMON,
                         "apPatchPokemonCapture")

    # patch purify main check
    proj.hook_branchlink(
        POKEMON_PURIFY_CEREMONY_SET_PURIFY_FLAG, "apPatchPurify")

    # proj.hook_branchlink(BATTLE_MAIN_FUNCTION, "apBattleMain")

    proj.hook_branchlink(BATTLE_END_FUNCTION, "apCheckBattleResult")
    # proj.hook_branchlink(BATTLE_END_FUNCTION2, "apCheckBattleResult")

    proj.hook_branchlink(BATTLE_LOOP_GET_RESULT_CHECK_1, "apQueueDeathLink")
    proj.hook_branchlink(BATTLE_LOOP_GET_RESULT_CHECK_2, "apQueueDeathLink")
    proj.hook_branchlink(BATTLE_LOOP_GET_RESULT_CHECK_3, "apClearDeathLink")

    proj.hook_branchlink(HERO_MAIN, "apMain")
    proj.hook_branchlink(HERO_INIT, "apHeroInit")
