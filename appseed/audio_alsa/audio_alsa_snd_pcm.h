#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class CLASS_DECL_AUDIO_MMSYSTEM snd_pcm :
         virtual public ::multimedia::audio::wave_base
      {
      public:



         snd_pcm_t *                      m_ppcm;
         snd_pcm_hw_params_t *            m_phwparams;



         snd_pcm(sp(base_application) papp);
         virtual ~snd_pcm();


         ::multimedia::e_result snd_pcm_open(snd_pcm_stream_t stream_type);
         ::multimedia::e_result snd_pcm_close();


      };


   } // namespace audio_alsa


} // namespace multimedia




