
namespace GOTHIC_NAMESPACE
{
	class UCS_Manager
	{
	private:

		struct fx
		{
			int id;

			int damage;
			oEDamageIndex damageIndex;
			int spellID;
			zSTRING strVisualFX;
			int dontKill;
			float loopInterval;
			int exCndFuncIndex;
			int iterCount;
		};

		std::unordered_map<int, fx> fxCollection;

		enum CtxType
		{
			CTX_UNKNOWN,
			CTX_REGULAR,
			CTX_LOOP,
		};

		struct ctx
		{
			int id;
			CtxType type = CTX_UNKNOWN;
			int fxID = -1;

			oCNpc* damageSender;
			oCNpc* damageReceiver;

			int damage;
			oEDamageIndex damageIndex;
			int spellID;
			zSTRING strVisualFX;
			int dontKill;
			float loopInterval;
			int exCndFuncIndex;
			int iterCount;

			float lastIterTime = -1.0f;
			int currIter = -1;
			bool isRunning = false;
			bool isApplying = false;
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

		fx* getFx(int fxID)
		{
			auto it = fxCollection.find(fxID);

			if (it == fxCollection.end()) return nullptr;

			return &it->second;
		}

		bool isFxValid(fx* currFx)
		{
			if (currFx == nullptr ||
				currFx->id < 0 ||
				currFx->damage < 0 ||
				currFx->damageIndex < 0 ||
				currFx->damageIndex >= oEDamageIndex_MAX ||
				currFx->spellID < -1 ||
				currFx->dontKill < 0 ||
				currFx->loopInterval < 500.0f ||
				currFx->iterCount < 0 && currFx->exCndFuncIndex < 0)
				return false;

			return true;
		}

		int addFX
		(
			int outerFxID,
			int damage,
			oEDamageIndex damageIndex,
			int spellID,
			zSTRING strVisualFX,
			int dontKill,
			float loopInterval,
			int iterCount,
			int exCndFuncIndex
		)
		{
			fx* currFx = getFx(outerFxID);

			if (currFx && isFxValid(currFx))
				return outerFxID;


			fx newFx{};

			newFx.id = nextFxID++;

			newFx.damage = damage;
			newFx.damageIndex = damageIndex;
			newFx.spellID = spellID;
			newFx.strVisualFX = strVisualFX;
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
			ctx* currCtx = getCtx(outerCtxID);
			fx* currFx = getFx(fxID);

			if (currCtx && currCtx->isRunning)
				return outerCtxID;
			if (!isFxValid(currFx))
				return outerCtxID;


			ctx newCtx{};

			newCtx.id = nextCtxID++;
			newCtx.type = CTX_LOOP;
			newCtx.fxID = fxID;

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damage = currFx->damage;
			newCtx.damageIndex = currFx->damageIndex;
			newCtx.spellID = currFx->spellID;
			newCtx.strVisualFX = currFx->strVisualFX;
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
			int damage,
			oEDamageIndex damageIndex,
			int spellID,
			zSTRING strVisualFX,
			int dontKill,
			float loopInterval = -1.0f,
			int iterCount = -1,
			int exCndFuncIndex = -1
		)
		{
			ctx* currCtx = getCtx(outerCtxID);

			if (currCtx && currCtx->isRunning)
				return outerCtxID;


			ctx newCtx{};

			newCtx.id = nextCtxID++;
			newCtx.type = (loopInterval >= 500.0f) && (iterCount >= 0 || exCndFuncIndex >= 0) ? CTX_LOOP : CTX_REGULAR;

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damage = damage;
			newCtx.damageIndex = damageIndex;
			newCtx.spellID = spellID;
			newCtx.strVisualFX = strVisualFX;
			newCtx.dontKill = dontKill;
			newCtx.loopInterval = loopInterval;
			newCtx.iterCount = iterCount;
			newCtx.exCndFuncIndex = exCndFuncIndex;

			ctxCollection[newCtx.id] = newCtx;

			return newCtx.id;
		}

		bool isCtxValid(ctx* currCtx)
		{
			if (currCtx == nullptr ||
				currCtx->id < 0 ||
				currCtx->type == CTX_UNKNOWN ||
				currCtx->fxID < -1 ||
				currCtx->damageSender == nullptr ||
				currCtx->damageReceiver == nullptr ||
				currCtx->damage < 0 ||
				currCtx->damageIndex < 0 ||
				currCtx->damageIndex >= oEDamageIndex_MAX ||
				currCtx->spellID < -1 ||
				currCtx->dontKill < 0 ||
				(currCtx->isRunning || currCtx->isApplying) && currCtx->isCompleted)
				return false;

			if (currCtx->type == CTX_LOOP)
			{
				if (currCtx->loopInterval < 500.0f ||
					currCtx->iterCount < 0 && currCtx->exCndFuncIndex < 0 ||
					(!!currCtx->currIter) != (!!currCtx->lastIterTime))
					return false;
			}

			return true;
		}

		void runCtx(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			if (!isCtxValid(currCtx)) return;

			if (currCtx->isRunning || currCtx->isApplying || currCtx->isCompleted) return;

			currCtx->isRunning = true;

			ctxQueue.push_back(ctxID);

			return;
		}

		void closeCtx(int ctxID)
		{
			auto it = ctxCollection.find(ctxID);

			if (it == ctxCollection.end()) return;

			Union::StringANSI::Format(zSTRING("Ctx with id: {0}, was closed! Params: type: {1}, isCtxValid: {2}, isRn: {3}, isCmp: {4}, dmg: {5}, currIter: {6}, currTime: {7}, lastIterTime: {8}, loopInterval: {9}, "), it->second.id, it->second.type, isCtxValid(getCtx(it->second.id)), getCtxIsRunning(it->second.id), getCtxIsCompleted(it->second.id), getCtxDamage(it->second.id), getCtxCurrIter(it->second.id), it->second.currIter, it->second.lastIterTime, it->second.loopInterval).StdPrintLine();

			it->second.isApplying = false;
			it->second.isRunning = false;
			it->second.isCompleted = true;
		}

		void updateCtxQueue()
		{
			/* for (auto& [id, currFx] : fxCollection)
			{
				Union::StringANSI::Format(zSTRING("fxCollection.id {0}: fxID: {1}"), id, currFx.id).StdPrintLine();
			} */
			/* for (auto& [id, currCtx] : ctxCollection)
			{
				Union::StringANSI::Format(zSTRING("size: {0}, ctxCollection.id {1}: ctxID: {2}"), ctxCollection.size(), id, currCtx.id).StdPrintLine();
			} */

			for (auto it = ctxQueue.begin(); it != ctxQueue.end();)
			{
				ctx* currCtx = getCtx(*it);

				if (!isCtxValid(currCtx) || !currCtx->isRunning || currCtx->isCompleted)
				{
					closeCtx(*it);
					it = ctxQueue.erase(it); continue;
				}


				if (currCtx->exCndFuncIndex >= 0)
				{
					void* pRet = parser->CallFunc(currCtx->exCndFuncIndex);
					int isFuncTrue = *reinterpret_cast<int*>(pRet);

					if (isFuncTrue)
					{
						closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}
				}
				//Union::StringANSI::Format(zSTRING("ctxQueue | id: {0}, type: {1}, isRn: {2}, isCmp: {3}, dmg: {4} currIter: {5}, iterCount: {6}, exCndFuncIdx: {7}, "), currCtx->id, currCtx->type, getCtxIsRunning(currCtx->id), getCtxIsCompleted(currCtx->id), getCtxDamage(currCtx->id), currCtx->currIter, currCtx->iterCount, currCtx->exCndFuncIndex).StdPrintLine();


				if (currCtx->type == CTX_REGULAR)
				{
					// filling damage descriptor
					
					oCNpc::oSDamageDescriptor dd {};

					dd.dwFieldsValid =
						oCNpc::oEDamageDescFlag_Damage |
						oCNpc::oEDamageDescFlag_DamageType |
						oCNpc::oEDamageDescFlag_Attacker |
						oCNpc::oEDamageDescFlag_Npc |
						oCNpc::oEDamageDescFlag_VisualFX |
						oCNpc::oEDamageDescFlag_SpellID |
						oCNpc::oEDamageDescFlag_HitLocation |
						oCNpc::oEDamageDescFlag_FlyDirection;

					dd.pVobAttacker = currCtx->damageSender;
					dd.pNpcAttacker = currCtx->damageSender;
					dd.pVobHit = currCtx->damageReceiver;
					dd.enuModeDamage = GetDamageType(currCtx->damageIndex);
					dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
					dd.fDamageTotal = currCtx->damage;
					dd.nSpellID = currCtx->spellID;
					dd.strVisualFX = currCtx->strVisualFX;

					#if ENGINE == Engine_G2A
						dd.nSpellCat = 2;
						dd.bDamageDontKill = currCtx->dontKill;
					#endif

					dd.fDamageMultiplier = 1.0f;
					dd.vecLocationHit = currCtx->damageReceiver->GetPositionWorld();
					dd.vecDirectionFly =
					(
						currCtx->damageReceiver->GetPositionWorld() -
						currCtx->damageSender->GetPositionWorld()
					).Normalize();

					// damage applying
					currCtx->isApplying = true;
					currCtx->damageReceiver->OnDamage(dd);
					currCtx->isApplying = false;
					// damage applying

					closeCtx(*it);
					it = ctxQueue.erase(it); continue;
				}
				if (currCtx->type == CTX_LOOP)
				{
					float currTime = ogame->GetWorldTimer()->GetFullTime();

					bool isFirstIter =
					(
						currCtx->currIter == -1 &&
						currCtx->lastIterTime == -1.0f
					);

					bool isCooldownPassed =
					(
						currTime >= (currCtx->lastIterTime + currCtx->loopInterval)
					);

					bool isItersExceeded =
					(
						currCtx->iterCount >= 0 && currCtx->currIter >= currCtx->iterCount
					);

					bool isReceiverUnconscious =
					(
						!currCtx->damageReceiver->IsMonster() && currCtx->damageReceiver->IsUnconscious()
					);

					bool isReceiverDead =
					(
						currCtx->damageReceiver->IsDead()
					);


					if (isItersExceeded || isReceiverUnconscious || isReceiverDead)
					{
						Union::StringANSI::Format(zSTRING("closed in unexpected place. isItersExceeded: {0}, isRecDead: {1}, isRecUnc: {2}"), isItersExceeded, isReceiverDead, isReceiverUnconscious).StdPrintLine();
						closeCtx(*it);
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

					currCtx->currIter++;
					currCtx->lastIterTime = currTime;


					// filling damage descriptor

					oCNpc::oSDamageDescriptor dd {};

					dd.dwFieldsValid =
						oCNpc::oEDamageDescFlag_Damage |
						oCNpc::oEDamageDescFlag_DamageType |
						oCNpc::oEDamageDescFlag_Attacker |
						oCNpc::oEDamageDescFlag_Npc |
						oCNpc::oEDamageDescFlag_VisualFX |
						oCNpc::oEDamageDescFlag_SpellID |
						oCNpc::oEDamageDescFlag_HitLocation |
						oCNpc::oEDamageDescFlag_FlyDirection;

					dd.pVobAttacker = currCtx->damageSender;
					dd.pNpcAttacker = currCtx->damageSender;
					dd.pVobHit = currCtx->damageReceiver;
					dd.enuModeDamage = GetDamageType(currCtx->damageIndex);
					dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
					dd.fDamageTotal = currCtx->damage;
					dd.nSpellID = currCtx->spellID;
					dd.strVisualFX = currCtx->strVisualFX;

					#if ENGINE == Engine_G2A
						dd.nSpellCat = 2;
						dd.bDamageDontKill = currCtx->dontKill;
					#endif

					dd.fDamageMultiplier = 1.0f;
					dd.vecLocationHit = currCtx->damageReceiver->GetPositionWorld();
					dd.vecDirectionFly =
					(
						currCtx->damageReceiver->GetPositionWorld() -
						currCtx->damageSender->GetPositionWorld()
					).Normalize();

					Union::StringANSI::Format(zSTRING("APPLYING CTX WITH ID {0} AND ITER {1} AND DAMAGE {2}"), currCtx->id, currCtx->currIter, currCtx->damage).StdPrintLine();
					Union::StringANSI::Format(zSTRING("Sender here is {0} AND receiverAdr is {1}"), currCtx->damageSender->GetName(0), (int)currCtx->damageReceiver).StdPrintLine();

					// damage applying
					currCtx->isApplying = true;
					Union::StringANSI::Format(zSTRING("filedsValid: {0}, dd.aryDamage {1} AND dd.fDamageTotal {2} AND damageType {3}"), dd.dwFieldsValid, dd.aryDamage[dd.enuModeDamage], dd.fDamageTotal, dd.enuModeDamage).StdPrintLine();
					currCtx->damageReceiver->OnDamage(dd);
					currCtx->isApplying = false;
					// damage applying
				}

				++it;
			}
		}

		void clearCtxQueue()
		{
			ctxQueue.clear();
			Union::StringANSI::Format(zSTRING("ctxQueue was cleared! ctxQueue size: {0}"), ctxQueue.size()).StdPrintLine();
		}


		//getters

		int getCtxFxID(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->fxID : -1;
		}
		oCNpc* getCtxDamageSender(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->damageSender : nullptr;
		}
		oCNpc* getCtxDamageReceiver(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->damageReceiver : nullptr;
		}
		int getCtxDamage(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->damage : -1;
		}
		int getCtxDamageIndex(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->damageIndex : -1;
		}
		int getCtxCurrIter(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->currIter : -1;
		}
		float getCtxLoopInterval(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->loopInterval : -1.0f;
		}
		int getCtxIterCount(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->iterCount : -1;
		}
		int getCtxSpellID(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->spellID : -1;
		}
		zSTRING getCtxStrVisualFX(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->strVisualFX : zSTRING("");
		}
		int getCtxDontKill(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->dontKill : -1;
		}
		float getCtxLastIterTime(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->lastIterTime : -1.0f;
		}

		bool getCtxIsRunning(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->isRunning : false;
		}
		bool getCtxIsApplying(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->isApplying : false;
		}
		bool getCtxIsCompleted(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			return currCtx ? currCtx->isCompleted : false;
		}


		//setters

		void setCtxDamage(int ctxID, int newDamage)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newDamage >= 0) currCtx->damage = newDamage;
		}
		void setCtxDamageIndex(int ctxID, oEDamageIndex newDamageIndex)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newDamageIndex >= 0 && newDamageIndex < oEDamageIndex_MAX)
				currCtx->damageIndex = newDamageIndex;
		}
		void setCtxSpellID(int ctxID, int newSpellID)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newSpellID >= -1) currCtx->spellID = newSpellID;
		}
		void setCtxStrVisualFX(int ctxID, zSTRING newStrVisualFX)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newStrVisualFX) currCtx->strVisualFX = newStrVisualFX;
		}
		void setCtxDontKill(int ctxID, int newDontKill)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newDontKill >= 0) currCtx->dontKill = newDontKill;
		}
		void setCtxLoopInterval(int ctxID, float newLoopInterval)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newLoopInterval >= 500.0f) currCtx->loopInterval = newLoopInterval;
		}
		void setCtxIterCount(int ctxID, int newIterCount)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newIterCount >= -1) currCtx->iterCount = newIterCount;
		}
		void setCtxExCndFuncIndex(int ctxID, int newExCndFuncIndex)
		{
			ctx* currCtx = getCtx(ctxID);

			if (currCtx && newExCndFuncIndex >= -1) currCtx->exCndFuncIndex = newExCndFuncIndex;
		}

	};

	UCS_Manager ucsManager;


	// EXTERNAL FUNCTIONS /////

	int __cdecl UCS_CreateLoopDamage()
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
		outerFxID = parser->PopVarAddress();

		if (damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 500.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int fxID = ucsManager.addFX(*outerFxID, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

			*outerFxID = fxID;

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("Call UCS_CLD FUNC")).StdPrintLine();
			Union::StringANSI(zSTRING("fxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(fxID)).StdPrintLine();
			Union::StringANSI(zSTRING(" ")).StdPrintLine();
		}

		return 0;
	};

	int __cdecl UCS_ApplyDamage()
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

			int ctxID = ucsManager.addCtx(-1, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill);

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

		Union::StringANSI::Format(zSTRING("STARTLOOP outerCtxID: {0}, fxID: {1}, receiverAdr: {2}!"), outerCtxID, fxID, (int)damageReceiver).StdPrintLine();

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

		outerCtxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 500.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int ctxID = ucsManager.addCtx(*outerCtxID, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

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

	int __cdecl UCS_StopLoopDamage()
	{
		int* outerCtxID; outerCtxID = parser->PopVarAddress();

		ucsManager.closeCtx(*outerCtxID);

		*outerCtxID = -1;

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("Call UCS_STOPLD FUNC")).StdPrintLine();
		Union::StringANSI(zSTRING("ctxCollection.id: ")).StdPrint(); Union::StringANSI(zSTRING(*outerCtxID)).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		return 0;
	};

	// external getter functions

	int __cdecl UCS_IsRunning()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int isRunning = ucsManager.getCtxIsRunning(outerCtxID);

		parser->SetReturn(isRunning); return 0;
	};
	int __cdecl UCS_IsCompleted()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int isCompleted = ucsManager.getCtxIsCompleted(outerCtxID);

		parser->SetReturn(isCompleted); return 0;
	};
	int __cdecl UCS_GetFxID()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int fxID = ucsManager.getCtxFxID(outerCtxID);

		parser->SetReturn(fxID); return 0;
	};
	int __cdecl UCS_GetSender()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		oCNpc* damageSender = ucsManager.getCtxDamageSender(outerCtxID);

		parser->SetReturn(damageSender); return 0;
	};
	int __cdecl UCS_GetReceiver()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		oCNpc* damageReceiver = ucsManager.getCtxDamageReceiver(outerCtxID);

		parser->SetReturn(damageReceiver); return 0;
	};
	int __cdecl UCS_GetDamage()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int damage = ucsManager.getCtxDamage(outerCtxID);

		Union::StringANSI::Format(zSTRING("getDamage: {0}"), damage).StdPrintLine();

		parser->SetReturn(damage); return 0;
	};
	int __cdecl UCS_GetDamageIndex()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int damageIndex = ucsManager.getCtxDamageIndex(outerCtxID);

		parser->SetReturn(damageIndex); return 0;
	};
	int __cdecl UCS_GetCurrentIter()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int currentIter = ucsManager.getCtxCurrIter(outerCtxID);

		Union::StringANSI::Format(zSTRING("EXTERNAL GET CURRENT ITER {0} OUT OF CTX WITH ID: {1}!"), currentIter, outerCtxID).StdPrintLine();

		parser->SetReturn(currentIter); return 0;
	};
	int __cdecl UCS_GetLoopInterval()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		float loopInterval = ucsManager.getCtxLoopInterval(outerCtxID);

		parser->SetReturn(loopInterval); return 0;
	};
	int __cdecl UCS_GetIterCount()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int iterCount = ucsManager.getCtxIterCount(outerCtxID);

		parser->SetReturn(iterCount); return 0;
	};
	int __cdecl UCS_GetSpellID()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int spellID = ucsManager.getCtxSpellID(outerCtxID);

		parser->SetReturn(spellID); return 0;
	};
	int __cdecl UCS_GetVisualFX()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		zSTRING visualFX = ucsManager.getCtxStrVisualFX(outerCtxID);

		parser->SetReturn(visualFX); return 0;
	};
	int __cdecl UCS_GetDontKill()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		int dontKill = ucsManager.getCtxDontKill(outerCtxID);

		parser->SetReturn(dontKill); return 0;
	};
	int __cdecl UCS_GetLastIterTime()
	{
		int outerCtxID; parser->GetParameter(outerCtxID);

		float lastIterTime = ucsManager.getCtxLastIterTime(outerCtxID);

		parser->SetReturn(lastIterTime); return 0;
	};

	// external setter functions

	int __cdecl UCS_SetDamage()
	{
		int outerCtxID; int newDamage;

		parser->GetParameter(newDamage); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) { ucsManager.setCtxDamage(outerCtxID, newDamage); Union::StringANSI::Format(zSTRING("EXTERNAL SET DAMAGE IN CTX WITH ID: {0} TO {1}!"), outerCtxID, newDamage).StdPrintLine(); } return 0;
	};
	int __cdecl UCS_SetDamageIndex()
	{
		int outerCtxID; int newDamageIndex;

		parser->GetParameter(newDamageIndex); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxDamageIndex(outerCtxID, (oEDamageIndex)newDamageIndex); return 0;
	};
	int __cdecl UCS_SetSpellID()
	{
		int outerCtxID; int newSpellID;

		parser->GetParameter(newSpellID); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxSpellID(outerCtxID, newSpellID); return 0;
	};
	int __cdecl UCS_SetVisualFX()
	{
		int outerCtxID; zSTRING newVisualFX;

		parser->GetParameter(newVisualFX); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxStrVisualFX(outerCtxID, newVisualFX); return 0;
	};
	int __cdecl UCS_SetDontKill()
	{
		int outerCtxID; int newDontKill;

		parser->GetParameter(newDontKill); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxDontKill(outerCtxID, newDontKill); return 0;
	};
	int __cdecl UCS_SetLoopInterval()
	{
		int outerCtxID; float newLoopInterval;

		parser->GetParameter(newLoopInterval); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxLoopInterval(outerCtxID, newLoopInterval); return 0;
	};
	int __cdecl UCS_SetIterCount()
	{
		int outerCtxID; int newIterCount;

		parser->GetParameter(newIterCount); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxIterCount(outerCtxID, newIterCount); return 0;
	};
	int __cdecl UCS_SetExitCondition()
	{
		int outerCtxID; int newExCndFuncIndex;

		parser->GetParameter(newExCndFuncIndex); parser->GetParameter(outerCtxID);

		if (ucsManager.getCtxIsApplying(outerCtxID)) ucsManager.setCtxExCndFuncIndex(outerCtxID, newExCndFuncIndex); return 0;
	};


	void Game_DefineExternals_ApplyDamage()
	{
		parser->DefineExternal("UCS_CreateLoopDamage", UCS_CreateLoopDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_ApplyDamage", UCS_ApplyDamage, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartLoopDamage", UCS_StartLoopDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartLoopDamageEx", UCS_StartLoopDamageEx, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_StopLoopDamage", UCS_StopLoopDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_IsRunning", UCS_IsRunning, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_IsCompleted", UCS_IsCompleted, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetFxID", UCS_GetFxID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetSender", UCS_GetSender, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetReceiver", UCS_GetReceiver, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamage", UCS_GetDamage, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamageIndex", UCS_GetDamageIndex, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetCurrentIter", UCS_GetCurrentIter, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLoopInterval", UCS_GetLoopInterval, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetIterCount", UCS_GetIterCount, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetSpellID", UCS_GetSpellID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetVisualFX", UCS_GetVisualFX, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDontKill", UCS_GetDontKill, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLastIterTime", UCS_GetLastIterTime, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_SetDamage", UCS_SetDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDamageIndex", UCS_SetDamageIndex, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetSpellID", UCS_SetSpellID, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetVisualFX", UCS_SetVisualFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDontKill", UCS_SetDontKill, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetLoopInterval", UCS_SetLoopInterval, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetIterCount", UCS_SetIterCount, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);
	}
}