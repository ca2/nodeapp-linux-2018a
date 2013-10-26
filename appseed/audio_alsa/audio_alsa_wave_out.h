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
            MessageReady = 5555,
            MessageFree,
            MessageDone
         };


         bool                    m_bWrite;
         int                     m_iBuffer;


         wave_out(sp(base_application) papp);
         virtual ~wave_out();


         void install_message_handling(::message::dispatch * pinterface);

         virtual imedia::time wave_out_get_position_millis();
         imedia::position wave_out_get_position();
         virtual void wave_out_buffer_ready(int iBuffer);
         virtual void alsa_out_buffer_ready(int iBuffer);
         //virtual void wave_out_buffer_ready(LPWAVEHDR lpwavehdr);

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample);
         virtual ::multimedia::e_result wave_out_stop();
         virtual ::multimedia::e_result wave_out_close();
         virtual ::multimedia::e_result wave_out_pause();
         virtual ::multimedia::e_result wave_out_restart();
         virtual void * get_os_data();
         snd_pcm_t * wave_out_get_safe_PCM();

         virtual void wave_out_on_playback_end();
         virtual void wave_out_free(int iBuffer);

         virtual bool initialize_instance();
         virtual int32_t exit_instance();

         DECL_GEN_SIGNAL(OnReady)
         DECL_GEN_SIGNAL(OnFree)
         DECL_GEN_SIGNAL(OnDone)

         virtual ::multimedia::e_result wave_out_start(const imedia::position & position);

         virtual void on_run_step();

         virtual int underrun_recovery(int err);

      };


   } // namespace audio_alsa


} // namespace multimedia



