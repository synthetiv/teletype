#include "ops/delay.h"

#include "helpers.h"
#include "teletype.h"
#include "teletype_io.h"

static void mod_DEL_func(scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs,
                         const tele_command_t *post_command);

static void op_DEL_CLR_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);

static void mod_RAT_func(scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs,
                         const tele_command_t *post_command);

const tele_mod_t mod_DEL = MAKE_MOD(DEL, mod_DEL_func, 1);
const tele_op_t op_DEL_CLR = MAKE_GET_OP(DEL.CLR, op_DEL_CLR_get, 0, false);
const tele_mod_t mod_RAT = MAKE_MOD(RAT, mod_RAT_func, 2);

static void mod_DEL_func(scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs,
                         const tele_command_t *post_command) {
    int16_t i = 0;
    int16_t a = cs_pop(cs);

    if (a < 1) a = 1;

    // 0 is the magic number for an empty slot.
    // Be careful not to set delay.time[i] to 0 before calling this function.
    while (ss->delay.time[i] != 0 && i != DELAY_SIZE) i++;

    if (i < DELAY_SIZE) {
        ss->delay.count++;
        ss->delay.time[i] = a;
        ss->delay.origin_script[i] = es_variables(es)->script_number;
        ss->delay.origin_i[i] = es_variables(es)->i;
        copy_command(&ss->delay.commands[i], post_command);
        tele_has_delays(ss->delay.count > 0);
    }
}

static void op_DEL_CLR_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es),
                           command_state_t *NOTUSED(cs)) {
    clear_delays(ss);
}

static void mod_RAT_func(scene_state_t *ss, exec_state_t *es, 
				command_state_t *cs,
				const tele_command_t *post_command) {
    int16_t i = 0;
    int16_t a = cs_pop(cs); //delay time
    int16_t b = cs_pop(cs); //number of chained delays
    int16_t c; //incremented delay time 

    if (a < 1) a = 1;
    c = a;

    // 0 is the magic number for an empty slot.
    // Be careful not to set delay.time[i] to 0 before calling this function.
    while (ss->delay.time[i] != 0 && i != DELAY_SIZE) i++;

    while (i < DELAY_SIZE && b > 0) {
        ss->delay.count++;
        ss->delay.time[i] = c;
        ss->delay.origin_script[i] = es_variables(es)->script_number;
        ss->delay.origin_i[i] = es_variables(es)->i;
        copy_command(&ss->delay.commands[i], post_command);
	
	    c += a; //increment delay time for next chained delay
	    b--; //decrement delay chain count
	    i++;
    }

    tele_has_delays(ss->delay.count > 0);
}
