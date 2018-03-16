#!/bin/sh


cp /ubuntu/nodeapp/thirdparty/lib/libavcodec*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavcodec*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavdevice*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavdevice*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavfilter*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavfilter*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavformat*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavformat*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavutil*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libavutil*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libpostproc*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libpostproc*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libswscale*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libswscale*.so.* /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libswresample*.so /ffmpeg/bin/
cp /ubuntu/nodeapp/thirdparty/lib/libswresample*.so.* /ffmpeg/bin/
/ffmpeg/bin/ffplay /home/camilo/Videos/ThomasBS/1080p_thomasbs_20170226153720.mp4