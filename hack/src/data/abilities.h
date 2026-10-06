const struct AbilityInfo gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Nessuna abilità."),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("Tanfo"),
        .description = COMPOUND_STRING("Può far tentennare."),
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("Piovischio"),
        .description = COMPOUND_STRING("Entrando, fa piovere."),
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("Acceleratore"),
        .description = COMPOUND_STRING("La Velocità sale ogni turno."),
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("Lottascudo"),
        .description = COMPOUND_STRING("Evita i brutti colpi."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("Vigore"),
        .description = COMPOUND_STRING("Evita i KO in un sol colpo."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("Umidità"),
        .description = COMPOUND_STRING("Blocca le mosse esplosive."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("Scioltezza"),
        .description = COMPOUND_STRING("Evita la paralisi."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("Sabbiavelo"),
        .description = COMPOUND_STRING("Più elusione con la sabbia."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("Statico"),
        .description = COMPOUND_STRING("Può paralizzare se toccato."),
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("Assorbivolt"),
        .description = COMPOUND_STRING("Assorbe Elettro e cura PS."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("Assorbacqua"),
        .description = COMPOUND_STRING("Assorbe Acqua e cura PS."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("Indifferenza"),
        .description = COMPOUND_STRING(
        #if B_OBLIVIOUS_TAUNT >= GEN_6
            "Evita infatuazione/provoc."),
        #else
            "Evita l'infatuazione."),
        #endif
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("Antimeteo"),
        .description = COMPOUND_STRING("Annulla gli effetti meteo."),
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("Insettocchi"),
        .description = COMPOUND_STRING("Aumenta la precisione."),
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("Insonnia"),
        .description = COMPOUND_STRING("Evita il sonno."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("Cambiacolore"),
        .description = COMPOUND_STRING("Assume il tipo del colpo."),
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("Immunità"),
        .description = COMPOUND_STRING("Evita l'avvelenamento."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("Fuocardore"),
        .description = COMPOUND_STRING("Assorbe Fuoco, si potenzia."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("Polvoscudo"),
        .description = COMPOUND_STRING("Evita effetti aggiuntivi."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("Mente Locale"),
        .description = COMPOUND_STRING("Evita la confusione."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("Ventose"),
        .description = COMPOUND_STRING("Non può essere scacciato."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("Prepotenza"),
        .description = COMPOUND_STRING("Riduce l'Attacco nemico."),
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("Pedinombra"),
        .description = COMPOUND_STRING("Impedisce la fuga ai nemici."),
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("Cartavetro"),
        .description = COMPOUND_STRING("Ferisce chi lo tocca."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("Magidifesa"),
        .description = COMPOUND_STRING("Teme solo i superefficaci."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("Levitazione"),
        .description = COMPOUND_STRING("Immune alle mosse Terra."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("Spargispora"),
        .description = COMPOUND_STRING("Contatto: avv./sonno/par."),
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("Sincronismo"),
        .description = COMPOUND_STRING("Ricambia scott./par./avv."),
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("Corpochiaro"),
        .description = COMPOUND_STRING("Le statistiche non calano."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("Alternacura"),
        .description = COMPOUND_STRING("Guarisce se sostituito."),
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("Parafulmine"),
        .description = COMPOUND_STRING(
        #if B_REDIRECT_ABILITY_IMMUNITY >= GEN_4
            "Attira Elettro, Att. Sp. su."),
        #else
            "Attira le mosse Elettro."),
        #endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("Leggiadro"),
        .description = COMPOUND_STRING("Più effetti aggiuntivi."),
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("Nuotovelox"),
        .description = COMPOUND_STRING("Più Velocità con la pioggia."),
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("Clorofilla"),
        .description = COMPOUND_STRING("Più Velocità con il sole."),
        .aiRating = 6,
    },

    [ABILITY_ILLUMINATE] =
    {
        .name = _("Risplendi"),
        .description = 
        #if B_ILLUMINATE_EFFECT >= GEN_9
            COMPOUND_STRING("La precisione non cala."),
        #else
            COMPOUND_STRING("Più incontri selvatici."),
        #endif
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_TRACE] =
    {
        .name = _("Traccia"),
        .description = COMPOUND_STRING("Copia l'abilità di un nemico."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE, //B_UPDATED_ABILITY_DATA >= GEN_4
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("Macroforza"),
        .description = COMPOUND_STRING("Raddoppia l'Attacco."),
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("Velenopunto"),
        .description = COMPOUND_STRING("Può avvelenare se toccato."),
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("Forza Interiore"),
        .description = COMPOUND_STRING("Evita di tentennare."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("Magmascudo"),
        .description = COMPOUND_STRING(
        #if B_USE_FROSTBITE
            "Evita il congelamento."),
        #else
            "Evita il congelamento."),
        #endif
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("Idrovelo"),
        .description = COMPOUND_STRING("Evita le scottature."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("Magnetismo"),
        .description = COMPOUND_STRING("Intrappola i tipi Acciaio."),
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("Antisuono"),
        .description = COMPOUND_STRING("Immune alle mosse sonore."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("Copripioggia"),
        .description = COMPOUND_STRING("Recupera PS se piove."),
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("Sabbiafiume"),
        .description = COMPOUND_STRING("Porta tempeste di sabbia."),
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("Pressione"),
        .description = COMPOUND_STRING("Il nemico usa più PP."),
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("Grassospesso"),
        .description = COMPOUND_STRING("Dimezza Fuoco e Ghiaccio."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("Sveglialampo"),
        .description = COMPOUND_STRING("Si sveglia in metà tempo."),
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("Corpodifuoco"),
        .description = COMPOUND_STRING("Può scottare se toccato."),
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("Fugafacile"),
        .description = COMPOUND_STRING("Garantisce la fuga."),
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("Sguardofermo"),
        .description = COMPOUND_STRING("La precisione non cala."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("Ipertaglio"),
        .description = COMPOUND_STRING("L'Attacco non può calare."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("Raccolta"),
        .description = COMPOUND_STRING("Può raccogliere strumenti."),
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("Pigrone"),
        .description = COMPOUND_STRING("Agisce un turno sì e uno no."),
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("Tuttafretta"),
        .description = COMPOUND_STRING("Più forza, meno precisione."),
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("Incantevole"),
        .description = COMPOUND_STRING("Può infatuare se toccato."),
        .aiRating = 2,
    },

    [ABILITY_PLUS] =
    {
        .name = _("Più"),
        .description =
        #if B_PLUS_MINUS_INTERACTION >= GEN_5
            COMPOUND_STRING("Più Att. Sp. con un Meno."),
        #else
            COMPOUND_STRING("Più Att. Sp. con Più o Meno."),
        #endif
        .aiRating = 0,
    },

    [ABILITY_MINUS] =
    {
        .name = _("Meno"),
        .description =
        #if B_PLUS_MINUS_INTERACTION >= GEN_5
            COMPOUND_STRING("Più Att. Sp. con un Più."),
        #else
            COMPOUND_STRING("Più Att. Sp. con Più o Meno."),
        #endif
        .aiRating = 0,
    },

    [ABILITY_FORECAST] =
    {
        .name = _("Previsioni"),
        .description = COMPOUND_STRING("Cambia in base al clima."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_4,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("Antifurto"),
        .description = COMPOUND_STRING("Strumento non rimovibile."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("Muta"),
        .description = COMPOUND_STRING("Può guarire dagli stati."),
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("Dentistretti"),
        .description = COMPOUND_STRING("Problemi di stato: Att. su."),
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("Pelledura"),
        .description = COMPOUND_STRING("Problemi di stato: Dif. su."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("Melma"),
        .description = COMPOUND_STRING("Ferisce chi gli assorbe PS."),
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("Erbaiuto"),
        .description = COMPOUND_STRING("Pochi PS: potenzia Erba."),
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("Aiutofuoco"),
        .description = COMPOUND_STRING("Pochi PS: potenzia Fuoco."),
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("Acquaiuto"),
        .description = COMPOUND_STRING("Pochi PS: potenzia Acqua."),
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("Aiutinsetto"),
        .description = COMPOUND_STRING("Pochi PS: Coleottero su."),
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("Testadura"),
        .description = COMPOUND_STRING("Evita i contraccolpi."),
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("Siccità"),
        .description = COMPOUND_STRING("Entrando, porta il sole."),
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("Trappoarena"),
        .description = COMPOUND_STRING("Impedisce la fuga ai nemici."),
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("Spiritovivo"),
        .description = COMPOUND_STRING("Evita il sonno."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("Fumochiaro"),
        .description = COMPOUND_STRING("Le statistiche non calano."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("Forzapura"),
        .description = COMPOUND_STRING("Raddoppia l'Attacco."),
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("Guscioscudo"),
        .description = COMPOUND_STRING("Evita i brutti colpi."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("Riparo"),
        .description = COMPOUND_STRING("Annulla gli effetti meteo."),
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("Intricopiedi"),
        .description = COMPOUND_STRING("Più elusione se è confuso."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("Elettrorapid"),
        .description = COMPOUND_STRING("Assorbe Elettro, Vel. su."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("Antagonismo"),
        .description = COMPOUND_STRING("Forte con lo stesso sesso."),
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("Cuordeciso"),
        .description = COMPOUND_STRING("Se tentenna, alza la Vel."),
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("Mantelneve"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Più elusione se grandina."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Più elusione se nevica."),
        #else
            "Grandine/neve: elusione su."),
        #endif
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("Voracità"),
        .description = COMPOUND_STRING("Mangia bacche a metà PS."),
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("Grancollera"),
        .description = COMPOUND_STRING("Brutto colpo: Att. al max."),
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("Agiltecnica"),
        .description = COMPOUND_STRING("Senza strumento, più Vel."),
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("Antifuoco"),
        .description = COMPOUND_STRING("Dimezza i danni da Fuoco."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("Disinvoltura"),
        .description = COMPOUND_STRING("Raddoppia i cambi di stat."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("Pellearsa"),
        .description = COMPOUND_STRING("Teme il Fuoco, l'Acqua cura."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("Download"),
        .description = COMPOUND_STRING("Studia il nemico, Att. su."),
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("Ferropugno"),
        .description = COMPOUND_STRING("Potenzia i pugni."),
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("Velencura"),
        .description = COMPOUND_STRING("Recupera PS se avvelenato."),
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("Adattabilità"),
        .description = COMPOUND_STRING("Potenzia il proprio tipo."),
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("Abillegame"),
        .description = COMPOUND_STRING("Multicolpo sempre al max."),
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("Idratazione"),
        .description = COMPOUND_STRING("Si cura con la pioggia."),
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("Solarpotere"),
        .description = COMPOUND_STRING("Sole: Att. Sp. su, perde PS."),
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("Piedisvelti"),
        .description = COMPOUND_STRING("Problemi di stato: Vel. su."),
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("Normalità"),
        .description = COMPOUND_STRING("Ogni mossa diventa Normale."),
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("Cecchino"),
        .description = COMPOUND_STRING("Potenzia i brutti colpi."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("Magicscudo"),
        .description = COMPOUND_STRING("Ferito solo dagli attacchi."),
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("Nullodifesa"),
        .description = COMPOUND_STRING("Ogni mossa va a segno."),
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("Rallentatore"),
        .description = COMPOUND_STRING("Agisce sempre per ultimo."),
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("Tecnico"),
        .description = COMPOUND_STRING("Potenzia le mosse deboli."),
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("Fogliamanto"),
        .description = COMPOUND_STRING("Niente stati con il sole."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("Impaccio"),
        .description = COMPOUND_STRING("Non può usare strumenti."),
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("Rompiforma"),
        .description = COMPOUND_STRING("Ignora le abilità altrui."),
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("Supersorte"),
        .description = COMPOUND_STRING("Più brutti colpi."),
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("Scoppio"),
        .description = COMPOUND_STRING("Se va KO, ferisce il nemico."),
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("Presagio"),
        .description = COMPOUND_STRING("Fiuta le mosse pericolose."),
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("Premonizione"),
        .description = COMPOUND_STRING("Rivela una mossa nemica."),
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("Imprudenza"),
        .description = COMPOUND_STRING("Ignora le stat. nemiche."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("Lentifumé"),
        .description = COMPOUND_STRING("Potenzia i poco efficaci."),
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("Filtro"),
        .description = COMPOUND_STRING("Riduce i superefficaci."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("Lentoinizio"),
        .description = COMPOUND_STRING("Lento nei primi 5 turni."),
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("Nervisaldi"),
        .description = COMPOUND_STRING("Normale e Lotta su Spettro."),
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("Acquascolo"),
        .description = COMPOUND_STRING(
        #if B_REDIRECT_ABILITY_IMMUNITY >= GEN_4
            "Attira Acqua, Att. Sp. su."),
        #else
            "Attira le mosse Acqua."),
        #endif
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("Corpogelo"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Recupera PS se grandina."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Recupera PS se nevica."),
        #else
            "Cura PS con grandine/neve."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("Solidroccia"),
        .description = COMPOUND_STRING("Riduce i superefficaci."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("Scendineve"),
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("Entrando, fa nevicare."),
    #else
        .description = COMPOUND_STRING("Entrando, fa grandinare."),
    #endif
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("Mielincetta"),
        .description = COMPOUND_STRING("Può raccogliere Miele."),
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("Indagine"),
        .description = COMPOUND_STRING("Rivela lo strumento nemico."),
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("Temerarietà"),
        .description = COMPOUND_STRING("Potenzia mosse temerarie."),
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("Multitipo"),
        .description = COMPOUND_STRING("Tipo da Lastra/Cristallo Z."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_FLOWER_GIFT] =
    {
        .name = _("Regalfiore"),
        .description = COMPOUND_STRING("Col sole Att. e Dif. Sp. su."),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_5,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("Sogniamari"),
        .description = COMPOUND_STRING("Ferisce i nemici dormienti."),
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("Arraffalesto"),
        .description = COMPOUND_STRING("Ruba strumenti se colpito."),
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("Forzabruta"),
        .description = COMPOUND_STRING("Più forza, niente effetti."),
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("Inversione"),
        .description = COMPOUND_STRING("Inverte i cambi di stat."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("Agitazione"),
        .description = COMPOUND_STRING("I nemici non usano bacche."),
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("Agonismo"),
        .description = COMPOUND_STRING("Cali di stat.: Attacco su."),
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("Sconforto"),
        .description = COMPOUND_STRING("Metà PS: Att. e Att. Sp. giù."),
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("Corpofunesto"),
        .description = COMPOUND_STRING("Può inibire chi lo colpisce."),
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("Curacuore"),
        .description = COMPOUND_STRING("Può curare gli alleati."),
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("Amicoscudo"),
        .description = COMPOUND_STRING("Riduce i danni agli alleati."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("Sottilguscio"),
        .description = COMPOUND_STRING("Colpito: Dif. giù, Vel. su."),
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("Metalpesante"),
        .description = COMPOUND_STRING("Raddoppia il peso."),
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("Metalleggero"),
        .description = COMPOUND_STRING("Dimezza il peso."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("Multisquame"),
        .description = COMPOUND_STRING("Meno danni se ha tutti i PS."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("Velenimpeto"),
        .description = COMPOUND_STRING("Più Attacco se avvelenato."),
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("Bruciaimpeto"),
        .description = COMPOUND_STRING("Più Att. Sp. se scottato."),
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("Coglibacche"),
        .description = COMPOUND_STRING("Può ricreare bacche usate."),
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("Telepatia"),
        .description = COMPOUND_STRING("Evita gli attacchi alleati."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("Altalena"),
        .description = COMPOUND_STRING("Cambia stat. a ogni turno."),
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("Copricapo"),
        .description = COMPOUND_STRING(
        #if B_POWDER_OVERCOAT >= GEN_6
            "Immune a clima e polveri."),
        #else
            "Evita i danni dal clima."),
        #endif
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("Velentocco"),
        .description = COMPOUND_STRING("Colpendo, può avvelenare."),
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("Rigenergia"),
        .description = COMPOUND_STRING("Recupera PS se sostituito."),
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("Pettinfuori"),
        .description = COMPOUND_STRING("La Difesa non può calare."),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("Remasabbia"),
        .description = COMPOUND_STRING("Più Velocità con la sabbia."),
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("Splendicute"),
        .description = COMPOUND_STRING("Può evitare mosse di stato."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("Ponderazione"),
        .description = COMPOUND_STRING("Più forte se agisce ultimo."),
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("Illusione"),
        .description = COMPOUND_STRING("Si traveste da compagno."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("Sosia"),
        .description = COMPOUND_STRING("Si trasforma nel nemico."),
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("Intrapasso"),
        .description = COMPOUND_STRING("Ignora le barriere."),
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("Mummia"),
        .description = COMPOUND_STRING("Il contatto passa l'abilità."),
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("Arroganza"),
        .description = COMPOUND_STRING("Più Attacco dopo un KO."),
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("Giustizia"),
        .description = COMPOUND_STRING("Il Buio gli alza l'Attacco."),
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("Paura"),
        .description = COMPOUND_STRING("Più Velocità se spaventato."),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("Magispecchio"),
        .description = COMPOUND_STRING("Rimanda le mosse di stato."),
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("Mangiaerba"),
        .description = COMPOUND_STRING("Assorbe Erba, alza l'Att."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("Burla"),
        .description = COMPOUND_STRING("Priorità a mosse di stato."),
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("Silicoforza"),
        .description = COMPOUND_STRING("Più forte con la sabbia."),
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("Spineferrate"),
        .description = COMPOUND_STRING("Ferisce chi lo tocca."),
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("Stato Zen"),
        .description = COMPOUND_STRING("Cambia forma a metà PS."),
        .aiRating = -1,
        .cantBeCopied = TRUE,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeOverwritten = B_UPDATED_ABILITY_DATA >= GEN_7,
        .failsOnImposter = TRUE,
    },

    [ABILITY_VICTORY_STAR] =
    {
        .name = _("Vittorstella"),
        .description = COMPOUND_STRING("Più precisione alla squadra."),
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("Piroturbina"),
        .description = COMPOUND_STRING("Ignora le abilità altrui."),
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("Teravolt"),
        .description = COMPOUND_STRING("Ignora le abilità altrui."),
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("Aromavelo"),
        .description = COMPOUND_STRING("Evita blocchi alle mosse."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("Fiorvelo"),
        .description = COMPOUND_STRING("Protegge gli alleati Erba."),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("Guancegonfie"),
        .description = COMPOUND_STRING("Le bacche gli ridanno PS."),
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("Mutatipo"),
        .description = COMPOUND_STRING("Prende il tipo della mossa."),
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("Foltopelo"),
        .description = COMPOUND_STRING("Dimezza i danni fisici."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("Prestigiatore"),
        .description = COMPOUND_STRING("Ruba lo strumento nemico."),
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("Antiproiettile"),
        .description = COMPOUND_STRING("Immune a sfere e bombe."),
        .breakable = TRUE,
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("Tenacia"),
        .description = COMPOUND_STRING("Cali di stat.: Att. Sp. su."),
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("Ferromascella"),
        .description = COMPOUND_STRING("Potenzia i morsi."),
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("Pellegelo"),
        .description = COMPOUND_STRING("Normale diventa Ghiaccio."),
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("Dolcevelo"),
        .description = COMPOUND_STRING("Squadra immune al sonno."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("Accendilotta"),
        .description = COMPOUND_STRING("Cambia forma mentre lotta."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GALE_WINGS] =
    {
        .name = _("Aliraffica"),
        .description = COMPOUND_STRING("Priorità alle mosse Volante."),
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("Megalancio"),
        .description = COMPOUND_STRING("Potenzia le mosse pulsar."),
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("Peloderba"),
        .description = COMPOUND_STRING("Campo Erboso: Difesa su."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("Simbiosi"),
        .description = COMPOUND_STRING("Dà lo strumento all'alleato."),
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("Unghiedure"),
        .description = COMPOUND_STRING("Potenzia i colpi diretti."),
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("Pellefolletto"),
        .description = COMPOUND_STRING("Normale diventa Folletto."),
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("Viscosità"),
        .description = COMPOUND_STRING("Chi lo tocca perde Velocità."),
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("Pellecielo"),
        .description = COMPOUND_STRING("Normale diventa Volante."),
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("Amorefiliale"),
        .description = COMPOUND_STRING("Attacca insieme al piccolo."),
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("Auratetra"),
        .description = COMPOUND_STRING("Potenzia il Buio di tutti."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("Aurafolletto"),
        .description = COMPOUND_STRING("Potenzia le mosse Folletto."),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("Frangiaura"),
        .description = COMPOUND_STRING("Inverte gli effetti aura."),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("Mare Primordiale"),
        .description = COMPOUND_STRING("Scatena un diluvio."),
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("Terra Estrema"),
        .description = COMPOUND_STRING("Rende il sole estremo."),
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("Flusso Delta"),
        .description = COMPOUND_STRING("Scatena forti venti."),
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("Sopportazione"),
        .description = COMPOUND_STRING("Se colpito, alza la Difesa."),
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("Fuggifuggi"),
        .description = COMPOUND_STRING("A metà PS lascia la lotta."),
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("Passoindietro"),
        .description = COMPOUND_STRING("A metà PS lascia la lotta."),
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("Idrorinforzo"),
        .description = COMPOUND_STRING("L'Acqua gli alza la Difesa."),
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("Spietatezza"),
        .description = COMPOUND_STRING("Brutti colpi su avvelenati."),
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("Scudosoglia"),
        .description = COMPOUND_STRING("A metà PS rompe il guscio."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STAKEOUT] =
    {
        .name = _("Sorveglianza"),
        .description = COMPOUND_STRING("Colpisce forte chi entra."),
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("Bolladacqua"),
        .description = COMPOUND_STRING("Para Fuoco e scottature."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("Tempracciaio"),
        .description = COMPOUND_STRING("Potenzia le mosse Acciaio."),
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("Furore"),
        .description = COMPOUND_STRING("A metà PS alza l'Att. Sp."),
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("Spalaneve"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "Più Velocità se grandina."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "Più Velocità se nevica."),
        #else
            "Più Vel. con grandine/neve."),
        #endif
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("Distacco"),
        .description = COMPOUND_STRING("Attacca senza contatto."),
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("Idrovoce"),
        .description = COMPOUND_STRING("Mosse sonore di tipo Acqua."),
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("Primacura"),
        .description = COMPOUND_STRING("Priorità a mosse curative."),
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("Pellelettro"),
        .description = COMPOUND_STRING("Normale diventa Elettro."),
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("Codasurf"),
        .description = COMPOUND_STRING("Campo Elettrico: Vel. su."),
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("Banco"),
        .description = COMPOUND_STRING("Molti PS: forma un banco."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_DISGUISE] =
    {
        .name = _("Fantasmanto"),
        .description = COMPOUND_STRING("Il panno para un colpo."),
        .aiRating = 8,
        .breakable = TRUE,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_BATTLE_BOND] =
    {
        .name = _("Morfosintonia"),
        .description = COMPOUND_STRING("Cambia forma dopo un KO."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_CONSTRUCT] =
    {
        .name = _("Sciamefusione"),
        .description = COMPOUND_STRING("Cambia forma a metà PS."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_CORROSION] =
    {
        .name = _("Corrosione"),
        .description = COMPOUND_STRING("Avvelena Acciaio e Veleno."),
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("Sonno Assoluto"),
        .description = COMPOUND_STRING("Sempre in dormiveglia."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_QUEENLY_MAJESTY] =
    {
        .name = _("Regalità"),
        .description = COMPOUND_STRING("Blocca mosse con priorità."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("Espellinterno"),
        .description = COMPOUND_STRING("Se va KO, ferisce il nemico."),
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("Sincrodanza"),
        .description = COMPOUND_STRING("Copia le mosse di danza."),
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("Batteria"),
        .description = COMPOUND_STRING("Più Att. Sp. agli alleati."),
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("Morbidone"),
        .description = COMPOUND_STRING("Para contatto, teme Fuoco."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("Corposgargiante"),
        .description = COMPOUND_STRING("Blocca mosse con priorità."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("Cuoreanima"),
        .description = COMPOUND_STRING("Ogni KO alza il suo Att. Sp."),
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("Boccolidoro"),
        .description = COMPOUND_STRING("Chi lo tocca perde Velocità."),
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("Ricezione"),
        .description = COMPOUND_STRING("Copia l'abilità di alleati KO."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("Forza Chimica"),
        .description = COMPOUND_STRING("Copia l'abilità di alleati KO."),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("Ultraboost"),
        .description = COMPOUND_STRING("KO: alza la stat. di punta."),
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("Sistema Primevo"),
        .description = COMPOUND_STRING("Tipo in base alla ROM."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_ELECTRIC_SURGE] =
    {
        .name = _("Elettrogenesi"),
        .description = COMPOUND_STRING("Crea un Campo Elettrico."),
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("Psicogenesi"),
        .description = COMPOUND_STRING("Crea un Campo Psichico."),
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("Nebbiogenesi"),
        .description = COMPOUND_STRING("Crea un Campo Nebbioso."),
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("Erbogenesi"),
        .description = COMPOUND_STRING("Crea un Campo Erboso."),
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("Metalprotezione"),
        .description = COMPOUND_STRING("Le statistiche non calano."),
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("Spettroguardia"),
        .description = COMPOUND_STRING("Meno danni se ha tutti i PS."),
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("Scudoprisma"),
        .description = COMPOUND_STRING("Riduce i superefficaci."),
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("Cerebroforza"),
        .description = COMPOUND_STRING("Potenzia i superefficaci."),
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("Spada Indomita"),
        .description = COMPOUND_STRING(
        #if B_INTREPID_SWORD >= GEN_9
            "Prima entrata: Attacco su."),
        #else
            "Entrando, alza l'Attacco."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("Scudo Saldo"),
        .description = COMPOUND_STRING(
        #if B_DAUNTLESS_SHIELD >= GEN_9
            "Prima entrata: Difesa su."),
        #else
            "Entrando, alza la Difesa."),
        #endif
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("Libero"),
        .description = COMPOUND_STRING("Prende il tipo della mossa."),
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("Raccattapalle"),
        .description = COMPOUND_STRING("Riprende la Ball fallita."),
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("Lanugine"),
        .description = COMPOUND_STRING("Colpito, cala la Vel. altrui."),
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("Elicopinna"),
        .description = COMPOUND_STRING("Ignora chi attira le mosse."),
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("Blindospecchio"),
        .description = COMPOUND_STRING("Rimanda i cali di stat."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("Inghiottimissile"),
        .description = COMPOUND_STRING("Sputa prede se colpito."),
        .aiRating = 3,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeCopied = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeTraced = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STALWART] =
    {
        .name = _("Volontà di Ferro"),
        .description = COMPOUND_STRING("Ignora chi attira le mosse."),
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("Vapormacchina"),
        .description = COMPOUND_STRING("Fuoco e Acqua alzano la Vel."),
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("Punk Rock"),
        .description = COMPOUND_STRING("Potenzia e resiste ai suoni."),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("Sputasabbia"),
        .description = COMPOUND_STRING("Colpito, scatena la sabbia."),
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("Geloscaglie"),
        .description = COMPOUND_STRING("Dimezza i danni speciali."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("Maturazione"),
        .description = COMPOUND_STRING("Raddoppia effetti bacche."),
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("Gelofaccia"),
        .description = COMPOUND_STRING(
        #if B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_HAIL
            "La grandine ricrea il gelo."),
        #elif B_PREFERRED_ICE_WEATHER == B_ICE_WEATHER_SNOW
            "La neve ricrea il gelo."),
        #else
            "Il freddo ricrea il gelo."),
        #endif
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .breakable = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_SPOT] =
    {
        .name = _("Fonte Energetica"),
        .description = COMPOUND_STRING("Potenzia le mosse alleate."),
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("Mimetismo"),
        .description = COMPOUND_STRING("Tipo in base al campo."),
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("Annullabarriere"),
        .description = COMPOUND_STRING("Entrando, annulla barriere."),
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("Spiritoferreo"),
        .description = COMPOUND_STRING("Potenzia l'Acciaio alleato."),
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("Ultimotocco"),
        .description = COMPOUND_STRING("Chi lo tocca: KO in 3 turni."),
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("Anima Errante"),
        .description = COMPOUND_STRING("Contatto: scambia abilità."),
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("Vigorilla"),
        .description = COMPOUND_STRING("Più Att., ma una sola mossa."),
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("Gas Reagente"),
        .description = COMPOUND_STRING("Annulla le abilità altrui."),
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("Pastelvelo"),
        .description = COMPOUND_STRING("Squadra immune al veleno."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("Pancialterna"),
        .description = COMPOUND_STRING("Cambia forma ogni turno."),
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("Colpolesto"),
        .description = COMPOUND_STRING("A volte agisce per primo."),
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("Pugni Invisibili"),
        .description = COMPOUND_STRING("Ignora le protezioni."),
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("Stranofarmaco"),
        .description = COMPOUND_STRING("Azzera stat. agli alleati."),
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("Transistor"),
        .description = COMPOUND_STRING("Potenzia le mosse Elettro."),
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("Dragomascelle"),
        .description = COMPOUND_STRING("Potenzia le mosse Drago."),
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("Nitrito Bianco"),
        .description = COMPOUND_STRING("Più Attacco dopo un KO."),
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("Nitrito Nero"),
        .description = COMPOUND_STRING("Più Att. Sp. dopo un KO."),
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("Sintonia Equina"),
        .description = COMPOUND_STRING("Agitazione, Nitrito Bianco."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .name = _("Sintonia Equina"),
        .description = COMPOUND_STRING("Agitazione, Nitrito Nero."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_LINGERING_AROMA] =
    {
        .name = _("Odore Tenace"),
        .description = COMPOUND_STRING("Il contatto passa l'abilità."),
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("Spargisemi"),
        .description = COMPOUND_STRING("Colpito, crea Campo Erboso."),
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("Termoscambio"),
        .description = COMPOUND_STRING("Il Fuoco gli alza l'Attacco."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("Iraguscio"),
        .description = COMPOUND_STRING("A metà PS cambia le stat."),
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("Sale Purificante"),
        .description = COMPOUND_STRING("Para stati e mosse Spettro."),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("Bentostato"),
        .description = COMPOUND_STRING("Assorbe Fuoco, Difesa su."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("Vento Propizio"),
        .description = COMPOUND_STRING("Il vento alza l'Attacco."),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("Cane da Guardia"),
        .description = COMPOUND_STRING("Prepotenza gli alza l'Att."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("Portamassi"),
        .description = COMPOUND_STRING("Potenzia le mosse Roccia."),
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("Energia Eolica"),
        .description = COMPOUND_STRING("Si carica col vento."),
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("Supercambio"),
        .description = COMPOUND_STRING("Cambia forma se sostituito."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_COMMANDER] =
    {
        .name = _("Torre di Comando"),
        .description = COMPOUND_STRING("Comanda da dentro Dondozo."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("Convertivolt"),
        .description = COMPOUND_STRING("Si carica se viene colpito."),
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("Paleoattivazione"),
        .description = COMPOUND_STRING("Sole: alza la stat. di punta."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("Carica Quark"),
        .description = COMPOUND_STRING("Campo Elettr.: stat. top su."),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("Corpo Aureo"),
        .description = COMPOUND_STRING("Immune alle mosse di stato."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("Vaso Nefasto"),
        .description = COMPOUND_STRING("Riduce l'Att. Sp. altrui."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("Spada Nefasta"),
        .description = COMPOUND_STRING("Riduce la Difesa altrui."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("Amuleto Nefasto"),
        .description = COMPOUND_STRING("Riduce l'Attacco altrui."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("Monile Nefasto"),
        .description = COMPOUND_STRING("Riduce la Dif. Sp. altrui."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("Ritmo d'Oricalco"),
        .description = COMPOUND_STRING("Chiama il sole e alza l'Att."),
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("Motore Adronico"),
        .description = COMPOUND_STRING("Campo Elettr. e Att. Sp. su."),
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("Scrocco"),
        .description = COMPOUND_STRING("Copia i bonus del nemico."),
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("Ruminante"),
        .description = COMPOUND_STRING("Rimangia la bacca usata."),
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("Affilama"),
        .description = COMPOUND_STRING("Potenzia mosse taglienti."),
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("Generale Supremo"),
        .description = COMPOUND_STRING("Alleati KO lo potenziano."),
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("Coprotagonismo"),
        .description = COMPOUND_STRING("Copia le stat. dell'alleato."),
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("Mantossina"),
        .description = COMPOUND_STRING("Colpito, sparge Fielepunte."),
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("Codarmatura"),
        .description = COMPOUND_STRING("Blocca mosse con priorità."),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("Mangiaterra"),
        .description = COMPOUND_STRING("Assorbe Terra e cura PS."),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("Micoforza"),
        .description = COMPOUND_STRING("Mosse di stato infallibili."),
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("Ospitalità"),
        .description = COMPOUND_STRING("Entrando, cura l'alleato."),
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("Occhio Interiore"),
        .description = COMPOUND_STRING("Ha l'effetto Preveggenza."),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("Albergamemorie"),
        .description = COMPOUND_STRING("Teracristal: alza la Vel."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("Albergamemorie"),
        .description = COMPOUND_STRING("Teracristal: alza l'Attacco."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("Albergamemorie"),
        .description = COMPOUND_STRING("Teracristal: alza la Dif. Sp."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("Albergamemorie"),
        .description = COMPOUND_STRING("Teracristal: alza la Difesa."),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("Catena Tossica"),
        .description = COMPOUND_STRING("Può avvelenare gravemente."),
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("Sciroppo Sublime"),
        .description = COMPOUND_STRING("Entrando, cala l'elusione."),
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("Teramorfosi"),
        .description = COMPOUND_STRING("Cambia forma entrando."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TERA_SHELL] =
    {
        .name = _("Teraguscio"),
        .description = COMPOUND_STRING("PS pieni: resiste a tutto."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("Zeroformazione"),
        .description = COMPOUND_STRING("Azzera clima e campo."),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("Malia Tossica"),
        .description = COMPOUND_STRING("Avvelena e confonde."),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_PIERCING_DRILL] =
    {
        .name = _("Punta Perforante"),
        .description = COMPOUND_STRING("Ignora le protezioni."),
    },

    [ABILITY_DRAGONIZE] =
    {
        .name = _("Pelledrago"),
        .description = COMPOUND_STRING("Normale diventa Drago."),
    },

    [ABILITY_EELEVATE] =
    {
        .name = _("Rapidascesa"),
        .description = COMPOUND_STRING("Levitazione e Ultraboost."),
    },

    [ABILITY_314] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Nessuna abilità speciale."),
    },

    [ABILITY_MEGA_SOL] =
    {
        .name = _("Megasolar"),
        .description = COMPOUND_STRING("Agisce come col sole."),
    },

    [ABILITY_FIRE_MANE] =
    {
        .name = _("Pirocriniera"),
        .description = COMPOUND_STRING("Potenzia le mosse Fuoco."),
    },

    [ABILITY_317] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("Nessuna abilità speciale."),
    },

    [ABILITY_SPICY_SPRAY] =
    {
        .name = _("Spargipiccante"),
        .description = COMPOUND_STRING("Scotta chi lo ferisce."),
    },

    [ABILITY_AURA_GUARD] =
    {
        .name = _("Ondascudo"),
        .description = COMPOUND_STRING("Non implementata."),
    },
};
