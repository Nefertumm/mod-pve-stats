/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 * Module released under MIT license. Read LICENSE file on the root folder of the repository.
 */

#include "Chat.h"
#include "Config.h"
#include "Group.h"
#include "Guild.h"
#include "Map.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "pve-stats.h"

void CheckRaidMembers(uint32 mapId, Map* map, uint32 creditEntry)
{
    Map::PlayerList const& playerList = map->GetPlayers();
    Player* player = nullptr;
    // Get a valid player
    for (auto& itr : playerList)
    {
        player = itr.GetSource();
        if (player)
            break;
    }

    // get the encounter data
    EncounterData data(creditEntry);

    // Still no valid player or not data found.
    if (!player || !data.IsValid())
        return;

    Group* playerGroup = player->GetGroup();
    if (!playerGroup || !playerGroup->isRaidGroup())
        return;

    // begin gathering data
    uint8 playerCount = 0; // total players on map, not using playerList.size()
    uint8 alivePlayers = 0;
    uint8 healers = 0;
    uint8 tanks = 0;
    uint8 dps = 0;
    std::unordered_map<uint32, uint8> guildMap; // guildId, count
    for (auto& itr : playerList)
    {
        Player* currentPlayer = itr.GetSource();
        if (!currentPlayer || currentPlayer->IsGameMaster())
            continue;

        playerCount++;
        if (currentPlayer->IsAlive())
            alivePlayers++;

        if (currentPlayer->HasHealSpec())
            healers++;
        else if (currentPlayer->HasTankSpec())
            tanks++;
        else
            dps++;

        if (currentPlayer->GetGuildId() != 0)
            guildMap[currentPlayer->GetGuildId()]++;
    }

    uint32 guildId = 0;
    for (auto& itr : guildMap)
    {
        if (itr.second >= (playerCount / 2.f)) // Only guilds that have more than the half of the players can take credit for the kill, pugs are not allowed.
            guildId = itr.first;
    }

    if (!guildId) // not enough players for a guild to take credit of the kill, skip this encounter.
        return;

    uint32 now = time(0);
    std::ostringstream ss;
    ss << (uint32)tanks << ' ' << (uint32)healers << ' ' << (uint32)dps;
    std::string raidComposition = ss.str();
    
    // All checks passed, save all into DB.
    CharacterDatabase.Execute("INSERT INTO pve_stats (GuildID, MapID, EncounterEntry, EncounterName, Difficulty, KillDate, RaidComposition, TotalAlive, TotalGuildPlayers, TotalPlayers) VALUES ({}, {}, {}, \"{}\", {}, {}, \"{}\", {}, {}, {})", guildId, mapId, data._encounterEntry, data._encounterName, uint32(map->GetDifficulty()), now, raidComposition, alivePlayers, guildMap[guildId], playerCount);
}

class PveStats : public GlobalScript
{
public:
    PveStats() : GlobalScript("PveStatsGlobalScript") { }

    void OnAfterUpdateEncounterState(Map* map, EncounterCreditType /*type*/, uint32 creditEntry, Unit* source,
        Difficulty /*difficulty_fixed*/, DungeonEncounterList const* /*encounters*/, uint32 /*dungeonCompleted*/, bool /*updated*/) override
    {
        Creature* creature = source->ToCreature();
        if (!creature || !map->IsRaidOrHeroicDungeon() || map->GetPlayersCountExceptGMs() < 1)
            return;

        CheckRaidMembers(creature->GetMapId(), map, creditEntry);
    }
};

// Add all scripts in one
void AddPveStatsScripts()
{
    new PveStats();
}
