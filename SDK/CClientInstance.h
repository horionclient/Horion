#pragma once

#include "CEntity.h"
#include "CLoopbackPacketSender.h"
#include "TextHolder.h"

class Minecraft;
class LevelRenderer {
private:
	char pad_0x0000[0x850]; //0x0000
public:
	vec3_t origin; //0x0850
};
class HitDetectSystem;

class MinecraftGame {
private:
	char filler[0xA0];//0x0000
	uintptr_t goodFont;//0x00A0
	char pad_0x00A8[0x170]; //0x00A8
	bool canUseKeys;//0x0218
public:
	
	uintptr_t getTheGoodFontThankYou() {
		return goodFont;
	};

	const bool canUseKeybinds() {
		return canUseKeys;
	};
};

class C_GuiData {
private:
	char pad_0x0000[0x18]; //0x0000
public:
	union {
		struct {
			float widthReal; //0x0018 
			float heightReal; //0x001C 
		};
		vec2_t windowSizeReal; //0x0018
	};
	
	float widthReal2; //0x0020 
	float heightReal2; //0x0024 
	union {
		struct {
			float widthGame; //0x0028 
			float heightGame; //0x002C 
		};
		vec2_t windowSize;  //0x0028 
	};
	

	void displayClientMessageVA(const char * fmt, va_list lis) {
		char message[300];
		int numCharacters = vsprintf_s(message, 300, fmt, lis);
		displayClientMessage(&std::string(message));
	}

	void displayClientMessageF(const char * fmt, ...) {
		va_list arg;
		va_start(arg, fmt);
		displayClientMessageVA(fmt, arg);
		va_end(arg);
	}
	void displayClientMessage(std::string* a2) {
		using displayClientMessage = void(__thiscall*)(void*, TextHolder); // This signature actually exists 2 times but we got luck that our function is the first in memory
		static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(Utils::FindSignature("4C 8B DC 53 48 ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 48 8B D9 49 ?? ?? ?? ?? ?? ?? ?? 33 C0 49 89 43 D8 41 88 43 C8 49"));
		
		TextHolder text = TextHolder(*a2);
		
		displayMessageFunc(this, text);
	};
};

class C_ClientInstance {
private:
	char firstPad[0x40]; //0x0008
public:
	MinecraftGame* minecraftGame; //0x0048 
private:
	MinecraftGame* N0000080C; //0x0050 
	MinecraftGame* N0000080D; //0x0058
	MinecraftGame* N0000080E; //0x0060
public:
	Minecraft* minecraft; //0x0068
private:
	char pad_0x0068[0x8]; //0x0070
public:
	LevelRenderer* levelRenderer; //0x0078
private:
	char pad_0x0078[0x8]; //0x0080
public:
	C_LoopbackPacketSender* loopbackPacketSender; //0x0088
private:
	char pad_0x0088[0x28]; //0x0090
public:
	HitDetectSystem* hitDetectSystem; //0x00B8
private:
	char pad_0x00B8[0x30]; //0x00C0
public:
	C_LocalPlayer* localPlayer; //0x00F0


private:
	virtual __int64 destructorClientInstance(); //1
	// Duplicate destructor
	virtual __int64 onInitMinecraftGame(void); //2
	virtual __int64 onDestroyMinecraftGame(void); //3
	virtual __int64 init(__int64 *, __int64 &, __int64 &, __int64 &, __int64 &, int); //4
	virtual __int64 initSceneFactory(__int64 *); //5
	virtual __int64 initGraphics(void); //6
	virtual __int64 stop(void); //7
	virtual __int64 teardown(void); //8
	virtual __int64 preShutDown(void); //9
	virtual __int64 requestLeaveGame(bool, bool); //10
	virtual __int64 stopPlayScreen(void); //11
public:
	virtual bool isLeaveGameDone(void)const; //12
	virtual void setupPlayScreenForLeaveGame(void); //13
private:
	virtual __int64 resetPrimaryClient(void); //14
	virtual __int64 resetGameSession(void); //15
	virtual __int64 tick(void); //16
	virtual __int64 frameUpdate(__int64 &); //17
	virtual __int64 update(bool); //18
	virtual __int64 endFrame(void); //19
	virtual __int64 startSubClientLateJoin(bool); //20
public:
	virtual void setupClientGame(__int64 &&, bool); //21
private:
	virtual __int64 sub_1400C5D00(void) const;
	virtual __int64 getLocalC_Player(void); //22
	virtual __int64 getLocalC_Player(void)const; //23
	virtual __int64 getCameraEntity(void)const; //24
public:
	virtual void setCameraEntity(C_Entity *); //25
private:
	virtual __int64 getCameraTargetEntity(void)const; //26
public:
	virtual void setCameraTargetEntity(C_Entity *); //27
	virtual bool isLeavingGame(void)const; //28
	virtual bool isDestroyingGame(void)const; //29
private:
	virtual __int64 sub_1400C98E0(void) const;
	virtual __int64 useLowFrequencyUIRender(void)const; //30
public:
	virtual bool isSplitScreenActive(void)const; //31
	virtual bool isVRClient(void)const; //32
	virtual bool isARClient(void)const; //33
	virtual bool isARVRClient(void)const; //34
	virtual bool isHoloviewerMode(void)const; //35
	virtual bool isHoloscreenMode(void)const; //36
	virtual bool isLivingroomMode(void)const; //37
	virtual bool isExitingLevel(void)const; //38
	virtual bool isNotVLRMode(void)const; //39
	virtual bool isHoloRealityMode(void)const; //40
	virtual bool isRealityFullVRMode(void)const; //41
	virtual bool isInBedScreen(void)const; //42
	virtual bool isKeyboardEnabled(void)const; //43
	virtual bool isVRTransitioning(void)const; //44
	virtual bool hasCommands(void)const; //45
	virtual bool isScriptingEnabled(void)const; //46
private:
	virtual __int64 getHoloviewerScale(void)const; //47
	virtual __int64 getSplitScreenCount(void)const; //48
public:
	virtual bool isStereoRendering(void)const; //49
	virtual bool isPlatformNX(void)const; //50
	virtual bool isLocalSplitscreenWith(__int64 const&)const; //51
	virtual bool isValidCrossPlatformSkin(void)const; //52
	virtual __int64 sub_1400C9B70(void);
private:
	virtual __int64 getSplitScreenInfo(void)const; //53
	virtual __int64 getCurrentMaxGUIScaleIndex(void)const; //54
	virtual __int64 getRealityModeFrameFactor(void)const;  //55
public:
	virtual void setRealityModeFrameFactor(float const&); //56
private:
	virtual __int64 getRealityModeToggleTriggered(void)const; //57
public:
	virtual void setRealityModeToggleTriggered(bool); //58
private:
	virtual bool sub_1400C9C50(void);
public:
	virtual void setOpenControllerDisconnectScreen(bool); //59
private:
	virtual __int64 getClientPlayMode(void)const; //60
public:
	virtual void setClientPlayMode(__int64 const&); //61
private:
	virtual __int64 getLivingRoomForCredits(void)const; //62
public:
	virtual void setLivingRoomForCredits(bool); //63
private:
	virtual __int64 getCreditsCallback(void); //64
public:
	virtual void setCreditsCallback(__int64); //65
	virtual void setupTransitionForCredits(__int64); //66
private:
	virtual __int64 refreshScreenSizeData(void); //67
	virtual __int64 onScreenSizeChanged(int, int, float); //68
	virtual __int64 onGuiScaleOffsetChanged(void); //70
	virtual __int64 onSafeZoneChanged(void); //71
	virtual __int64 quit(std::string const&, std::string const&); //72
	virtual __int64 getMinecraftGame(void)const; //73
	virtual __int64 getAutomationClient(void)const; //74
	virtual __int64 getScreenshotStreamer(void)const; //75
	virtual __int64 getEventing(void)const; //76
	virtual __int64 sub_1400CA2E0(__int64 a1);
	virtual __int64 sub_1400CA2E8(__int64 a1);
	virtual __int64 sub_1400CA2F0(__int64 a1);
	virtual __int64 getFont(void)const; //77
	virtual __int64 getRuneFont(void)const; //78
	virtual __int64 getUnicodeFont(void)const; //79
	virtual __int64 getGeometryGroup(void)const; //80
	virtual __int64 getMultiplayerServiceManager(void)const; //81
	virtual __int64 getLocalServerLevel(void); //82
	virtual __int64 getResourcePackRepository(void)const; //83
	virtual __int64 getResourcePackManager(void)const; //84
	virtual __int64 getSkinRepository(void)const; //89
	virtual __int64 getSkinRepositoryClientInterface(void)const; //90
	virtual __int64 sub_1400CA400(void) const;
	virtual __int64 getTextures(void)const; //91
	virtual __int64 getStoreCacheTextures(void)const; //92
	virtual __int64 getMinecraftGraphics(void)const; //93
	virtual __int64 getUIRepository(void)const; //94
	virtual __int64 getUserManager(void)const; //95
	virtual __int64 wantToQuit(void)const; //96
public:
	virtual bool isPrimaryLevelCrossPlatformMultiplayer(void)const; //97
	virtual bool isPrimaryLevelMultiplayer(void)const; //98
	virtual bool isAdhocEnabled(void)const; //99
private:
	virtual __int64 linkToOffer(std::string const&, bool, std::string const&, __int64); // 100
	virtual __int64 linkToAllOffers(std::string const&, std::string const&, bool, std::string const&, bool);//101
	virtual __int64 navigateToBrazeScreen(std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&); //102
	virtual __int64 navigateToStoreHomeScreen(void); //103
	virtual __int64 navigateToCoinPurchaseScreen(int, __int64); //104
	virtual __int64 navigateToPurchaseOfferScreen(__int64, bool); //105
	virtual __int64 navigateToOfferCollectionScreen(__int64 &); //106
	virtual __int64 navigateToStoreSeeAllByCreatorScreen(std::string const&, bool); //107
	virtual __int64 navigateToServersScreen(bool); //108
	virtual __int64 navigateToHowToPlayScreen(std::string const&); //109
	virtual __int64 tryPushLeaveGameScreen(void); //110
	virtual char sub_1400D16A0(void) const;
public:
	virtual bool isReadyToRender(void)const; //111
private:
	virtual __int64 onDimensionChanged(void); //112
	virtual __int64 onGameEventNotification(__int64); //113
	virtual __int64 getTopScreenName(void)const; //114
public:
	virtual void setLeaveGameInProgressAsReadyToContinue(void); //115
private:
	virtual __int64 stopDestroying(void); //116
	virtual __int64 onClientCreatedLevel(__int64); //117
	virtual __int64 getClientRandomId(void)const; //118
	virtual __int64 getUserAuthentication(void); //119
	virtual __int64 sub_1400CAC50(__int64* a2);
	virtual __int64 registerToUserManager(__int64 &, int); //120
	virtual __int64 resumeWithUserManager(__int64 &, int); //121
	virtual __int64 createUserAuthentication(std::string); //122
	virtual __int64 createUserAuthentication(unsigned long long, std::string const&); //123
	virtual __int64 getPlatformId(void)const; //124
	virtual __int64 getPlatformOnlineId(void)const; //124
public:
	virtual bool isHoloCursorNeeded(void)const; //125
private:
	virtual __int64 useController(void)const; //126
	virtual __int64 useTouchscreen(void)const; //127
	virtual __int64 getMouseGrabbed(void)const; //128
	virtual __int64 currentInputModeIsGamePadOrMotionController(void)const; //129
	virtual __int64 currentInputModeIsMouseAndKeyboard(void)const; //130
	virtual __int64 getForceMonoscopic(void)const; //131
	virtual __int64 allowPicking(void)const; //132
public:
	virtual bool isShowingMenu(void)const; //133
	virtual bool isShowingProgressScreen(void)const; //134
	virtual bool isScreenReplaceable(void)const; //135
	virtual bool isInGame(void)const; //136
	virtual bool isInRealm(void); //137
private:
	virtual __int64 readyForShutdown(void)const; //138
public:
	virtual bool isPrimaryClient(void)const; //139
	virtual bool isEduMode(void)const; //140
	virtual bool isGamepadCursorEnabled(void)const; //141
private:
	virtual __int64 getServerData(void); //142 //148
	virtual __int64 getServerData(void)const; //143
public:
	virtual PointingStruct* getLevel(void); //144
	virtual PointingStruct* getLevel(void)const; //145
	virtual bool isPreGame(void)const; //146
	virtual bool isInMultiplayerGame(void)const; // 147
	virtual bool isMultiC_PlayerClient(void)const; //148
private:
	virtual __int64 getOptions(void); //149
	virtual __int64 getOptions(void)const; //150
	virtual __int64 getOptionsPtr(void); //151
	virtual __int64 getUser(void); //152
	virtual __int64 getUser(void)const; //153
	virtual __int64 getGameRenderer(void)const; //154
	virtual __int64 getHolosceneRenderer(void)const; //155
	virtual __int64 getLevelRenderer(void)const; //156
	virtual __int64 getLevelRendererCameraProxy(void)const; //157
	virtual __int64 sub_1400CCC00(void) const;
	virtual __int64 sub_1400CCC08(void) const;
	virtual __int64 getLightTexture(void); //158
public:
	virtual void setupLevelRendering(__int64 &, C_Entity &); //159
private:
	virtual __int64 getViewportInfo(void)const; //160
public:
	virtual void setViewportInfo(__int64 const&); //161
private:
	virtual __int64 getNormalizedViewportSize(void)const; //162
	virtual __int64 updateChunkRadius(void); //163
public:
	virtual void setUITexture(__int64 *); //164
private:
	virtual __int64 getUITexture(void); //165
public:
	virtual void setLevelTexture(__int64*); //166
private:
	virtual __int64 getLevelTexture(void); //167
public:
	virtual void setUICursorTexture(__int64); //168
private:
	virtual __int64 getUICursorTexture(void)const; //169
	virtual __int64 getCamera(void); //170
	virtual __int64 getShaderColor(void); //171
	virtual __int64 getDarkShaderColor(void); //172
	virtual __int64 clearGraphicsCache(void); //173
	virtual __int64 getNormalizedUICursorTransform(__int64 &, float); //174
	virtual __int64 shouldRenderUICursor(void)const; //175
	virtual bool sub_1400CD3B0(void);
	virtual bool sub_1400CD3B1(void);
	virtual __int64 onLatencyUpdated(std::chrono::duration<long long, std::ratio<1ll, 1000ll>> const&); //176
	virtual __int64 getGuiScale(void)const; //177
	virtual __int64 getGuiScaleOption(void)const; //178
	virtual __int64 getGuiScaleOffset(void)const; //179
public:
	virtual void setGuiScaleOffset(int); //780
private:
	virtual __int64 renderEditorGui(__int64 &, bool); //181
public:
	virtual C_GuiData* getGuiData(void); //182       //*1.13 //192
	virtual C_GuiData* getGuiData(void)const; 
private:
	virtual __int64 getGuidedFlowManager(void); 
	virtual __int64 getDpadScale(void)const; 
	virtual __int64 getDateManager(void)const;
	virtual __int64 addOverrideHoursToDateTime(unsigned int); 
	virtual __int64 getToastManager(void);
	virtual __int64 getTopScene(void); 
	virtual __int64 getTopScene(void)const;
	virtual __int64 getActiveScene(void);  
	virtual __int64 getActiveScene(void)const; 
	virtual __int64 getSceneFactory(void)const; 
	virtual __int64 getClientSceneStack(void)const; 
	virtual __int64 getClientSceneStack(void); 
	virtual __int64 getMainSceneStack(void);
	virtual __int64 getMainSceneStack(void)const; 
	virtual __int64 getCurrentSceneStack(void)const; 
	virtual __int64 getCurrentSceneStack(void); 
	virtual __int64 getCurrentUIRouter(void); 
	virtual __int64 getCurrentUIRouter(void)const;
	virtual __int64 prepareSceneFor(__int64); 
	virtual __int64 getCachedScenes(void); 
	virtual __int64 getScreenName(void)const; 
	virtual __int64 getScreenTelemetry(void)const;
	virtual __int64 getTopSceneType(void)const; 
	virtual __int64 getMobEffectsLayout(void); 
	virtual __int64 onMobEffectsChange(void); 
public:
	virtual void setUISizeAndScale(int, int, float); 
private:
	virtual __int64 forEachVisibleScreen(__int64, bool); 
	virtual __int64 forEachScreen(__int64, bool); 
	virtual __int64 forEachScreenConst(__int64, bool)const; 
	virtual __int64 updateSceneStack(void); 
	virtual __int64 forEachAlwaysAcceptInputScreen(__int64); 
	virtual __int64 forEachAlwaysAcceptInputScreenWithTop(__int64); 
	virtual __int64 showC_PlayerProfile(std::string const&, __int64); 
	virtual __int64 getCurrentInputMode(void)const; 
public:
	virtual bool isTouchGameplayAllowed(void)const; 
private:
	virtual __int64 getMinecraftInput(void)const; 
public:
	virtual void setHoloInput(__int64);
private:
	virtual __int64 getHoloInput(void)const;
	virtual __int64 getVoiceSystem(void)const; 
	virtual __int64 getKeyboardManager(void); 
public:
	virtual void setVoiceSystem(__int64); 
	virtual void setLastPointerLocation(float, float, float); 
private:
	virtual __int64 getLastPointerLocation(void);
	virtual __int64 shouldUseLastPointerLocationOnFocusChange(void); 
	virtual __int64 adjustGazeCursorByMouse(float, float); 
	virtual __int64 currentScreenShouldStealMouse(void); 
	virtual __int64 getInProgressBAI(void)const; 
	virtual __int64 newDictationDataAvailable(void)const; 
	virtual __int64 clearDictationDataAvailable(void); 
	virtual __int64 getDictationText(void)const; 
	virtual __int64 getPacketSender(void); 
	virtual __int64 getClientNetworkSystem(void); 
	virtual __int64 getClientNetworkSystem(void)const; 
public:
	virtual void setMoveTurnInput(__int64); 
private:
	virtual __int64 getMoveTurnInput(void); 
public:
	virtual void setupPersistentControls(__int64); 
private:
	virtual __int64 resetC_PlayerMovement(void); 
	virtual __int64 onClientInputInitComplete(void); 
public:
	virtual void setClientInputHandler(__int64);
private:
	virtual __int64 getInput(void)const; 
	virtual __int64 getControllerId(void)const; 
public:
	virtual bool hasConnectedController(void)const; 
private:
	virtual __int64 getClientSubId(void)const;
public:
	virtual void setSuspendInput(bool); 
	virtual void setDisableInput(bool); 

	virtual void grabMouse(void); 
	virtual void releaseMouse(void); 
	virtual void refocusMouse(void); 
private:
	virtual __int64 resetBai(int);
	virtual __int64 clearInProgressBAI(void);
	virtual __int64 tickBuildAction(void);
	virtual __int64 getSoundEngine(void)const;
	virtual __int64 play(std::string const&, vec3_t const&, float, float);
	virtual __int64 playUI(std::string const&, float, float);
	virtual __int64 muteAudio(void);
	virtual __int64 unMuteAudio(void);
	virtual __int64 fadeOutMusic(void)const;
	virtual __int64 getTaskGroup(void);
	virtual __int64 onFullVanillaPackOnStack(void);
public:
	virtual bool isFullVanillaPackOnStack(void)const;
private:
	virtual __int64 onPlayerLoaded(C_Player &);
public:
	virtual void setClientGameMode(__int64);
private:
	virtual __int64 resetToDefaultGameMode(void);
	virtual __int64 connectToThirdParyServer(std::string const&, std::string const&, int);
	virtual __int64 startExternalNetworkWorld(__int64, std::string const&, bool);
	virtual __int64 checkForPiracy(void);
	virtual __int64 updateChatFilterStatus(void);
	virtual void sub_1400D2A20(void);
public:
	virtual void setBehaviorCommandCallback(__int64);
	virtual void setBehaviorCommandStatus(std::string, __int64);
private:
	virtual __int64 requestScreenshot(__int64 &);
	virtual void sub_1400BE0A0(char a2);
	virtual __int64 getDevConsoleLogger(void)const;
	virtual __int64 requestImageFromUrl(std::string const&, __int64);
	virtual __int64 initializeRenderResources(void);
	virtual __int64 postInitRenderResources(void);
	virtual __int64 onAppSuspended(void);
	virtual __int64 onActiveResourcePacksChanged(__int64 const&);
	virtual __int64 reloadEntityRenderers(__int64 const&);
	virtual __int64 getBlockTessellator(void);
	virtual __int64 getBlockEntityRenderDispatcher(void);
	virtual __int64 getEntityRenderDispatcher(void);
	virtual __int64 getEntityBlockRenderer(void);
	virtual __int64 getItemInHandRenderer(void);
	virtual __int64 getItemRenderer(void);
	virtual __int64 getSentMessageHistory(void);
	virtual __int64 getUIProfanityContext(void)const;
	virtual __int64 initTTSClient(__int64 &);
	virtual __int64 getTTSClient(void);
	virtual __int64 getTTSClient(void)const;
	virtual __int64 getTTSEventManager(void);
	virtual __int64 addTTSMessage(std::string const&, __int64, bool, bool, bool);
	virtual __int64 initCommands(void);
	virtual __int64 getUserId(void)const;
	virtual __int64 getServerConnectionTime(void)const;
	virtual __int64 getHMDState(void);
	virtual __int64 getHMDState(void)const;
public:
	virtual void setServerPingTime(unsigned int);
private:
	virtual __int64 getServerPingTime(void)const;
public:
	virtual void setDefaultPlayscreenTab(__int64);
	virtual void setClientInstanceState(__int64 const&);
	virtual void setUIEventCoordinator(__int64 &&);
private:
	virtual __int64 getUIEventCoordinator(void);
	virtual __int64 getEventCoordinator(void);
	virtual __int64 computeScreenCoordsFromScreenNormCoords(float, float, short &, short &);
	virtual __int64 getNoBlockBreakUntil(void);
public:
	virtual void setNoBlockBreakUntil(__int64);
	virtual void setDictation(std::string const&);
	virtual void setNewDictationString(bool);
	virtual void setGameModule(__int64);
private:
	virtual __int64 getGameModule(void);
	virtual __int64 getGameCallbacks(void);
	virtual __int64 getStorageAreaStateListener(void);
	virtual __int64 getScriptEngine(void);
	virtual __int64 sendClientEnteredLevel(void);
	virtual __int64 getHitDetectSystem(void);
public:
	virtual bool isPlaying(void)const;
private:
	virtual __int64 getLatencyGraphDisplay(void)const;
	virtual __int64 createSkin(void);
	virtual __int64 onExtendDiskSpace(bool, __int64 &, unsigned long long const&, __int64);
	virtual __int64 onLowDiskSpace(bool);
	virtual __int64 onOutOfDiskSpace(bool);
	virtual __int64 onCriticalDiskError(bool, __int64 const&);
	virtual __int64 onLevelCorrupt(void);
	virtual __int64 onGameModeChanged(void);
	virtual __int64 onTick(int, int);
	virtual __int64 onInternetUpdate(void);
	virtual __int64 onGameSessionReset(void);
	virtual __int64 onLevelExit(void);
	virtual __int64 updateScreens(void);
public:


	C_LocalPlayer* getLocalPlayer() {
		return localPlayer;
	};

	PointingStruct* getPointerStruct() {
		return this->getLevel();
	}

	glmatrixf* getRefDef() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		//logF("refderf %llX", _this + 0x258);
		return reinterpret_cast<glmatrixf*>(_this + 0x268);
	};

	vec2_t* getMousePos(){
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<vec2_t*>(_this + 0x3D0);
	}

	vec2_t getFov() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		vec2_t fov;
		fov.x = *reinterpret_cast<float*>(_this + 0x5D0);
		fov.y = *reinterpret_cast<float*>(_this + 0x5E4);
		return fov;
	}

	uintptr_t _getFont() {
		
		return this->getFont();
	}

	uintptr_t _getRuneFont() {

		return this->getRuneFont();
	}

	uintptr_t _getUnicodeFont() {

		return this->getUnicodeFont();
	}
};