#include <Sound/SoundEffect.h>
#include <Engine.h>

namespace SandboxSimulator {
    SoundEffect::SoundEffect(SSEngine* Eng) : m_Loaded(false)
    {
        m_Engine = Eng;
        m_Volume = 100;
    }

    SoundEffect::~SoundEffect()
    {
        m_BackendSound.stop();
    }

    bool SoundEffect::Load(Literal Path)
    {
        m_LoadPath = Path;
        if(!m_Buffer.loadFromFile(Path)) {
            m_Engine->Log("Could not load sound file at '%s'", Path);
            return false;
        }
        m_BackendSound.setBuffer(m_Buffer);
        m_BackendSound.setVolume(m_Volume);
        m_Loaded = true;
        return true;
    }

    bool SoundEffect::IsPlaying() 
    {
        return m_BackendSound.getStatus() == sf::SoundSource::Playing;
    }

    void SoundEffect::SetVolume(i32 Vol) {
        if(Vol <= 100 && Vol >= 0) {
            m_Volume = Vol;
        } else {
            m_Engine->Log("Volume %d is not a valid setting, please make the volume between 0 and 100.", Vol);
        }
        m_BackendSound.setVolume(m_Volume*m_Engine->GetVolumeFactor());
    }
}