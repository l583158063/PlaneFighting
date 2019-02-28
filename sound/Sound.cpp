#include "Sound.h"
#include <fstream>
#include <cassert>
Sound & Sound::Get()
{
	static Sound instance;
	return instance;
}

bool Sound::LoadWaveFile(std::string soundName, std::string filename)
{
	IDirectSoundBuffer8* buffer = nullptr;
	if (!LoadWaveFile(filename.c_str(), &buffer))
	{
		return false;
	}
	m_secondaryBuffer[soundName] = buffer;
	return true;
}

bool Sound::PlayWaveFile(std::string soundName, bool loop)
{
	HRESULT result;

	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer[soundName]->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%.
	result = m_secondaryBuffer[soundName]->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary sound buffer.
	if (loop)
	{
		result = m_secondaryBuffer[soundName]->Play(0, 0, 1);
	}
	else
	{
		result = m_secondaryBuffer[soundName]->Play(0, 0, 0);
	}

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Sound::Stop(std::string soundName)
{
	if (FAILED(m_secondaryBuffer[soundName]->Stop()))
	{
		return false;
	}
	return true;
}

Sound::Sound()
{
	m_DirectSound = 0;
		m_primaryBuffer = 0;
}

Sound::~Sound()
{
}


bool Sound::Initialize(HWND hwnd)
{
	bool result;


	// Initialize direct sound and the primary sound buffer.
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}


void Sound::Shutdown()
{
	// Release the secondary buffer.
	for (auto& it : m_secondaryBuffer)
	{
		ShutdownWaveFile(&(it.second));
	}

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();

	return;
}


bool Sound::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void Sound::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}


bool Sound::LoadWaveFile(const char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	const auto IsFourCC = [](const BYTE* pData, const char* pFourcc)
	{
		assert(strlen(pFourcc) == 4);
		for (int i = 0; i < 4; i++)
		{
			if (char(pData[i]) != pFourcc[i])
			{
				return false;
			}
		}
		return true;
	};

	unsigned int fileSize = 0;
	std::unique_ptr<BYTE[]> pFileIn;

	{
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(fileName, std::ios::binary);

		{
			BYTE fourcc[4];
			file.read(reinterpret_cast<char*>(fourcc), 4u);
			//std::cout << "Chunk ID: " << fourcc[0] << fourcc[1] << fourcc[2] << fourcc[3] << std::endl;
			if (!IsFourCC(fourcc, "RIFF"))
			{
				//std::cout << "RIFF ERROR!" << std::endl;
			}
		}

		file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
		//std::cout << "Chunk Size: " << (fileSize + 8u) / 1024u / 1024u << "MB" << std::endl;
		fileSize += 8u; // entry doesn't include the fourcc or itself
		if (fileSize <= 44u)
		{
			//std::cout << "FILE TOO SMALL!" << std::endl;
		}

		file.seekg(0, std::ios::beg);
		pFileIn = std::make_unique<BYTE[]>(fileSize);
		file.read(reinterpret_cast<char*>(pFileIn.get()), fileSize);
	}

	//std::cout << "Format: " << pFileIn[8] << pFileIn[9] << pFileIn[10] << pFileIn[11] << std::endl;;
	if (!IsFourCC(&pFileIn[8], "WAVE"))
	{
		//std::cout << "WAVE ERROR!" << std::endl;
	}

	//look for 'fmt ' chunk id
	WAVEFORMATEX format;
	bool bFilledFormat = false;
	for (size_t i = 12u; i < fileSize; )
	{
		if (IsFourCC(&pFileIn[i], "fmt "))
		{
			memcpy(&format, &pFileIn[i + 8u], sizeof(format));
			bFilledFormat = true;
			break;
		}
		// chunk size + size entry size + chunk id entry size + word padding
		unsigned int chunkSize;
		memcpy(&chunkSize, &pFileIn[i + 4u], sizeof(chunkSize));
		i += (chunkSize + 9u) & 0xFFFFFFFEu;
	}

	if (!bFilledFormat)
	{
		;
	}

	//look for 'data' chunk id
	std::unique_ptr<BYTE[]> pData;
	UINT32 nBytes = 0u;
	bool bFilledData = false;
	for (size_t i = 12u; i < fileSize; )
	{
		unsigned int chunkSize;
		memcpy(&chunkSize, &pFileIn[i + 4u], sizeof(chunkSize));

		if (IsFourCC(&pFileIn[i], "data"))
		{
			//std::cout << "Duration: " << chunkSize / format.nAvgBytesPerSec << "S" << std::endl;
			pData = std::make_unique<BYTE[]>(chunkSize);
			nBytes = chunkSize;
			memcpy(pData.get(), &pFileIn[i + 8u], nBytes);

			bFilledData = true;
			break;
		}
		// chunk size + size entry size + chunk id entry size + word padding
		i += (chunkSize + 9u) & 0xFFFFFFFEu;
	}

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = nBytes;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &format;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	// Create a temporary sound buffer with the specific buffer settings.
	IDirectSoundBuffer* tempBuffer;
	HRESULT result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	unsigned char* bufferPtr;
	unsigned long bufferSize;
	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, nBytes, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, pData.get(), nBytes);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}