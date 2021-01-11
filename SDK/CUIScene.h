#pragma once

#include "TextHolder.h"

class C_UIScene {
	/*
private:
	virtual ~C_UIScene();
	// Duplicate destructor
	virtual __int64 init(__int64 const &);

public:
	virtual void setSize(__int64 const &);

private:
	virtual void unknown();
	virtual __int64 onSetKeyboardHeight(float);
	virtual __int64 onInternetUpdate(void);
	virtual __int64 getInputAreas(void) const;
	virtual __int64 onFocusGained(void);
	virtual __int64 onFocusLost(void);
	virtual __int64 terminate(void);
	virtual __int64 onCreation(void);
	virtual __int64 onLeave(void);
	virtual __int64 onGameEventNotification(__int64);
	virtual __int64 _handleDirtyVisualTree(bool);
	virtual __int64 leaveScreen(void);
	virtual __int64 tick(int, int);
	virtual __int64 updateEvents(void);
	virtual __int64 applyInput(float);
	virtual __int64 update(double);
	virtual __int64 preRenderUpdate(__int64 &);
	virtual __int64 prepareFrame(__int64 &);
	virtual __int64 render(__int64 &, __int64 const &);
	virtual __int64 postRenderUpdate(__int64 &);

public:
	virtual void setupAndRender(__int64 &);

private:
	virtual __int64 handleInputModeChanged(__int64);
	virtual __int64 handleHoloInputModeChanged(__int64);
	virtual __int64 handleButtonPress(unsigned int, __int64);
	virtual __int64 handleButtonRelease(unsigned int, __int64);
	virtual __int64 handleRawInputEvent(int, __int64, __int64, bool);
	virtual __int64 handlePointerLocation(__int64 const &, __int64);
	virtual __int64 handlePointerPressed(bool);
	virtual __int64 handleDirection(__int64, float, float, __int64);
	virtual __int64 handleBackEvent(bool);
	virtual __int64 handleTextChar(__int64 const &, bool, __int64);
	virtual __int64 handleCaretLocation(int, __int64);
	virtual __int64 unk();
public:
	virtual void setTextboxText(__int64 const &);

private:
	virtual __int64 onKeyboardDismissed(void);
	virtual __int64 handleLicenseChanged(void);
	virtual __int64 handleIdentityGained(void);
	virtual __int64 handleIdentityLost(void);
	virtual __int64 handleGazeGestureInput(short, float, float, float, __int64);
	virtual __int64 handleDictationEvent(__int64 const &);
	virtual __int64 handleCommandEvent(__int64 const &);
	virtual __int64 renderGameBehind(void) const;
	virtual __int64 absorbsInput(void) const;
	virtual __int64 closeOnPlayerHurt(void) const;

public:
	virtual bool isModal(void) const;
	virtual bool isShowingMenu(void) const;

private:
	virtual __int64 shouldStealMouse(void) const;
	virtual __int64 screenIsNotFlushable(void) const;
	virtual __int64 alwaysAcceptsInput(void) const;
	virtual __int64 screenDrawsLast(void) const;

public:
	virtual bool isPlayScreen(void) const;

private:
	virtual __int64 renderOnlyWhenTopMost(void) const;
	virtual __int64 lowFreqRendering(void) const;
	virtual __int64 ignoreAsTop(void) const;
	virtual __int64 screenHandlesGamepadMenuButton(void) const;
	virtual __int64 getWidth(void);
	virtual __int64 getHeight(void);
	virtual __int64 reload(void);
	virtual __int64 getRenderingAABB(void);
	virtual __int64 getEyeRenderingMode(void) const;
	virtual __int64 getSceneType(void) const;

public:
	virtual TextHolder *getScreenName(TextHolder *text) const;

private:
	virtual __int64 getScreenTelemetryName(void) const;
	virtual __int64 getScreenNameW(void) const;
	virtual __int64 addEventProperties(__int64 const &) const;
	virtual __int64 getScreenVersion(void) const;
	virtual __int64 processBufferedTextCharEvents(__int64 const &);
	virtual __int64 getShouldSendEvents(void);

public:
	virtual void setShouldSendEvents(bool);

private:
	virtual __int64 getWantsTextOnly(void);

public:
	virtual void setWantsTextOnly(bool);

private:
	virtual __int64 onDelete(__int64 &, __int64 &);

public:
	virtual bool isGamepadCursorEnabled(void) const;

private:
	virtual __int64 getGamepadCursorPosition(void) const;
	virtual __int64 cleanInputComponents(void);
	virtual __int64 getProxy(void);

public:
	virtual bool canBePushed(void) const;
	virtual bool canBePopped(void) const;
	virtual bool canBeTransitioned(void) const;

private:
	virtual __int64 onScreenExit(bool, bool);
	virtual __int64 onScreenEntrance(bool, bool);

public:
	virtual bool isEntering(void) const;
	virtual bool isExiting(void) const;

private:
	virtual __int64 schedulePop(void);

public:
	virtual bool isTerminating(void) const;

private:
	virtual __int64 loadScreenImmediately(void) const;
	virtual __int64 forceUpdateActiveSceneStackWhenPushed(void) const;

public:
	virtual bool hasFinishedLoading(void) const;

private:
	virtual __int64 sendScreenEvent(__int64 const &, __int64 const &);

public:
	virtual void setDebugFeature(__int64, bool) const;
	virtual void setupForRendering(__int64 &);

private:
	virtual __int64 cleanupForRendering(__int64 &);*/

public:
	bool isPlayScreen() {
		return Utils::CallVFunc<55, bool>(this);
	}

	void getScreenName(TextHolder* txt) {
		Utils::CallVFunc<67, void, TextHolder*>(this, txt);
	}
};
