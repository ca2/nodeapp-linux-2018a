#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      out::out(sp(base_application) papp) :
         element(papp),
         ::music::midi::out(papp)
      {

         m_hmidiout = NULL;

      }


      out::~out()
      {

         close();

      }


      ::multimedia::result out::open()
      {

         MMRESULT mmrc;
         
         uint32_t uDeviceID = 0;
         
         m_hmidiout = NULL;
         
         mmrc = midiOutOpen(&m_hmidiout, uDeviceID,  0, 0, CALLBACK_NULL);

         if(mmrc != MMSYSERR_NOERROR)
            return -1;


         return 0;

      }

      ::multimedia::result out::close()
      {

         if(m_hmidiout != NULL)
         {

            midiOutClose(m_hmidiout);

         }


         return 0;

      }

      ::multimedia::result out::send_short_message(::music::midi::e_type etype, int iChannel, int iData1, int iData2)
      {

         return midiOutShortMsg(m_hmidiout, MIDIMSG(((int) etype) >> 4, iChannel, iData1, iData2));

      }


   } // namespace midi_mmsystem


} // namespace music




