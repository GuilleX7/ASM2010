/** @file cs_instructions.c */

#include "../hash_table.h"

#include "cs_registers.h"
#include "cs_op.h"
#include "cs_instructions.h"

#define CS_INS_DEFINE(index, name, opcode, format, exec, stepper, microstepper, fl1, fl2, fl3, fl4, fl5) \
    {{fl1, fl2, fl3, fl4, fl5}, index, name, stepper, microstepper, opcode, format, exec}
#define CS_INS_DEFINE_EMPTY {0}

cs_instruction const cs_ins_list[] = {
    CS_INS_DEFINE(CS_INS_I_ST, "ST", 0x0, CS_INS_FORMAT_A, true, cs_op_st_stepper, cs_op_st_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
        CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
        CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
        CS_SIGNAL_WMEM | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_LD, "LD", 0x1, CS_INS_FORMAT_A, true, cs_op_st_stepper, cs_op_st_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
        CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
        CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
        CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_STS, "STS", 0x2, CS_INS_FORMAT_B, true, cs_op_sts_stepper, cs_op_sts_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
        CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
        CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
        CS_SIGNAL_WMEM | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_LDS, "LDS", 0x3, CS_INS_FORMAT_B, true, cs_op_lds_stepper, cs_op_lds_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
        CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
        CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
        CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_CALL, "CALL", 0x4, CS_INS_FORMAT_C, true, cs_op_call_stepper, cs_op_call_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM | CS_SIGNAL_RPC | CS_SIGNAL_WMDR,
        CS_SIGNAL_RSP | CS_SIGNAL_DSP | CS_SIGNAL_WMAR,
        CS_SIGNAL_RAC | CS_SIGNAL_WPC | CS_SIGNAL_WMEM,
        CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_RET, "RET", 0x5, CS_INS_FORMAT_E, true, cs_op_ret_stepper, cs_op_ret_microstepper,
        CS_SIGNAL_ISP,
        CS_SIGNAL_RSP | CS_SIGNAL_WMAR,
        CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
        CS_SIGNAL_WPC | CS_SIGNAL_IOMDR,
        CS_SIGNALS_FETCH
    ),
    CS_INS_DEFINE(CS_INS_I_BRXX, 0, 0x6, CS_INS_FORMAT_C, true, cs_op_brxx_stepper, cs_op_brxx_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
        CS_SIGNAL_RAC | CS_SIGNAL_WPC,
        CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_JMP, "JMP", 0x7, CS_INS_FORMAT_C, true, cs_op_jmp_stepper, cs_op_jmp_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
        CS_SIGNAL_RAC | CS_SIGNAL_WPC,
        CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_ADD, "ADD", 0x8, CS_INS_FORMAT_A, true, cs_op_add_stepper, cs_op_add_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_SUB, "SUB", 0xa, CS_INS_FORMAT_A, true, cs_op_sub_stepper, cs_op_sub_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_CP, "CP", 0xb, CS_INS_FORMAT_A, true, cs_op_cp_stepper, cs_op_cp_stepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_MOV, "MOV", 0xf, CS_INS_FORMAT_A, true, cs_op_mov_stepper, cs_op_mov_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_CLC, "CLC", 0x12, CS_INS_FORMAT_E, true, cs_op_clc_stepper, cs_op_clc_stepper,
        CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_SEC, "SEC", 0x13, CS_INS_FORMAT_E, true, cs_op_sec_stepper, cs_op_sec_stepper,
        CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP0 | CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_ROR, "ROR", 0x14, CS_INS_FORMAT_D, true, cs_op_ror_stepper, cs_op_ror_microstepper,
        CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_ROL, "ROL", 0x15, CS_INS_FORMAT_D, true, cs_op_rol_stepper, cs_op_rol_microstepper,
        CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP0 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_STOP, "STOP", 0x17, CS_INS_FORMAT_E, true, cs_op_stop_stepper, cs_op_stop_stepper,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_ADDI, "ADDI", 0x18, CS_INS_FORMAT_B, true, cs_op_addi_stepper, cs_op_addi_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_SUBI, "SUBI", 0x1a, CS_INS_FORMAT_B, true, cs_op_subi_stepper, cs_op_subi_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_CPI, "CPI", 0x1b, CS_INS_FORMAT_B, true, cs_op_cpi_stepper, cs_op_cpi_stepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_SRW | CS_SIGNAL_INM | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE_EMPTY,
    CS_INS_DEFINE(CS_INS_I_LDI, "LDI", 0x1f, CS_INS_FORMAT_B, true, cs_op_ldi_stepper, cs_op_ldi_microstepper,
        CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
        CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    /* BR-like instructions */
    CS_INS_DEFINE(CS_INS_I_BRZS, "BRZS", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_BREQ, "BREQ", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_BRCS, "BRCS", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_BRLO, "BRLO", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_BRVS, "BRVS", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    ),
    CS_INS_DEFINE(CS_INS_I_BRLT, "BRLT", 0x6, CS_INS_FORMAT_C, false, 0, 0,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE,
        CS_SIGNALS_NONE
    )
};

static hash_table ht = { 0 };
static bool ht_initiated = false;

bool cs_ins_search_start(void) {
    size_t i = 0;
    if (ht_initiated || !hash_table_init(&ht)) {
        return false;
    }
    for (i = 0; i < CS_INS_LEN; i++) {
        if (cs_ins_list[i].name) {
            if (!hash_table_put(&ht, cs_ins_list[i].name, &i)) {
                hash_table_free(&ht);
                return false;
            }
        }
    }
    ht_initiated = true;
    return true;
}

void cs_ins_search_stop(void) {
    if (ht_initiated) {
        hash_table_free(&ht);
        ht_initiated = false;
    }
}

cs_instruction const *cs_ins_search_by_name(char *name) {
    void *search = hash_table_get(&ht, name);
    cs_instruction const *result = { 0 };
    if (search) {
        result = &cs_ins_list[*(int *) search];
    }
    return result;
}

cs_instruction const *cs_ins_get_from_sentence(unsigned short sentence) {
    unsigned char opcode = CS_GET_OPCODE(sentence);
    cs_instruction const *cs_instruction = &cs_ins_list[opcode];
    /* Check for BR-like instruction */
    unsigned char jmp_condition = 0;
    if (opcode == cs_ins_list[CS_INS_I_BRXX].opcode) {
        jmp_condition = CS_GET_JMP_CONDITION(sentence);
        switch (jmp_condition) {
        case CS_JMP_COND_EQUAL:
            cs_instruction = &cs_ins_list[CS_INS_I_BRZS];
        case CS_JMP_COND_LOWER:
            cs_instruction = &cs_ins_list[CS_INS_I_BRCS];
        case CS_JMP_COND_OVERFLOW:
            cs_instruction = &cs_ins_list[CS_INS_I_BRVS];
        case CS_JMP_COND_SLOWER:
            cs_instruction = &cs_ins_list[CS_INS_I_BRLT];
        }
    }
    /* Check for a valid instruction */
    if (!cs_instruction->name) {
        cs_instruction = 0;
    }
    return cs_instruction;
}

unsigned char cs_ins_get_jmp_condition(cs_instruction const *const cs_instruction) {
    switch (cs_instruction->index) {
    case CS_INS_I_BRCS:
    case CS_INS_I_BRLO:
        return CS_JMP_COND_LOWER;
    case CS_INS_I_BRVS:
        return CS_JMP_COND_OVERFLOW;
    case CS_INS_I_BRLT:
        return CS_JMP_COND_SLOWER;
    case CS_INS_I_BRZS:
    case CS_INS_I_BREQ:
    default:
        return CS_JMP_COND_EQUAL;
    }
}