#ifndef BAR_H
#define BAR_H

// whether the order of the blocks is inversed
#define INVERSED 1

// maximum number of chars a block's command can output
#define CMDLENGTH 50

static const Block blocks[]   = {                                 // the blocks to display in the status bar
   // fg color    command                                         interval signal
   {  gbbg3,      "exec $XDG_DATA_HOME/bin/you.sh",               0,       1     },    // username@hostname
   {  gbbll,      "exec $XDG_DATA_HOME/bin/clock.sh",             30,      2     },    // date and time
   {  gbaql,      "exec $XDG_DATA_HOME/bin/vol.sh",               0,       3     },    // volume
   {  gbyel,      "exec $XDG_DATA_HOME/bin/bat.sh",               30,      4     },    // battery % and charging status
   {  gbgrl,      "exec $XDG_DATA_HOME/bin/net.sh rx",            1,       5     },    // received network traffic
   {  gbgrd,      "exec $XDG_DATA_HOME/bin/net.sh tx",            1,       6     },    // transmitted network traffic
   {  gbaqd,      "exec $XDG_DATA_HOME/bin/check_newsboat.sh",    120,     7     },    // check newsboat feed
   {  gbaql,      "echo \" : \"",                                 0,       8     },    // seperator
};

static char delimiter[]       = " ";                              // delimiter between blocks; rendered with SchemeNorm

#endif
