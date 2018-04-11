# Purple History Since

Some servers, mainly on the XMPP protocol, send a fixed part of the chat
history to each client, regardless of whether or not the client has already
received those messages. This plugin for libpurple clients, such as Pidgin or
finch, sets the `history_since` property of a chat when a message is received.
When connecting to the server the next time, the XMPP protocol plugin uses this
property to tell the server to exclude messages before that timestamp from the
history.

Purple History Since is not limited to XMPP, but it is the only libpurple
protocol plugin known to use the `history_since` property.


## Building on Windows

In order to build the plugin for Windows an already-compiled source tree of
Pidgin is required. Please see the [Pidgin for Windows Build Instructions](https://developer.pidgin.im/wiki/BuildingWinPidgin)
for details.

After that you need to create a file named `local.mak` that points to the
Pidgin source tree, e.g.:

    PIDGIN_TREE_TOP=$(PLUGIN_TOP)/../../pidgin-2.10.11

Now you can build the plugin:

    make -f Makefile.mingw


## Contribution

We love patches. :heart: Please fork the project, do your changes and make a
pull request.
