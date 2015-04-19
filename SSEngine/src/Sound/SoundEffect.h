#ifndef SandboxSimulator_SoundEffect_h
#define SandboxSimulator_SoundEffect_h

#include <System/File.h>
#include <System/PlatformIncludes.h>

#include <SFML/Audio.hpp>

namespace SandboxSimulator {
class SSEngine;

class SoundEffect : public Asset
{
    public:
        SoundEffect(SSEngine* Eng);
        ~SoundEffect();

        virtual bool Load(Literal Path);
        virtual bool OnLoad() { return true; }
        void SetVolume(i32 Volume);
        i32 GetVolume() { return m_Volume; }
        bool IsPlaying();

        void Play() { if(m_Loaded) m_BackendSound.play(); }
        void Pause() { if(IsPlaying() && m_Loaded) m_BackendSound.pause(); }
        void Stop() { if(IsPlaying() && m_Loaded) m_BackendSound.stop(); }
        void SetLoop(bool Loop) { if(m_Loaded) m_BackendSound.setLoop(Loop); }
        sf::Sound* GetPtr() { return &m_BackendSound; };
    protected:
        sf::Sound m_BackendSound;
        sf::SoundBuffer m_Buffer;
        SSEngine* m_Engine;
        bool m_Loaded;
        i32 m_Volume;
};
}

#endif