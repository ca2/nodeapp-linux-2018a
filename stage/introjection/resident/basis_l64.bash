#!/bin/bash


function link()
{
	
	cd %CA2_ROOT%nodeapp/seed/metaseed



g++ -shared -L"%CA2_ROOT%time/x64/basis/"  "%ITEM_NAME%.o" -o "%TARGET_PATH%.so" -Wl,-rpath,\\$ORIGIN -Wl,-E -ldl -lpthread -laura -lcrypto -lssl -lz -laxisfreeimage -lfreetype -lcairo -laxis -lbase -lcore -lprogramming -lX11 -lXmu -lprogramming -lvericard -lbeampro -lirc -lxmpp -lresident_beampro -lresident_twitchtv -lresident_liveedutv -lapp_core_resident -Wl,-z,defs 


}


link
