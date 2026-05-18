
namespace GOTHIC_NAMESPACE
{
	struct ctx
	{
		int id;

		oCNpc* damageSender;
		oCNpc* damageReceiver;

		oEDamageIndex damageIndex;
		int spellID;
		int damage;
		int dontKill;
		int loopInterval;
		int exCndFuncIndex;
		int iterCount;

		unsigned long lastIterTime;
		bool isDead;
	};

	static std::vector<ctx> ctxCollection;

	unsigned int generateCtxID()
	{
		static unsigned int callCounter = 1337;
		callCounter++;

		unsigned long currentTime = ztimer->totalTime;

		size_t seed = static_cast<size_t>(currentTime) ^ static_cast<size_t>(callCounter);

		std::hash<size_t> hasher;
		unsigned int hashedValue = static_cast<unsigned int>(hasher(seed));

		return 1000 + (hashedValue % 99000);
	}


	// EXTERNAL FUNCTIONS /////

	int __cdecl UCS_ApplyDamage()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (!gOnDamageIsRunning && damageReceiver != nullptr && damageSender != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0)
		{
			oCNpc::oSDamageDescriptor dd{};
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			dd.pVobAttacker = damageSender;
			dd.pNpcAttacker = damageSender;
			dd.pVobHit = damageReceiver;
			dd.enuModeDamage = GetDamageType(resultDamageIndex);
			dd.nSpellID = spellID;
			dd.nSpellCat = 2;
			dd.aryDamage[resultDamageIndex] = damage;
			dd.fDamageTotal = damage;
			dd.bDamageDontKill = dontKill;

			dd.fDamageMultiplier = 1.0f;
			dd.bOnce = 1;
			dd.bFinished = 1;
			dd.dwFieldsValid = 0;

			gOnDamageIsRunning = true;

			damageReceiver->OnDamage(dd);

			Union::StringANSI(zSTRING("Call THIS FUNC")).StdPrintLine();

			Union::StringANSI(zSTRING("Attacker: ")).StdPrint(); Union::StringANSI(zSTRING(dd.pNpcAttacker != nullptr ? dd.pNpcAttacker->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("Receiver: ")).StdPrint(); Union::StringANSI(zSTRING(damageReceiver != nullptr ? damageReceiver->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dt: ")).StdPrint(); Union::StringANSI(zSTRING(dd.enuModeDamage >= 0 ? dd.enuModeDamage : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("aryDamage: ")).StdPrint(); Union::StringANSI(zSTRING(resultDamageIndex && dd.aryDamage[resultDamageIndex] >= 0 ? dd.aryDamage[resultDamageIndex] : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bDamageDontKill: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bDamageDontKill >= 0 ? dd.bDamageDontKill : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bIsUnconscious: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsUnconscious >= 0 ? dd.bIsUnconscious : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bIsDead: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsDead >= 0 ? dd.bIsDead : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dwFieldsValid: ")).StdPrint(); Union::StringANSI(dd.dwFieldsValid >= 0 ? zSTRING(dd.dwFieldsValid) : zSTRING("nothing")).StdPrintLine();

			gOnDamageIsRunning = false;
		}

		return 0;
	};

	int __cdecl UCS_CreateDamage()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ctx newCtx{};

			newCtx.id = generateCtxID();

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damageIndex = resultDamageIndex;
			newCtx.spellID = spellID;
			newCtx.damage = damage;
			newCtx.dontKill = dontKill;

			newCtx.isDead = false;

			ctxCollection.push_back(newCtx);

			parser->SetReturn(newCtx.id);
		}

		Union::StringANSI(zSTRING("Call UCS_CLD_EC FUNC")).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].id)).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.ds: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageSender->GetName(0))).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.dr: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageReceiver->GetName(0))).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.didx: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageIndex)).StdPrintLine();

		return 0;
	};
	int __cdecl UCS_CreateLoopDamage_EC()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;
		int loopInterval;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0 && loopInterval >= 0 && exCndFuncIndex >= 0)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ctx newCtx{};

			newCtx.id = generateCtxID();

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damageIndex = resultDamageIndex;
			newCtx.spellID = spellID;
			newCtx.damage = damage;
			newCtx.dontKill = dontKill;
			newCtx.loopInterval = loopInterval;
			newCtx.exCndFuncIndex = exCndFuncIndex;

			newCtx.lastIterTime = -1; //ztimer->totalTime
			newCtx.isDead = false;

			ctxCollection.push_back(newCtx);

			parser->SetReturn(newCtx.id);
		}

		Union::StringANSI(zSTRING("Call UCS_CLD_EC FUNC")).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].id)).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.ds: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageSender->GetName(0))).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.dr: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageReceiver->GetName(0))).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.didx: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].damageIndex)).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.lit: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].lastIterTime)).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.excndfunc: ")).StdPrint(); Union::StringANSI(zSTRING(ctxCollection[0].exCndFuncIndex)).StdPrintLine();

		return 0;
	};
	int __cdecl UCS_CreateLoopDamage_IC()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;
		int loopInterval;
		int iterationCount;

		parser->GetParameter(iterationCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0 && loopInterval >= 0 && iterationCount >= 1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ctx newCtx{};

			newCtx.id = generateCtxID();

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damageIndex = resultDamageIndex;
			newCtx.spellID = spellID;
			newCtx.damage = damage;
			newCtx.dontKill = dontKill;
			newCtx.loopInterval = loopInterval;
			newCtx.exCndFuncIndex = iterationCount;

			newCtx.lastIterTime = -1; //ztimer->totalTime
			newCtx.isDead = false;

			ctxCollection.push_back(newCtx);

			parser->SetReturn(newCtx.id);
		}

		return 0;
	};


	void Game_DefineExternals_ApplyDamage()
	{
		parser->DefineExternal("UCS_ApplyDamage", UCS_ApplyDamage, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_CreateDamage", UCS_CreateDamage, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_CreateLoopDamage_EC", UCS_CreateLoopDamage_EC, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_CreateLoopDamage_IC", UCS_CreateLoopDamage_IC, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
	}
}
