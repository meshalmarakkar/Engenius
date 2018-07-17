#ifndef AUDIOMANAGER
#define AUDIOMANAGER

#include <al.h>
#include <alc.h>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <iostream>
#include <fstream>

class AudioManager {
public:
	AudioManager();
	bool isBigEndian();
	int convertToInt(const char* buffer, const int& len);
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
	~AudioManager();
	void LoadAudioFiles(const std::string& name, const char * filepath);
	void InsertSource(const std::string& name, const glm::vec3& pos, const glm::vec3& velocity, const float& volume, const float& pitch, const float& referenceDist, const bool& loop, const bool& ambient = false);
	void StopAll();
	void SetVolume(const std::string& name, const float& volume);
	void SetPitch(const std::string& name, const float& pitch);
	void SetPosition(const std::string& name, const glm::vec3& pos);
	void SetVelocity(const std::string& name, const glm::vec3& vel);
	void SetLooping(const std::string& name, const bool& loop);
	bool IfPlaying(const std::string& name);
	void updateListenerPosition(const glm::vec3& pos);
	
	void playMenu(const glm::vec3& listenerPos);
	void startGame(const glm::vec3& listenerPos);
	void playAlarm(const glm::vec3& listenerPos);
	void stopTense();
	void playSFX_correct(const glm::vec3& listenerPos);
	void playSFX_wrong(const glm::vec3& listenerPos);
	void playSFX_gearSpinning(const glm::vec3& listenerPos);

	void Pause(const std::string& name);
	void Continue(const std::string& name);
	void Stop(const std::string& name);


private:
	void PlayAudio(const std::string& name, const std::string& soundname, const glm::vec3& listenerPos);

	float temp_volume;//for transitioning

	ALCdevice * device;
	ALCcontext * context;
	std::unordered_map<std::string, ALuint> buffers;
	std::unordered_map<std::string, ALuint> sourceIDs;
};

#endif // AUDIOMANAGER
