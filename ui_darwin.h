// 7 april 2015

/*
This file assumes that you have imported <Cocoa/Cocoa.h> and "ui.h" beforehand. It provides API-specific functions for interfacing with foreign controls on Mac OS X.
*/

#ifndef __LIBUI_UI_DARWIN_H__
#define __LIBUI_UI_DARWIN_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uiDarwinControl uiDarwinControl;
struct uiDarwinControl {
	uiControl c;
	uiControl *parent;
	BOOL enabled;
	BOOL visible;
	void (*SyncEnableState)(uiDarwinControl *, int);
	void (*SetSuperview)(uiDarwinControl *, NSView *);
	void (*SetRealHuggingPriority)(uiDarwinControl *, NSLayoutPriority, NSLayoutConstraintOrientation);
};
#define uiDarwinControl(this) ((uiDarwinControl *) (this))
// TODO document
_UI_EXTERN void uiDarwinControlSyncEnableState(uiDarwinControl *, int);
_UI_EXTERN void uiDarwinControlSetSuperview(uiDarwinControl *, NSView *);
_UI_EXTERN void uiDarwinControlSetRealHuggingPriority(uiDarwinControl *, NSLayoutPriority, NSLayoutConstraintOrientation);

#define uiDarwinControlDefaultDestroy(type, handlefield) \
	static void type ## Destroy(uiControl *c) \
	{ \
		uiControlVerifyDestroy(c); \
		[type(c)->handlefield release]; \
		uiFreeControl(c); \
	}
#define uiDarwinControlDefaultHandle(type, handlefield) \
	static uintptr_t type ## Handle(uiControl *c) \
	{ \
		return (uintptr_t) (type(c)->handlefield); \
	}
#define uiDarwinControlDefaultParent(type, handlefield) \
	static uiControl *type ## Parent(uiControl *c) \
	{ \
		return uiDarwinControl(c)->parent; \
	}
#define uiDarwinControlDefaultSetParent(type, handlefield) \
	static void type ## SetParent(uiControl *c, uiControl *parent) \
	{ \
		uiControlVerifySetParent(c, parent); \
		uiDarwinControl(c)->parent = parent; \
	}
#define uiDarwinControlDefaultToplevel(type, handlefield) \
	static int type ## Toplevel(uiControl *c) \
	{ \
		return 0; \
	}
#define uiDarwinControlDefaultVisible(type, handlefield) \
	static int type ## Visible(uiControl *c) \
	{ \
		return uiDarwinControl(c)->visible; \
	}
#define uiDarwinControlDefaultShow(type, handlefield) \
	static void type ## Show(uiControl *c) \
	{ \
		uiDarwinControl(c)->visible = YES; \
		[type(c)->handlefield setHidden:NO]; \
	}
#define uiDarwinControlDefaultHide(type, handlefield) \
	static void type ## Hide(uiControl *c) \
	{ \
		uiDarwinControl(c)->visible = NO; \
		[type(c)->handlefield setHidden:YES]; \
	}
#define uiDarwinControlDefaultEnabled(type, handlefield) \
	static int type ## Enabled(uiControl *c) \
	{ \
		return uiDarwinControl(c)->enabled; \
	}
#define uiDarwinControlDefaultEnable(type, handlefield) \
	static void type ## Enable(uiControl *c) \
	{ \
		uiDarwinControl(c)->enabled = YES; \
		uiDarwinControlSyncEnableState(uiDarwinControl(c), uiControlEnabledToUser(c)); \
	}
#define uiDarwinControlDefaultDisable(type, handlefield) \
	static void type ## Disable(uiControl *c) \
	{ \
		uiDarwinControl(c)->enabled = NO; \
		uiDarwinControlSyncEnableState(uiDarwinControl(c), uiControlEnabledToUser(c)); \
	}
#define uiDarwinControlDefaultSyncEnableState(type, handlefield) \
	static void type ## SyncEnableState(uiDarwinControl *c, int enabled) \
	{ \
		if (uiDarwinShouldStopSyncEnableState(c, enabled)) \
			return; \
		if ([type(c)->handlefield respondsToSelector:@selector(setEnabled:)]) \
			[((id) type(c)->handlefield) setEnabled:enabled]; /* id cast to make compiler happy; thanks mikeash in irc.freenode.net/#macdev */ \
	}
#define uiDarwinControlDefaultSetSuperview(type, handlefield) \
	static void type ## SetSuperview(uiDarwinControl *c, NSView *superview) \
	{ \
		[type(c)->handlefield setTranslatesAutoresizingMaskIntoConstraints:NO]; \
		if (superview == nil) \
			[type(c)->handlefield removeFromSuperview]; \
		else \
			[superview addSubview:type(c)->handlefield]; \
	}
#define uiDarwinControlDefaultSetRealHuggingPriority(type, handlefield) \
	static void type ## SetRealHuggingPriority(uiDarwinControl *c, NSLayoutPriority priority, NSLayoutConstraintOrientation orientation) \
	{ \
		[type(c)->handlefield setContentHuggingPriority:priority forOrientation:orientation]; \
	}

#define uiDarwinControlAllDefaultsExceptDestroy(type, handlefield) \
	uiDarwinControlDefaultHandle(type, handlefield) \
	uiDarwinControlDefaultParent(type, handlefield) \
	uiDarwinControlDefaultSetParent(type, handlefield) \
	uiDarwinControlDefaultToplevel(type, handlefield) \
	uiDarwinControlDefaultVisible(type, handlefield) \
	uiDarwinControlDefaultShow(type, handlefield) \
	uiDarwinControlDefaultHide(type, handlefield) \
	uiDarwinControlDefaultEnabled(type, handlefield) \
	uiDarwinControlDefaultEnable(type, handlefield) \
	uiDarwinControlDefaultDisable(type, handlefield) \
	uiDarwinControlDefaultSyncEnableState(type, handlefield) \
	uiDarwinControlDefaultSetSuperview(type, handlefield) \
	uiDarwinControlDefaultSetRealHuggingPriority(type, handlefield)

#define uiDarwinControlAllDefaults(type, handlefield) \
	uiDarwinControlDefaultDestroy(type, handlefield) \
	uiDarwinControlAllDefaultsExceptDestroy(type, handlefield)

// TODO document
#define uiDarwinNewControl(type, var) \
	var = type(uiDarwinAllocControl(sizeof (type), type ## Signature, #type)); \
	uiControl(var)->Destroy = type ## Destroy; \
	uiControl(var)->Handle = type ## Handle; \
	uiControl(var)->Parent = type ## Parent; \
	uiControl(var)->SetParent = type ## SetParent; \
	uiControl(var)->Toplevel = type ## Toplevel; \
	uiControl(var)->Visible = type ## Visible; \
	uiControl(var)->Show = type ## Show; \
	uiControl(var)->Hide = type ## Hide; \
	uiControl(var)->Enabled = type ## Enabled; \
	uiControl(var)->Enable = type ## Enable; \
	uiControl(var)->Disable = type ## Disable; \
	uiDarwinControl(var)->SyncEnableState = type ## SyncEnableState; \
	uiDarwinControl(var)->SetSuperview = type ## SetSuperview; \
	uiDarwinControl(var)->SetRealHuggingPriority = type ## SetRealHuggingPriority; \
	uiDarwinControl(var)->visible = YES; \
	uiDarwinControl(var)->enabled = YES;
// TODO document
_UI_EXTERN uiDarwinControl *uiDarwinAllocControl(size_t n, uint32_t typesig, const char *typenamestr);

// Use this function as a shorthand for setting control fonts.
_UI_EXTERN void uiDarwinSetControlFont(NSControl *c, NSControlSize size);

// You can use this function from within your control implementations to return text strings that can be freed with uiFreeText().
_UI_EXTERN char *uiDarwinNSStringToText(NSString *);

// TODO document
_UI_EXTERN BOOL uiDarwinShouldStopSyncEnableState(uiDarwinControl *, BOOL);

#ifdef __cplusplus
}
#endif

#endif
