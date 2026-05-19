
namespace GOTHIC_NAMESPACE
{
	class UCS_Manager
	{
		private:

			struct fx
			{
				int id;

				oEDamageIndex damageIndex;
				int spellID;
				int damage;
				int dontKill;
				float loopInterval;
				int exCndFuncIndex;
				int iterCount;
			};

			std::unordered_map<int, fx> fxCollection;

			enum CtxType
			{
				CTX_REGULAR,
				CTX_LOOP
			};

			struct ctx
			{
				int id;

				CtxType type;

				int fxID = -1;

				oCNpc* damageSender;
				oCNpc* damageReceiver;

				oEDamageIndex damageIndex;
				int spellID;
				int damage;
				int dontKill;
				float loopInterval;
				int exCndFuncIndex;
				int iterCount;

				unsigned long lastIterTime;
				int currentIter;
				bool isRunning = 0;
				bool isCompleted = 0;
			};

			std::unordered_map<int, ctx> ctxCollection;

			int nextFxID = 0;
			int nextCtxID = 0;

			std::deque<int> pipelineQueue;
			std::vector<int> activeLoops;

		public:

			int addFX
			(
				oEDamageIndex damageIndex,
				int spellID,
				int damage,
				int dontKill,
				float loopInterval,
				int iterCount,
				int exCndFuncIndex
			)
			{
				fx newFx{};

				newFx.id = nextFxID++;

				newFx.damageIndex = damageIndex;
				newFx.spellID = spellID;
				newFx.damage = damage;
				newFx.dontKill = dontKill;
				newFx.loopInterval = loopInterval;
				newFx.iterCount = iterCount;
				newFx.exCndFuncIndex = exCndFuncIndex;

				fxCollection[newFx.id] = newFx;

				return newFx.id;
			}
			int addCtx
			(
				int fxID,
				oCNpc* damageSender,
				oCNpc* damageReceiver
			)
			{
				ctx newCtx{};

				newCtx.id = nextCtxID++;

				newCtx.type = CTX_LOOP;

				newCtx.fxID = fxID;
				newCtx.damageSender = damageSender;
				newCtx.damageReceiver = damageReceiver;
				newCtx.damageIndex = fxCollection[fxID].damageIndex;
				newCtx.spellID = fxCollection[fxID].spellID;
				newCtx.damage = fxCollection[fxID].damage;
				newCtx.dontKill = fxCollection[fxID].dontKill;
				newCtx.loopInterval = fxCollection[fxID].loopInterval;
				newCtx.iterCount = fxCollection[fxID].iterCount;
				newCtx.exCndFuncIndex = fxCollection[fxID].exCndFuncIndex;

				ctxCollection[newCtx.id] = newCtx;

				return newCtx.id;
			}
			int addCtx
			(
				oCNpc* damageSender,
				oCNpc* damageReceiver,
				oEDamageIndex damageIndex,
				int spellID,
				int damage,
				int dontKill,
				float loopInterval = -1.0f,
				int iterCount = -1,
				int exCndFuncIndex = -1
			)
			{
				ctx newCtx{};

				newCtx.id = nextCtxID++;

				newCtx.type = (loopInterval >= 500.0f) && (iterCount >= 0 || exCndFuncIndex >= 0) ? CTX_LOOP : CTX_REGULAR;

				newCtx.damageSender = damageSender;
				newCtx.damageReceiver = damageReceiver;
				newCtx.damageIndex = damageIndex;
				newCtx.spellID = spellID;
				newCtx.damage = damage;
				newCtx.dontKill = dontKill;
				newCtx.loopInterval = loopInterval;
				newCtx.iterCount = iterCount;
				newCtx.exCndFuncIndex = exCndFuncIndex;

				ctxCollection[newCtx.id] = newCtx;

				return newCtx.id;
			}
			void runCtx(int ctxID)
			{

			}
	};

	UCS_Manager ucsManager;


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

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= -1 && damage >= 0 && dontKill >= 0)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int ctxID = ucsManager.addCtx(damageSender, damageReceiver, resultDamageIndex, spellID, damage, dontKill);

			ucsManager.runCtx(ctxID);

			Union::StringANSI(zSTRING("Call UCS_AD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
		}

		return 0;
	};

	int __cdecl UCS_StartLoopDamage()
	{
		int fxID;

		parser->GetParameter(fxID);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && fxID >= 0)
		{
			int ctxID = ucsManager.addCtx(fxID, damageSender, damageReceiver);

			ucsManager.runCtx(ctxID);

			parser->SetReturn(ctxID);

			Union::StringANSI(zSTRING("Call UCS_SLD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.fxID: ")).StdPrint(); Union::StringANSI(zSTRING(fxID)).StdPrintLine();
		}

		return 0;
	};
	int __cdecl UCS_StartLoopDamageEx()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= -1 && damage >= 0 && dontKill >= 0 && loopInterval >= 500.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int ctxID = ucsManager.addCtx(damageSender, damageReceiver, resultDamageIndex, spellID, damage, dontKill, loopInterval, iterCount, exCndFuncIndex);

			ucsManager.runCtx(ctxID);

			parser->SetReturn(ctxID);

			Union::StringANSI(zSTRING("Call UCS_SLDE FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.fxID: ")).StdPrint(); Union::StringANSI(zSTRING(-1)).StdPrintLine();
		}

		return 0;
	};
	int __cdecl UCS_CreateLoopDamage()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		if (damageIndex >= 0 && spellID >= -1 && damage >= 0 && dontKill >= 0 && loopInterval >= 500.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int fxID = ucsManager.addFX(resultDamageIndex, spellID, damage, dontKill, loopInterval, iterCount, exCndFuncIndex);

			parser->SetReturn(fxID);

			Union::StringANSI(zSTRING("Call UCS_CLD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("fxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(fxID)).StdPrintLine();
		}

		return 0;
	};


	void Game_DefineExternals_ApplyDamage()
	{
		parser->DefineExternal("UCS_ApplyDamage", UCS_ApplyDamage, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_StartLoopDamage", UCS_StartLoopDamage, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartLoopDamageEx", UCS_StartLoopDamageEx, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_CreateLoopDamage", UCS_CreateLoopDamage, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
	}
}
