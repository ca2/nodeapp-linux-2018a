#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class  CLASS_DECL_AUDIO_MMSYSTEM wave_out :
         virtual public snd_pcm,
         virtual public ::multimedia::audio::wave_out
      {
      public:


         enum e_impl_message
         {

            message_ready = 5555,
            message_free

         };


         bool                    m_bWrite;
         int                     m_iBuffer;
         //imedia_time           m_timeCache;
         bool                    m_bStarted;
         uint64_t                m_uiStart;
         snd_pcm_status_t *      m_pstatus;





         wave_out(sp(::base::application) papp);
         virtual ~wave_out();


         void install_message_handling(::message::dispatch * pinterface);

         virtual imedia_time wave_out_get_position_millis();
         imedia_position wave_out_get_position();

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount) override;
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose) override;
         virtual ::multimedia::e_result wave_out_stop() override;
         virtual ::multimedia::e_result wave_out_close() override;
         virtual ::multimedia::e_result wave_out_pause() override;
         virtual ::multimedia::e_result wave_out_restart() override;
         virtual void * get_os_data();
         snd_pcm_t * wave_out_get_safe_PCM();

         virtual void wave_out_on_playback_end() override;
         virtual void wave_out_buffer_ready(int iBuffer) override;
         virtual void alsa_out_buffer_ready(int iBuffer);
         virtual void wave_out_free(int iBuffer) override;
         virtual void alsa_out_free(int iBuffer);

         virtual bool initialize_thread();
         virtual int32_t exit_thread();

         DECL_GEN_SIGNAL(OnReady);
         DECL_GEN_SIGNAL(OnFree);

         virtual ::multimedia::e_result wave_out_start(const imedia_position & position);

         virtual bool on_run_step();

         virtual int underrun_recovery(int err);

         //int32_t wave_out_get_buffered_buffer_count();

      };


   } // namespace audio_alsa


} // namespace multimedia



