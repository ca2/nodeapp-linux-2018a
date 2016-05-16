#!/bin/bash


function link()
{
	
	cd %CA2_ROOT%nodeapp/seed/metaseed



g++ -shared -L"%CA2_ROOT%stage/x86/"  "%ITEM_NAME%.o" -o "%TARGET_PATH%.so" -Wl,-rpath,\\$ORIGIN -Wl,-E -ldl -lpthread -laura -laxiscrypto -laxisopenssl -laxiszlib -laxisfreeimage -laxisfreetype -laxispixman -laxiscairo -laxis -lbase -lcore -lprogramming -lX11 -lXmu -lprogramming -lvericard -lirc -lxmpp -lapp_core_irc_bot -lapp_core_irc_client -lapp_core_irc_resident -lapp_core_xmpp_bot -lapp_core_xmpp_client -lapp_core_xmpp_resident -lapp_core_resident -Wl,-z,defs 


}


link
