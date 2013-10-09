#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM out :
         virtual public ::music::midi::out
      {
      public:


         HMIDIOUT       m_hmidiout;


         out(sp(base_application) papp);
         virtual ~out();


         virtual ::multimedia::result open();
         virtual ::multimedia::result close();

         ::multimedia::result send_short_message(::music::midi::e_type etype, int iChannel, int iData1, int iData2);

      };


   } // namespace midi_mmsystem


} // namespace music




