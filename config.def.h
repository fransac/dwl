/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }

/* colors */
#define COLORBG  0x121212ff
#define COLORDIM 0x848485ff
#define COLORFG  0xf5f6f8ff
#define COLORRED 0xd84452ff

/* appearance */
static const int tabletmaptosurface        = 0;  /* map tablet input to surface(1) or monitor(0) */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 10; /* gap pixel between windows */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const int showbar                   = 1; /* 0 means no bar */
static const int topbar                    = 0; /* 0 means bottom bar */
static const char *fonts[]                 = {"monospace:size=10"};
static const float rootcolor[]             = COLOR(COLORBG);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = COLOR(COLORBG); /* You can also use glsl colors */
static const double tablet_area[4]         = { 0.0, 0.0, 1.0, 1.0 }; /* pen active area: value 0.0 => 1.0; x, y, width, height */
static uint32_t colors[][3]                = {
	/*               fg       bg        border  */
	[SchemeNorm] = { COLORFG, COLORBG,  COLORDIM },
	[SchemeSel]  = { COLORBG, COLORFG,  COLORFG  },
	[SchemeUrg]  = { COLORFG, COLORRED, COLORRED },
};

/* animation */
static const unsigned int animation_duration = 300; /* ms */
static const float animation_curve[4] = {0.0f, 0.85f, 0.2f, 1.0f}; /* cubic-bezier x1, y1, x2, y2 */

/* tagging */
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
	"/bin/sh", "-c", ". ~/.autostart", NULL,
	NULL /* terminate */
};

static const Rule rules[] = {
	/* app_id      title  tags mask  isfloating   monitor */
	{ NULL,        NULL,  0,         0,           -1 },
	{ "termfloat", NULL,  0,         1,           -1 },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients due to
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899 */
static const MonitorRule monrules[] = {
	/* name  scale  rotate/reflect              x   y */
	{  NULL, 2,     WL_OUTPUT_TRANSFORM_NORMAL, -1, -1 },
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
static const int disable_while_typing = 0;
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

#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_F##n, chvt, {.ui = (n)} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define TERMCMD          "foot -a term"
#define TERMFLOATCMD     "foot -a termfloat"
#define MENUCMD          "wmenu-desktop"
#define SCREENSHOTCMD    "grim - | wl-copy"
#define SCREENSHOTSELCMD "grim -g \"$(slurp)\" - | wl-copy"

#define LOCKCMD "lock"

#define VOLDOWN    "amixer set Master 5%-"
#define VOLUP      "amixer set Master 5%+"
#define VOLMUTE    "amixer set Master toggle"

#define PLAYERNEXT  "playerctl next"
#define PLAYERPREV  "playerctl previous"
#define PLAYERPAUSE "playerctl play-pause"

#define LIGHTDOWN "brightnessctl set 5%-"
#define LIGHTUP   "brightnessctl set 5%+"

static const Key keys[] = {
	/* modifier                  key                       function          argument */
	{ MODKEY,                    XKB_KEY_bracketleft,      spawn,            SHCMD(VOLDOWN) },
	{ MODKEY,                    XKB_KEY_bracketright,     spawn,            SHCMD(VOLUP) },
	{ MODKEY,                    XKB_KEY_backslash,        spawn,            SHCMD(VOLMUTE) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_bracketleft,      spawn,            SHCMD(PLAYERPREV) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_bracketright,     spawn,            SHCMD(PLAYERNEXT) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_backslash,        spawn,            SHCMD(PLAYERPAUSE) },
	{ MODKEY,                    XKB_KEY_minus,            spawn,            SHCMD(LIGHTDOWN) },
	{ MODKEY,                    XKB_KEY_equal,            spawn,            SHCMD(LIGHTUP) },

	{ MODKEY,                    XKB_KEY_t,                spawn,            SHCMD(TERMCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_t,                spawn,            SHCMD(TERMFLOATCMD) },
	{ MODKEY,                    XKB_KEY_p,                spawn,            SHCMD(MENUCMD) },
	{ MODKEY,                    XKB_KEY_o,                spawn,            SHCMD(SCREENSHOTCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_o,                spawn,            SHCMD(SCREENSHOTSELCMD) },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_x,                spawn,            SHCMD(LOCKCMD) },

	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_q,                killclient,       {0} },
	{ MODKEY,                    XKB_KEY_j,                focusstack,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,                focusstack,       {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_j,                pushdown,         {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_k,                pushup,           {0} },
	{ MODKEY,                    XKB_KEY_space,            togglefloating,   {0} },
	{ MODKEY,                    XKB_KEY_f,                togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_comma,            focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,           focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_comma,            tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_period,           tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY,                    XKB_KEY_s,                moveresizekb,     {.v = (int []){0, 40, 0, 0}} },
	{ MODKEY,                    XKB_KEY_w,                moveresizekb,     {.v = (int []){0, -40, 0, 0}} },
	{ MODKEY,                    XKB_KEY_d,                moveresizekb,     {.v = (int []){40, 0, 0, 0}} },
	{ MODKEY,                    XKB_KEY_a,                moveresizekb,     {.v = (int []){-40, 0, 0, 0}} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_s,                moveresizekb,     {.v = (int []){0, 0, 0, 40}} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_w,                moveresizekb,     {.v = (int []){0, 0, 0, -40}} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_d,                moveresizekb,     {.v = (int []){0, 0, 40, 0}} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_a,                moveresizekb,     {.v = (int []){0, 0, -40, 0}} },

	{ MODKEY,                    XKB_KEY_1,                view,            {.ui = 1 << 0} },
	{ MODKEY,                    XKB_KEY_2,                view,            {.ui = 1 << 1} },
	{ MODKEY,                    XKB_KEY_3,                view,            {.ui = 1 << 2} },
	{ MODKEY,                    XKB_KEY_4,                view,            {.ui = 1 << 3} },
	{ MODKEY,                    XKB_KEY_5,                view,            {.ui = 1 << 4} },
	{ MODKEY,                    XKB_KEY_6,                view,            {.ui = 1 << 5} },
	{ MODKEY,                    XKB_KEY_7,                view,            {.ui = 1 << 6} },
	{ MODKEY,                    XKB_KEY_8,                view,            {.ui = 1 << 7} },
	{ MODKEY,                    XKB_KEY_9,                view,            {.ui = 1 << 8} },
	{ MODKEY,                    XKB_KEY_0,                view,            {.ui = 1 << 9} },

	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_1,                tag,             {.ui = 1 << 0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_2,                tag,             {.ui = 1 << 1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_3,                tag,             {.ui = 1 << 2} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_4,                tag,             {.ui = 1 << 3} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_5,                tag,             {.ui = 1 << 4} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_6,                tag,             {.ui = 1 << 5} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_7,                tag,             {.ui = 1 << 6} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_8,                tag,             {.ui = 1 << 7} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_9,                tag,             {.ui = 1 << 8} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_0,                tag,             {.ui = 1 << 9} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_BackSpace, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Key lockedkeys[] = {
	keys[0], keys[1], keys[2], keys[3], keys[4], keys[5],
	keys[6], keys[7],

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ ClkClient,   MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
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

static const TabletButton tabletbuttons[] = {
    /* button       state                  function    argument */
/*  { BTN_STYLUS,   WLR_BUTTON_PRESSED,    spawn,      {.v = termcmd} }, */
/*  { BTN_STYLUS2,  WLR_BUTTON_PRESSED,    killclient, {0} }, */
	{ 0,            0,                     NULL,       {0} }
};

static const TabletPadButton tabletpadbuttons[] = {
	/* button  state                  function          argument */
/*  { 0,       WLR_BUTTON_PRESSED,    spawn,            {.v = termcmd} }, */
/*  { 1,       WLR_BUTTON_PRESSED,    killclient,       {0} },*/
/*  { 2,       WLR_BUTTON_PRESSED,    togglefloating,   {0} },*/
/*  { 3,       WLR_BUTTON_PRESSED,    zoom,             {0} },*/
	{ 0,            0,                     NULL,       {0} }
};
