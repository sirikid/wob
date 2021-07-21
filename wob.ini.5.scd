wob.ini(5)

# NAME

wob.ini - wob configuration file

# SECTION: default

*timeout*
	Timeout after which wob hides itself, in milliseconds.

	Default: 1000

*max*
	This value will be represented as fully filled bar.

	Default: 100

*width*
	Width of wob, in pixels.

	Default: 400

*height*
	Height of wob, in pixels.

	Default: 50

*offset*
	Border offset, in pixels.

	Default: 4

*border*
	Border size, in pixels.

	Default: 4

*padding*
	Bar padding, in pixels.

	Default: 4

*anchor*
	Anchor point, combination of *top*, *left*, *right*, *bottom*, *center*.

	Example: *bottom* *right*

	Default: *center*

*margin*
	Anchor margin, in pixels.

	Default: 0

*border_color*
	Border color, in #AARRGGBB format.

	Default: #FFFFFFFF

*background_color*
	Background color, in #AARRGGBB format.

	Default: #FF000000

*bar_color* <#AARRGGBB>
	Bar color, in #AARRGGBB format.

	Default: #FFFFFFFF

*overflow_mode*
	Overflow mode, one of *none*, *wrap*, and *nowrap*.

	*none*: Do not accept values above *max*

	*wrap*: Wrap value so *max* + 20 will be displayed as 20

	*nowrap* Values > *max* will be displayed as just *max*

	Default: *wrap*

*overflow_bar_color*
	Overflow bar color, in #AARRGGBB format.

	Default: #FFFFFFFF

*overflow_background_color*
	Overflow background color, in #AARRGGBB format.

	Default: #FF000000

*overflow_border_color*
	Overflow border color, in #AARRGGBB format.

	Default: #FFFFFFFF

*output_mode*
	Output mode, one of *whitelist*, *all*.

	*whitelist*: show wob only on outputs defined in *output.\** sections of config file

	*all*: show wob on all available outputs

	Default: *all*

# SECTION: output.*

This section will be used in future releases of wob to provide per output configuration.
Currently this servers only as list of outputs to show wob on in *whitelist* mode

Replace *\** with user friendly name of your choosing.

*name*
	Output name.

	Example: DP-1

# EXAMPLE

```
timeout = 1000
max = 100
output_mode = whitelist

[output.left]
name = DP-1

[output.big_guy]
name = HDMI-1
```

# See also

*wob*(1)
