#include <X11/XF86keysym.h>

/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }

#define LOCKCMD "swaylock -eFfkl -i ~/.config/background -s fill"

/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 10; /* gap pixel between windows */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const unsigned int snap             = 32; /* snap pixel */
static const int showbar                   = 1; /* 0 means no bar */
static const int topbar                    = 1; /* 0 means bottom bar */
static const char *fonts[]                 = {"monospace:size=10"};
static const float rootcolor[]             = COLOR(0x000000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.0f, 0.0f, 0.0f, 1.0f}; /* You can also use glsl colors */
static uint32_t colors[][3]                = {
	/*               fg          bg          border    */
	[SchemeNorm] = { 0xffffffff, 0x000000b2, 0x000000b2},
	[SchemeSel]  = { 0x000000b2, 0xffffffff, 0xffffffff},
	[SchemeUrg]  = { 0xffffffff, 0xff0000b2, 0xff0000b2},
};

/* animation */
static const unsigned int animation_duration = 300; /* ms */
static const float animation_curve[4] = {0.0f, 0.85f, 0.2f, 1.0f}; /* cubic-bezier x1, y1, x2, y2 */

/* tagging */
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
        "sh", "-c", "swaybg -m fill -i ~/.config/background", NULL,
        "sh", "-c", "wayidle -t 60" LOCKCMD, NULL,
        "sh", "-c", "dbus-update-activation-environment XDG_CURRENT_DESKTOP=sway", NULL,
        "sh", "-c", "dbus-update-activation-environment WAYLAND_DISPLAY", NULL,
        NULL /* terminate */
};

static const Rule rules[] = {
	/* app_id      title  tags mask  isfloating   monitor */
	{ NULL,        NULL,  0,         0,           -1 },
	{ "termfloat", NULL,  0,         1,           -1 },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol  arrange function */
	{  " ",    grid },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients due to
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899 */
static const MonitorRule monrules[] = {
	/* name  scale  layout       rotate/reflect              x   y */
	{  NULL, 2,     &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, -1, -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_ALT

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, KEY,            tag,             {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define TERMCMD       "foot -a term"
#define TERMFLOATCMD  "foot -a termfloat"
#define MENUCMD       "wmenu-desktop"
#define SCREENSHOTCMD "grim -g \"$(slurp)\" - | wl-copy"

#define VOLDOWN    "amixer set Master 5%-"
#define VOLUP      "amixer set Master 5%+"
#define VOLMUTE    "amixer set Master toggle"
#define VOLMICMUTE "amixer set Capture toggle"

#define LIGHTDOWN "brightnessctl set 5%-"
#define LIGHTUP   "brightnessctl set 5%+"

#define PLAYERNEXT  "playerctl next"
#define PLAYERPREV  "playerctl previous"
#define PLAYERPAUSE "playerctl play-pause"

static const Key keys[] = {
	/* modifier                  key                       function          argument */
	{ MODKEY,                    XKB_KEY_Return,           spawn,            SHCMD(TERMCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,           spawn,            SHCMD(TERMFLOATCMD) },
	{ MODKEY,                    XKB_KEY_d,                spawn,            SHCMD(MENUCMD) },
	{ MODKEY,                    XKB_KEY_s,                spawn,            SHCMD(SCREENSHOTCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_x,                spawn,            SHCMD(LOCKCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_q,                killclient,       {0} },
	{ MODKEY,                    XKB_KEY_j,                focusstack,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,                focusstack,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_e,                zoom,             {0} },
	{ MODKEY,                    XKB_KEY_space,            togglefloating,   {0} },
	{ MODKEY,                    XKB_KEY_f,                togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_comma,            focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,           focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_comma,            tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_period,           tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	{ 0,                         XF86XK_AudioLowerVolume,  spawn,            SHCMD(VOLDOWN) },
	{ 0,                         XF86XK_AudioRaiseVolume,  spawn,            SHCMD(VOLUP) },
	{ 0,                         XF86XK_AudioMute,         spawn,            SHCMD(VOLMUTE) },
	{ 0,                         XF86XK_AudioMicMute,      spawn,            SHCMD(VOLMICMUTE) },
	{ 0,                         XF86XK_MonBrightnessDown, spawn,            SHCMD(LIGHTDOWN) },
	{ 0,                         XF86XK_MonBrightnessUp,   spawn,            SHCMD(LIGHTUP) },
	{ 0,                         XF86XK_AudioNext,         spawn,            SHCMD(PLAYERNEXT) },
	{ 0,                         XF86XK_AudioPrev,         spawn,            SHCMD(PLAYERPREV) },
	{ 0,                         XF86XK_AudioPlay,         spawn,            SHCMD(PLAYERPAUSE) },
	TAGKEYS(                     XKB_KEY_1,                0),
	TAGKEYS(                     XKB_KEY_2,                1),
	TAGKEYS(                     XKB_KEY_3,                2),
	TAGKEYS(                     XKB_KEY_4,                3),
	TAGKEYS(                     XKB_KEY_5,                4),
	TAGKEYS(                     XKB_KEY_6,                5),
	TAGKEYS(                     XKB_KEY_7,                6),
	TAGKEYS(                     XKB_KEY_8,                7),
	TAGKEYS(                     XKB_KEY_9,                8),
	TAGKEYS(                     XKB_KEY_0,                9),

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_BackSpace, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_F##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ ClkTitle,    0,      BTN_MIDDLE, zoom,           {0} },
	{ ClkClient,   MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ ClkClient,   MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ ClkClient,   MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
	{ ClkTagBar,   0,      BTN_LEFT,   view,           {0} },
	{ ClkTagBar,   MODKEY, BTN_LEFT,   tag,            {0} },
};

static const Axis axes[] = {
	/* example of volume control:
	{ MODKEY, AxisUp,   spawn, SHCMD("volume-up_EXAMPLE") },
	{ MODKEY, AxisDown, spawn, SHCMD("volume-down_EXAMPLE") }, */
	{ 0, 0, NULL, {0} },
};
