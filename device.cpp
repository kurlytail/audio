#include "device.h"

SoundDeviceFactory factory;

std::ostream &operator<<(std::ostream &os, std::shared_ptr<SoundDevice> dev)
{
    os << std::string(dev->info->name);
    return os;
}

std::ostream &operator<<(std::ostream &os, const PaDeviceInfo *info)
{
    os << "maxio: " << info->maxInputChannels << ":" << info->maxOutputChannels;
    return os;
}

SoundDeviceFactory::SoundDeviceFactory()
{
    LOG(INFO) << Pa_GetVersionText();

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        auto e = SoundException(Pa_GetErrorText(err));
        LOG(ERROR) << "Pa_Initialize returned " << e.what();
        throw(e);
    }

    auto numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        auto e = SoundException(Pa_GetErrorText(numDevices));
        LOG(ERROR) << "Pa_GetDeviceCount returned " << e.what();
        throw(e);
    }

    for (auto i = 0; i < numDevices; i++) {
        auto device = std::make_shared<SoundDevice>(i);
        LOG(INFO) << "Detected device " << device;
        LOG(INFO) << "     " << device->info;

        std::unique_lock<std::mutex> lock(this->mutex);
        this->devices.push_back(device);
    }
}

SoundDeviceFactory::~SoundDeviceFactory() noexcept(false)
{
    auto err = Pa_Terminate();
    if (err != paNoError) {
        auto e = SoundException(Pa_GetErrorText(err));
        LOG(ERROR) << "Pa_Terminate returned " << e.what();
        throw(e);
    }
}