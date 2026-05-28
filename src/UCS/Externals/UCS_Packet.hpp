
namespace GOTHIC_NAMESPACE
{
	// EXTERNAL FUNCTIONS /////

	int __cdecl UCS_CreateFXProto()
	{
		int* outerFxProtoID;
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);
		outerFxProtoID = parser->PopVarAddress();

		if (outerFxProtoID != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.createFxProto(outerFxProtoID, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);
		}

		return 0;
	};

	int __cdecl UCS_Hit()
	{
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.startInstantFx(damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill);
		}

		return 0;
	};
	int __cdecl UCS_StartFX()
	{
		int* outerFxID;
		int* outerFxProtoID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxProtoID = parser->PopVarAddress();
		outerFxID = parser->PopVarAddress();

		if (outerFxID != nullptr && outerFxProtoID != nullptr && damageSender != nullptr && damageReceiver != nullptr)
		{
			ucsManager.startFx(outerFxID, outerFxProtoID, damageSender, damageReceiver);
		}

		return 0;
	};
	int __cdecl UCS_StartFXEX()
	{
		int* outerFxID;
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.startFxEx(outerFxID, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);
		}

		return 0;
	};

	int __cdecl UCS_StopFX()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.stopFx(outerFxID, damageSender, damageReceiver);

		return 0;
	};

	int __cdecl UCS_RestartFX()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.restartFx(outerFxID, damageSender, damageReceiver);

		return 0;
	};

	// external getter functions

	int __cdecl UCS_IsRunning()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int isRunning = ucsManager.getFxIsRunning(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(isRunning); return 0;
	};
	int __cdecl UCS_IsCompleted()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int isCompleted = ucsManager.getFxIsCompleted(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(isCompleted); return 0;
	};
	int __cdecl UCS_GetDamage()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int damage = ucsManager.getFxDamage(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(damage); return 0;
	};
	int __cdecl UCS_GetDamageIndex()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int damageIndex = ucsManager.getFxDamageIndex(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(damageIndex); return 0;
	};
	int __cdecl UCS_GetCurrentIter()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int currentIter = ucsManager.getFxCurrIter(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(currentIter); return 0;
	};
	int __cdecl UCS_GetLoopInterval()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		float loopInterval = ucsManager.getFxLoopInterval(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(loopInterval); return 0;
	};
	int __cdecl UCS_GetIterCount()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int iterCount = ucsManager.getFxIterCount(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(iterCount); return 0;
	};
	int __cdecl UCS_GetSpellID()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int spellID = ucsManager.getFxSpellID(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(spellID); return 0;
	};
	int __cdecl UCS_GetVisualFX()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		zSTRING visualFX = ucsManager.getFxStrVisualFX(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(visualFX); return 0;
	};
	int __cdecl UCS_GetDontKill()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int dontKill = ucsManager.getFxDontKill(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(dontKill); return 0;
	};
	int __cdecl UCS_GetLastIterTime()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		float lastIterTime = ucsManager.getFxLastIterTime(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(lastIterTime); return 0;
	};

	// external setter functions

	int __cdecl UCS_SetDamage()
	{
		int* outerFxID; int newDamage;

		parser->GetParameter(newDamage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxDamage(outerFxID, damageSender, damageReceiver, newDamage); return 0;
	};
	int __cdecl UCS_SetDamageIndex()
	{
		int* outerFxID; int newDamageIndex;

		parser->GetParameter(newDamageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxDamageIndex(outerFxID, damageSender, damageReceiver, (oEDamageIndex)newDamageIndex); return 0;
	};
	int __cdecl UCS_SetSpellID()
	{
		int* outerFxID; int newSpellID;

		parser->GetParameter(newSpellID);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxSpellID(outerFxID, damageSender, damageReceiver, newSpellID); return 0;
	};
	int __cdecl UCS_SetVisualFX()
	{
		int* outerFxID; zSTRING newVisualFX;

		parser->GetParameter(newVisualFX);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxStrVisualFX(outerFxID, damageSender, damageReceiver, newVisualFX); return 0;
	};
	int __cdecl UCS_SetDontKill()
	{
		int* outerFxID; int newDontKill;

		parser->GetParameter(newDontKill);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxDontKill(outerFxID, damageSender, damageReceiver, newDontKill); return 0;
	};
	int __cdecl UCS_SetLoopInterval()
	{
		int* outerFxID; float newLoopInterval;

		parser->GetParameter(newLoopInterval);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxLoopInterval(outerFxID, damageSender, damageReceiver, newLoopInterval); return 0;
	};
	int __cdecl UCS_SetIterCount()
	{
		int* outerFxID; int newIterCount;

		parser->GetParameter(newIterCount);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxIterCount(outerFxID, damageSender, damageReceiver, newIterCount); return 0;
	};
	int __cdecl UCS_SetExitCondition()
	{
		int* outerFxID; int newExCndFuncIndex;

		parser->GetParameter(newExCndFuncIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		ucsManager.setFxExCndFuncIndex(outerFxID, damageSender, damageReceiver, newExCndFuncIndex); return 0;
	};


	void Game_DefineExternals_UCS_Packet()
	{
		parser->DefineExternal("UCS_CreateFXProto", UCS_CreateFXProto, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_Hit", UCS_Hit, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartFX", UCS_StartFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartFXEX", UCS_StartFXEX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StopFX", UCS_StopFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_RestartFX", UCS_RestartFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_IsRunning", UCS_IsRunning, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_IsCompleted", UCS_IsCompleted, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamage", UCS_GetDamage, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamageIndex", UCS_GetDamageIndex, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetCurrentIter", UCS_GetCurrentIter, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLoopInterval", UCS_GetLoopInterval, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetIterCount", UCS_GetIterCount, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetSpellID", UCS_GetSpellID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetVisualFX", UCS_GetVisualFX, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDontKill", UCS_GetDontKill, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLastIterTime", UCS_GetLastIterTime, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_SetDamage", UCS_SetDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDamageIndex", UCS_SetDamageIndex, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetSpellID", UCS_SetSpellID, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetVisualFX", UCS_SetVisualFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDontKill", UCS_SetDontKill, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetLoopInterval", UCS_SetLoopInterval, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetIterCount", UCS_SetIterCount, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);

		// void parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
	}
}