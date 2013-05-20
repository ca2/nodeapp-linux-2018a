#!/bin/bash


function build()
{
	
	cd %CA2_ROOT%nodeapp/seed/metaseed


g++ -std=c++0x -fPIC -fexceptions -fnon-call-exceptions -fno-pie -fno-stack-protector -Wno-pointer-arith -Wno-attributes -Winvalid-pch  -g -D_DEBUG    -I../../../app/thirdparty/appseed -I. -I.. -I../.. -I../ca2 -I../../nodeapp/linux -I../../nodeapp/linux/include -I../../.. -I../../../app/appseed -I../../../app/appseed/ca -I../../../app/appseed/ca2 -I../../../app/thirdparty/include  -c /ca2/app-core/appseed/netnodelite_script/ds_script.cpp -o "%CA2_ROOT%time/intermediate/%PLATFORM%/netnodelite_script%ITEM_NAME%/%ITEM_TITLE%.o" &> "%CA2_ROOT%time/dynamic_source%ITEM_NAME%-compile-log.txt"

g++ -shared -L"/ca2/stage/x86/"  "%CA2_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/framework.o" "%CA2_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/netnodelite_script.o" "%CA2_ROOT%time/intermediate/%PLATFORM%/netnodelite_script%ITEM_NAME%/%ITEM_TITLE%.o" -o "%TARGET_PATH%.so" -Wl,-rpath,\\$ORIGIN -Wl,-E -ldl -lpthread -l_ca2openssl -l_ca2zlib -l_ca2freeimage -l_ca2freetype -lca2c -lca2optca -lca2ca -lca2 -lcairo -lX11 -lXmu -Wl,-z,defs &> "%CA2_ROOT%time/dynamic_source%ITEM_NAME%-link-log.txt"


}


build
