#include "instructions.h"

ins_data const INS_LIST[] = {
	[INS_I_ST]   = { .index = INS_I_ST,   .name = "ST",   .opcode = 0x0,  .format = INS_FORMAT_A, },
	[INS_I_LD]   = { .index = INS_I_LD,   .name = "LD",   .opcode = 0x1,  .format = INS_FORMAT_A, },
	[INS_I_STS]  = { .index = INS_I_STS,  .name = "STS",  .opcode = 0x2,  .format = INS_FORMAT_B, },
	[INS_I_LDS]  = { .index = INS_I_LDS,  .name = "LDS",  .opcode = 0x3,  .format = INS_FORMAT_B, },
	[INS_I_CALL] = { .index = INS_I_CALL, .name = "CALL", .opcode = 0x4,  .format = INS_FORMAT_C, },
	[INS_I_RET]  = { .index = INS_I_RET,  .name = "RET",  .opcode = 0x5,  .format = INS_FORMAT_E, },
	[INS_I_BRZS] = { .index = INS_I_BRZS, .name = "BRZS", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_BREQ] = { .index = INS_I_BREQ, .name = "BREQ", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_BRCS] = { .index = INS_I_BRCS, .name = "BRCS", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_BRLO] = { .index = INS_I_BRLO, .name = "BRLO", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_BRVS] = { .index = INS_I_BRVS, .name = "BRVS", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_BRLT] = { .index = INS_I_BRLT, .name = "BRLT", .opcode = 0x6,  .format = INS_FORMAT_C, },
	[INS_I_JMP]  = { .index = INS_I_JMP,  .name = "JMP",  .opcode = 0x7,  .format = INS_FORMAT_C, },
	[INS_I_ADD]  = { .index = INS_I_ADD,  .name = "ADD",  .opcode = 0x8,  .format = INS_FORMAT_A, },
	[INS_I_SUB]  = { .index = INS_I_SUB,  .name = "SUB",  .opcode = 0xa,  .format = INS_FORMAT_A, },
	[INS_I_CP]   = { .index = INS_I_CP,   .name = "CP",   .opcode = 0xb,  .format = INS_FORMAT_A, },
	[INS_I_MOV]  = { .index = INS_I_MOV,  .name = "MOV",  .opcode = 0xf,  .format = INS_FORMAT_A, },
	[INS_I_CLC]  = { .index = INS_I_CLC,  .name = "CLC",  .opcode = 0x12, .format = INS_FORMAT_E, },
	[INS_I_SEC]  = { .index = INS_I_SEC,  .name = "SEC",  .opcode = 0x13, .format = INS_FORMAT_E, },
	[INS_I_ROR]  = { .index = INS_I_ROR,  .name = "ROR",  .opcode = 0x14, .format = INS_FORMAT_D, },
	[INS_I_ROL]  = { .index = INS_I_ROL,  .name = "ROL",  .opcode = 0x15, .format = INS_FORMAT_D, },
	[INS_I_STOP] = { .index = INS_I_STOP, .name = "STOP", .opcode = 0x17, .format = INS_FORMAT_E, },
	[INS_I_ADDI] = { .index = INS_I_ADDI, .name = "ADDI", .opcode = 0x18, .format = INS_FORMAT_B, },
	[INS_I_SUBI] = { .index = INS_I_SUBI, .name = "SUBI", .opcode = 0x1a, .format = INS_FORMAT_B, },
	[INS_I_CPI]  = { .index = INS_I_CPI,  .name = "CPI",  .opcode = 0x1b, .format = INS_FORMAT_B, },
	[INS_I_LDI]  = { .index = INS_I_LDI,  .name = "LDI",  .opcode = 0x1f, .format = INS_FORMAT_B, },
};

static hash_table ht;

void ins_search_start(void) {
	hash_table_init(&ht);
	for (size_t i = 0; i < INS_LEN; i++) {
		hash_table_put(&ht, INS_LIST[i].name, &i);
	}
}

void ins_search_stop(void) {
	hash_table_free(&ht);
}

ins_data const *ins_search(char *name) {
	void *search = hash_table_get(&ht, name);
	ins_data const *result = { 0 };
	if (search) {
		result = &INS_LIST[*(int *) search];
	}
	return result;
}
