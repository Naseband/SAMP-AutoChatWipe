/*

Auto Chat-Wipe Include by NaS

This include will "spam" each Player's Chat once no new client messages are recieved.

To make all Scripts compatible, you must include it in all Scripts that are loaded.
Make sure to define FILTERSCRIPT before including it in a Filterscript (the Gamemode will then act as Master Script).

*/

#if !defined __AUTOCHATWIPE_INCLUDED__
#define __AUTOCHATWIPE_INCLUDED__
#else
#endinput
#endif


#if defined FILTERSCRIPT // SLAVE


stock ACW_SendClientMessage(playerid, color, text[])
{
	if(!IsPlayerConnected(playerid)) return 0;

	if(IsPlayerNPC(playerid)) return SendClientMessage(playerid, color, text);
	
	CallRemoteFunction("ACW_OnSendClientMessage", "i", playerid);
	
	return SendClientMessage(playerid, color, text);
}

stock ACW_SendClientMessageToAll(color, text[])
{
	CallRemoteFunction("ACW_OnSendClientMessageToAll", "");

	return SendClientMessageToAll(color, text);
}

#define SendClientMessage ACW_SendClientMessage
#define SendClientMessageToAll ACW_SendClientMessageToAll


#else // MASTER


#if !defined ACW_DELAY
#define ACW_DELAY 	 		(5) // Delay (sec.) after a client message to start sending empty lines
#endif

#if !defined ACW_INTERVAL
#define ACW_INTERVAL    	(3) // Interval (sec.) for sending empty lines
#endif

#if !defined ACW_MAX_LINES
#define ACW_MAX_LINES       (7)
#endif


new ACW_LastPlayerSCM[MAX_PLAYERS], ACW_LastGlobalSCM, ACW_EmptyCount[MAX_PLAYERS], bool:ACW_ResetCount = false, bool:ACW_Enabled[MAX_PLAYERS];

public OnGameModeInit()
{
	SetTimer("ACW_Timer", ACW_INTERVAL*1000, 1);
	
	new tick = GetTickCount();
	ACW_LastGlobalSCM = tick;
	for(new i = 0; i < MAX_PLAYERS; i ++)
	{
	    if(!IsPlayerConnected(i)) ACW_LastPlayerSCM[i] = 0;
	    else if(IsPlayerNPC(i)) ACW_LastPlayerSCM[i] = 0;
	    else ACW_LastPlayerSCM[i] = (tick == 0 ? 1 : tick);

	    ACW_Enabled[i] = true;
	}

    return CallLocalFunction("ACW_OnGameModeInit", "");
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit ACW_OnGameModeInit
forward ACW_OnGameModeInit();

public OnPlayerConnect(playerid)
{
	new tick = GetTickCount();
	if(!IsPlayerNPC(playerid)) ACW_LastPlayerSCM[playerid] = (tick == 0 ? 1 : tick);
	else ACW_LastPlayerSCM[playerid] = 0;
	
	ACW_EmptyCount[playerid] = 0;
	ACW_Enabled[playerid] = true;

    return CallLocalFunction("ACW_OnPlayerConnect", "d", playerid);
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ACW_OnPlayerConnect
forward ACW_OnPlayerConnect(playerid);

public OnPlayerDisconnect(playerid, reason)
{
	ACW_LastPlayerSCM[playerid] = 0;

    return CallLocalFunction("ACW_OnPlayerDisconnect", "dd", playerid, reason);
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ACW_OnPlayerDisconnect
forward ACW_OnPlayerDisconnect(playerid, reason);

public OnPlayerText(playerid, text[])
{
	ACW_LastGlobalSCM = GetTickCount();
	ACW_ResetCount = true;

    return CallLocalFunction("ACW_OnPlayerText", "ds", playerid, text), 1;
}

#if defined _ALS_OnPlayerText
    #undef OnPlayerText
#else
    #define _ALS_OnPlayerText
#endif
#define OnPlayerText ACW_OnPlayerText
forward ACW_OnPlayerText(playerid, text[]);

forward ACW_Timer();
public ACW_Timer()
{
	new tick = GetTickCount();
	if(tick - ACW_LastGlobalSCM < ACW_DELAY*1000) return 1;

	for(new i = 0; i < MAX_PLAYERS; i ++)
	{
	    if(ACW_LastPlayerSCM[i] == 0 || !ACW_Enabled[i]) continue;
	    
	    if(ACW_ResetCount) ACW_EmptyCount[i] = 0;
	    
	    if(ACW_EmptyCount[i] == ACW_MAX_LINES || tick - ACW_LastPlayerSCM[i] < ACW_DELAY*1000) continue;
	    
	    SendClientMessage(i, -1, " ");
	    ACW_EmptyCount[i] ++;
	}
	
	ACW_ResetCount = false;
	
	return 1;
}

forward ACW_OnSendClientMessage(playerid);
public ACW_OnSendClientMessage(playerid)
{
	if(playerid < 0 || playerid >= MAX_PLAYERS) return;

	if(!ACW_Enabled[playerid]) return;

	new tick = GetTickCount();
	ACW_LastPlayerSCM[playerid] = (tick == 0 ? 1 : tick);
	
	ACW_EmptyCount[playerid] = 0;

	return;
}

forward ACW_OnSendClientMessageToAll();
public ACW_OnSendClientMessageToAll()
{
	ACW_LastGlobalSCM = GetTickCount();
	ACW_ResetCount = true;

	return;
}

stock ACW_SendClientMessage(playerid, color, text[])
{
	if(!IsPlayerConnected(playerid)) return 0;
	
	if(IsPlayerNPC(playerid) || !ACW_Enabled[playerid]) return SendClientMessage(playerid, color, text);
	
	new tick = GetTickCount();
	ACW_LastPlayerSCM[playerid] = (tick == 0 ? 1 : tick);
	
	ACW_EmptyCount[playerid] = 0;
	
	return SendClientMessage(playerid, color, text);
}

stock ACW_SendClientMessageToAll(color, text[])
{
	ACW_LastGlobalSCM = GetTickCount();
	ACW_ResetCount = true;

	return SendClientMessageToAll(color, text);
}

#define SendClientMessage ACW_SendClientMessage
#define SendClientMessageToAll ACW_SendClientMessageToAll

#endif // from #if defined FILTERSCRIPT

//EOF
