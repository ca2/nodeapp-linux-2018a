#pragma once


#define MIDIMSG(status,channel,data1,data2) ( (uint32_t)((status<<4) | channel | (data1<<8) | (data2<<16)) )


namespace music
{

   namespace midi_mmsystem
   {


      ::multimedia::result TranslateSMFResult(::music::midi::e_file_result smfrc);


   } // namespace midi_mmsystem


} // namespace music