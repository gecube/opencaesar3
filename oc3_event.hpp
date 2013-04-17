// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.


#ifndef __OPENCAESAR3_EVENT_H_INCLUDED__
#define __OPENCAESAR3_EVENT_H_INCLUDED__

#include "oc3_positioni.hpp"

class Widget;

enum OC3_KEY_CODE
{
    KEY_LBUTTON          = 0x01,  // Left mouse button
    KEY_MBUTTON          = 0x02,  // Right mouse button
    KEY_CANCEL           = 0x03,  // Control-break processing
    KEY_RBUTTON          = 0x04,  // Middle mouse button (three-button mouse)
    KEY_XBUTTON1         = 0x05,  // Windows 2000/XP: X1 mouse button
    KEY_XBUTTON2         = 0x06,  // Windows 2000/XP: X2 mouse button
    KEY_BACK             = 0x08,  // BACKSPACE key
    KEY_TAB              = 0x09,  // TAB key
    KEY_CLEAR            = 0x0C,  // CLEAR key
    KEY_RETURN           = 0x0D,  // ENTER key
    KEY_SHIFT            = 0x10,  // SHIFT key
    KEY_CONTROL          = 0x11,  // CTRL key
    KEY_MENU             = 0x12,  // ALT key
    KEY_PAUSE            = 0x13,  // PAUSE key
    KEY_CAPITAL          = 0x14,  // CAPS LOCK key
    KEY_KANA             = 0x15,  // IME Kana mode
    KEY_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
    KEY_HANGUL           = 0x15,  // IME Hangul mode
    KEY_JUNJA            = 0x17,  // IME Junja mode
    KEY_FINAL            = 0x18,  // IME final mode
    KEY_HANJA            = 0x19,  // IME Hanja mode
    KEY_KANJI            = 0x19,  // IME Kanji mode
    KEY_ESCAPE           = 0x1B,  // ESC key
    KEY_CONVERT          = 0x1C,  // IME convert
    KEY_NONCONVERT       = 0x1D,  // IME nonconvert
    KEY_ACCEPT           = 0x1E,  // IME accept
    KEY_MODECHANGE       = 0x1F,  // IME mode change request
    KEY_SPACE            = 0x20,  // SPACEBAR
    KEY_PRIOR            = 0x21,  // PAGE UP key
    KEY_NEXT             = 0x22,  // PAGE DOWN key
    KEY_END              = 0x23,  // END key
    KEY_HOME             = 0x24,  // HOME key
    KEY_LEFT             = 0x25,  // LEFT ARROW key
    KEY_UP               = 0x26,  // UP ARROW key
    KEY_RIGHT            = 0x27,  // RIGHT ARROW key
    KEY_DOWN             = 0x28,  // DOWN ARROW key
    KEY_SELECT           = 0x29,  // SELECT key
    KEY_PRINT            = 0x2A,  // PRINT key
    KEY_EXECUT           = 0x2B,  // EXECUTE key
    KEY_SNAPSHOT         = 0x2C,  // PRINT SCREEN key
    KEY_INSERT           = 0x2D,  // INS key
    KEY_DELETE           = 0x2E,  // DEL key
    KEY_HELP             = 0x2F,  // HELP key
    KEY_KEY_0            = 0x30,  // 0 key
    KEY_KEY_1            = 0x31,  // 1 key
    KEY_KEY_2            = 0x32,  // 2 key
    KEY_KEY_3            = 0x33,  // 3 key
    KEY_KEY_4            = 0x34,  // 4 key
    KEY_KEY_5            = 0x35,  // 5 key
    KEY_KEY_6            = 0x36,  // 6 key
    KEY_KEY_7            = 0x37,  // 7 key
    KEY_KEY_8            = 0x38,  // 8 key
    KEY_KEY_9            = 0x39,  // 9 key
    KEY_KEY_A            = 0x41,  // A key
    KEY_KEY_B            = 0x42,  // B key
    KEY_KEY_C            = 0x43,  // C key
    KEY_KEY_D            = 0x44,  // D key
    KEY_KEY_E            = 0x45,  // E key
    KEY_KEY_F            = 0x46,  // F key
    KEY_KEY_G            = 0x47,  // G key
    KEY_KEY_H            = 0x48,  // H key
    KEY_KEY_I            = 0x49,  // I key
    KEY_KEY_J            = 0x4A,  // J key
    KEY_KEY_K            = 0x4B,  // K key
    KEY_KEY_L            = 0x4C,  // L key
    KEY_KEY_M            = 0x4D,  // M key
    KEY_KEY_N            = 0x4E,  // N key
    KEY_KEY_O            = 0x4F,  // O key
    KEY_KEY_P            = 0x50,  // P key
    KEY_KEY_Q            = 0x51,  // Q key
    KEY_KEY_R            = 0x52,  // R key
    KEY_KEY_S            = 0x53,  // S key
    KEY_KEY_T            = 0x54,  // T key
    KEY_KEY_U            = 0x55,  // U key
    KEY_KEY_V            = 0x56,  // V key
    KEY_KEY_W            = 0x57,  // W key
    KEY_KEY_X            = 0x58,  // X key
    KEY_KEY_Y            = 0x59,  // Y key
    KEY_KEY_Z            = 0x5A,  // Z key
    KEY_LWIN             = 0x5B,  // Left Windows key (Microsoft� Natural� keyboard)
    KEY_RWIN             = 0x5C,  // Right Windows key (Natural keyboard)
    KEY_APPS             = 0x5D,  // Applications key (Natural keyboard)
    KEY_SLEEP            = 0x5F,  // Computer Sleep key
    KEY_NUMPAD0          = 0x60,  // Numeric keypad 0 key
    KEY_NUMPAD1          = 0x61,  // Numeric keypad 1 key
    KEY_NUMPAD2          = 0x62,  // Numeric keypad 2 key
    KEY_NUMPAD3          = 0x63,  // Numeric keypad 3 key
    KEY_NUMPAD4          = 0x64,  // Numeric keypad 4 key
    KEY_NUMPAD5          = 0x65,  // Numeric keypad 5 key
    KEY_NUMPAD6          = 0x66,  // Numeric keypad 6 key
    KEY_NUMPAD7          = 0x67,  // Numeric keypad 7 key
    KEY_NUMPAD8          = 0x68,  // Numeric keypad 8 key
    KEY_NUMPAD9          = 0x69,  // Numeric keypad 9 key
    KEY_MULTIPLY         = 0x6A,  // Multiply key
    KEY_ADD              = 0x6B,  // Add key
    KEY_SEPARATOR        = 0x6C,  // Separator key
    KEY_SUBTRACT         = 0x6D,  // Subtract key
    KEY_DECIMAL          = 0x6E,  // Decimal key
    KEY_DIVIDE           = 0x6F,  // Divide key
    KEY_F1               = 0x70,  // F1 key
    KEY_F2               = 0x71,  // F2 key
    KEY_F3               = 0x72,  // F3 key
    KEY_F4               = 0x73,  // F4 key
    KEY_F5               = 0x74,  // F5 key
    KEY_F6               = 0x75,  // F6 key
    KEY_F7               = 0x76,  // F7 key
    KEY_F8               = 0x77,  // F8 key
    KEY_F9               = 0x78,  // F9 key
    KEY_F10              = 0x79,  // F10 key
    KEY_F11              = 0x7A,  // F11 key
    KEY_F12              = 0x7B,  // F12 key
    KEY_F13              = 0x7C,  // F13 key
    KEY_F14              = 0x7D,  // F14 key
    KEY_F15              = 0x7E,  // F15 key
    KEY_F16              = 0x7F,  // F16 key
    KEY_F17              = 0x80,  // F17 key
    KEY_F18              = 0x81,  // F18 key
    KEY_F19              = 0x82,  // F19 key
    KEY_F20              = 0x83,  // F20 key
    KEY_F21              = 0x84,  // F21 key
    KEY_F22              = 0x85,  // F22 key
    KEY_F23              = 0x86,  // F23 key
    KEY_F24              = 0x87,  // F24 key
    KEY_NUMLOCK          = 0x90,  // NUM LOCK key
    KEY_SCROLL           = 0x91,  // SCROLL LOCK key
    KEY_LSHIFT           = 0xA0,  // Left SHIFT key
    KEY_RSHIFT           = 0xA1,  // Right SHIFT key
    KEY_LCONTROL         = 0xA2,  // Left CONTROL key
    KEY_RCONTROL         = 0xA3,  // Right CONTROL key
    KEY_LMENU            = 0xA4,  // Left MENU key
    KEY_RMENU            = 0xA5,  // Right MENU key
    KEY_PLUS             = 0xBB,  // Plus Key   (+)
    KEY_COMMA            = 0xBC,  // Comma Key  (,)
    KEY_MINUS            = 0xBD,  // Minus Key  (-)
    KEY_PERIOD           = 0xBE,  // Period Key (.)
    KEY_ATTN             = 0xF6,  // Attn key
    KEY_CRSEL            = 0xF7,  // CrSel key
    KEY_EXSEL            = 0xF8,  // ExSel key
    KEY_EREOF            = 0xF9,  // Erase EOF key
    KEY_PLAY             = 0xFA,  // Play key
    KEY_ZOOM             = 0xFB,  // Zoom key
    KEY_PA1              = 0xFD,  // PA1 key
    KEY_OEM_CLEAR        = 0xFE,   // Clear key

    KEY_KEY_CODES_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
};

//! Enumeration for all events which are sendable by the gui system
enum OC3_GUI_EVENT_TYPE
{
	//! A gui element has lost its focus.
	OC3_ELEMENT_FOCUS_LOST = 0,

	//! A gui element has got the focus.
	/** If the event is absorbed then the focus will not be changed. */
	OC3_ELEMENT_FOCUSED,

	//! The mouse cursor hovered over a gui element.
	/** If an element has sub-elements you also get this message for the subelements */
	OC3_ELEMENT_HOVERED,

	//! The mouse cursor left the hovered element.
	/** If an element has sub-elements you also get this message for the subelements */
	OC3_ELEMENT_LEFT,

	//! An element would like to close.
	/** Windows and context menus use this event when they would like to close,
	this can be cancelled by absorbing the event. */
	OC3_ELEMENT_CLOSED,

	//! A button was clicked.
	OC3_BUTTON_CLICKED,

	//! A scrollbar has changed its position.
	OC3_SCROLL_BAR_CHANGED,

	//! A checkbox has changed its check state.
	OC3_CHECKBOX_CHANGED,

	//! A new item in a listbox was selected.
	/** NOTE: You also get this event currently when the same item was clicked again after more than 500 ms. */
	OC3_LISTBOX_CHANGED,

	//! An item in the listbox was selected, which was already selected.
	/** NOTE: You get the event currently only if the item was clicked again within 500 ms or selected by "enter" or "space". */
	OC3_LISTBOX_SELECTED_AGAIN,

	//! In an editbox 'ENTER' was pressed
	OC3_EDITBOX_ENTER,

	//! The text in an editbox was changed. This does not include automatic changes in text-breaking.
	OC3_EDITBOX_CHANGED,

    //! The marked area in an editbox was changed.
	OC3_EDITBOX_MARKING_CHANGED,

	//! The tab was changed in an tab control
	OC3_TAB_CHANGED,

	//! A menu item was selected in a (context) menu
	OC3_MENU_ITEM_SELECTED,

	//! The selection in a combo box has been changed
	OC3_COMBO_BOX_CHANGED,

	//! The value of a spin box has changed
	OC3_SPINBOX_CHANGED,

	//! No real event. Just for convenience to get number of events
	OC3_GUI_EVENT_COUNT
};

//! Enumeration for all event types there are.
enum OC3_EVENT_TYPE
{
    //! An event of the graphical user interface.
    OC3_GUI_EVENT = 0,

    //! A mouse input event.
    OC3_MOUSE_EVENT,

    //! A key input event.
    /** Like mouse events, keyboard events are created by the device and passed to
    handleEvent. They take the same path as mouse events. */
    OC3_KEYBOARD_EVENT,

    OC3_USER_EVENT,

    //! This enum is never used, it only forces the compiler to
    //! compile these enumeration values to 32 bit.
    OC3_EGUIET_FORCE_32_BIT = 0x7fffffff

};

//! Enumeration for all mouse input events
enum OC3_MOUSE_INPUT_EVENT
{
    //! Left mouse button was pressed down.
    OC3_LMOUSE_PRESSED_DOWN = 0,

    //! Middle mouse button was pressed down.
    OC3_MMOUSE_PRESSED_DOWN,

    //! Right mouse button was pressed down.
    OC3_RMOUSE_PRESSED_DOWN,

    //! Left mouse button was left up.
    OC3_LMOUSE_LEFT_UP,

    //! Middle mouse button was left up.
    OC3_MMOUSE_LEFT_UP,

    //! Right mouse button was left up.
    OC3_RMOUSE_LEFT_UP,

    //! The mouse cursor changed its position.
    OC3_MOUSE_MOVED,

    //! The mouse wheel was moved. Use Wheel value in event data to find out
    //! in what direction and how fast.
    OC3_MOUSE_WHEEL,

    //! Left mouse button double click.
    OC3_LMOUSE_DOUBLE_CLICK,

    //! Right mouse button double click.
    OC3_RMOUSE_DOUBLE_CLICK,

    //! Middle mouse button double click.
    OC3_MMOUSE_DOUBLE_CLICK,

    //! Left mouse button triple click.
    OC3_LMOUSE_TRIPLE_CLICK,

    //! Right mouse button triple click.
    OC3_RMOUSE_TRIPLE_CLICK,

    //! Middle mouse button triple click.
    OC3_MMOUSE_TRIPLE_CLICK,

    //! No real event. Just for convenience to get number of events
    OC3_MOUSE_EVENT_COUNT
};

//! Masks for mouse button states
enum OC3_MOUSE_BUTTON_STATE_MASK
{
    OC3_EMBSM_LEFT    = 0x01,
    OC3_EMBSM_MIDDLE  = 0x02,
    OC3_EMBSM_RIGHT   = 0x04,

    //! currently only on windows
    OC3_EMBSM_EXTRA1  = 0x08,

    //! currently only on windows
    OC3_EMBSM_EXTRA2  = 0x10,

    OC3_EMBSM_FORCE_32_BIT = 0x7fffffff
};

//! NEvent hold information about an event
struct NEvent
{
	//! Any kind of GUI event.
	struct _GuiEvent
	{      
		//! IGUIElement who called the event
		Widget* Caller;

		//! If the event has something to do with another element, it will be held here.
		Widget* Element;

		//! Type of GUI Event
		OC3_GUI_EVENT_TYPE EventType;
	};
    
	//! Any kind of mouse event.
	struct _MouseEvent
	{
		//! X position of mouse cursor
		unsigned int X;

		//! Y position of mouse cursor
		int Y;

		//! mouse wheel delta, usually 1.0 or -1.0.
		/** Only valid if event was OC3_MOUSE_WHEEL */
		float Wheel;

		//! True if shift was also pressed
		bool Shift:1;

		//! True if ctrl was also pressed
		bool Control:1;

		//! A bitmap of button states. You can use isButtonPressed() to determine
		//! if a button is pressed or not.
		//! Currently only valid if the event was EMIE_MOUSE_MOVED
		unsigned int ButtonStates;

        const Point getPosition() { return Point( X, Y ); }
		const Point getPosition() const { return Point( X, Y ); }

		//! Is the left button pressed down?
		bool isLeftPressed() const { return 0 != ( ButtonStates & OC3_EMBSM_LEFT ); }

		//! Is the right button pressed down?
		bool isRightPressed() const { return 0 != ( ButtonStates & OC3_EMBSM_RIGHT ); }

		//! Is the middle button pressed down?
		bool isMiddlePressed() const { return 0 != ( ButtonStates & OC3_EMBSM_MIDDLE ); }

		//! Type of mouse event
        OC3_MOUSE_INPUT_EVENT Event;
	};

	//! Any kind of keyboard event.
	struct _KeyboardEvent
	{
		//! Character corresponding to the key (0, if not a character)
		wchar_t Char;

		//! Key which has been pressed or released
		OC3_KEY_CODE Key;

		//! If not true, then the key was left up
		bool PressedDown:1;

		//! True if shift was also pressed
		bool Shift:1;

		//! True if ctrl was also pressed
		bool Control:1;
	};

	//! Any kind of user event.
	struct _UserEvent
	{
		//! Some user specified data as int
		unsigned int UserData1;

		//! Another user specified data as int
		unsigned int UserData2;
	};

	OC3_EVENT_TYPE EventType;
	union
	{
		struct _GuiEvent GuiEvent;
		struct _MouseEvent MouseEvent;
		struct _KeyboardEvent KeyboardEvent;
		struct _UserEvent UserEvent;
	};

    static NEvent Gui( Widget* caller, Widget* elm, OC3_GUI_EVENT_TYPE type )
    {
        NEvent ret;
        ret.GuiEvent.Caller = caller;
        ret.GuiEvent.Element = elm;
        ret.GuiEvent.EventType = type;
        ret.EventType = OC3_GUI_EVENT;

        return ret;
    }
};

#endif //__OPENCAESAR3_EVENT_H_INCLUDED__

