
#pragma once

#include <cstdlib>
#include <exception>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <list>
#include <memory>
#include <mutex>
#include <portaudio.h>
#include <rtmidi/RtMidi.h>
#include <yaml-cpp/yaml.h>

struct SoundException : public std::exception {
    const char *err = 0;
    SoundException(const char *e) : err(e) { ; }
    virtual const char *what() const noexcept { return err; }
};

struct SoundDeviceBase {
};

struct AudioDevice : public SoundDeviceBase {
};

struct MIDIDevice : public SoundDeviceBase {
};

struct SoundDevice {
    const PaDeviceInfo *info;
    const int index;

    SoundDevice(int id) : index(id), info(Pa_GetDeviceInfo(id)) { ; }
};

struct SoundDeviceFactory {
    std::list<std::shared_ptr<SoundDevice>> devices;
    std::mutex mutex;

    SoundDeviceFactory();
    ~SoundDeviceFactory() noexcept(false);

    decltype(devices) getDevices() { return devices; }
};

std::ostream &operator<<(std::ostream &os, std::shared_ptr<SoundDevice> dev);
std::ostream &operator<<(std::ostream &os, const PaDeviceInfo *info);

extern SoundDeviceFactory factory;
