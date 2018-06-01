#include "help_mode.h"

// this
#include "globals.h"
#include "keyboard_helper.h"

// libavr32
#include "font.h"
#include "region.h"

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

////////////////////////////////////////////////////////////////////////////////
// Help text ///////////////////////////////////////////////////////////////////

#define HELP_PAGES 10

#define HELP1_LENGTH 47
const char* help1[HELP1_LENGTH] = { "1/10 HELP",
                                    "[ ] NAVIGATE HELP PAGES",
                                    "UP/DOWN TO SCROLL",
                                    " ",
                                    "TAB|EDIT/LIVE/PATTERN",
                                    "PRT SC|JUMP TO LIVE",
                                    "NUM LOCK|JUMP TO PATTERN",
                                    "F1-F10|EXECUTE SCRIPT",
                                    "ALT-F1-F10|EDIT SCRIPT",
                                    "CTRL-F1-F8|MUTE SCRIPT",
                                    "CTRL-F9|STOP/START METRO",
                                    "ESC|SCENE",
                                    "ALT-ESC|WRITE",
                                    " ",
                                    "ALT-X|CUT",
                                    "ALT-C|COPY",
                                    "ALT-V|PASTE",
                                    " ",
                                    "// LIVE",
                                    "ENTER|EXECUTE",
                                    "UP|PREVIOUS",
                                    "SH-BSP|CLEAR",
                                    "~|TOGGLE VARS",
                                    " ",
                                    "// EDIT",
                                    "[ ]|PREV, NEXT SCRIPT",
                                    "ENTER|ADD/OVERWRITE",
                                    "SH-ENTER|INSERT",
                                    "SH-BSP|CLEAR",
                                    "ALT-SLASH|DISABLE LINE",
                                    " ",
                                    "// PATTERN",
                                    "ARROWS|NAVIGATE",
                                    "ALT-ARROWS|JUMP",
                                    "0-9|NUMERIC ENTRY",
                                    "-|FLIP SIGN",
                                    "SPACE|TOGGLE 0/1",
                                    "ENTER|COMMIT CHANGE",
                                    "[ ]|NUDGE UP, DOWN",
                                    "SH-ALT-V|INSERT PASTE",
                                    "SH-BSP|DELETE",
                                    "SH-ENTER|DUPE INSERT",
                                    "SH-L|SET LENGTH",
                                    "SH-S|SET START",
                                    "SH-E|SET END",
                                    "ALT-L,S,E|JUMP",
                                    "SHIFT-2|SHOW/HIDE TURTLE" };

#define HELP2_LENGTH 13
const char* help2[HELP2_LENGTH] = { "2/10 VARIABLES",
                                    " ",
                                    "X, Y, Z|GENERAL PURPOSE",
                                    "T|USE FOR TIME",
                                    "A, B, C, D|ASSIGNED TO 1-4",
                                    " ",
                                    "// SPECIAL VARIABLES",
                                    "I|USED BY LOOP",
                                    "O|INCREMENTS ON READ",
                                    "DRUNK|INC BY -1, 0, +1",
                                    "Q|SHIFT REGISTER",
                                    "Q.N|SET Q LENGTH",
                                    "Q.AVG|AVERAGE OF ALL Q" };

#define HELP3_LENGTH 26
const char* help3[HELP3_LENGTH] = { "3/10 PARAMETERS",
                                    " ",
                                    "TR A-D|SET TR VALUE (0,1)",
                                    "TR.TIME A-D|TR PULSE TIME",
                                    "CV 1-4|CV TARGET VALUE",
                                    "CV.SLEW 1-4|CV SLEW TIME (MS)",
                                    "CV.SET 1-4|SET CV (NO SLEW)",
                                    "CV.OFF 1-4|ADD CV OFFSET",
                                    " ",
                                    "IN|GET IN JACK VAL",
                                    "IN.SCALE X Y",
                                    "    SCALE IN RANGE",
                                    "PARAM|GET KNOB VAL",
                                    "PARAM.SCALE X Y",
                                    "    SCALE KNOB RANGE",
                                    " ",
                                    "M|METRO TIME (MS)",
                                    "M.ACT|ENABLE METRO (0/1)",
                                    "M.RESET|HARD RESET TIMER",
                                    " ",
                                    "TIME|TIMER COUNT (MS)",
                                    "TIME.ACT|ENABLE TIMER (0/1)",
                                    " ",
                                    "SCRIPT A|GET/RUN SCRIPT",
                                    "SCENE|GET/SET SCENE #",
                                    "LAST N|GET SCRIPT LAST RUN" };

#define HELP4_LENGTH 11
const char* help4[HELP4_LENGTH] = { "4/10 DATA AND TABLES",
                                    " ",
                                    "ALL PARAMS HAVE 16B RANGE",
                                    "-32768 TO 32767",
                                    " ",
                                    "// LOOKUP TABLES",
                                    "N 0-127|CONVERT TO 1V/8VE",
                                    "V 0-10|VOLT LOOKUP",
                                    "VV 0-1000|V WITH 2 DECIMALS",
                                    "BPM 2-MAX|MS PER BPM",
                                    "EXP X|EXPO LOOKUP" };

#define HELP5_LENGTH 55
const char* help5[HELP5_LENGTH] = { "5/10 OPERATORS",
                                    " ",
                                    "RAND A|RANDOM 0 - A",
                                    "RRAND A B|RANDOM A - B",
                                    "TOSS|RANDOM 0 OR 1",
                                    " ",
                                    "CHAOS|CHAOTIC NUMBER",
                                    "CHAOS.R|CHAOS PARAMETER",
                                    "CHAOS.ALG|CHAOS ALGORITHM",
                                    " ",
                                    "AVG A B|AVERAGE OF INS",
                                    "MIN A B|LESSER OF INS",
                                    "MAX A B|GREATER OF INS",
                                    " ",
                                    "ADD A B|A + B",
                                    "SUB A B|A - B",
                                    "MUL A B|A * B",
                                    "DIV A B|A / B",
                                    "MOD A B|REMAINDER OF A / B",
                                    " ",
                                    "// LOGIC COMPARISONS",
                                    "RETURNS 1 (TRUE) OR 0 (FALSE)",
                                    "EQ A B|A EQUALS B",
                                    "NE A B|A NOT EQUAL TO B",
                                    "GT A B|A GREATER THAN B",
                                    "LT A B|A LESS THAN B",
                                    "EZ A|A EQUALS 0",
                                    "NZ A|A NOT EQUAL TO 0",
                                    " ",
                                    "RSH A B|BITSHIFT A RIGHT B",
                                    "LSH A B|BITSHIFT A LEFT B",
                                    "| A B|BITWISE A OR B",
                                    "& A B|BITWISE A AND B",
                                    "^ A B|BITWISE A XOR B",
                                    "~ A|BITWISE NOT A",
                                    "BSET A B|SET BIT B OF A",
                                    "BGET A B|GET BIT B OF A",
                                    "BCLR A B|CLEAR BIT B OF A",
                                    " ",
                                    "LIM A B C|CLAMP A WITHIN B-C",
                                    "WRAP A B C|WRAP A AROUND B-C",
                                    "SCALE A B X Y I",
                                    "    SCALE I FROM A..B TO X..Y",
                                    "QT A B|QUANTIZE A TO B*X",
                                    " ",
                                    "// EUCLIDEAN OPERATOR",
                                    "ER F L I",
                                    "   F = FILL (1-32)",
                                    "   L = LENGTH (1-32)",
                                    "   I = STEP (ANY)",
                                    "   RETURNS 0 OR 1",
                                    " ",
                                    "// SPECIAL OPERATORS",
                                    "TR.TOG X|FLIP STATE OF TR X",
                                    "TR.PULSE X|PULSE TR X" };

#define HELP6_LENGTH 31
const char* help6[HELP6_LENGTH] = { "6/10 PRE :",
                                    " ",
                                    "EACH PRE NEEDS A : FOLLOWED",
                                    "BY A COMMAND TO OPERATE ON",
                                    " ",
                                    "PROB A: | PROBABALITY 0-100",
                                    " ",
                                    "DEL A: |DELAY BY A (MS)",
                                    "DEL.CLR|KILL ALL DELAYS",
                                    " ",
                                    "S: |PUSH ONTO STACK",
                                    "S.CLR|CLEAR S",
                                    "S.ALL|EXECUTE ALL S",
                                    "S.POP|EXECUTE NEWEST S",
                                    "S.L|QUERY LENGTH OF S",
                                    " ",
                                    "IF A: |EXECUTE IF NON-ZERO",
                                    "ELIF A: |AFTER IF WHEN TRUE",
                                    "ELSE: |AFTER FAILED IF",
                                    " ",
                                    "L A B: |ITERATE FROM A-B",
                                    "NB: I IS UPDATED EACH TIME",
                                    " ",
                                    "W X:|ITERATE WHILE X",
                                    " ",
                                    "EVERY X:|EXECUTE EACH X",
                                    "SKIP X:|EXECUTE EACH BUT X",
                                    "OTHER:|EXECUTE OTHERWISE",
                                    "SYNC X|SYNC TO STEP X",
                                    " ",
                                    "BREAK|STOP EXECUTION" };

#define HELP7_LENGTH 26
const char* help7[HELP7_LENGTH] = { "7/10 PATTERNS",
                                    " ",
                                    "// DIRECT ACCESS",
                                    "P A|GET VAL AT INDEX A",
                                    "P A B|SET VAL AT A TO B",
                                    "P.N A|SELECT BANK A",
                                    "PN A B|GET BANK A, IDX B",
                                    "PN A B C|BANK A, IDX B TO C",
                                    " ",
                                    "// CHANGES LENGTH",
                                    "P.INS A B|INSERT B AT IDX A",
                                    "P.RM A|DELETE ENTRY AT A",
                                    "P.PUSH A|ADD A TO END",
                                    "P.POP|RETURN & REM LAST",
                                    " ",
                                    "// ACTIVE RANGE",
                                    "P.L A|GET/SET LENGTH",
                                    "P.WRAP A|ENABLE WRAPPING",
                                    "P.START A|GET/SET START",
                                    "P.END A|GET/SET END",
                                    " ",
                                    "// READ HEAD",
                                    "P.I A|GET/SET POSITION",
                                    "P.HERE A|GET/SET VAL AT P.I",
                                    "P.NEXT A|GET/SET NEXT POS",
                                    "P.PREV A|GET/SET PREV POS" };
#define HELP8_LENGTH 17
const char* help8[HELP8_LENGTH] = { "8/10 TURTLE",
                                    " ",
                                    "// CRAWLS TRACKER DATA",
                                    "@|GET/SET DATA",
                                    "@X/@Y|GET/SET POSITION",
                                    "@F X1 Y1 X2 Y2",
                                    " |SET FENCE",
                                    "  OR @FX1/@FY1/@FX2/@FY2",
                                    "@BUMP 1|STOP AT FENCE",
                                    "@WRAP 1|WRAP AT FENCE",
                                    "@BOUNCE 1|BOUNCE OFF FENCE",
                                    "@MOVE X Y|MOVE RELATIVE",
                                    "@DIR 0-360|GET/SET DIRECTION",
                                    "@SPEED|GET/SET CENTICELLS",
                                    "@STEP|MOVE AT SPEED/DIR",
                                    "@SCRIPT N|GET/SET EDGE SCRIPT",
                                    "@SHOW 1/0|DISPLAY < ON TRACKER" };

#define HELP9_LENGTH 36
const char* help9[HELP9_LENGTH] = { "9/10 TELEX INPUT",
                                    " ",
                                    "TI.PARAM X|(TI.PRM)",
                                    "    GET KNOB VALUE",
                                    "TI.PARAM.QT X",
                                    "    KNOB VALUE QUANTIZED",
                                    "TI.PARAM.N X",
                                    "    KNOB AS SCALE NOTE",
                                    "TI.PARAM.SCALE X",
                                    "    SELECT KNOB QUANT SCALE",
                                    "TI.PARAM.MAP X Y Z",
                                    "    MAP KNOB VALUE TO Y..Z",
                                    "TI.IN X",
                                    "    GET INPUT VAL",
                                    "TI.IN.QT X",
                                    "    IN VALUE QUANTIZED",
                                    "TI.IN.N X",
                                    "    IN AS SCALE NOTE",
                                    "TI.IN.SCALE X",
                                    "    SELECT IN QUANT SCALE",
                                    "TI.IN.MAP X Y Z",
                                    "    MAP IN VALUE TO Y..Z",
                                    "TI.PARAM.INIT X",
                                    "    RESET KNOB X",
                                    "TI.IN.INIT X",
                                    "    RESET IN X",
                                    "TI.INIT D",
                                    "    RESET DEVICE D",
                                    "TI.PARAM.CALIB X Y",
                                    "    CALIBRATE KNOB X",
                                    "TI.IN.CALIB X Y",
                                    "    CALIBRATE IN X",
                                    "TI.STORE D",
                                    "    STORE CALIB FOR DEVICE D",
                                    "TI.RESET D",
                                    "    RESET CALIB FOR DEVICE D" };

#define HELP10_LENGTH 164
const char* help10[HELP10_LENGTH] = {
                                    "10/10 TELEX OUTPUT",
                                    " ",
                                    "TO.TR X Y",
                                    "    SET TR VALUE (0/1)",
                                    "TO.TR.TOG X",
                                    "    TOGGLE TR",
                                    "TO.TR.P X",
                                    "    PULSE TR",
                                    "TO.TR.P.DIV X Y",
                                    "    SET TR X CLOCK DIV TO Y",
                                    "TO.TR.P.MUTE X Y",
                                    "    MUTE TR X (0/1)",
                                    "TO.TR.TIME X Y",
                                    "    SET TR PULSE TIME (MS)",
                                    "TO.TR.TIME.S X Y",
                                    "    SET TR PULSE TIME (SEC)",
                                    "TO.TR.TIME.M",
                                    "    SET TR PULSE TIME (MIN)",
                                    "TO.TR.WIDTH X Y",
                                    "    SET TR DUTY (%)",
                                    "TO.TR.POL X Y",
                                    "    SET TR POLARITY",
                                    "TO.TR.M.ACT X Y",
                                    "    ENABLE METRO (0/1)",
                                    "TO.TR.M X Y",
                                    "    SET METRO RATE (MS)",
                                    "TO.TR.M.S X Y",
                                    "    SET METRO RATE (SEC)",
                                    "TO.TR.M.M X Y",
                                    "    SET METRO RATE (MIN)",
                                    "TO.TR.M.BPM X Y",
                                    "    SET METRO RATE (BPM)",
                                    "TO.TR.M.COUNT X Y",
                                    "    SET METRO REPEAT",
                                    "TO.TR.M.MUL X Y",
                                    "    SET METRO MULT",
                                    "TO.TR.M.SYNC X",
                                    "    SYNC METRO",
                                    "TO.M.ACT D Y",
                                    "    ENABLE DEVICE METRO (0/1)",
                                    "TO.M D Y",
                                    "    SET DEVICE METRO RATE (MS)",
                                    "TO.M.S D Y",
                                    "    AS ABOVE IN SECONDS",
                                    "TO.M.M D Y",
                                    "    AS ABOVE IN MINUTES",
                                    "TO.M.BPM D Y",
                                    "    AS ABOVE IN BPM",
                                    "TO.M.COUNT D Y",
                                    "    SET DEVICE METRO REPEAT",
                                    "TO.M.SYNC D",
                                    "   SYNC DEVICE METRO",
                                    "TO.CV X Y",
                                    "    SET CV WITH SLEW",
                                    "TO.CV.SLEW X Y",
                                    "    SET CV SLEW (MS)",
                                    "TO.CV.SLEW.S X Y",
                                    "    SET CV SLEW (SEC)",
                                    "TO.CV.SLEW.M",
                                    "    SET CV SLEW (MIN)",
                                    "TO.CV.SET X Y",
                                    "    SET CV WITH NO SLEW",
                                    "TO.CV.OFF X Y",
                                    "    SET CV OFFSET",
                                    "TO.CV.QT X Y",
                                    "    SET CV QUANTIZED",
                                    "TO.CV.QT.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.CV.N X Y",
                                    "    SET CV TO SCALE NOTE",
                                    "TO.CV.N.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.CV.LOG X Y",
                                    "    SET CV LOG MODE",
                                    "TO.OSC X Y",
                                    "    SET CV OSC FREQ",
                                    "TO.OSC.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.QT X Y",
                                    "    SET CV OSC QUANTIZED",
                                    "TO.OSC.QT.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.N X Y",
                                    "    SET CV OSC TO SCALE NOTE",
                                    "TO.OSC.N.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.FQ X Y",
                                    "    SET CV OSC (HZ)",
                                    "TO.OSC.LFO X Y",
                                    "    SET CV LFO RATE (MILLIHZ)",
                                    "TO.OSC.LFO.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.CYC X Y",
                                    "    SET CV OSC CYCLE (MS)",
                                    "TO.OSC.CYC.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.CYC.S X Y",
                                    "    SET CV OSC CYCLE (SEC)",
                                    "TO.OSC.CYC.S.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.CYC.M X Y",
                                    "    SET CV OSC CYCLE (MIN)",
                                    "TO.OSC.CYC.M.SET X Y",
                                    "    AS ABOVE NO SLEW",
                                    "TO.OSC.SCALE X Y",
                                    "    CV X QT SCALE Y",
                                    "TO.OSC.WAVE X Y",
                                    "    SELECT CV OSC WAVEFORM",
                                    "TO.OSC.RECT X Y",
                                    "    SET OSC RECTIFY (-2..2)",
                                    "TO.OSC.WIDTH X Y",
                                    "    SET OSC PULSEWIDTH",
                                    "TO.OSC.SYNC X",
                                    "    RESET OSC PHASE",
                                    "TO.OSC.PHASE X Y",
                                    "    SET OSC PHASE",
                                    "TO.OSC.SLEW X Y",
                                    "    SET OSC SLEW (MS)",
                                    "TO.OSC.SLEW.S X Y",
                                    "    SET OSC SLEW (SEC)",
                                    "TO.OSC.SLEW.M X Y",
                                    "    SET OSC SLEW (MIN)",
                                    "TO.OSC.CTR X Y",
                                    "    SET OSC CENTER",
                                    "TO.ENV.ACT X Y",
                                    "    ENABLE ENV (1/0)",
                                    "TO.ENV X Y",
                                    "    SET ENV GATE (1/0)",
                                    "TO.ENV.TRIG X Y",
                                    "    TRIGGER ENV",
                                    "TO.ENV.ATT X Y",
                                    "    SET ENV ATTACK (MS)",
                                    "TO.ENV.ATT.S X Y",
                                    "    SET ENV ATTACK (SEC)",
                                    "TO.ENV.ATT.M X Y",
                                    "    SET ENV ATTACK (MIN)",
                                    "TO.ENV.DEC X Y",
                                    "    SET ENV DECAY (MS)",
                                    "TO.ENV.DEC.S X Y",
                                    "    SET ENV DECAY (SEC)",
                                    "TO.ENV.DEC.M X Y",
                                    "    SET ENV DECAY (MIN)",
                                    "TO.ENV.EOR X N",
                                    "    PULSE TR N AT END OF RISE",
                                    "    TR ON SAME DEVICE",
                                    "TO.ENV.EOC X N",
                                    "    AS ABOVE FOR END OF CYCLE",
                                    "TO.ENV.LOOP X Y",
                                    "    LOOP ENV Y TIMES",
                                    "    0 TO LOOP INDEFINITELY",
                                    "TO.TR.INIT X",
                                    "    RESET TR",
                                    "TO.CV.INIT X",
                                    "    RESET CV",
                                    "TO.INIT D",
                                    "    RESET DEVICE",
                                    "TO.KILL D",
                                    "    CANCEL DEVICE TR PULSES",
                                    "    AND CV SLEW",
                                    "TO.CV.CALIB X",
                                    "    SAVE TO.CV.OFF X",
                                    "    AS CALIB VALUE",
                                    "TO.CV.RESET X",
                                    "    RESET CV CALIB"};


////////////////////////////////////////////////////////////////////////////////
// Help mode ///////////////////////////////////////////////////////////////////

const char** help_pages[HELP_PAGES] = { help1, help2, help3, help4, help5,
                                        help6, help7, help8, help9, help10 };
const uint8_t help_length[HELP_PAGES] = {
    HELP1_LENGTH, HELP2_LENGTH, HELP3_LENGTH, HELP4_LENGTH, HELP5_LENGTH,
    HELP6_LENGTH, HELP7_LENGTH, HELP8_LENGTH, HELP9_LENGTH, HELP10_LENGTH
};

static uint8_t page_no;
static uint8_t offset;

static bool dirty;

void set_help_mode() {
    dirty = true;
}

void process_help_keys(uint8_t k, uint8_t m, bool is_held_key) {
    // <down> or C-n: line down
    if (match_no_mod(m, k, HID_DOWN) || match_ctrl(m, k, HID_N)) {
        if (offset < help_length[page_no] - 8) {
            offset++;
            dirty = true;
        }
    }
    // <up> or C-p: line up
    else if (match_no_mod(m, k, HID_UP) || match_ctrl(m, k, HID_P)) {
        if (offset) {
            offset--;
            dirty = true;
        }
    }
    // <left> or [: previous page
    else if (match_no_mod(m, k, HID_LEFT) ||
             match_no_mod(m, k, HID_OPEN_BRACKET)) {
        if (page_no) {
            offset = 0;
            page_no--;
            dirty = true;
        }
    }
    // <right> or ]: next page
    else if (match_no_mod(m, k, HID_RIGHT) ||
             match_no_mod(m, k, HID_CLOSE_BRACKET)) {
        if (page_no < HELP_PAGES - 1) {
            offset = 0;
            page_no++;
            dirty = true;
        }
    }
}

uint8_t screen_refresh_help() {
    if (!dirty) { return 0; }

    // clamp value of page_no
    if (page_no >= HELP_PAGES) page_no = HELP_PAGES - 1;

    // clamp value of offset
    if (offset >= help_length[page_no] - 8) offset = help_length[page_no] - 8;

    const char** text = help_pages[page_no];

    for (uint8_t y = 0; y < 8; y++) {
        region_fill(&line[y], 0);
        font_string_region_clip_tab(&line[y], text[y + offset], 2, 0, 0xa, 0);
    }

    dirty = false;
    return 0xFF;
};
