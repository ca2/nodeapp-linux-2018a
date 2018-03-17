#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace alsa
      {


         midi_out::midi_out(::aura::application * papp, string driver) :
            ::object(papp),
            ::music::midi::message_out(papp),
            m_file(papp)
         {

            m_iWrite = 0;

            m_prawmidi = NULL;

            int err = snd_rawmidi_open(NULL, &m_prawmidi, driver, SND_RAWMIDI_SYNC);

            if(err)
            {

               fprintf(stderr,"snd_rawmidi_open %s failed: %d\n", driver, err);

            }

            snd_rawmidi_params_t * params = NULL;

            err = snd_rawmidi_params_malloc(&params);

            err = snd_rawmidi_params_current(m_prawmidi,	params);

            err = snd_rawmidi_params_set_avail_min(m_prawmidi, params, 768);

            err = snd_rawmidi_params_set_buffer_size(m_prawmidi, params, 1024);

            err = snd_rawmidi_params(m_prawmidi, params);

            snd_rawmidi_params_free(params);


         }


         void midi_out::reset_all_controllers()
         {

            ::music::midi::message_out::reset_all_controllers();

         }


         midi_out::~midi_out()
         {

            if(m_prawmidi != NULL)
            {

               snd_rawmidi_close(m_prawmidi);

            }

         }


         void midi_out::note_on(int channel, unsigned char note, unsigned char volume)
         {

            byte message[3];

            message[0] = 0x90 | channel;

            message[1] = note;

            message[2] = volume;

            add_short_message(message, 3);

         }


         void midi_out::note_off(int channel, unsigned char note, unsigned char velocity)
         {

            byte message[3];

            message[0] = 0x80 | channel;

            message[1] = note;

            message[2] = velocity;

            add_short_message(message, 3);

         }


         void midi_out::program_change(int channel, unsigned char instrument)
         {

            byte message[2];

            message[0] = 0xC0 | channel;

            message[1] = instrument;

            add_short_message(message, 2);

         }


         void midi_out::step()
         {

            if(m_iWrite > 0)
            {

               snd_rawmidi_write(m_prawmidi, m_file.get_data(), m_file.get_size());

               //snd_rawmidi_drain(m_prawmidi);

               m_file.Truncate(0);

            }

            m_iWrite = 0;

         }


         void midi_out::start()
         {

         }


         void midi_out::add_short_message(byte * pmessage, int iSize)
         {

            m_file.write(pmessage, iSize);

            m_iWrite += iSize;

            //step();

         }


         void midi_out::control_change(int channel, unsigned char controller, unsigned char value)
         {

            byte message[3];

            message[0] = 0xB0 | channel;

            message[1] = controller;

            message[2] = value;

            add_short_message(message, 3);

         }


         void midi_out::pitch_bend(int channel, unsigned short value)
         {

            int c1 = (value & 0x7F);

            int c2 = ((value >> 7) & 0x7F);

            byte message[3];

            message[0] = 0xE0 | channel;

            message[1] = c1;

            message[2] = c2;

            add_short_message(message, 3);

         }


      } // namespace alsa


   } // namespace midi


} // namespace music



