#ifndef ASM2010_H
#define ASM2010_H

#include <stddef.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define ASM2010_API __declspec(dllexport)
#elif __GNUC__ >= 4
#define ASM2010_API __attribute__((visibility("default")))
#else
#define ASM2010_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Export */

/* AS_PARSE */
#define CS_AS_PARSE_INIT_OK                0
#define CS_AS_PARSE_INIT_NOT_ENOUGH_MEMORY 1
#define CS_AS_PARSE_INIT_INVALID_PLATFORM  2

#define CS_AS_PARSE_OK      0
#define CS_AS_PARSE_WARNING 1
#define CS_AS_PARSE_ERROR   2

struct cs_as_parse_info;

struct cs_as_machine_code {
    /** @brief Amount of assembled instructions  */
    size_t machine_instructions_amount;
    /** @brief Array containing assembled machine instructions */
    unsigned short *machine_instructions;
    /** @brief Array matching each machine instruction to the source assembly line
     *       that generated it. Uses the same indexes as in machine_instructions */
    size_t *matching_source_assembly_lines;
};

/**
 * @brief Creates a new parsing struct
 * @return Pointer to a new parsing struct. It must be freed using as_parse_free
 */
ASM2010_API struct cs_as_parse_info *cs_as_parse_create();

/**
 * @brief Initializes a given parsing struct
 * @param parsing_info Pointer to the parsing struct to be initialized
 * @param max_parsed_sentences_amount Maximum amount of sentences to parse
 * @param platform CS platform for which the machine code will be generated
 * @return CS_AS_PARSE_INIT_OK if success,
 *         CS_AS_PARSE_INIT_NOT_ENOUGH_MEMORY if no enough memory is available or
 *         AS_PARSE_INVALID_PLATFORM if the specified platform is invalid
 */
ASM2010_API
int cs_as_parse_init(struct cs_as_parse_info *parsing_info, size_t max_parsed_sentences_amount, unsigned char platform);

/**
 * @brief Parses a multiline source of CS assembly code
 * @param parsing_info Pointer to the parsing struct
 * @param line Pointer to the string containing the assembly source
 * @param stop_on_error Whether the process should be stopped if an error is
 *  thrown
 * @return CS_AS_PARSE_OK if assembly was parsed successfully,
 *         CS_AS_PARSE_WARNING if a warning raised or
 *         CS_AS_PARSE_ERROR if parsing was aborted due to an error
 */
ASM2010_API
int cs_as_parse_source(struct cs_as_parse_info *parsing_info, char const *source, unsigned char stop_on_error);

/**
 * @brief Parses a line of CS assembly code
 * @param parsing_info Pointer to the parsing struct
 * @param line Pointer to the string containing the assembly source
 * @return CS_AS_PARSE_OK if assembly was parsed successfully,
 *         CS_AS_PARSE_WARNING if a warning raised or
 *         CS_AS_PARSE_ERROR if parsing was aborted due to an error
 */
ASM2010_API int cs_as_parse_line(struct cs_as_parse_info *parsing_info, char const *line);

/**
 * @brief Assembles the parsing struct into CS machine instructions
 * @param parsing_info Pointer to the parsing struct
 * @param stop_on_error Whether the process should be stopped if any
 *  error is thrown
 * @return CS_AS_PARSE_OK if assembled successfully,
 *         CS_AS_PARSE_ERROR otherwise
 */
ASM2010_API
int cs_as_parse_assemble(struct cs_as_parse_info *parsing_info, unsigned char stop_on_error);

/**
 * @brief Extracts the output machine code from a parsing structure after
 *      assembling successfully. The returned structure is a read-only view.
 *      Freeing the parsing structure will invalidate the returned pointer
 * @param machine_code Pointer to the parsing struct
 * @return Pointer to the view containing the output machine code
 */
ASM2010_API
struct cs_as_machine_code *cs_as_get_machine_code(struct cs_as_parse_info *parsing_info);

/**
 * @brief Reads the log of a given parsing struct
 * @param parsing_info Pointer to the parsing struct
 * @return Pointer to the log string
 */
ASM2010_API char const *cs_as_parse_get_log(struct cs_as_parse_info *parsing_info);

/**
 * @brief Disassembles a CS machine instruction into CS assembly.
 *      The returned string must be freed by the caller.
 * @param machine_instruction CS machine instruction
 * @param platform CS platform for which the assembly will be generated
 * @return Pointer to a string containing the dissasembly if success,
 *        null pointer otherwise
 */
ASM2010_API
char *cs_as_disassemble_instruction(unsigned short machine_instruction, unsigned char platform);

/**
 * @brief Disassembles a chunk of CS machine instructions into CS assembly.
 *      Each instruction will output a single line. Lines are split by new lines.
 *      The returned string must be freed by the caller.
 * @param machine_instructions Pointer to CS machine instruction array
 * @param machine_instructions_amount Amount of CS machine instructions in the array
 * @param platform CS platform for which the assembly will be generated
 * @return Pointer to a string containing the dissasembly if success,
 *        null pointer otherwise
 */
ASM2010_API
char *cs_as_disassemble_instructions(unsigned short *machine_instructions, size_t machine_instructions_amount,
                                     unsigned char platform);

/**
 * @brief Frees a parsing struct and its associated memory
 * @param parsing_info Pointer to the parsing struct
 */
ASM2010_API void cs_as_parse_free(struct cs_as_parse_info *parsing_info);

/* M2010 */
#define CS_PLATFORM_2010 (1ul << 0)
#define CS_PLATFORM_3    (1ul << 1)

#define CS_ROM_SIZE 256
#define CS_RAM_SIZE 256

#define CS_SR_C_OFFSET 0
#define CS_SR_Z_OFFSET 1
#define CS_SR_N_OFFSET 2
#define CS_SR_V_OFFSET 3

#define CS_SR_C (1u << CS_SR_C_OFFSET)
#define CS_SR_Z (1u << CS_SR_Z_OFFSET)
#define CS_SR_N (1u << CS_SR_N_OFFSET)
#define CS_SR_V (1u << CS_SR_V_OFFSET)

#define CS_SIGNAL_WMAR            (1ul << 0)
#define CS_SIGNAL_WMDR            (1ul << 1)
#define CS_SIGNAL_IOMDR           (1ul << 2)
#define CS_SIGNAL_WMEM            (1ul << 3)
#define CS_SIGNAL_RMEM            (1ul << 4)
#define CS_SIGNAL_WIR             (1ul << 5)
#define CS_SIGNAL_WPC             (1ul << 6)
#define CS_SIGNAL_RPC             (1ul << 7)
#define CS_SIGNAL_CPC             (1ul << 8)
#define CS_SIGNAL_IPC             (1ul << 9)
#define CS_SIGNAL_ISP             (1ul << 10)
#define CS_SIGNAL_DSP             (1ul << 11)
#define CS_SIGNAL_CSP             (1ul << 12)
#define CS_SIGNAL_RSP             (1ul << 13)
#define CS_SIGNAL_INM             (1ul << 14)
#define CS_SIGNAL_SRW             (1ul << 15)
#define CS_SIGNAL_WAC             (1ul << 16)
#define CS_SIGNAL_RAC             (1ul << 17)
#define CS_SIGNAL_IMPL_SPECIFIC_0 (1ul << 18)
#define CS_SIGNAL_IMPL_SPECIFIC_1 (1ul << 19)
#define CS_SIGNAL_IMPL_SPECIFIC_2 (1ul << 20)
#define CS_SIGNAL_IMPL_SPECIFIC_3 (1ul << 21)
#define CS_SIGNAL_WREG            (1ul << 22)

#define CS2010_SIGNAL_ALUOP0 CS_SIGNAL_IMPL_SPECIFIC_0
#define CS2010_SIGNAL_ALUOP1 CS_SIGNAL_IMPL_SPECIFIC_1
#define CS2010_SIGNAL_ALUOP2 CS_SIGNAL_IMPL_SPECIFIC_2
#define CS2010_SIGNAL_ALUOP3 CS_SIGNAL_IMPL_SPECIFIC_3

#define CS2010_SIGNALS_ALUOP (CS2010_SIGNAL_ALUOP0 | CS2010_SIGNAL_ALUOP1 | CS2010_SIGNAL_ALUOP2 | CS2010_SIGNAL_ALUOP3)

#define CS2010_SIGNALS_ALUOP_CLC        0
#define CS2010_SIGNALS_ALUOP_SEC        (CS2010_SIGNAL_ALUOP0 | CS2010_SIGNAL_ALUOP1)
#define CS2010_SIGNALS_ALUOP_SHR        CS2010_SIGNAL_ALUOP2
#define CS2010_SIGNALS_ALUOP_SHL        (CS2010_SIGNAL_ALUOP0 | CS2010_SIGNAL_ALUOP2)
#define CS2010_SIGNALS_ALUOP_TRANSFER_A (CS2010_SIGNAL_ALUOP1 | CS2010_SIGNAL_ALUOP2)
#define CS2010_SIGNALS_ALUOP_ADD        CS2010_SIGNAL_ALUOP3
#define CS2010_SIGNALS_ALUOP_SUB        (CS2010_SIGNAL_ALUOP3 | CS2010_SIGNAL_ALUOP1)
#define CS2010_SIGNALS_ALUOP_TRANSFER_B (CS2010_SIGNAL_ALUOP3 | CS2010_SIGNAL_ALUOP2)

#define CS3_SIGNAL_ALU_S  CS_SIGNAL_IMPL_SPECIFIC_0
#define CS3_SIGNAL_ALU_R  CS_SIGNAL_IMPL_SPECIFIC_1
#define CS3_SIGNAL_ALU_TA CS_SIGNAL_IMPL_SPECIFIC_2
#define CS3_SIGNAL_ALU_TB CS_SIGNAL_IMPL_SPECIFIC_3

#define CS_SIGNALS_NONE  0
#define CS_SIGNALS_FETCH (CS_SIGNAL_IPC | CS_SIGNAL_WIR)

#define CS_IO_READ_NOT_CONTROLLED  0x100
#define CS_IO_WRITE_NOT_CONTROLLED 0
#define CS_IO_WRITE_CONTROLLED     1

#define CS_INIT_OK                0
#define CS_INIT_NOT_ENOUGH_MEMORY 1
#define CS_INIT_INVALID_PLATFORM  2

#define CS_LOAD_OK                       0
#define CS_LOAD_FAILED                   1
#define CS_LOAD_NOT_ENOUGH_ROM           2
#define CS_LOAD_ROM_INVALID_INSTRUCTIONS 3

typedef unsigned short cs_io_read_fn(unsigned char);
typedef unsigned char  cs_io_write_fn(unsigned char, unsigned char);

/** @brief CS registers */
struct cs_registers {
    unsigned char *regfile[8];
    unsigned short ir;
    unsigned char  r0;
    unsigned char  r1;
    unsigned char  r2;
    unsigned char  r3;
    unsigned char  r4;
    unsigned char  r5;
    unsigned char  r6;
    unsigned char  r7;
    unsigned char  sp;
    unsigned char  pc;
    unsigned char  ac;
    unsigned char  sr;
    unsigned char  mdr;
    unsigned char  mar;
};

/** CS memory */
struct cs_memory {
    unsigned short *rom;
    unsigned char  *ram;
};

struct cs_instruction_op;

/** @brief CS computer */
struct cs_machine {
    /** @brief CS memory (RAM and ROM ) */
    struct cs_memory memory;
    /** @brief CS registers */
    struct cs_registers registers;
    /** @brief CS UC signals */
    unsigned long signals;
    /** @brief I/O handlers */
    cs_io_read_fn  *io_read_fn;
    cs_io_write_fn *io_write_fn;
    /** @brief Opcode implementation (for internal use only) */
    struct cs_instruction_op const *opcodes;
    /** @brief CS platform */
    unsigned char platform;
    /** @brief Current microoperation counter (starts at 0) */
    unsigned char microop;
    /** @brief CS stop signal */
    unsigned char stopped;
};

/**
 * @brief Creates a new CS emulation instance
 * @return Pointer to a new CS emulation instance. It must be freed using cs_free
 */
ASM2010_API struct cs_machine *cs_create();

/**
 * @brief Initialize a given CS emulation instance
 * @param cs Pointer to the CS emulation instance
 * @param platform CS platform to initialize
 * @return CS_INIT_OK if success,
 *         CS_INIT_NOT_ENOUGH_MEMORY if no enough memory is available or
 *         CS_INIT_INVALID_PLATFORM if the specified platform is invalid
 */
ASM2010_API int cs_init(struct cs_machine *cs, unsigned char platform);

/**
 * @brief Sets the emulation instance's I/O handlers
 * @param io_read_fn Function to read from I/O
 * @param io_write_fn Function to write to I/O
 */
ASM2010_API
void cs_set_io_functions(struct cs_machine *cs, cs_io_read_fn io_read_fn, cs_io_write_fn io_write_fn);

/**
 * @brief Clears the selected memories
 * @param cs Pointer to the emulation instance
 * @param clear_rom Whether to wipe ROM
 * @param clear_ram Whether to wipe RAM
 */
ASM2010_API
void cs_clear_memory(struct cs_machine *cs, unsigned char clear_rom, unsigned char clear_ram);

/**
 * @brief Resets all registers and signals
 * @param cs Pointer to the emulation instance
 */
ASM2010_API void cs_reset_registers(struct cs_machine *cs);

/**
 * @brief Frees a given emulation instance
 * @param cs Pointer to the emulation instance
 */
ASM2010_API void cs_free(struct cs_machine *cs);

/**
 * @brief Loads CS machine code into the emulation instance
 * @param cs Pointer to the emulation instance
 * @param machine_code Pointer to the machine code. Since machine instructions will get
 *                copied to the emulation instance, the associated parsing structure can
 *                be freed safely hereupon
 * @param platform CS platform which the machine code belongs to
 * @return CS_LOAD_OK if success,
 *         CS_LOAD_NOT_ENOUGH_ROM if there's no enough ROM for the machine code,
 *         CS_LOAD_ROM_INVALID_INSTRUCTIONS if machine instructions don't belong to the specified
 *         platform or
 *         CS_LOAD_FAILED otherwise
 */
ASM2010_API
int cs_load_machine_code(struct cs_machine *cs, struct cs_as_machine_code *machine_code);

/**
 * @brief Loads CS machine instructions into the emulation instance
 * @param cs Pointer to the emulation instance
 * @param machine_instructions Pointer to the machine instructions
 * @param machine_instructions_amount Amount of machine instructions
 * @param platform CS platform which the machine instructions belong to
 * @return CS_LOAD_OK if success,
 *         CS_LOAD_NOT_ENOUGH_ROM if there's no enough ROM for the machine code,
 *         CS_LOAD_ROM_INVALID_INSTRUCTIONS if machine instructions don't belong to the specified
 *         platform or
 *         CS_LOAD_FAILED otherwise
 */
ASM2010_API
int cs_load_machine_instructions(struct cs_machine *cs, unsigned short *machine_instructions,
                                 size_t machine_instructions_amount);

/**
 * @brief Performs a microstep, executing the current
 *      microoperation, and fetching the next instruction
 *      if needed
 * @param cs Pointer to the emulation instance
 * @param cs_ins_op_list Instruction opcode implementation
 */
ASM2010_API void cs_microstep(struct cs_machine *cs);

/**
 * @brief Performs a fullstep, which means:
 *      - If the machine state is in the middle of an
 *        operation, it will finish the remaining
 *        microoperations
 *      - Otherwise, this is the same as calling cs_step
 * @param cs Pointer to the emulation instance
 * @param cs_ins_op_list Instruction opcode implementation
 */
ASM2010_API void cs_fullstep(struct cs_machine *cs);

/**
 * @brief Performs a blockstep, which means executing
 *      instructions until a BRxx/JMP/CALL is found
 *      or the machine halts
 * @param cs Pointer to the emulation instance
 * @param max_instructions Maximum number of instructions to execute
 *      until the execution halts because no stopping condition
 *      is met
 * @param cs_ins_op_list Instruction opcode implementation
 * @return 1 if a stopping condition was met,
 *         0 if the execution halted because of exhaustion
 */
ASM2010_API unsigned char cs_blockstep(struct cs_machine *cs, size_t max_instructions);

/**
 * @brief Performs a hard reset
 *      This includes clearing all the registers and
 *      memories
 * @param cs Pointer to the emulation instance
 * @param clear_rom Whether the ROM should be wiped out
 * @param cs_ins_op_list Instruction opcode implementation
 */
ASM2010_API void cs_hard_reset(struct cs_machine *cs, unsigned char clear_rom);

/**
 * @brief Performs a soft reset
 *      Doesn't clear memories, just resets PC and SP
 * @param cs Pointer to the emulation instance
 * @param cs_ins_op_list Instruction opcode implementation
 */
ASM2010_API void cs_soft_reset(struct cs_machine *cs);

/**
 * @brief Stub method for I/O. Always returns data from memory
 *      Default I/O handler for CS instances
 */
ASM2010_API cs_io_read_fn cs_io_read_stub;

/**
 * @brief Stub method for I/O. Always writes data to memory
 *      Default I/O handler for CS instances
 */
ASM2010_API cs_io_write_fn cs_io_write_stub;

#ifdef __cplusplus
}
#endif

#endif /* ASM2010_H */
