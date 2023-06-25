project('asm2010', 'c', version: '1.0.0')

asm2010_source = [
    'src/utils.h',
    'src/trace_log.h',
    'src/trace_log.c',
    'src/parse.h',
    'src/parse.c',
    'src/hash_table.c',
    'src/hash_table.h',
    'src/m2010/cs.h',
    'src/m2010/cs.c',
    'src/m2010/cs_signals.h',
    'src/m2010/cs_registers.h',
    'src/m2010/cs_platforms.h',
    'src/m2010/cs_opcodes.h',
    'src/m2010/cs_opcodes.c',
    'src/m2010/cs_memory.h',
    'src/m2010/cs_io.c',
    'src/m2010/cs_instructions.h',
    'src/m2010/cs_instructions.c',
    'src/m2010/cs2010/cs2010_platform.h',
    'src/m2010/cs2010/cs2010_platform.c',
    'src/m2010/cs2010/cs2010_opcodes.h',
    'src/m2010/cs2010/cs2010_opcodes.c',
    'src/m2010/cs3/cs3_registers.h',
    'src/m2010/cs3/cs3_platform.h',
    'src/m2010/cs3/cs3_platform.c',
    'src/m2010/cs3/cs3_opcodes.h',
    'src/m2010/cs3/cs3_opcodes.c',
    'src/as_parse/as_parse.h',
    'src/as_parse/as_parse.c',
    'src/as_parse/as_disassemble.h',
    'src/as_parse/as_disassemble.c',
    'include/asm2010.h'
]


asm2010_static = static_library(
    'asm2010',
    asm2010_source,
    override_options: 'b_vscrt=static_from_buildtype'
)
asm2010_static_dep = declare_dependency(link_with: asm2010_static)

if get_option('build_shared')
    asm2010_shared = shared_library(
        'asm2010',
        asm2010_source,
        override_options: 'b_vscrt=from_buildtype',
        gnu_symbol_visibility: 'hidden'
    )
    asm2010_shared_dep = declare_dependency(link_with: asm2010_shared)
endif
