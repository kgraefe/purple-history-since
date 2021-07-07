plugindir = $(shell $(PKG_CONFIG) --variable=plugindir purple)
DEFS = -DLOCALEDIR=\"$(localedir)\" -DDATADIR=\"$(datadir)\"

plugin_LTLIBRARIES = purple_history_since.la
purple_history_since_la_SOURCES = \
	history-since.c

purple_history_since_la_LDFLAGS = -module -avoid-version

