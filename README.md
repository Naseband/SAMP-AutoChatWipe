# acw.inc - Auto Chat Wipe

This Include will slowly send empty lines to players that did not recieve any client messages for a while.
The purpose of this is to make the chat smaller if nothing is happening. Delay, Interval and number of empty lines can be configured.

# Usage

Include it in every Script that can potentially send client messages.

__Important__: If the Script is a Filterscript, define *FILTERSCRIPT* before including *acw.inc*. Otherwise it may execute twice.
The Gamemode Script will always be the master Script.

# Defines/Settings

__ACW_DELAY__       Pause Delay in seconds after sending a regular client message.
__ACW_INTERVAL__    Interval in seconds for sending empty lines.
__ACW_MAX_LINES__   Max. number of empty Lines to send.

You can define any of these in the Master (Gamemode) Script before including *acw.inc*.

# Known Issues:

RCON Responses and Messages from Client Commands or Server Plugins are not considered.
