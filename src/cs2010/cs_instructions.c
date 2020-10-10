/** @file cs_instructions.c */

#include "../hash_table.h"

#include "cs_registers.h"
#include "cs_op.h"
#include "cs_instructions.h"

cs_instruction const cs_ins_list[] = {
    [CS_INS_I_ST] = {
        .index = CS_INS_I_ST,
        .name = "ST",
        .opcode = 0x0,
        .format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
            CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
            CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
            CS_SIGNAL_WMEM | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_st_stepper,
        .microstepper = cs_op_st_microstepper,
    },
    [CS_INS_I_LD] = {
        .index = CS_INS_I_LD,
        .name = "LD",
        .opcode = 0x1,
        .format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
            CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
            CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
            CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_ld_stepper,
        .microstepper = cs_op_ld_microstepper,
    },
    [CS_INS_I_STS] = {
        .index = CS_INS_I_STS,
        .name = "STS",
        .opcode = 0x2,
        .format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
            CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
            CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
            CS_SIGNAL_WMEM | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_sts_stepper,
        .microstepper = cs_op_sts_microstepper,
    },
    [CS_INS_I_LDS] = {
        .index = CS_INS_I_LDS,
        .name = "LDS",
        .opcode = 0x3,
        .format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
            CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
            CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
            CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_lds_stepper,
        .microstepper = cs_op_lds_microstepper,
    },
    [CS_INS_I_CALL] = {
        .index = CS_INS_I_CALL,
        .name = "CALL",
        .opcode = 0x4,
        .format = CS_INS_FORMAT_C,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM | CS_SIGNAL_RPC | CS_SIGNAL_WMDR,
            CS_SIGNAL_RSP | CS_SIGNAL_DSP | CS_SIGNAL_WMAR,
            CS_SIGNAL_RAC | CS_SIGNAL_WPC | CS_SIGNAL_WMEM,
            CS_SIGNALS_FETCH
        },
        .stepper = cs_op_call_stepper,
        .microstepper = cs_op_call_microstepper,
    },
    [CS_INS_I_RET] = {
        .index = CS_INS_I_RET,
        .name = "RET",
        .opcode = 0x5,
        .format = CS_INS_FORMAT_E,
        .signals = {
            CS_SIGNAL_ISP,
            CS_SIGNAL_RSP | CS_SIGNAL_WMAR,
            CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
            CS_SIGNAL_WPC | CS_SIGNAL_IOMDR,
            CS_SIGNALS_FETCH
        },
        .stepper = cs_op_ret_stepper,
        .microstepper = cs_op_ret_microstepper,
    },
    [CS_INS_I_BRXX] = {
        .index = CS_INS_I_BRXX,
        .name = 0,
        .opcode = 0x6,
        .format = CS_INS_FORMAT_C,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
            CS_SIGNAL_RAC | CS_SIGNAL_WPC,
            CS_SIGNALS_FETCH
        },
        .stepper = cs_op_brxx_stepper,
        .microstepper = cs_op_brxx_microstepper,
    },
    [CS_INS_I_JMP] = {
		.index = CS_INS_I_JMP,
		.name = "JMP",
		.opcode = 0x7,
		.format = CS_INS_FORMAT_C,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
            CS_SIGNAL_RAC | CS_SIGNAL_WPC,
            CS_SIGNALS_FETCH
        },
        .stepper = cs_op_jmp_stepper,
        .microstepper = cs_op_jmp_microstepper,
	},
    [CS_INS_I_ADD] = {
		.index = CS_INS_I_ADD,
		.name = "ADD",
		.opcode = 0x8,
		.format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_add_stepper,
        .microstepper = cs_op_add_microstepper,
	},
    [CS_INS_I_SUB] = {
		.index = CS_INS_I_SUB,
		.name = "SUB",
		.opcode = 0xa,
		.format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_sub_stepper,
        .microstepper = cs_op_sub_microstepper,
	},
    [CS_INS_I_CP] = {
		.index = CS_INS_I_CP,
		.name = "CP",
		.opcode = 0xb,
		.format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_SRW | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_cp_stepper,
        .microstepper = cs_op_cp_microstepper,
	},
    [CS_INS_I_MOV] = {
		.index = CS_INS_I_MOV,
		.name = "MOV",
		.opcode = 0xf,
		.format = CS_INS_FORMAT_A,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_mov_stepper,
        .microstepper = cs_op_mov_microstepper,
	},
    [CS_INS_I_CLC] = {
		.index = CS_INS_I_CLC,
		.name = "CLC",
		.opcode = 0x12,
		.format = CS_INS_FORMAT_E,
        .signals = {
            CS_SIGNAL_SRW | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_clc_stepper,
        .microstepper = cs_op_clc_microstepper,
	},
    [CS_INS_I_SEC] = {
		.index = CS_INS_I_SEC,
		.name = "SEC",
		.opcode = 0x13,
		.format = CS_INS_FORMAT_E,
        .signals = {
            CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP0 | CS_SIGNAL_SRW | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_sec_stepper,
        .microstepper = cs_op_sec_microstepper,
	},
    [CS_INS_I_ROR] = {
		.index = CS_INS_I_ROR,
		.name = "ROR",
		.opcode = 0x14,
		.format = CS_INS_FORMAT_D,
        .signals = {
            CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_ror_stepper,
        .microstepper = cs_op_ror_microstepper,
	},
    [CS_INS_I_ROL] = {
		.index = CS_INS_I_ROL,
		.name = "ROL",
		.opcode = 0x15,
		.format = CS_INS_FORMAT_D,
        .signals = {
            CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP0 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_rol_stepper,
        .microstepper = cs_op_rol_microstepper,
	},
    [CS_INS_I_STOP] = {
		.index = CS_INS_I_STOP,
		.name = "STOP",
		.opcode = 0x17,
		.format = CS_INS_FORMAT_E,
        .signals = {
            CS_SIGNALS_NONE
        },
        .stepper = cs_op_stop_stepper,
        .microstepper = cs_op_stop_microstepper,
	},
    [CS_INS_I_ADDI] = {
		.index = CS_INS_I_ADDI,
		.name = "ADDI",
		.opcode = 0x18,
		.format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_addi_stepper,
        .microstepper = cs_op_addi_microstepper,
	},
    [CS_INS_I_SUBI] = {
		.index = CS_INS_I_SUBI,
		.name = "SUBI",
		.opcode = 0x1a,
		.format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_subi_stepper,
        .microstepper = cs_op_subi_microstepper,
	},
    [CS_INS_I_CPI] = {
        .index = CS_INS_I_CPI,
        .name = "CPI",
        .opcode = 0x1b,
        .format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_SRW | CS_SIGNAL_INM | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_cpi_stepper,
        .microstepper = cs_op_cpi_microstepper,
    },
    [CS_INS_I_LDI] = {
        .index = CS_INS_I_LDI,
        .name = "LDI",
        .opcode = 0x1f,
        .format = CS_INS_FORMAT_B,
        .signals = {
            CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_WAC | CS_SIGNAL_INM,
            CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH
        },
        .stepper = cs_op_ldi_stepper,
        .microstepper = cs_op_ldi_microstepper,
    },
    /* BR-like instructions */
    [CS_INS_I_BRZS] = { .index = CS_INS_I_BRZS, .name = "BRZS", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
    [CS_INS_I_BREQ] = { .index = CS_INS_I_BREQ, .name = "BREQ", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
    [CS_INS_I_BRCS] = { .index = CS_INS_I_BRCS, .name = "BRCS", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
    [CS_INS_I_BRLO] = { .index = CS_INS_I_BRLO, .name = "BRLO", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
    [CS_INS_I_BRVS] = { .index = CS_INS_I_BRVS, .name = "BRVS", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
    [CS_INS_I_BRLT] = { .index = CS_INS_I_BRLT, .name = "BRLT", .opcode = 0x6, .format = CS_INS_FORMAT_C, },
};

static hash_table ht = { 0 };
static bool ht_initiated = false;

bool cs_ins_search_start(void) {
    if (ht_initiated || !hash_table_init(&ht)) {
        return false;
    }
    for (size_t i = 0; i < CS_INS_LEN; i++) {
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

cs_instruction const *cs_ins_get_from_sentence(uint16_t sentence) {
    uint8_t opcode = CS_GET_OPCODE(sentence);
    cs_instruction const *cs_instruction = &cs_ins_list[opcode];
    /* Check for BR-like instruction */
    uint8_t jmp_condition = 0;
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

uint8_t cs_ins_get_jmp_condition(cs_instruction const *const cs_instruction) {
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