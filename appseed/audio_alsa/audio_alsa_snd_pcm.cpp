#include "framework.h"


namespace multimedia
{


   namespace audio_alsa
   {


      snd_pcm::snd_pcm(sp(base_application) papp) :
         element(papp)
      {

         m_ppcm = NULL;
         m_phwparams = NULL;

      }

      snd_pcm::~snd_pcm()
      {

         if(m_ppcm != NULL)
         {

            snd_pcm_close();

         }

      }

      ::multimedia::result snd_pcm::snd_pcm_open(snd_pcm_stream_t stream_type, ::multimedia::audio::wave_format * & pformat)
      {

         ::multimedia::result mmr;
         ASSERT(m_ppcm == NULL);


         int i;
         int err;
         short buf[128];


         int cardNum = -1;
         int devNum = -1;
         int subDevNum = -1;


         // Start with first card
         cardNum = -1;

         for (;;)
         {
            snd_ctl_t * cardHandle;

            // Get next sound card's card number. When "cardNum" == -1, then ALSA
            // fetches the first card
            if ((err = snd_card_next(&cardNum)) < 0)
            {
               TRACE("Can't get the next card number: %s\n", snd_strerror(err));
               break;
            }

            // No more cards? ALSA sets "cardNum" to -1 if so
            if (cardNum < 0)
               break;

            // Open this card's control interface. We specify only the card number -- not
            // any device nor sub-device too
            {

               char   str[64];

               sprintf(str, "hw:%i", cardNum);
               if ((err = snd_ctl_open(&cardHandle, str, 0)) < 0)
               {
                  TRACE("Can't open card %i: %s\n", cardNum, snd_strerror(err));
                  continue;
               }

            }


            {

               // Start with the first wave device on this card
               devNum = -1;

               for (;;)
               {

                  snd_pcm_info_t  *pcmInfo;
                  register int        subDevCount, i;

                  // Get the number of the next wave device on this card
                  if ((err = snd_ctl_pcm_next_device(cardHandle, &devNum)) < 0)
                  {
                     TRACE("Can't get next wave device number: %s\n", snd_strerror(err));
                     break;
                  }

                  // No more wave devices on this card? ALSA sets "devNum" to -1 if so.
                  // NOTE: It's possible that this sound card may have no wave devices on it
                  // at all, for example if it's only a MIDI card
                  if (devNum < 0)
                     break;

                  // To get some info about the subdevices of this wave device (on the card), we need a
                  // snd_pcm_info_t, so let's allocate one on the stack
                  snd_pcm_info_alloca(&pcmInfo);
                  memset(pcmInfo, 0, snd_pcm_info_sizeof());

                  // Tell ALSA which device (number) we want info about
                  snd_pcm_info_set_device(pcmInfo, devNum);

                  // Get info on the wave outs of this device
                  snd_pcm_info_set_stream(pcmInfo, stream_type);

                  subDevCount = 1;
                  subDevNum = -1;

                  // More subdevices?
                  while (++subDevNum < subDevCount)
                  {
                     // Tell ALSA to fill in our snd_pcm_info_t with info on this subdevice
                     snd_pcm_info_set_subdevice(pcmInfo, subDevNum);
                     if ((err = snd_ctl_pcm_info(cardHandle, pcmInfo)) < 0)
                     {
                        TRACE("Can't get info for wave output subdevice hw:%i,%i,%i: %s\n", cardNum, devNum, i, snd_strerror(err));
                        continue;
                     }

                     // Print out how many subdevices (once only)
                     if (!subDevNum)
                     {
                        subDevCount = snd_pcm_info_get_subdevices_count(pcmInfo);
                        TRACE("\nFound %i wave output subdevices on card %i\n", subDevCount, cardNum);
                     }

                     // NOTE: If there's only one subdevice, then the subdevice number is immaterial,
                     // and can be omitted when you specify the hardware name
                     TRACE((subDevCount > 1 ? "    hw:%i,%i,%i\n" : "    hw:%i,%i\n"), cardNum, devNum, i);
                  }

               }
            }

            // Close the card's control interface after we're done with it
            snd_ctl_close(cardHandle);

         }

         snd_config_update_free_global();

         string strHw;

         if(cardNum < 0)
            return MMSYSERR_ERROR;

         strHw = "hw:" + str::from(cardNum);

         if(devNum >= 0)
            strHw += "," + str::from(devNum);

         if(subDevNum >= 0)
            strHw += "," + str::from(subDevNum);

         if ((err = ::snd_pcm_open (&m_ppcm, strHw, stream_type, 0)) < 0)
         {
            TRACE ("cannot open audio device %s (%s)\n", strHw, snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         if ((err = snd_pcm_hw_params_malloc (&m_phwparams)) < 0)
         {
            TRACE("cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         if ((err = snd_pcm_hw_params_any (m_ppcm, m_phwparams)) < 0) {
            TRACE ("cannot initialize hardware parameter structure (%s)\n",
                snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         if ((err = snd_pcm_hw_params_set_access (m_ppcm, m_phwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
         {
            TRACE ("cannot set access type (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

         if(pformat->wBitsPerSample == 16)
         {

            format = SND_PCM_FORMAT_S16_LE;

         }

         if ((err = snd_pcm_hw_params_set_format (m_ppcm, m_phwparams, format)) < 0)
         {
            TRACE ("cannot set sample format (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         unsigned int uiFreq = pformat->nSamplesPerSec;

         if ((err = snd_pcm_hw_params_set_rate_near (m_ppcm, m_phwparams, &uiFreq, 0)) < 0)
         {
            TRACE ("cannot set sample rate (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         pformat->nSamplesPerSec = uiFreq;

         if ((err = snd_pcm_hw_params_set_channels (m_ppcm, m_phwparams, pformat->nChannels)) < 0)
         {
            TRACE ("cannot set channel count (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         if ((err = snd_pcm_hw_params (m_ppcm, m_phwparams)) < 0)
         {
            TRACE("cannot set parameters (%s)\n", snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         snd_pcm_hw_params_free (m_phwparams);

         if ((err = snd_pcm_prepare (m_ppcm)) < 0)
         {
            TRACE ("cannot prepare audio interface for use (%s)\n",snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         return MMSYSERR_NOERROR;

      }


      ::multimedia::result snd_pcm::snd_pcm_close()
      {

         if(m_ppcm == NULL)
            return MMSYSERR_NOERROR;


         int err;

         if ((err = ::snd_pcm_close(m_ppcm)) < 0)
         {
            TRACE ("failed to close successfully sound interface (%s)\n",snd_strerror (err));
            return MMSYSERR_ERROR;
         }

         return MMSYSERR_NOERROR;

      }


   } // namespace audio_alsa


} // namespace multimedia







