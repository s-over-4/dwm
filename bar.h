#ifndef BAR_H
#define BAR_H

// whether the order of the blocks is inversed
#define INVERSED 1

// maximum number of chars a block's command can output
#define CMDLENGTH 50

static const Block blocks[]   = {                                 // the blocks to display in the status bar
   // fg color    command                   interval signal
   {  gbbg3,      "exec $SCR/you.sh",              0,       1     },    // username@hostname
   {  gbbll,      "exec $SCR/clock.sh",            30,      2     },    // date and time
   {  gbaql,      "exec $SCR/vol.sh",              0,       3     },    // volume
   {  gbyel,      "exec $SCR/bat.sh",              30,      4     },    // battery % and charging status
   {  gbgrl,      "exec $SCR/net.sh rx",           1,       5     },    // received network traffic
   {  gbgrd,      "exec $SCR/net.sh tx",           1,       6     },    // transmitted network traffic
   {  gbaqd,      "exec $SCR/check_newsboat.sh",   120,     7     },    // check newsboat feed
};

static char delimiter[]       = " ";                              // delimiter between blocks; rendered with SchemeNorm

#endif
