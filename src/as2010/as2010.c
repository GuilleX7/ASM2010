/** @file as2010.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "../utils.h"
#include "../mcs.h"
#include "../about.h"
#include "../trace_log.h"

#include "as_parse.h"

#define PROGRAM_NAME "AS2010"
#define PROGRAM_DESCRIPTION "CS2010 assembler"

static void show_help(void) {
	puts(
		"USAGE: " PROGRAM_NAME " file [parameters]\n\n"
		"PARAMETERS:\n"
		"-help\tShow this help\n"
		"-bin\tUse binary format as output (default)\n"
		"-hex\tUse textual hexadecimal format as output (suitable for Logisim)\n"
		"-o file\tOutput to the given file (will be overwritten)\n"
		"-v\tShows about information\n"
	);
}

static void show_about(void) {
	puts(
		MAKE_ABOUT_TEXT(
			PROGRAM_NAME,
			PROGRAM_DESCRIPTION
		)
	);
}

int main(int argc, char **argv) {
	char *file_str;
	char line[AS_MAX_LINE_LENGTH + 2];
	size_t offset = 0;
	int output_format = MCS_FORMAT_BIN;
	char *output_path = { 0 };
	bool must_free_path = false;
	char const *arg = { 0 };
	as_parse_info pinfo = { 0 };
	int status = { 0 };

	if (argc < 2) {
		show_help();
		return EXIT_SUCCESS;
	}

	if (!strcmp(argv[1], "-help")) {
		show_help();
		return EXIT_SUCCESS;
	} else if (!strcmp(argv[1], "-v")) {
		show_about();
		return EXIT_SUCCESS;
	}

	for (int i = 2; i < argc; i++) {
		arg = argv[i];
		if (!strcmp(arg, "-bin")) {
			output_format = MCS_FORMAT_BIN;
		} else if (!strcmp(arg, "-hex")) {
			output_format = MCS_FORMAT_HEX;
		} else if (!strcmp(arg, "-o")) {
			if (i == argc - 1) {
				printf("-o: File output not specified\n");
				continue;
			} else {
				output_path = argv[i + 1];
				i++;
			}
		} else if (!strcmp(arg, "-help")) {
			show_help();
			return EXIT_SUCCESS;
		} else if (!strcmp(arg, "-v")) {
			show_about();
			return EXIT_SUCCESS;
		} else {
			printf("Error: unknown parameter '%s'\n", arg);
		}
	}

	if (!as_parse_init(&pinfo, CS_ROM_SIZE)) {
		printf("Error: couldn't initialize assembly due to memory exhaustion\n");
		return EXIT_FAILURE;
	}

	file_str = read_file(argv[1]);
	if (!file_str) {
		printf("Error: couldn't read the file %s\n", argv[1]);
		as_parse_free(&pinfo);
		return EXIT_FAILURE;
	}

	while (read_upper_line(line, AS_MAX_LINE_LENGTH + 2, file_str, &offset)) {
		status = as_parse_line(&pinfo, line);
		if (!trace_log_is_empty(&pinfo.log)) {
			puts(trace_log_get(&pinfo.log));
		}
		if (status == AS_PARSE_ERROR) {
			puts("Aborting assembly...\n");
			as_parse_free(&pinfo);
			free(file_str);
			return EXIT_FAILURE;
		}
	}

	status = as_parse_assemble(&pinfo);
	if (!trace_log_is_empty(&pinfo.log)) {
		puts(trace_log_get(&pinfo.log));
	}
	if (status == AS_PARSE_ERROR) {
		puts("Aborting assembly...\n");
		as_parse_free(&pinfo);
		free(file_str);
		return EXIT_FAILURE;
	}
	puts("Successfully assembled!\n");

	if (!output_path) {
		output_path = change_path_extension(argv[1], (output_format == MCS_FORMAT_BIN) ? MCS_FILE_BIN_EXT : MCS_FILE_HEX_EXT);
		must_free_path = true;
	}
	switch (mcs_export_file(output_path, pinfo.machine_code, pinfo.sentence_index, output_format)) {
	case MCS_EXPORT_FILE_ERROR:
		printf("Error: couldn't open output file '%s'\n", argv[1]);
		break;
	case MCS_EXPORT_ERROR:
		printf("Error: couldn't write output file '%s'\n", argv[1]);
		break;
	default:
		break;
	}
	puts("Successfully exported file!\n");

	if (must_free_path) {
		free(output_path);
	}
	as_parse_free(&pinfo);
	free(file_str);
	return EXIT_SUCCESS;
}
