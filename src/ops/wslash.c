#include "ops/wslash.h"

#include "helpers.h"
#include "ii.h"
#include "teletype_io.h"

static void op_WS_REC_get(const void *data, scene_state_t *ss,
                          exec_state_t *es, command_state_t *cs);
static void op_WS_PLAY_get(const void *data, scene_state_t *ss, exec_state_t *es,
                           command_state_t *cs);
static void op_WS_LOOP_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_WS_CUE_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);

// clang-format off
const tele_op_t op_WS_REC  = MAKE_GET_OP(WS.REC , op_WS_REC_get , 1, false);
const tele_op_t op_WS_PLAY = MAKE_GET_OP(WS.PLAY, op_WS_PLAY_get, 1, false);
const tele_op_t op_WS_LOOP = MAKE_GET_OP(WS.LOOP, op_WS_LOOP_get, 1, false);
const tele_op_t op_WS_CUE  = MAKE_GET_OP(WS.CUE , op_WS_CUE_get , 1, false);
// clang-format on


static void op_WS_REC_get(const void *NOTUSED(data),
                          scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    uint8_t d[] = { WS_REC, a };
    tele_ii_tx(WS_ADDR, d, 2);
}

static void op_WS_PLAY_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    uint8_t d[] = { WS_PLAY, a };
    tele_ii_tx(WS_ADDR, d, 2);
}

static void op_WS_LOOP_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    uint8_t d[] = { WS_LOOP, a };
    tele_ii_tx(WS_ADDR, d, 2);
}

static void op_WS_CUE_get(const void *NOTUSED(data),
                          scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    uint8_t d[] = { WS_CUE, a };
    tele_ii_tx(WS_ADDR, d, 2);
}
