wob(1)

# NAME

wob - Wayland Overlay Bar

# DESCRIPTION

wob is a lightweight overlay volume/backlight/progress/anything bar for Wayland.

# SYNOPSIS

*wob* [options...]

# OPTIONS

*-c <file>*
	Configuration file location.

*-h*
	Show help message and quit.

*-v*
	Increase verbosity of messages, defaults to errors and warnings only.

# USAGE

Wob reads values to display from standard input in the following formats:

<value>

or

<value> <#background_color> <#border_color> <#bar_color>

Where <value> is number in interval from 0 to *--max* and <#\*color> is color in #RRGGBBAA format.

# ENVIRONMENT

The following environment variables have an effect on wob:

*WOB_DISABLE_PLEDGE*
	Disable seccomp syscall filtering on Linux. Set this if you are having trouble running wob with tools like valgrind or strace.
