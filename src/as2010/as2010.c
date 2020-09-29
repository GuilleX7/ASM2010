#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>

#include "../file.h"

#include "as2010_parse.h"

static int read_upper_line(char *line, int max_length, FILE *fp) {
	if (!fgets(line, max_length, fp)) {
		return 0;
	}

	while (*line != '\0' && *line != '\n') {
		*line = toupper((unsigned) *line);
		line++;
	}

	return 1;
}

static void show_help(void) {
	puts(
		"USAGE: asm2010 file [parameters]\n\n"
		"PARAMETERS:\n"
		"-help\tShow this help\n"
		"-bin\tUse binary format as output (default)\n"
		"-hex\tUse textual hexadecimal format as output (suitable for Logisim)\n"
		"-o file\tOutput to the given file (will be overwritten)\n"
		"-v\tShows about information\n");
}

static void show_about(void) {
	puts(
		"AS2010 v" STRINGIFY(PARSER_MAJOR_VERSION) "." STRINGIFY(PARSER_MINOR_VERSION) "." STRINGIFY(PARSER_PATCH_VERSION) " - CS2010 assembler\n"
		"Developed by GuilleX7 - guillermox7@gmail.com\n"
		"https://github.com/GuilleX7/\n");
}

int main(int argc, char **argv) {
	parse_info pinfo = { 0 };
	char line[MAX_LINE_LENGTH + 2];
	FILE *fp = { 0 };
	int output_format = EXPORT_FORMAT_BIN;
	char *output_path = { 0 };
	char const *arg = { 0 };
	bool must_free_path = false;
	parse_status status = { 0 };

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
			output_format = EXPORT_FORMAT_BIN;
		} else if (!strcmp(arg, "-hex")) {
			output_format = EXPORT_FORMAT_HEX;
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

	ins_search_start();

	if (!parse_init(&pinfo)) {
		printf("Error: couldn't initialize assembly due to memory exhaustion");
		goto end;
	}

	fp = fopen(argv[1], "r");
	if (!fp) {
		printf("Error: couldn't open input file '%s'\n", argv[1]);
		goto end;
	}

	while (read_upper_line(line, MAX_LINE_LENGTH + 2, fp)) {
		status = parse_line(&pinfo, line);
		if (pinfo.trace && *pinfo.trace) {
			puts(pinfo.trace);
		}
		if (status == PARSE_ERROR) {
			printf("Aborting assembly...\n");
			goto end;
		}
	}
	fclose(fp);
	fp = 0;

	status = parse_assemble(&pinfo);
	if (pinfo.trace && *pinfo.trace) {
		puts(pinfo.trace);
	}
	if (status == PARSE_ERROR) {
		printf("Aborting assembly...\n");
		goto end;
	}
	printf("Successfully assembled!\n");

	if (!output_path) {
		output_path = change_path_extension(argv[1], (output_format == EXPORT_FORMAT_BIN) ? ".bin" : ".hex");
		must_free_path = true;
	}
	switch (export_code_to_file(output_path, pinfo.bincode, pinfo.sentence_index, output_format)) {
	case EXPORT_FILE_ERROR:
		printf("Error: couldn't open output file '%s'\n", argv[1]);
		break;
	case EXPORT_ERROR:
		printf("Error: couldn't write output file '%s'\n", argv[1]);
		break;
	default:
		break;
	}
	printf("Successfully exported file!\n");
end:
	if (fp)
		fclose(fp);
	if (must_free_path)
		free(output_path);
	parse_free(&pinfo);
	ins_search_stop();
	return EXIT_SUCCESS;
}
