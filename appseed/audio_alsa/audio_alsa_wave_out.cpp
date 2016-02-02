#include "framework.h"



namespace multimedia
{


   namespace audio_alsa
   {


      wave_out::wave_out(sp(::base::application) papp) :
         object(papp),
         ::thread(papp),
        wave_base(papp),
         snd_pcm(papp),
         ::multimedia::audio::wave_out(papp)
      {

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_iBufferedCount     = 0;
         m_mmr                = result_success;
         m_peffect            = NULL;
         m_dwLostSampleCount  = 0;
         m_bWrite             = false;
      }

      wave_out::~wave_out()
      {

      }

      void wave_out::install_message_handling(::message::dispatch * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_handling(pinterface);

         IGUI_WIN_MSG_LINK(MessageReady, pinterface, this, &wave_out::OnReady);
         IGUI_WIN_MSG_LINK(MessageFree, pinterface, this, &wave_out::OnFree);
         IGUI_WIN_MSG_LINK(MessageDone, pinterface, this, &wave_out::OnDone);

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

      ::multimedia::e_result wave_out::wave_out_open(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount)
      {
         single_lock sLock(&m_mutex, TRUE);
         if(m_ppcm != NULL &&
            m_estate != state_initial)
            return result_success;
         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
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

         if((m_mmr = this->snd_pcm_open(SND_PCM_STREAM_PLAYBACK)) != result_success)
         {

            return result_error;

         }



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

         //uiBufferCount = 1;

         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, uiBufferCount, 128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, uiBufferCount, iBufferSampleCount); // group sample count

         int iFrameSize = (m_pwaveformat->nChannels * m_pwaveformat->wBitsPerSample) / 8;

         int err;

         snd_pcm_sw_params_alloca(&m_pswparams);

         /* get the current m_pswparams */
         err = snd_pcm_sw_params_current(m_ppcm, m_pswparams);

         if (err < 0)
         {

            TRACE("Unable to determine current m_pswparams for playback: %s\n", snd_strerror(err));

            return result_error;

         }


         /* start the transfer when the buffer is almost full: */
         /* (buffer_size / avail_min) * avail_min */
         err = snd_pcm_sw_params_set_start_threshold(m_ppcm, m_pswparams, (buffer_size / period_size) * period_size);
         if (err < 0)
         {

            TRACE("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         /* allow the transfer when at least period_size samples can be processed */
         err = snd_pcm_sw_params_set_avail_min(m_ppcm, m_pswparams, period_size);
         if (err < 0)
         {

            TRACE("Unable to set avail min for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         /* write the parameters to the playback device */
         err = snd_pcm_sw_params(m_ppcm, m_pswparams);
         if (err < 0)
         {

            TRACE("Unable to set sw params for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         m_estate = state_opened;
         return result_success;
      }





      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose)
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_ppcm != NULL && m_estate != state_initial)
            return result_success;

         m_pthreadCallback = pthreadCallback;

         ASSERT(m_ppcm == NULL);

         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag        = 0;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = 0;

         if((m_mmr = this->snd_pcm_open(SND_PCM_STREAM_PLAYBACK)) != result_success)
         {

            return m_mmr;

         }

         uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         uint32_t uiAnalysisSize;
         uint32_t uiAllocationSize;
         uint32_t uiInterestSize;
         uint32_t uiSkippedSamplesCount;

         iBufferSampleCount      = period_size;

         if(true)
         {
            uiBufferSizeLog2 = 16;
            //uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            /*while(((double)(buffer_size * 8) / (double)(uiBitsPerSample * uiSamplesPerSec)) > 0.084)
            {
                buffer_size /= 2;
            }*/
            uiBufferSize = period_size;
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

         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, iBufferCount, 128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(this, m_pwaveformat->nChannels, iBufferCount, iBufferSampleCount);

         m_pprebuffer->SetMinL1BufferCount(wave_out_get_buffer()->GetBufferCount());

         int err;

         snd_pcm_sw_params_alloca(&m_pswparams);

         // get the current m_pswparams
         if((err = snd_pcm_sw_params_current(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("Unable to determine current m_pswparams for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         // start the transfer when the buffer is almost full:
         if((err = snd_pcm_sw_params_set_start_threshold(m_ppcm, m_pswparams, buffer_size)) < 0)
         {

            TRACE("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         // allow the transfer when at least period_size samples can be processed
         if((err = snd_pcm_sw_params_set_avail_min(m_ppcm, m_pswparams, period_size)) < 0)
         {

            TRACE("Unable to set avail min for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         // write the parameters to the playback device
         if((err = snd_pcm_sw_params(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("Unable to set sw params for playback: %s\n", snd_strerror(err));

            return result_error;

         }

         m_estate = state_opened;

         m_epurpose = epurpose;

         return result_success;

      }



      ::multimedia::e_result wave_out::wave_out_close()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return result_success;

         ::multimedia::e_result mmr;

/*         int32_t i, iSize;

         iSize =  wave_out_get_buffer()->GetBufferCount();

         for(i = 0; i < iSize; i++)
         {

            if(result_success != (mmr = waveOutUnprepareHeader(m_ppcm, wave_hdr(i), sizeof(WAVEHDR))))
            {
               TRACE("ERROR OPENING Unpreparing INPUT DEVICE buffer =%d", mmr);
            }

            delete wave_hdr(i);

         }*/

         mmr = this->snd_pcm_close();

         m_ppcm = NULL;

         m_pprebuffer->Reset();

         m_estate = state_initial;

         return result_success;

      }





      ::multimedia::e_result wave_out::wave_out_stop()
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate != state_playing && m_estate != state_paused)
            return result_error;

         m_eventStopped.ResetEvent();

         m_pprebuffer->Stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = translate_alsa(snd_pcm_drain(m_ppcm));


         if(m_mmr == result_success)
         {

            m_estate = state_opened;

         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_pause()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = translate_alsa(snd_pcm_pause(m_ppcm, 1));

         ASSERT(m_mmr == result_success);

         if(m_mmr == result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_restart()
      {

         single_lock sLock(&m_mutex, TRUE);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = translate_alsa(snd_pcm_pause(m_ppcm, 0));

         ASSERT(m_mmr == result_success);

         if(m_mmr == result_success)
         {
            m_estate = state_playing;
         }

         return m_mmr;

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

      imedia_time wave_out::wave_out_get_position_millis()
      {

         single_lock sLock(&m_mutex, TRUE);

         ::multimedia::e_result                mmr;

         snd_htimestamp_t                  mmt;

         snd_pcm_uframes_t ut;

         if(m_ppcm != NULL)
         {

            mmr = translate_alsa(snd_pcm_htimestamp(m_ppcm, &ut, &mmt));

            try
            {
               if (result_success != mmr)
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

         ::multimedia::e_result                mmr;

         snd_htimestamp_t                    mmt;

         snd_pcm_uframes_t                   ut;

         if(m_ppcm != NULL)
         {

            mmr = translate_alsa(snd_pcm_htimestamp(m_ppcm, &ut, &mmt));

            try
            {

               if (result_success != mmr)
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
         {

            return 0;

         }

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


      void wave_out::wave_out_free(int iBuffer)
      {

         m_iBufferedCount++;

         post_thread_message(MessageFree, iBuffer);

      }


      void wave_out::wave_out_buffer_ready(int iBuffer)
      {

         post_thread_message(MessageReady, iBuffer);

      }



      void wave_out::OnReady(::signal_details * pobj)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         int iBuffer = pbase->m_wparam;

         alsa_out_buffer_ready(iBuffer);

      }


      void wave_out::OnFree(::signal_details * pobj)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         int iBuffer = pbase->m_wparam;

         multimedia::audio::wave_out::wave_out_free(iBuffer);

      }

      void wave_out::OnDone(::signal_details * pobj)
      {

         SCAST_PTR(::message::base, pbase, pobj);

         int iBuffer = pbase->m_wparam;

         m_iBufferedCount--;

         if(m_iBufferedCount < 0)
            m_iBufferedCount = 0;

         wave_out_out_buffer_done(iBuffer);

      }


      void wave_out::alsa_out_buffer_ready(int iBuffer)
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_peffect != NULL)
         {

            m_peffect->Process16bits((int16_t *) wave_out_get_buffer_data(iBuffer), wave_out_get_buffer_size());

         }


         int result = 0;


         int cptr = period_size;

         ::multimedia::e_result mmr = result_success;

         signed short * ptr = (signed short *) wave_out_get_buffer_data(iBuffer);


         snd_pcm_sframes_t avail = 0;

         if(m_ppcm == NULL)
         {

            goto finalize;

         }



         while(avail >= 0 && avail < period_size)
         {

            if((result = snd_pcm_wait (m_ppcm, 1984)) < 0)
            {
               m_estate = state_opened;

               m_mmr = result_error;

               TRACE("alsa wave_out wait error: %s\n", snd_strerror(result));

               goto finalize;

            }

            avail = snd_pcm_avail_update(m_ppcm);

         }





         /*

         int l = 0;
         for(int i = 0; i < period_size; i++)
         {
            if(l %  40 < 20)
            {
               ptr[0] = 1000;
               ptr[1] = 1000;
            }
            else
            {
               ptr[0] = -1000;
               ptr[1] = -1000;
            }

            ptr+=2;
            l++;
         }

         ptr = (signed short *) wave_out_get_buffer_data(iBuffer);

         */

         while (cptr > 0)
         {

            result = snd_pcm_writei(m_ppcm, ptr, cptr);

            if(result == -EAGAIN)
               continue;

            if(result < 0)
            {

               if((result = underrun_recovery(result)) < 0)
               {

                  m_estate = state_opened;

                  m_mmr = result_error;

                  TRACE("alsa wave_out Write error: %s\n", snd_strerror(result));

               }

               goto finalize;

            }

            ptr += result * m_pwaveformat->nChannels;

            cptr -= result;


         }

         finalize:

         sLock.unlock();

         post_thread_message(MessageDone, iBuffer);

      }


      ::multimedia::e_result wave_out::wave_out_start(const imedia::position & position)
      {

         single_lock sLock(&m_mutex, TRUE);

         if(m_estate == state_playing)
            return result_success;

         if(m_estate != state_opened && m_estate != state_stopped)
            return result_error;

         int err = 0;

         if ((err = snd_pcm_prepare (m_ppcm)) < 0)
         {

            TRACE ("cannot prepare audio interface for use (%s)\n",snd_strerror (err));

            return result_error;

         }

         m_mmr = ::multimedia::audio::wave_out::wave_out_start(position);

         if(failed(m_mmr))
            return m_mmr;

         return result_success;

      }


      bool wave_out::on_run_step()
      {

         return ::multimedia::audio::wave_out::on_run_step();

         /*if(m_estate == state_playing)
         {

            wave_out_out_buffer_done(m_iCurrentBuffer);

            m_iCurrentBuffer++;

            if(m_iCurrentBuffer >= wave_out_get_buffer()->GetBufferCount())
               m_iCurrentBuffer = 0;

         }*/


      }


      int wave_out::underrun_recovery(int err)
      {

         //if(verbose)
           //printf("stream recovery\n");

         if(m_pprebuffer->IsEOF() || wave_out_get_state() == state_stopping)
         {

            return 0;

         }
         else if (err == -EPIPE)
         {

            // under-run
            err = snd_pcm_prepare(m_ppcm);

            if (err < 0)
            {

               TRACE("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));

            }
            else if (err == -ESTRPIPE)
            {

               while ((err = snd_pcm_resume(m_ppcm)) == -EAGAIN)
               {

                  sleep(1); /* wait until the suspend flag is released */

               }

               if (err < 0)
               {

                  err = snd_pcm_prepare(m_ppcm);

                  if (err < 0)
                  {

                     TRACE("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));

                  }

               }

            }

            return 0;

         }

         return err;

      }


      int32_t wave_out::wave_out_get_buffered_buffer_count()
      {

         return m_iBufferedCount;

      }


   } // namespace audio_alsa


} // namespace multimedia






