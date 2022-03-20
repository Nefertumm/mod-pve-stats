/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 * Module released under MIT license. Read LICENSE file on the root folder of the repository.
 */

#ifndef _PVE_STATS_H
#define _PVE_STATS_H

#include "Config.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

enum instanceMap
{
    // WOTLK
    INSTANCE_RUBY_SANCTUM       = 724,
    INSTANCE_ICECROWN_CITADEL   = 631,
    INSTANCE_TOC                = 649,
    INSTANCE_ULDUAR             = 603,
    INSTANCE_VAULT_OF_ARCHAVON  = 624,
    INSTANCE_NAXXRAMAS          = 533,
    INSTANCE_EYE_OF_ETERNITY    = 616,
    INSTANCE_OBSIDIAN_SANCTUM   = 615,
    // TBC
    INSTANCE_SUNWELL_PLATEAU    = 580,
    INSTANCE_BLACK_TEMPLE       = 564,
    INSTANCE_GRUULS_LAIR        = 565,
    INSTANCE_SERPENT_SHRINE     = 548,
    INSTANCE_MAGTHERIDONS_LAIR  = 544,
    INSTANCE_THE_EYE            = 550,
    INSTANCE_HYJAL              = 534,
    INSTANCE_KARAZHAN           = 532,
    // Vanilla
    INSTANCE_TEMPLE_OF_AHNQIRAJ = 531,
    INSTANCE_RUINS_OF_AHNQIRAJ  = 509,
    INSTANCE_BLACKWING_LAIR     = 469,
    INSTANCE_MOLTEN_CORE        = 409
};

// Structure from instance_encounters
struct EncounterData
{
    EncounterData(uint32 creditEntry)
    {
        QueryResult res = WorldDatabase.Query("SELECT entry, comment FROM instance_encounters WHERE creditEntry = {} LIMIT 1", creditEntry);
        if (res)
        {
            Field* fields = res->Fetch();
            _encounterEntry = fields[0].Get<uint32>();
            _encounterName = fields[1].Get<std::string>();
            _creditEntry = creditEntry;
        }
    }

    bool IsValid() { return _encounterEntry != 0; }

    uint32 _encounterEntry; // column: entry (encounter identifier only)
    uint32 _creditEntry; // column: creditEntry (already obtained from hook)
    std::string _encounterName; // column: comment
};

#endif // _PVE_STATS_H
