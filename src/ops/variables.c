#include "ops/variables.h"

#include "helpers.h"
#include "ops/op.h"
#include "random.h"
#include "teletype.h"
#include "teletype_io.h"

static void op_LAST_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_DRUNK_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_DRUNK_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_FLIP_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_FLIP_set(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_O_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_O_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_I_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_I_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_TIME_get(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_TIME_set(const void *data, scene_state_t *ss, exec_state_t *es,
                        command_state_t *cs);
static void op_TIME_ACT_get(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_TIME_ACT_set(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_J_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_J_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_K_get(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);
static void op_K_set(const void *data, scene_state_t *ss, exec_state_t *es,
                     command_state_t *cs);

// clang-format off
const tele_op_t op_A          = MAKE_SIMPLE_VARIABLE_OP(A         , variables.a         );
const tele_op_t op_B          = MAKE_SIMPLE_VARIABLE_OP(B         , variables.b         );
const tele_op_t op_C          = MAKE_SIMPLE_VARIABLE_OP(C         , variables.c         );
const tele_op_t op_D          = MAKE_SIMPLE_VARIABLE_OP(D         , variables.d         );
const tele_op_t op_DRUNK_MAX  = MAKE_SIMPLE_VARIABLE_OP(DRUNK.MAX , variables.drunk_max );
const tele_op_t op_DRUNK_MIN  = MAKE_SIMPLE_VARIABLE_OP(DRUNK.MIN , variables.drunk_min );
const tele_op_t op_DRUNK_WRAP = MAKE_SIMPLE_VARIABLE_OP(DRUNK.WRAP, variables.drunk_wrap);
const tele_op_t op_O_INC      = MAKE_SIMPLE_VARIABLE_OP(O.INC     , variables.o_inc     );
const tele_op_t op_O_MAX      = MAKE_SIMPLE_VARIABLE_OP(O.MAX     , variables.o_max     );
const tele_op_t op_O_MIN      = MAKE_SIMPLE_VARIABLE_OP(O.MIN     , variables.o_min     );
const tele_op_t op_O_WRAP     = MAKE_SIMPLE_VARIABLE_OP(O.WRAP    , variables.o_wrap    );
const tele_op_t op_T          = MAKE_SIMPLE_VARIABLE_OP(T         , variables.t         );
const tele_op_t op_TIME       = MAKE_GET_SET_OP(TIME, op_TIME_get, op_TIME_set, 0, true);
const tele_op_t op_TIME_ACT   = MAKE_GET_SET_OP(TIME.ACT, op_TIME_ACT_get, op_TIME_ACT_set, 0, true);
const tele_op_t op_LAST       = MAKE_GET_OP(LAST  , op_LAST_get, 1, true);
const tele_op_t op_X          = MAKE_SIMPLE_VARIABLE_OP(X         , variables.x         );
const tele_op_t op_Y          = MAKE_SIMPLE_VARIABLE_OP(Y         , variables.y         );
const tele_op_t op_Z          = MAKE_SIMPLE_VARIABLE_OP(Z         , variables.z         );

const tele_op_t op_DRUNK = MAKE_GET_SET_OP(DRUNK, op_DRUNK_get, op_DRUNK_set, 0, true);
const tele_op_t op_FLIP  = MAKE_GET_SET_OP(FLIP , op_FLIP_get , op_FLIP_set , 0, true);
const tele_op_t op_O     = MAKE_GET_SET_OP(O    , op_O_get    , op_O_set    , 0, true);
const tele_op_t op_I     = MAKE_GET_SET_OP(I    , op_I_get, op_I_set, 0, true);
const tele_op_t op_J     = MAKE_GET_SET_OP(J    , op_J_get, op_J_set, 0, true);
const tele_op_t op_K     = MAKE_GET_SET_OP(K    , op_K_get, op_K_set, 0, true);
// clang-format on

static void op_TIME_get(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int64_t delta = ss->variables.time_act
                        ? tele_get_ticks() - ss->variables.time
                        : ss->variables.time;
    cs_push(cs, delta & 0x7fff);
}

static void op_TIME_set(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t new_time = cs_pop(cs);
    ss->variables.time =
        ss->variables.time_act ? tele_get_ticks() - new_time : new_time;
}

static void op_TIME_ACT_get(const void *NOTUSED(data), scene_state_t *ss,
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, ss->variables.time_act ? 1 : 0);
}

static void op_TIME_ACT_set(const void *NOTUSED(data), scene_state_t *ss,
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t act = cs_pop(cs);
    if (act && ss->variables.time_act) return;
    if (!act && !ss->variables.time_act) return;
    ss->variables.time_act = act ? 1 : 0;
    ss->variables.time = tele_get_ticks() - ss->variables.time;
}

static void op_LAST_get(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t script_number = cs_pop(cs) - 1;

    // when run in LIVE mode, SCRIPT will be 0.
    // LIVE SCRIPT should give time since INIT
    // was run in this case.
    if (script_number < 0) {
        script_number = 9;
    }
    int16_t last = ss_get_script_last(ss, script_number);
    cs_push(cs, last);
}

static void op_DRUNK_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t min = ss->variables.drunk_min;
    int16_t max = ss->variables.drunk_max;
    int16_t wrap = ss->variables.drunk_wrap;
    random_state_t *r = &ss->rand_states.s.drunk.rand;

    // restrict current_value to (wrapped) bounds
    int16_t current_value =
        normalise_value(min, max, wrap, ss->variables.drunk);
    cs_push(cs, current_value);

    // calculate new value
    int16_t new_value = current_value + (random_next(r) % 3) - 1;
    ss->variables.drunk = normalise_value(min, max, wrap, new_value);
}

static void op_DRUNK_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    ss->variables.drunk = cs_pop(cs);
}

static void op_FLIP_get(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t flip = ss->variables.flip;
    cs_push(cs, flip);
    ss->variables.flip = flip == 0;
}

static void op_FLIP_set(const void *NOTUSED(data), scene_state_t *ss,
                        exec_state_t *NOTUSED(es), command_state_t *cs) {
    ss->variables.flip = cs_pop(cs) != 0;
}

static void op_O_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t min = ss->variables.o_min;
    int16_t max = ss->variables.o_max;
    int16_t wrap = ss->variables.o_wrap;

    // restrict current_value to (wrapped) bounds
    int16_t current_value = normalise_value(min, max, wrap, ss->variables.o);
    cs_push(cs, current_value);

    // calculate new value
    ss->variables.o =
        normalise_value(min, max, wrap, current_value + ss->variables.o_inc);
}

static void op_O_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *NOTUSED(es), command_state_t *cs) {
    ss->variables.o = cs_pop(cs);
}

static void op_I_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                     exec_state_t *es, command_state_t *cs) {
    cs_push(cs, es_variables(es)->i);
}

static void op_I_set(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                     exec_state_t *es, command_state_t *cs) {
    es_variables(es)->i = cs_pop(cs);
}

static void op_J_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *es, command_state_t *cs) {
    int16_t sn = es_variables(es)->script_number;
    cs_push(cs, ss->variables.j[sn]);
}

static void op_J_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *es, command_state_t *cs) {
    int16_t sn = es_variables(es)->script_number;
    ss->variables.j[sn] = cs_pop(cs);
}

static void op_K_get(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *es, command_state_t *cs) {
    int16_t sn = es_variables(es)->script_number;
    cs_push(cs, ss->variables.k[sn]);
}

static void op_K_set(const void *NOTUSED(data), scene_state_t *ss,
                     exec_state_t *es, command_state_t *cs) {
    int16_t sn = es_variables(es)->script_number;
    ss->variables.k[sn] = cs_pop(cs);
}
