
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

				float lastIterTime = -1;
				int currIter = -1;
				bool isRunning = false;
				bool isCompleted = false;
			};

			std::unordered_map<int, ctx> ctxCollection;
			std::vector<int> ctxQueue;

			int nextFxID = 0;
			int nextCtxID = 0;

		public:

			ctx* getCtx(int ctxID)
			{
				auto it = ctxCollection.find(ctxID);

				if (it == ctxCollection.end()) return nullptr;

				return &it->second;
			}

			void closeCtx(int ctxID)
			{
				auto it = ctxCollection.find(ctxID);

				if (it == ctxCollection.end()) return;

				it->second.isRunning = false;
				it->second.isCompleted = true;
			}

			fx* getFx(int fxID)
			{
				auto it = fxCollection.find(fxID);

				if (it == fxCollection.end()) return nullptr;

				return &it->second;
			}

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
				int outerCtxID,
				int fxID,
				oCNpc* damageSender,
				oCNpc* damageReceiver
			)
			{
				ctx* currCtx = this->getCtx(outerCtxID);
				fx* currFx = this->getFx(fxID);

				if (currCtx && currCtx->isRunning)
					return outerCtxID;
				if (!currFx)
					return outerCtxID;


				ctx newCtx{};

				newCtx.id = nextCtxID++;

				newCtx.type = CTX_LOOP;

				newCtx.fxID = fxID;
				newCtx.damageSender = damageSender;
				newCtx.damageReceiver = damageReceiver;
				newCtx.damageIndex = currFx->damageIndex;
				newCtx.spellID = currFx->spellID;
				newCtx.damage = currFx->damage;
				newCtx.dontKill = currFx->dontKill;
				newCtx.loopInterval = currFx->loopInterval;
				newCtx.iterCount = currFx->iterCount;
				newCtx.exCndFuncIndex = currFx->exCndFuncIndex;

				ctxCollection[newCtx.id] = newCtx;

				return newCtx.id;
			}
			int addCtx
			(
				int outerCtxID,
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
				ctx* currCtx = this->getCtx(outerCtxID);

				if (currCtx && currCtx->isRunning)
					return outerCtxID;


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
				ctx* currCtx = this->getCtx(ctxID);

				if (!currCtx) return;

				if (currCtx->isRunning || currCtx->isCompleted) return;


				currCtx->isRunning = true;

				ctxQueue.push_back(ctxID);

				return;
			}

			void updateCtxQueue()
			{
				oCNpc::oSDamageDescriptor dd {};

				for (auto it = ctxQueue.begin(); it != ctxQueue.end();)
				{
					ctx* currCtx = this->getCtx(*it);

					if (!currCtx || !currCtx->isRunning || currCtx->isCompleted)
					{
						this->closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}


					if (currCtx->exCndFuncIndex >= 0)
					{
						void* pRet = parser->CallFunc(currCtx->exCndFuncIndex);
						int isFuncTrue = *reinterpret_cast<int*>(pRet);

						if (isFuncTrue)
						{
							this->closeCtx(*it);
							it = ctxQueue.erase(it); continue;
						}
					}


					if (currCtx->type == CTX_REGULAR)
					{
						dd.pVobAttacker = currCtx->damageSender;
						dd.pNpcAttacker = currCtx->damageSender;
						dd.pVobHit = currCtx->damageReceiver;
						dd.enuModeDamage = currCtx->damageIndex;
						dd.nSpellID = currCtx->spellID;
						dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
						dd.fDamageTotal = currCtx->damage;

						#if ENGINE == Engine_G2A
							dd.nSpellCat = 2;
							dd.bDamageDontKill = currCtx->dontKill;
						#endif

						dd.fDamageMultiplier = 1.0f;
						dd.dwFieldsValid = 0;


						currCtx->damageReceiver->OnDamage(dd);

						this->closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}
					if (currCtx->type == CTX_LOOP)
					{
						float currTime = ogame->GetWorldTimer()->GetFullTime();

						bool isCtxBroken =
						(
							!currCtx->currIter != !currCtx->lastIterTime
						);

						bool isFirstIter =
						(
							currCtx->currIter == -1 &&
							currCtx->lastIterTime == -1
						);

						bool isCooldownPassed =
						(
							currTime >= (currCtx->lastIterTime + currCtx->loopInterval)
						);

						bool isItersExceeded = 
						(
							currCtx->iterCount >= 0 && currCtx->currIter >= currCtx->iterCount
						);


						if (isCtxBroken || isItersExceeded)
						{
							this->closeCtx(*it);
							it = ctxQueue.erase(it); continue;
						}

						if (!isFirstIter && !isCooldownPassed)
						{
							++it; continue;
						}

						if (isFirstIter)
						{
							currCtx->currIter = 0;
						}
						
						dd.pVobAttacker = currCtx->damageSender;
						dd.pNpcAttacker = currCtx->damageSender;
						dd.pVobHit = currCtx->damageReceiver;
						dd.enuModeDamage = currCtx->damageIndex;
						dd.nSpellID = currCtx->spellID;
						dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
						dd.fDamageTotal = currCtx->damage;

						#if ENGINE == Engine_G2A
							dd.nSpellCat = 2;
							dd.bDamageDontKill = currCtx->dontKill;
						#endif

						dd.fDamageMultiplier = 1.0f;
						dd.dwFieldsValid = 0;


						currCtx->damageReceiver->OnDamage(dd);


						currCtx->currIter++;
						currCtx->lastIterTime = currTime;
						
					}

					++it;

					Union::StringANSI(zSTRING(" ")).StdPrintLine();
					Union::StringANSI::Format(zSTRING("ctxQueue | id: {0}, type: {1}, currTime: {2}, lastIterTime: {3}, loopInterval: {4},"), currCtx->id, currCtx->type, ogame->GetWorldTimer()->GetFullTime(), currCtx->lastIterTime, currCtx->loopInterval).StdPrintLine();
					Union::StringANSI(zSTRING(" ")).StdPrintLine();
				}
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

			int ctxID = ucsManager.addCtx(-1, damageSender, damageReceiver, resultDamageIndex, spellID, damage, dontKill);

			ucsManager.runCtx(ctxID);

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("Call UCS_AD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
			Union::StringANSI(zSTRING(" ")).StdPrintLine();
		}

		return 0;
	};

	int __cdecl UCS_StartLoopDamage()
	{
		int* outerCtxID;
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);
		outerCtxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && fxID >= 0)
		{
			int ctxID = ucsManager.addCtx(*outerCtxID, fxID, damageSender, damageReceiver);

			ucsManager.runCtx(ctxID);

			*outerCtxID = ctxID;

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("Call UCS_SLD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.fxID: ")).StdPrint(); Union::StringANSI(zSTRING(fxID)).StdPrintLine();
			Union::StringANSI(zSTRING(" ")).StdPrintLine();
		}

		return 0;
	};
	int __cdecl UCS_StartLoopDamageEx()
	{
		int* outerCtxID;
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

		outerCtxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && damageIndex >= 0 && spellID >= -1 && damage >= 0 && dontKill >= 0 && loopInterval >= 500.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int ctxID = ucsManager.addCtx(*outerCtxID, damageSender, damageReceiver, resultDamageIndex, spellID, damage, dontKill, loopInterval, iterCount, exCndFuncIndex);

			ucsManager.runCtx(ctxID);

			*outerCtxID = ctxID;

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("Call UCS_SLDE FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(ctxID)).StdPrintLine();
			Union::StringANSI(zSTRING("ctxCollection.fxID: ")).StdPrint(); Union::StringANSI(zSTRING(-1)).StdPrintLine();
			Union::StringANSI(zSTRING(" ")).StdPrintLine();
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

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("Call UCS_CLD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("fxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(fxID)).StdPrintLine();
			Union::StringANSI(zSTRING(" ")).StdPrintLine();
		}

		return 0;
	};


	void Game_DefineExternals_ApplyDamage()
	{
		parser->DefineExternal("UCS_ApplyDamage", UCS_ApplyDamage, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_StartLoopDamage", UCS_StartLoopDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartLoopDamageEx", UCS_StartLoopDamageEx, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_CreateLoopDamage", UCS_CreateLoopDamage, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
	}
}
