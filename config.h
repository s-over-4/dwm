//       #
//       #
// # # # #   #   # # # # #
// #     #   #   #   #   #
// # # # # # # # #   #   #

#define TERMINAL  "st"
#define TERMCLASS "St"
#define BROWSER   "qutebrowser"

// force to clients to always split vertically?
#define FORCE_VSPLIT 1

// includes
#include <theme.h>
#include "bar.h"

static unsigned int borderpx                 = 3;        // width of window borders, in pixels
static unsigned int snap                     = 32;       // distance snapped with floating windows
static unsigned int gappih                   = 10;       // horz. inner gap b/w windows
static unsigned int gappiv                   = 10;       // vert. inner gap b/w windows
static unsigned int gappoh                   = 10;       // horz. gap b/w windows and screen edge
static unsigned int gappov                   = 10;       // vert. gap b/w windows and screen edge
static unsigned int bar_height               = 6;        // how tall the bar will be (in addition to the space taken up by text)
static int swallowfloating                   = 0;        // swallow floating windows by default?
static int smartgaps                         = 0;        // no outer gap when there is only one window?
static int vertpad                           = 10;       // vert. gap b/w bar and screen edge
static int sidepad                           = 10;       // horz. gap b/w bar and screen edge
static int showbar                           = 1;        // draw a status bar?
static int topbar                            = 1;        // draw the bar on the top of the screen?
static char normbgcolor[]                    = gbbgh;    // bg. color of inactive status bar elements
static char normbordercolor[]                = gbbld;    // border color of inactive windows
static char normborderfcolor[]               = gbord;    // border color of inactive floating windows            
static char normfgcolor[]                    = gbfg0;    // fg. color of inactive status bar elements
static char selfgcolor[]                     = gbfg1;    // fg. color of active status bar elements
static char selbordercolor[]                 = gbrel;    // border color of active windows
static char selborderfcolor[]                = gborl;    // border color of active floating windows
static char selbgcolor[]                     = gbbgh;    // bg. color of active status bar elements
static const unsigned int barnormalpha       = OPAQUE;   // alpha of inactive status bar elements
static const unsigned int barselalpha        = OPAQUE;   // alpha of active status bar elements
static const unsigned int normborderalpha    = OPAQUE;   // alpha of inactive window borders
static const unsigned int selborderalpha     = OPAQUE;   // alpha of active window borders
static const unsigned int ulinepad           = 5;        // horizontal padding between the underline and the tag
static const unsigned int ulinestroke        = 3;        // thickness/height of the underline
static const unsigned int ulinevoffset       = 0;        // how far above the bottom of the bar the underline should appear
static const int ulineall                    = 0;        // should the underline appear beneath all tags, or just active ones?

static float mfact                           = 0.55;     // factor of master area size [0.05..0.95]
static int nmaster                           = 1;        // number of clients in master area (most likely 1)
static int resizehints                       = 0;        // respect size hints in tiled resizals?
static const int lockfullscreen              = 1;        // force focus on the fullscreen window?

static const char *tags[]                    = {         // window tags 
   "1", 
   "2", 
   "3", 
   "4", 
   "5", 
   "6", 
   "7", 
   "8", 
   "9", 
};

static const char ptagf[]                    = "[%s | %s]"; // format of tag label
static const char etagf[]                    = "[%s]";      // format of an empty tag
static const int lcaselbl                    = 1;           // make tag label lowercase?

static char *fonts[]                         = {         // font definitions
   FONT_MONO,
   "Apple Color Emoji"
};

static char *colors[][4]                     = {         // color definitions

    //                     fg             bg             border            float
    [SchemeNorm]     = {   normfgcolor,   normbgcolor,   normbordercolor,  normborderfcolor  }, // inactive window/status bar element
    [SchemeSel]      = {   selfgcolor,    selbgcolor,    selbordercolor,   selborderfcolor   }, // active window/status bar element
    [SchemeStatus]   = {   gbfg0,         normbgcolor,   NULL,             NULL              }, // default status bar block
};

static const unsigned int alphas[][4]        = {         // alpha definitions

   //                      fg          bg             border               float
   [SchemeNorm]      = {   OPAQUE,     barnormalpha,  normborderalpha,     normborderalpha      }, // inactive status bar element 
   [SchemeSel]       = {   OPAQUE,     barselalpha,   selborderalpha,      selborderalpha       }, // active status bar element
};

static const Rule rules[]                    = {         // application-specific rule definitions

   // class          instance       title             tags mask      isfloating     isterminal  noswallow   monitor
   {  "Gimp",        NULL,          NULL,             0,             0,             0,          0,          -1,   },
   {  TERMCLASS,     NULL,          NULL,             0,             0,             1,          0,          -1,   },
   {  NULL,          NULL,          "Event Tester",   0,             0,             0,          1,          -1,   },
   {  "float",       NULL,          NULL,             0,             1,             1,          1,          -1,   },
};


#include "vanitygaps.c"

static const char *termcmd[]                 = {
   TERMINAL, 
   NULL 
};


static const Layout layouts[]                = {         // layout definitions

   // status bar symbol    layout name                   behavior
   {  "[]=",               tile                    },    // master on left, slaves landscape on right; default
   {  "TTT",               bstack                  },    // master on top, slaves portrait on bottom 

   {  "[@]",               spiral                  },    // fibonacci spiral
   {  "[]\\",              dwindle                 },    // decreasing in size right and leftward

   {  "[]M",               deck                    },    // master on left, slaves in monocle-like mode on right
   {  "[M]",               monocle                 },    // all windows on top of each other

   {  "|M|",               centeredmaster          },    // master in middle, slaves on sides
   {  "-M-",               centeredfloatingmaster  },    // slaves on sides, master floats in middle

   {  "><>",               NULL                    },    //  no layout function means floating behavior
   {  NULL,                NULL                    },
};

// key definitions
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
   { MODKEY,                        KEY,     view,             {.ui = 1 << TAG} }, \
   { MODKEY|ControlMask,            KEY,     toggleview,       {.ui = 1 << TAG} }, \
   { MODKEY|ShiftMask,              KEY,     tag,              {.ui = 1 << TAG} }, \
   { MODKEY|ControlMask|ShiftMask,  KEY,     toggletag,        {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
   { MOD,                           XK_j,    ACTION##stack,    {.i = INC(+1) } }, \
   { MOD,                           XK_k,    ACTION##stack,    {.i = INC(-1) } }, \
   { MOD,                           XK_v,    ACTION##stack,    {.i = 0 } }, \

// helper for spawning shell commands in the pre dwm-5.0 fashion
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }


ResourcePref resources[]                     = {         // .Xresources preferences to load at startup

   // setting           type        value
   {  "color4",         STRING,     &normbordercolor  },
   {  "color12",        STRING,     &selbordercolor   },
   {  "background",     STRING,     &normbgcolor      },
   {  "color4",         STRING,     &normfgcolor      },
   {  "color12",        STRING,     &selfgcolor       },
   {  "background",     STRING,     &selbgcolor       },
   {  "borderpx",       INTEGER,    &borderpx         },
   {  "snap",           INTEGER,    &snap             },
   {  "showbar",        INTEGER,    &showbar          },
   {  "topbar",         INTEGER,    &topbar           },
   {  "nmaster",        INTEGER,    &nmaster          },
   {  "resizehints",    INTEGER,    &resizehints      },
   {  "mfact",          FLOAT,      &mfact            },
   {  "gappih",         INTEGER,    &gappih           },
   {  "gappiv",         INTEGER,    &gappiv           },
   {  "gappoh",         INTEGER,    &gappoh           },
   {  "gappov",         INTEGER,    &gappov           },
   {  "swallowfloating",INTEGER,    &swallowfloating  },
   {  "smartgaps",      INTEGER,    &smartgaps        },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[]                      = {         // keyboard shortcut definitions

   STACKKEYS(MODKEY, focus)            // focus window at tag
   STACKKEYS(MODKEY|ShiftMask, push)   // push window to tag
   
   TAGKEYS(XK_1,0)
   TAGKEYS(XK_2,1)
   TAGKEYS(XK_3,2)
   TAGKEYS(XK_4,3)
   TAGKEYS(XK_5,4)
   TAGKEYS(XK_6,5)
   TAGKEYS(XK_7,6)
   TAGKEYS(XK_8,7)
   TAGKEYS(XK_9,8)

   //    modifier                key                        function          argument 
// {     MODKEY                  XK_Escape,                 spawn,            SHCMD("")                                                },
// {     MODKEY|ShiftMask,       XK_Escape,                 spawn,            SHCMD("")                                                },
// {     MODKEY,                 XK_grave,                  spawn,            SHCMD("")                                                },
// {     MODKEY|ShiftMask,       XK_grave,                  spawn,            SHCMD("")                                                },

   // numbers already bound in TAGKEYS

   { MODKEY,                     XK_Tab,                    view,             {0}                                                      },
// { MODKEY|ShiftMask,           XK_Tab,                    spawn,            SHCMD("")                                                },
   { MODKEY,                     XK_q,                      killclient,       {0}                                                      },
   { MODKEY|ShiftMask,           XK_q,                      spawn,            SHCMD("pgrep dwm | xargs kill -s 9")                     },
   { MODKEY,                     XK_w,                      spawn,            SHCMD(BROWSER)                                           },
// { MODKEY|ShiftMask,           XK_w,                      spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_e,                      spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_e,                      spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_r,                      spawn,            SHCMD("")                                                },
   { MODKEY|ShiftMask,           XK_r,                      spawn,            {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } }  },

   { MODKEY,                     XK_t,                      setlayout,        {.v = &layouts[0]}                                       }, // tile
   { MODKEY|ShiftMask,           XK_t,                      setlayout,        {.v = &layouts[1]}                                       }, // bstack
   { MODKEY,                     XK_y,                      setlayout,        {.v = &layouts[2]}                                       }, // fibonacci spiral
   { MODKEY|ShiftMask,           XK_y,                      setlayout,        {.v = &layouts[3]}                                       }, // dwindle
   { MODKEY,                     XK_u,                      setlayout,        {.v = &layouts[4]}                                       }, // deck
   { MODKEY|ShiftMask,           XK_u,                      setlayout,        {.v = &layouts[5]}                                       }, // monocle
   { MODKEY,                     XK_i,                      setlayout,        {.v = &layouts[6]}                                       }, // centeredmaster
   { MODKEY|ShiftMask,           XK_i,                      setlayout,        {.v = &layouts[7]}                                       }, // centeredfloatingmaster

   { MODKEY,                     XK_o,                      incnmaster,       {.i = +1 }                                               },  
   { MODKEY|ShiftMask,           XK_o,                      incnmaster,       {.i = -1 }                                               },

   { MODKEY,                     XK_p,                      spawn,            SHCMD("$SCR/wallpaper.sh")                               },
// { MODKEY|ShiftMask,           XK_p,                      togglescratch,    {.ui = 2}                                                },
// { MODKEY,                     XK_bracketleft,            spawn,            SHCMD("")                                                }, 
// { MODKEY|ShiftMask,           XK_bracketleft,            spawn,            SHCMD("")                                                }, 
// { MODKEY,                     XK_bracketright,           spawn,            SHCMD("")                                                }, 
// { MODKEY|ShiftMask,           XK_bracketright,           spawn,            SHCMD("")                                                }, 
   { MODKEY,                     XK_backslash,              view,             {0}                                                      },
// { MODKEY|ShiftMask,           XK_backslash,              spawn,            SHCMD("")                                                },

   { MODKEY,                     XK_a,                      togglegaps,       {0}                                                      },
   { MODKEY|ShiftMask,           XK_a,                      defaultgaps,      {0}                                                      },
   { MODKEY,                     XK_s,                      togglesticky,     {0}                                                      },
// { MODKEY|ShiftMask,           XK_s,                      spawn,            SHCMD("")                                                },
   { MODKEY,                     XK_d,                      zoom,             {0}                                                      },
   { MODKEY|ShiftMask,           XK_d,                      togglefloating,   {0}                                                      },
   { MODKEY,                     XK_f,                      togglefullscr,    {0}                                                      },
   { MODKEY|ShiftMask,           XK_f,                      setlayout,        {.v = &layouts[8]}                                       }, // floating
   { MODKEY,                     XK_g,                      shiftview,        { .i = -1 }                                              },
   { MODKEY|ShiftMask,           XK_g,                      shifttag,         { .i = -1 }                                              },
   { MODKEY,                     XK_h,                      setmfact,         {.f = -0.05}                                             },
   { MODKEY|ShiftMask,           XK_h,                      setmfact,         {.f = -0.1}                                              },

   // j and k already bound in STACKKEYS

   { MODKEY,                     XK_l,                      setmfact,         {.f = +0.05}                                             },
   { MODKEY|ShiftMask,           XK_l,                      setmfact,         {.f = +0.1}                                              },
   { MODKEY,                     XK_semicolon,              shiftview,        { .i = 1 }                                               },
   { MODKEY|ShiftMask,           XK_semicolon,              shifttag,         { .i = -1 }                                              },
   { MODKEY,                     XK_apostrophe,             togglesmartgaps,  {0}                                                      },
// { MODKEY|ShiftMask,           XK_apostrophe,             spawn,            SHCMD("")                                                },
   { MODKEY,                     XK_Return,                 spawn,            {.v = termcmd }                                          },
// { MODKEY|ShiftMask,           XK_Return,                 spawn,            SHCMD(""),                                               },

   { MODKEY,                     XK_z,                      incrgaps,         {.i = +3 }                                               },
// { MODKEY|ShiftMask,           XK_z,                      spawn,            SHCMD("")                                                }, 
   { MODKEY,                     XK_x,                      incrgaps,         {.i = -3 }                                               },
// { MODKEY|ShiftMask,           XK_x,                      spawn,            SHCMD("")                                                }, 
// { MODKEY,                     XK_c,                      togglescratch,    {.ui = 0}                                                },
// { MODKEY|ShiftMask,           XK_c,                      spawn,            SHCMD("")                                                },

   // v already bound in STACKKEYS

   { MODKEY,                     XK_b,                      togglebar,        {0}                                                      },
// { MODKEY|ShiftMask,           XK_b,                      spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_n,                      spawn,            SHCMD("$SCR/bmks")                                       },
// { MODKEY|ShiftMask,           XK_n,                      spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_m,                      spawn,            SHCMD("$SCR/bmks_add.sh")                                },
// { MODKEY|ShiftMask,           XK_m,                      spawn,            SHCMD("$SCR/bmks del")                                   },
// { MODKEY,                     XK_comma,                  spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_comma,                  spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_period,                 spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_period,                 spawn,            SHCMD("")                                                },

   { MODKEY,                     XK_Left,                   focusmon,         {.i = -1 }                                               },
// { MODKEY|ShiftMask,           XK_Left,                   spawn,            SHCMD("")                                                },
   { MODKEY,                     XK_Right,                  focusmon,         {.i = +1 }                                               },
// { MODKEY|ShiftMask,           XK_Right,                  spawn,            SHCMD("")                                                },

   { MODKEY,                     XK_Page_Up,                shiftview,        { .i = -1 }                                              },
// { MODKEY|ShiftMask,           XK_Page_Up,                spawn,            SHCMD("")                                                },
   { MODKEY,                     XK_Page_Down,              shiftview,        { .i = +1 }                                              },
// { MODKEY|ShiftMask,           XK_Page_Down,              spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_Insert,                 spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_Insert,                 spawn,            SHCMD("")                                                },

   { MODKEY,                     XK_space,                  spawn,            SHCMD("dmenu_run")                                       },
   { MODKEY|ShiftMask,           XK_space,                  spawn,            SHCMD("$SCR/dmenu_run_term")                             },

// { MODKEY,                     XK_F1,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F1,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F2,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F2,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F3,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F3,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F4,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F4,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F5,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F5,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F6,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F6,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F7,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F7,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F8,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F8,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F9,                     spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F9,                     spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F10,                    spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F10,                    spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F11,                    spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F11,                    spawn,            SHCMD("")                                                },
// { MODKEY,                     XK_F12,                    spawn,            SHCMD("")                                                },
// { MODKEY|ShiftMask,           XK_F12,                    spawn,            SHCMD("")                                                },

   { 0,                          XK_Print,                  spawn,            SHCMD("$SCR/screenshot_ful.sh")                          },
   { ShiftMask,                  XK_Print,                  spawn,            SHCMD("$HOME/scripts/screenshot_sel.sh")                 },
   { 0,                          XF86XK_Sleep,              spawn,            {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } }     },
   { 0,                          XF86XK_AudioRaiseVolume,   spawn,            SHCMD("amixer set Master 1%+ && $SCR/ublock.sh 3")       },
   { 0,                          XF86XK_AudioLowerVolume,   spawn,            SHCMD("amixer set Master 1%- && $SCR/ublock.sh 3")       },
   { 0,                          XF86XK_AudioMute,          spawn,            SHCMD("amixer set Master toggle && $SCR/ublock.sh 3")    },
};

// button defs
// name is one of: ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, ClkRootWin 
static const Button buttons[] = {
   // name                 event mask     button         function          argument
   {  ClkWinTitle,         0,             Button1,       zoom,             {0}                                                },
   {  ClkTagBar,           0,             Button1,       view,             {0}                                                },
   {  ClkTagBar,           0,             Button3,       toggleview,       {0}                                                },
   {  ClkTagBar,           MODKEY,        Button1,       tag,              {0}                                                },
   {  ClkTagBar,           MODKEY,        Button3,       toggletag,        {0}                                                },
   {  ClkTagBar,           0,             Button4,       shiftview,        {.i = -1}                                          },
   {  ClkTagBar,           0,             Button5,       shiftview,        {.i = 1}                                           },
   {  ClkRootWin,          0,             Button2,       togglebar,        {0}                                                },

   {  ClkStatusText,       ShiftMask,     Button3,       spawn,            SHCMD(TERMINAL " -e vim $CONF/dwm/bar.h")          },
   {  ClkStatusText,       0,             Button1,       sigdwmblocks,     {.i = 1}                                           },
   {  ClkStatusText,       0,             Button2,       sigdwmblocks,     {.i = 2}                                           },
   {  ClkStatusText,       0,             Button3,       sigdwmblocks,     {.i = 3}                                           },
   {  ClkStatusText,       0,             Button4,       sigdwmblocks,     {.i = 4}                                           },
   {  ClkStatusText,       0,             Button5,       sigdwmblocks,     {.i = 5}                                           },

   {  ClkClientWin,        MODKEY,        Button1,       movemouse,        {0}                                                },
   {  ClkClientWin,        MODKEY,        Button3,       resizemouse,      {0}                                                },
};

