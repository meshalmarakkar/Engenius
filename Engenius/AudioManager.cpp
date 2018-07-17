#include "AudioManager.h"
/*
Killers Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 3.0 License
http://creativecommons.org/licenses/by/3.0/

Corruption Kevin MacLeod (incompetech.com)
Licensed under Creative Commons: By Attribution 3.0 License
http://creativecommons.org/licenses/by/3.0/

pickup
https://www.freesound.org/people/juancamiloorjuela/sounds/204318/

enemy hit
http://www.wolfensteingoodies.com/archives/olddoom/music.htm
	
http://www.youtube.com/user/thecplusplusguy
*/

bool AudioManager::isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int AudioManager::convertToInt(const char* buffer, const int& len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* AudioManager::loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}

AudioManager::~AudioManager() {
	std::unordered_map<std::string, ALuint>::iterator it;
	for (it = sourceIDs.begin(); it != sourceIDs.end(); it++) {
		Stop((*it).first);
		alDeleteSources(1, &(*it).second);
	}
}

AudioManager::AudioManager(){
	device = alcOpenDevice(NULL);
	if (!device) std::cout << "no sound device" << std::endl;

	context = alcCreateContext(device, NULL);
	if(!context) std::cout << "no sound context" << std::endl;
	alcMakeContextCurrent(context);

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	temp_volume = 100.0f;

	LoadAudioFiles("tense", "../Engenius/Audio/bkg_tense.wav");
	LoadAudioFiles("menu_background", "../Engenius/Audio/ambient_creepy.wav");
	LoadAudioFiles("correct", "../Engenius/Audio/correct.wav");
	LoadAudioFiles("wrong", "../Engenius/Audio/wrong.wav");
	LoadAudioFiles("gear_spinning", "../Engenius/Audio/gear_spinning.wav");
	LoadAudioFiles("machine", "../Engenius/Audio/ambient_sci-fiLab.wav");
	LoadAudioFiles("alarm", "../Engenius/Audio/alarm.wav");

	InsertSource("menu_background", glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), 1.0f, 1.0f, 10.0f, true, true);
	InsertSource("tense", glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), 1.0f, 1.0f, 10.0f, true, true);
	InsertSource("alarm", glm::vec3(-4.04f, 3.78f, -32.05f), glm::vec3(0.0f), 1.0f, 1.0f, 15.0f, true);
	InsertSource("machine", glm::vec3(-7.44509f, 0.0f, -18.0f), glm::vec3(0.0f), 0.4f, 1.0f, 0.25f, true);
	InsertSource("machine2", glm::vec3(7.49402f, 0.0f, -43.0f), glm::vec3(0.0f), 0.4f, 1.0f, 0.25f, true);
	InsertSource("correct", glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), 0.75f, 1.0f, 10.0f, false, true);
	InsertSource("wrong", glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), 0.75f, 1.0f, 10.0f, false, true);
	InsertSource("gear_spinning", glm::vec3(2.02, -0.058, 4.58), glm::vec3(0.0f), 1.0f, 1.0f, 5.0f, false);

	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}

void AudioManager::LoadAudioFiles(const std::string& name, const char * filepath) {
	int channel, sampleRate, bps, size;
	char* data = loadWAV(filepath, channel, sampleRate, bps, size);

	ALuint format;

	if (channel == 1)
	{
		if (bps == 8)
			format = AL_FORMAT_MONO8;
		else
			format = AL_FORMAT_MONO16;
	}
	else {
		if (bps == 8)
			format = AL_FORMAT_STEREO8;
		else
			format = AL_FORMAT_STEREO16;
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);
	buffers.insert(std::pair<std::string, ALuint>(name, buffer));
	alBufferData(buffer, format, data, size, sampleRate);
}

void AudioManager::InsertSource(const std::string& name, const glm::vec3& pos, const glm::vec3& vel, const float& volume, const float& pitch, const float& referenceDist, const bool& loop, const bool& ambient) {
	ALuint sourceID;
	alGenSources(1, &sourceID);
	sourceIDs.insert(std::pair<std::string, ALuint>(name, sourceID));
	SetVolume(name, volume);
	SetPitch(name, pitch);
	SetPosition(name, pos);
	SetVelocity(name, vel);
	SetLooping(name, loop);
	if (ambient) {
		alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
		alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	}
	else {
		alSourcef(sourceID, AL_ROLLOFF_FACTOR, 1.0f);
		alSourcef(sourceID, AL_REFERENCE_DISTANCE, 10.0f); // full volume until this distance
		//alSourcef(sourceID, AL_MAX_DISTANCE, 10.0f); // no longer heard at this distance
	}

}

void AudioManager::StopAll() {
	std::unordered_map<std::string, ALuint>::iterator it;
	for (it = sourceIDs.begin(); it != sourceIDs.end(); it++) {
		Stop((*it).first);
	}
}

void AudioManager::SetVolume(const std::string& name, const float& volume) {
	alSourcef(sourceIDs.at(name), AL_GAIN, volume);
}
void AudioManager::SetPitch(const std::string& name, const float& pitch) {
	alSourcef(sourceIDs.at(name), AL_PITCH, pitch);
}
void AudioManager::SetPosition(const std::string& name, const glm::vec3& pos) {
	alSource3f(sourceIDs.at(name), AL_POSITION, pos.x, pos.y, pos.z);
}
void AudioManager::SetVelocity(const std::string& name, const glm::vec3& vel) {
	alSource3f(sourceIDs.at(name), AL_VELOCITY, vel.x, vel.y, vel.z);
}
void AudioManager::SetLooping(const std::string& name, const bool& loop) {
	alSourcei(sourceIDs.at(name), AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}
bool AudioManager::IfPlaying(const std::string& name) {
	ALint playing;
	alGetSourcei(sourceIDs.at(name), AL_SOURCE_STATE, &playing);
	return playing;
}

void AudioManager::PlayAudio(const std::string& name, const std::string& soundname, const glm::vec3& listenerPos) {
	updateListenerPosition(listenerPos);
	alListener3f(AL_POSITION, listenerPos.x, listenerPos.y, listenerPos.z);
	alSourcei(sourceIDs.at(name), AL_BUFFER, buffers.at(soundname));
	alSourcePlay(sourceIDs.at(name));
}
void AudioManager::playMenu(const glm::vec3& listenerPos) {
	std::unordered_map<std::string, ALuint>::iterator it;
	for (it = sourceIDs.begin(); it != sourceIDs.end(); it++) {
		Stop((*it).first);
	}
	PlayAudio("menu_background", "menu_background", listenerPos);
}

void AudioManager::startGame(const glm::vec3& listenerPos) {
	Stop("menu_background");
	PlayAudio("machine", "machine", listenerPos);
	SetVolume("machine", 0.1f);
	PlayAudio("machine2", "machine", listenerPos);
	SetVolume("machine2", 0.1f);
	PlayAudio("tense", "tense", listenerPos);

	//stopAllMusicForShowreel
	StopAll();
}

void AudioManager::playAlarm(const glm::vec3& listenerPos) {
	PlayAudio("alarm", "alarm", listenerPos);
}

void AudioManager::stopTense() {
	Stop("tense");
	SetVolume("machine", 0.4f);
	SetVolume("machine2", 0.4f);
}

void AudioManager::playSFX_correct(const glm::vec3& listenerPos) {
	PlayAudio("correct", "correct", listenerPos);
}

void AudioManager::playSFX_wrong(const glm::vec3& listenerPos) {
	PlayAudio("wrong", "wrong", listenerPos);
}

void AudioManager::playSFX_gearSpinning(const glm::vec3& listenerPos) {
	Stop("machine");
	Stop("machine2");
	Stop("tense");
	PlayAudio("gear_spinning", "gear_spinning", listenerPos);
}

void AudioManager::Pause(const std::string& name) {
	alSourcePause(sourceIDs.at(name));
}
void AudioManager::Continue(const std::string& name) {
	alSourcePlay(sourceIDs.at(name));
}
void AudioManager::Stop(const std::string& name) {
	alSourceStop(sourceIDs.at(name));
}

void AudioManager::updateListenerPosition(const glm::vec3& pos) {
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}