#!/bin/bash


function link()
{
	
	cd /ca2/nodeapp/seed/metaseed



g++ -shared -L"/ca2/stage/x86/"  %OBJS% -o "%CA2_ROOT%stage/%PLATFORM%/libnetnodelibrary.so" -Wl,-rpath,\\$ORIGIN -Wl,-E -ldl -lpthread -l_ca2openssl -l_ca2zlib -l_ca2freeimage -l_ca2freetype -lca2c -lca2 -lcairo -lX11 -lXmu -lapp_core_netnodelite -Wl,-z,defs &> "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"


}


link
