#ifndef BAR_H
#define BAR_H

// whether the order of the blocks is inversed
#define INVERSED 1

// maximum number of chars a block's command can output
#define CMDLENGTH 50

static const Block blocks[]   = {                                 // the blocks to display in the status bar
   // fg color    command                   interval signal
   {  gbbg3,      "exec $SCR/you.sh",       0,       1      },
   {  gbbll,      "exec $SCR/clock.sh",     30,      2      },
   {  gbyel,      "exec $SCR/bat.sh",       30,      3      },
   {  gbgrl,      "exec $SCR/net.sh rx",    1,       5      },
   {  gbgrd,      "exec $SCR/net.sh tx",    1,       4      },
};

static char delimiter[]       = " ";                              // delimiter between blocks; rendrered with SchemeNorm

#endif
