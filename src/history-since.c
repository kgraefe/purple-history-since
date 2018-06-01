/* Copyright (C) 2018 Konrad Gräfe <konradgraefe@aol.com>
 *
 * This software may be modified and distributed under the terms
 * of the GPLv2 license. See the COPYING file for details.
 */

#define PURPLE_PLUGINS

#include <plugin.h>
#include <connection.h>
#include <version.h>
#include <debug.h>

#include "config.h"
#include "internal.h"

#define debug(fmt, ...) \
	purple_debug_info(PLUGIN_STATIC_NAME, fmt, ##__VA_ARGS__)
#define error(fmt, ...) \
	purple_debug_error(PLUGIN_STATIC_NAME, fmt, ##__VA_ARGS__)


static void wrote_chat_msg_cb(
	PurpleAccount *account, const char *who, char *message,
	PurpleConversation *conv, PurpleMessageFlags flags
) {
	PurpleChat *chat;
	GHashTable *components;
	GList *history;
	PurpleConvMessage *msg;
	time_t history_since, history_since_old;
	struct tm *history_since_tm;
	const char *str, *name;

	if(!(flags & (PURPLE_MESSAGE_SEND | PURPLE_MESSAGE_RECV))) {
		return;
	}

	name = purple_conversation_get_name(conv);
	chat = purple_blist_find_chat(account, name);
	if(!chat) {
		debug("Chat '%s' is not in the buddy list! Aborting.\n", name);
		return;
	}

	history = purple_conversation_get_message_history(conv);
	if(!history) {
		debug("Could not get history of '%s'! Aborting.\n", name);
		return;
	}
	msg = history->data;
	if(!msg) {
		return;
	}

	history_since = msg->when + 1;

	components = purple_chat_get_components(chat);
	if(!components) {
		debug("Could not get components of '%s'! Aborting.\n", name);
		return;
	}

	str = g_hash_table_lookup(components, "history_since");
	if(str) {
		history_since_old = purple_str_to_time(str, TRUE, NULL, NULL, NULL);
		if(history_since_old > time(NULL)) {
			history_since_old = time(NULL);
		}
		if(history_since < history_since_old) {
			debug("Saved history_since timestamp of %s newer than current time! Aborting.\n", name);
			return;
		}
	}

	history_since_tm = gmtime(&history_since);
	if(!history_since_tm) {
		error("Could not convert timestamp! Aborting.\n");
		return;
	}

	str = purple_utf8_strftime("%Y-%m-%dT%H:%M:%SZ", history_since_tm);
	g_hash_table_replace(components, g_strdup("history_since"), g_strdup(str));
}

static gboolean plugin_load(PurplePlugin *plugin) {
	purple_signal_connect(
		purple_conversations_get_handle(),
		"wrote-chat-msg", plugin,
		PURPLE_CALLBACK(wrote_chat_msg_cb), NULL
	);
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	purple_signal_disconnect(
		purple_conversations_get_handle(),
		"wrote-chat-msg", plugin,
		PURPLE_CALLBACK(wrote_chat_msg_cb)
	);
	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,     /* type           */
	NULL,                       /* ui_requirement */
	0,                          /* flags          */
	NULL,                       /* dependencies   */
	PURPLE_PRIORITY_DEFAULT,    /* priority       */

	PLUGIN_ID,                  /* id             */
	NULL,                       /* name           */
	PLUGIN_VERSION,             /* version        */
	NULL,                       /* summary        */
	NULL,                       /* description    */
	PLUGIN_AUTHOR,              /* author         */
	PLUGIN_WEBSITE,             /* homepage       */

	plugin_load,                /* load           */
	plugin_unload,              /* unload         */
	NULL,                       /* destroy        */

	NULL,                       /* ui_info        */
	NULL,                       /* extra_info     */
	NULL,                       /* prefs_info     */
	NULL,                       /* actions        */
	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin) {
	info.name        = _("History Since");
	info.summary     = _("Avoid duplicated chat history on some XMPP servers.");
	info.description = _("Avoid duplicated chat history on some XMPP servers.");
}

PURPLE_INIT_PLUGIN(PLUGIN_STATIC_NAME, init_plugin, info)
