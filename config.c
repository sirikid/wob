#define WOB_FILE "config.c"

#define MIN_PERCENTAGE_BAR_WIDTH 1
#define MIN_PERCENTAGE_BAR_HEIGHT 1

#define _POSIX_C_SOURCE 200809L
#include <ini.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

bool
parse_output_mode(const char *str, enum wob_output_mode *value)
{
	if (strcmp(str, "whitelist") == 0) {
		*value = WOB_OUTPUT_MODE_WHITELIST;
		return true;
	}

	if (strcmp(str, "all") == 0) {
		*value = WOB_OUTPUT_MODE_ALL;
		return true;
	}

	return false;
}

bool
parse_overflow_mode(const char *str, enum wob_overflow_mode *value)
{
	if (strcmp(str, "wrap") == 0) {
		*value = WOB_OVERFLOW_MODE_WRAP;
		return true;
	}

	if (strcmp(str, "nowrap") == 0) {
		*value = WOB_OVERFLOW_MODE_NOWRAP;
		return true;
	}

	return false;
}

bool
parse_number(const char *str, unsigned long *value)
{
	char *str_end;
	unsigned long ul = strtoul(str, &str_end, 10);
	if (*str_end != '\0') {
		return false;
	}

	*value = ul;

	return true;
}

bool
parse_anchor(const char *str, unsigned long *anchor)
{
	char *delimiter = " ";
	char *str_dup = strdup(str);
	if (str_dup == NULL) {
		wob_log_error("strdup failed");
		exit(EXIT_FAILURE);
	}

	char *token = strtok(str_dup, delimiter);

	*anchor = WOB_ANCHOR_CENTER;
	while (token) {
		if (strcmp(str, "left") == 0) {
			*anchor |= WOB_ANCHOR_LEFT;
		}
		else if (strcmp(str, "right") == 0) {
			*anchor |= WOB_ANCHOR_RIGHT;
		}
		else if (strcmp(str, "top") == 0) {
			*anchor |= WOB_ANCHOR_TOP;
		}
		else if (strcmp(str, "bottom") == 0) {
			*anchor |= WOB_ANCHOR_BOTTOM;
		}
		else if (strcmp(str, "center") != 0) {
			free(str_dup);

			return false;
		}

		token = strtok(NULL, delimiter);
	}

	free(str_dup);

	return true;
}

bool
parse_color(const char *str, struct wob_color *color)
{
	char *str_end;
	if (wob_color_from_string(str, &str_end, color) == false || *str_end != '\0') {
		return false;
	}

	return true;
}

int
handler(void *user, const char *section, const char *name, const char *value)
{
	struct wob_config *config = (struct wob_config *) user;

	unsigned long ul;
	struct wob_color color;

	if (strcmp(section, "") == 0) {
		if (strcmp(name, "max") == 0) {
			if (parse_number(value, &ul) == false) {
				wob_log_error("Maximum must be a value between 1 and %lu.", ULONG_MAX);
				return 0;
			}
			config->max = ul;
		}
		else if (strcmp(name, "timeout") == 0) {
			if (parse_number(value, &ul) == false) {
				wob_log_error("Timeout must be a value between 1 and %lu.", ULONG_MAX);
				return 0;
			}
			config->timeout_msec = ul;
		}
	}
	else if (strncmp(section, "output.", sizeof("output.") - 1) == 0) {
		if (strcmp(name, "name") == 0) {
			struct wob_output_config *output_config = malloc(sizeof(struct wob_output_config));
			output_config->name = strdup(value);

			wl_list_insert(&config->outputs, &output_config->link);
		}
	}
	else {
		wob_log_error("Unknown section %s", section);
		return 0;
	}

	if (strcmp(name, "width") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Width must be a positive value.");
			return 0;
		}
		config->dimensions.width = ul;
	}
	else if (strcmp(name, "height") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Height must be a positive value.");
			return 0;
		}
		config->dimensions.height = ul;
	}
	else if (strcmp(name, "border_offset") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Border offset must be a positive value.");
			return 0;
		}
		config->dimensions.border_offset = ul;
	}
	else if (strcmp(name, "border_size") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Border size must be a positive value.");
			return 0;
		}
		config->dimensions.border_size = ul;
	}
	else if (strcmp(name, "bar_padding") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Bar padding must be a positive value.");
			return 0;
		}
		config->dimensions.bar_padding = ul;
	}
	else if (strcmp(name, "margin") == 0) {
		if (parse_number(value, &ul) == false) {
			wob_log_error("Anchor margin must be a positive value.");
			return 0;
		}
		config->margin = ul;
	}
	else if (strcmp(name, "anchor") == 0) {
		if (parse_anchor(value, &ul) == false) {
			wob_log_error("Anchor must be one of 'top', 'bottom', 'left', 'right', 'center'.");
			return 0;
		}
		config->anchor = ul;
	}
	else if (strcmp(name, "background_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Background color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->colors.background = color;
	}
	else if (strcmp(name, "border_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Border color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->colors.border = color;
	}
	else if (strcmp(name, "bar_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Bar color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->colors.value = color;
	}
	else if (strcmp(name, "overflow_background_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Overflow background color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->overflow_colors.background = color;
	}
	else if (strcmp(name, "overflow_border_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Overflow border color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->overflow_colors.border = color;
	}
	else if (strcmp(name, "overflow_bar_color") == 0) {
		if (!parse_color(value, &color)) {
			wob_log_error("Overflow bar color must be a value between #00000000 and #FFFFFFFF.");
			return 0;
		}
		config->overflow_colors.value = color;
	}
	else if (strcmp(name, "overflow_mode") == 0) {
		if (parse_overflow_mode(value, &config->overflow_mode) == false) {
			wob_log_error("Invalid argument for overflow-mode. Valid options are wrap and nowrap.");
			return 0;
		}
	}
	else if (strcmp(name, "output_mode") == 0) {
		if (parse_output_mode(value, &config->output_mode) == false) {
			wob_log_error("Invalid argument for output_mode. Valid options are whitelist and all.");
			return 0;
		}
	}

	return 1;
}

void
wob_config_init(struct wob_config *config)
{
	wl_list_init(&config->outputs);

	config->max = 100;
	config->timeout_msec = 1000;
	config->dimensions.width = 400;
	config->dimensions.height = 50;
	config->dimensions.border_offset = 4;
	config->dimensions.border_size = 4;
	config->dimensions.bar_padding = 4;
	config->margin = 0;
	config->anchor = WOB_ANCHOR_CENTER;
	config->overflow_mode = WOB_OVERFLOW_MODE_WRAP;
	config->output_mode = WOB_OUTPUT_MODE_ALL;
	config->colors.background = (struct wob_color){.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f};
	config->colors.value = (struct wob_color){.a = 1.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f};
	config->colors.border = (struct wob_color){.a = 1.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f};
	config->overflow_colors.background = (struct wob_color){.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f};
	config->overflow_colors.value = (struct wob_color){.a = 1.0f, .r = 1.0f, .g = 0.0f, .b = 0.0f};
	config->overflow_colors.border = (struct wob_color){.a = 1.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f};
}

void
wob_config_destroy(struct wob_config *config)
{
	struct wob_output_config *output, *output_tmp;
	wl_list_for_each_safe (output, output_tmp, &config->outputs, link) {
		free(output->name);
		free(output);
	}
}

bool
wob_config_parse(FILE *file, struct wob_config *config)
{
	int res = ini_parse_file(file, handler, config);

	if (res != 0) {
		wob_log_error("Ini parse error at line %d", res);
		return false;
	}

	struct wob_dimensions dimensions = config->dimensions;
	if (dimensions.width < MIN_PERCENTAGE_BAR_WIDTH + 2 * (dimensions.border_offset + dimensions.border_size + dimensions.bar_padding)) {
		wob_log_error("Invalid geometry: width is too small for given parameters");
		return false;
	}

	if (dimensions.height < MIN_PERCENTAGE_BAR_HEIGHT + 2 * (dimensions.border_offset + dimensions.border_size + dimensions.bar_padding)) {
		wob_log_error("Invalid geometry: height is too small for given parameters");
		return false;
	}

	return true;
}
