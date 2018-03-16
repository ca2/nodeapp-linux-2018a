#!/bin/bash


function m()
{

   export MAKEFLAGS="-j4"
   cd /ffmpeg
   rm -Rf ffmpeg*
   wget -O ffmpeg-snapshot.tar.bz2 http://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2
   tar xjvf ffmpeg-snapshot.tar.bz2
   cd ffmpeg
   PATH="/ffmpeg/bin:$PATH" PKG_CONFIG_PATH="/ubuntu/nodeapp/lib/pkgconfig" ./configure --prefix="/ubuntu/nodeapp" --pkg-config-flags="--static" --extra-cflags="-I/ffmpeg/ffmpeg_build/include -fPIC" --extra-ldflags="-L/ffmpeg/ffmpeg_build/lib -Wl,-rpath=\$ORIGIN/" --extra-libs="-lpthread -lm" --bindir="/ffmpeg/bin" --enable-gpl --enable-libass --enable-libfdk-aac --enable-libfreetype --enable-libmp3lame --enable-libopus --enable-libtheora --enable-libvorbis  --enable-libvpx --enable-libx264 --enable-libx265 --enable-nonfree && 
   PATH="/ffmpeg/bin:$PATH" make
   make install
   hash -r

}

m