#pragma once
#include "SDL_keycode.h"
enum SDL_EventType
{
	SDL_KEYDOWN = 0x300,
	SDL_KEYUP,
	SDL_TEXTEDITING,/**< Keyboard text editing (composition) */
	SDL_TEXTINPUT   /**< Keyboard text input */
};

typedef struct SDL_Keysym
{
	SDL_Scancode	scancode;      /**< SDL physical key code - see ::SDL_Scancode for details */
	SDL_Keycode		sym;            /**< SDL virtual key code - see ::SDL_Keycode for details */
	UInt16			mod;                 /**< current key modifiers */
	UInt32			unused;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent
{
	UInt32			type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
	UInt32			timestamp;   /**< In milliseconds, populated using SDL_GetTicks() */
	UInt32			windowID;    /**< The window with keyboard focus, if any */
	UInt8			state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
	UInt8			repeat;       /**< Non-zero if this is a key repeat */
	UInt8			padding2;
	UInt8			padding3;
	SDL_Keysym		keysym;  /**< The key that was pressed or released */
} SDL_KeyboardEvent;

typedef struct SDL_Event
{
	UInt32			type;
} SDL_Event;