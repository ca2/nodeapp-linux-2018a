#include "framework.h"


namespace multimedia
{


   namespace audio_alsa
   {


      wave_out::wave_out(sp(base_application) papp) :
         element(papp),
         ::thread(papp),
        wave_base(papp),
         snd_pcm(papp),
         ::multimedia::audio::wave_out(papp)
      {

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_iBufferedCount     = 0;
         m_mmr                = MMSYSERR_NOERROR;
         m_peffect            = NULL;
         m_dwLostSampleCount  = 0;
      }

      wave_out::~wave_out()
      {

      }

      void wave_out::install_message_handling(::message::dispatch * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_handling(pinterface);

/*         IGUI_WIN_MSG_LINK(MM_WOM_OPEN, pinterface, this, &wave_out::OnMultimediaOpen);
         IGUI_WIN_MSG_LINK(MM_WOM_DONE, pinterface, this, &wave_out::OnMultimediaDone);
         IGUI_WIN_MSG_LINK(MM_WOM_CLOSE, pinterface, this, &wave_out::OnMultimediaClose);*/

      }


      bool wave_out::initialize_instance()
      {

         if(!::multimedia::audio::wave_out::initialize_instance())
            return false;

         return true;

      }

      int32_t wave_out::exit_instance()
      {

         ::multimedia::audio::wave_out::exit_instance();

         return thread::exit_instance();

      }

      ::multimedia::result wave_out::wave_out_open(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount)
      {
         single_lock sLock(&m_mutex, TRUE);
         if(m_ppcm != NULL &&
            m_estate != state_initial)
            return MMSYSERR_NOERROR;
         m_pthreadCallback = pthreadCallback;
         ::multimedia::result mmr;
         ASSERT(m_ppcm == NULL);
         ASSERT(m_estate == state_initial);

         //m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->wFormatTag = 0;
         m_pwaveformat->nChannels = 2;
         m_pwaveformat->nSamplesPerSec = 44100;
         m_pwaveformat->wBitsPerSample = sizeof(multimedia::audio::WAVEBUFFERDATA) * 8;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;

         if(snd_pcm_open(SND_PCM_STREAM_PLAYBACK) != MMSYSERR_NOERROR)
            return MMSYSERR_ERROR;


/*       sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
            &m_ppcm,
            audiowave->m_uiWaveInDevice,
            wave_format(),
            get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 22050;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
            &m_ppcm,
            WAVE_MAPPER,
            wave_format(),
            (uint32_t) get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;
         m_pwaveformat->nSamplesPerSec = 11025;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
            &m_ppcm,
            WAVE_MAPPER,
            wave_format(),
            (uint32_t) get_os_int(),
            (uint32_t) 0,
            CALLBACK_THREAD)))
            goto Opened;

         if(mmr !=MMSYSERR_NOERROR)
         {
            if(mmr == MMSYSERR_ALLOCATED)
            {
               TRACE("Specified resource is already allocated.");
            }
            else if(mmr == MMSYSERR_BADDEVICEID)
            {
               TRACE("Specified device identifier is out of range.");
            }
            else if(mmr == WAVERR_BADFORMAT)
            {
               TRACE("Attempted to open with an unsupported waveform-audio_alsa format.");
            }
            TRACE("ERROR OPENING WAVE INPUT DEVICE");
            return mmr;
         }*/

Opened:
         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;
         uint32_t uiBufferCount = iBufferCount;

         if(m_pwaveformat->nSamplesPerSec == 44100)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            if(iBufferCount > 0)
            {
               uiAllocationSize = iBufferCount * uiAnalysisSize;
            }
            else
            {
               uiAllocationSize = 8 * uiAnalysisSize;
            }
            uiInterestSize = 200;
            uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }

         wave_out_get_buffer()->PCMOutOpen(uiBufferSize, uiBufferCount, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(
            this, // callback thread (thread)
            m_pwaveformat->nChannels, // channel count
            uiBufferCount, // group count
            iBufferSampleCount); // group sample count

         /*int32_t i, iSize;
         iSize = wave_out_get_buffer()->GetBufferCount();
         for(i = 0; i < iSize; i++)
         {

            if(MMSYSERR_NOERROR != (mmr =  waveOutPrepareHeader(m_ppcm, create_new_WAVEHDR(m_pwavebuffer, i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
               return mmr;
            }
         }*/
         m_estate = state_opened;
         return MMSYSERR_NOERROR;
      }

      ::multimedia::result wave_out::wave_out_open_ex(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample)
      {
         single_lock sLock(&m_mutex, TRUE);
         if(m_ppcm != NULL &&
            m_estate != state_initial)
            return MMSYSERR_NOERROR;
         m_pthreadCallback = pthreadCallback;
         ::multimedia::result mmr;
         ASSERT(m_ppcm == NULL);
         ASSERT(m_estate == state_initial);



         //m_pwaveformat->wFormatTag        = WAVE_FORMAT_PCM;
         m_pwaveformat->wFormatTag        = 0;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = 0;
         //m_pwaveformat->cbSize            = sizeof(m_waveformatex);

         if(snd_pcm_open(SND_PCM_STREAM_PLAYBACK) != MMSYSERR_NOERROR)
            return MMSYSERR_ERROR;

/*         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         try
         {
            if(MMSYSERR_NOERROR == (mmr = waveOutOpen(
               &m_ppcm,
               audiowave->m_uiWaveInDevice,
               wave_format(),
               get_os_int(),
               (uint32_t) 0,
               CALLBACK_THREAD)))
               goto Opened;
         }
         catch(const ::exception::exception &)
         {
            return MMSYSERR_ERROR;
         }
         catch(...)
         {
            return MMSYSERR_ERROR;
         }

         if(mmr != MMSYSERR_NOERROR)
         {
            if(mmr == MMSYSERR_ALLOCATED)
            {
               TRACE("Specified resource is already allocated.");
            }
            else if(mmr == MMSYSERR_BADDEVICEID)
            {
               TRACE("Specified device identifier is out of range.");
            }
            else if(mmr == WAVERR_BADFORMAT)
            {
               TRACE("Attempted to open with an unsupported waveform-audio_alsa format.");
            }
            TRACE("ERROR OPENING WAVE INPUT DEVICE");
            return mmr;
         }*/

Opened:
         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;
         uint32_t uiBufferCount = iBufferCount;

         if(iBufferSampleCount < 1024 * 512)
         {
//            iBufferSampleCount = 1024 * 512;
         }

         //   if(m_pwaveformat->nSamplesPerSec == 44100)
         if(true)
         {
            uiBufferSizeLog2 = 16;
            uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            if(iBufferCount > 0)
            {
               uiAllocationSize = iBufferCount * uiAnalysisSize;
            }
            else
            {
               uiAllocationSize = 8 * uiAnalysisSize;
            }
            uiInterestSize = 200;
            uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            uiBufferSizeLog2 = 10;
            uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            uiAllocationSize = 4 * uiAnalysisSize;
            uiInterestSize = 200;
            uiSkippedSamplesCount = 1;
         }

         wave_out_get_buffer()->PCMOutOpen(uiBufferSize, uiBufferCount, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, uiBufferCount, iBufferSampleCount);

         /*int32_t i, iSize;

         iSize = wave_out_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            if(MMSYSERR_NOERROR != (mmr =  waveOutPrepareHeader(m_ppcm, create_new_WAVEHDR(wave_out_get_buffer(), i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Preparing INPUT DEVICE buffer");
               return mmr;
            }

            //wave_out_add_buffer(i);

         }
         */

         m_pprebuffer->SetMinL1BufferCount(wave_out_get_buffer()->GetBufferCount() + 4);

         m_estate = state_opened;

         return MMSYSERR_NOERROR;

      }



      ::multimedia::result wave_out::wave_out_close()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return MMSYSERR_NOERROR;

         ::multimedia::result mmr;

/*         int32_t i, iSize;

         iSize =  wave_out_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            if(MMSYSERR_NOERROR != (mmr = waveOutUnprepareHeader(m_ppcm, wave_hdr(i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", mmr);
            }

            delete wave_hdr(i);

         }*/

         mmr = snd_pcm_close();

         m_ppcm = NULL;

         m_pprebuffer->Reset();

         m_estate = state_initial;

         return MMSYSERR_NOERROR;

      }


      /*void wave_out::OnMultimediaOpen(::signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }


      void wave_out::OnMultimediaDone(::signal_details * pobj)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         m_iBufferedCount--;

         LPWAVEHDR lpwavehdr = (LPWAVEHDR) pbase->m_lparam.m_lparam;

         wave_out_out_buffer_done((int32_t) lpwavehdr->dwUser);

      }

      void wave_out::OnMultimediaClose(::signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }

      /*void wave_out::wave_out_on_buffer_ready(::signal_details * pobj)
      {
         UNREFERENCED_PARAMETER(pobj);
      }*/

      void wave_out::wave_out_buffer_ready(int iBuffer)
      {


         if(wave_out_get_state() != state_playing)
         {

            TRACE("ERROR wave_out::BufferReady while wave_out_get_state() != state_playing");

            return;

         }

         ::multimedia::result mmr;

         if(m_peffect != NULL)
         {

            m_peffect->Process16bits((int16_t *) wave_out_get_buffer_data(iBuffer), wave_out_get_buffer_size());

         }

         single_lock sLock(&m_mutex, TRUE);


         int iWrittenFrameCount;

         int iFrameSize = (m_pwaveformat->nChannels * m_pwaveformat->wBitsPerSample) / 8;

         if(iFrameSize == 0)
            return;

         int iFrameCount = wave_out_get_buffer_size() / iFrameSize;

         int iBytePos = 0;

         mmr = MMSYSERR_NOERROR;

         while(iFrameCount > 0)
         {

            iWrittenFrameCount = snd_pcm_writei(m_ppcm, &((byte *)wave_out_get_buffer_data(iBuffer))[iBytePos], iFrameCount);

            if(iWrittenFrameCount == -EAGAIN)
               continue;

            if(iWrittenFrameCount < 0)
            {

               mmr = MMSYSERR_ERROR;

               break;

            }

            iFrameCount -= iWrittenFrameCount;

            iBytePos += iWrittenFrameCount * iFrameSize;

         }

         //VERIFY(MMSYSERR_NOERROR == mmr);

         if(mmr == MMSYSERR_NOERROR)
         {

            m_iBufferedCount++;

            wave_out_free(iBuffer);

         }

      }

      bool wave_out::wave_out_stop()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate != state_playing && m_estate != state_paused)
            return false;

         m_eventStopped.ResetEvent();

         m_pprebuffer->Stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = snd_pcm_drain(m_ppcm);



         if(m_mmr == MMSYSERR_NOERROR)
         {
            m_estate = state_opened;
         }

         return m_mmr == MMSYSERR_NOERROR;

      }

      bool wave_out::wave_out_pause()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return false;



         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = snd_pcm_pause(m_ppcm, 1);


         ASSERT(m_mmr == MMSYSERR_NOERROR);

         if(m_mmr == MMSYSERR_NOERROR)
         {
            m_estate = state_paused;
         }

         return m_mmr == MMSYSERR_NOERROR;

      }

      bool wave_out::wave_out_restart()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return false;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = snd_pcm_pause(m_ppcm, 0);


         ASSERT(m_mmr == MMSYSERR_NOERROR);

         if(m_mmr == MMSYSERR_NOERROR)
         {
            m_estate = state_playing;
         }

         return m_mmr == MMSYSERR_NOERROR;

      }


      /*imedia::time wave_out::GetPositionMillisForSynch()
      {
         int64_t dwMillis = GetPositionMillis();
         int64_t dwPosition = m_pprebuffer->m_position * 8;
         dwPosition /= m_pwaveformat->wBitsPerSample;
         dwPosition *= 1000;
         dwPosition /= m_pwaveformat->nChannels * m_pwaveformat->nSamplesPerSec;
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return dwMillis + dwPosition - m_pprebuffer->m_pdecoder->DecoderGetLostMillis(dwMillis + dwPosition) - (((int64_t) m_dwLostSampleCount) /  ((int64_t) m_pwaveformat->nSamplesPerSec));
         else
            return dwMillis + dwPosition - ((m_dwLostSampleCount) * 1000 / m_pwaveformat->nSamplesPerSec);
      }*/

      imedia::time wave_out::wave_out_get_position_millis()
      {
         single_lock sLock(&m_mutex, TRUE);



         ::multimedia::result                mmr;
         snd_htimestamp_t                  mmt;
         snd_pcm_uframes_t ut;

         if(m_ppcm != NULL)
         {
            mmr = snd_pcm_htimestamp(m_ppcm, &ut, &mmt);
            try
            {
               if (MMSYSERR_NOERROR != mmr)
               {
                  TRACE( "snd_pcm_status_get_htstamp() returned %s", snd_strerror(mmr));
                  //      return MCIERR_DEVICE_NOT_READY;
                  return 0;
               }
            }
            catch(...)
            {
               //return MCIERR_DEVICE_NOT_READY;
               return 0;
            }
            {
               //*pTicks += mmt.u.ticks;
               return mmt.tv_nsec / (1000 * 1000) + mmt.tv_sec * 1000;
            }
         }
         else
            return 0;


      }

      /*imedia::position wave_out::get_position_for_synch()
      {
         imedia::position position = get_position();
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->DecoderGetLostPositionOffset(position) - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         else
            return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      }*/

      imedia::position wave_out::wave_out_get_position()
      {
         single_lock sLock(&m_mutex, TRUE);

         ::multimedia::result                mmr;
         snd_htimestamp_t                  mmt;
         snd_pcm_uframes_t ut;

         if(m_ppcm != NULL)
         {
            mmr = snd_pcm_htimestamp(m_ppcm, &ut, &mmt);
            try
            {
               if (MMSYSERR_NOERROR != mmr)
               {
                  TRACE( "snd_pcm_status_get_htstamp() returned %s", snd_strerror(mmr));
                  //      return MCIERR_DEVICE_NOT_READY;
                  return 0;
               }
            }
            catch(...)
            {
               //return MCIERR_DEVICE_NOT_READY;
               return 0;
            }
            {
               //*pTicks += mmt.u.ticks;
               return mmt.tv_nsec / (1000 * 1000) + mmt.tv_sec * 1000;
            }
         }
         else
            return 0;


      }

      void wave_out::wave_out_free(int iBuffer)
      {

         //wave_out_free(wave_hdr(iBuffer));

         multimedia::audio::wave_out::wave_out_free(iBuffer);

      }


      void wave_out::wave_out_free(LPWAVEHDR lpwavehdr)
      {


      }

      void wave_out::wave_out_on_playback_end()
      {

         wave_out_stop();

         if(m_pprebuffer->m_pstreameffectOut != NULL)
         {
            ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
            m_pprebuffer->m_pstreameffectOut = NULL;
            delete peffect;
         }

         m_eventStopped.SetEvent();

         m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);

      }



/*      WAVEFORMATEX * wave_out::wave_format()
      {

         translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }*/

      snd_pcm_t * wave_out::wave_out_get_safe_PCM()
      {

         if(this == NULL)
            return NULL;

         return m_ppcm;

      }

      void * wave_out::get_os_data()
      {
         return m_ppcm;
      }

/*
      LPWAVEHDR wave_out::wave_hdr(int iBuffer)
      {
         return ::multimedia::audio_alsa::get_os_data(wave_out_get_buffer(), iBuffer);
      }
*/



   } // namespace audio_alsa


} // namespace multimedia






