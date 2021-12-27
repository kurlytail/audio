

#include <memory>

#include <CoreMIDI/MIDIServices.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <include/yaml.h>
#include <portaudio.h>

struct SoundDevice {
    const PaDeviceInfo *info;
    const int index;

    SoundDevice(int id) : index(id), info(Pa_GetDeviceInfo(id)) { ; }
};

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

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    auto node = YAML::Load("[1, 2, 3]");
    assert(node.Type() == YAML::NodeType::Sequence);
    assert(node.IsSequence()); // a shortcut!

    FLAGS_stderrthreshold = 0;

    LOG(INFO) << Pa_GetVersionText();

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        LOG(ERROR) << "Pa_Initialize returned " << Pa_GetErrorText(err);
        return -1;
    }

    auto numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        LOG(ERROR) << "Pa_CountDevices returned "
                   << Pa_GetErrorText(numDevices);
        return -1;
    }

    std::shared_ptr<SoundDevice> inputDevice;
    std::shared_ptr<SoundDevice> outputDevice;

    for (auto i = 0; i < numDevices; i++) {
        auto device = std::make_shared<SoundDevice>(i);
        LOG(INFO) << "Detected device " << device;
        LOG(INFO) << "     " << device->info;

        if (device->info->maxInputChannels) {
            if (!inputDevice)
                inputDevice = device;
        }

        if (device->info->maxOutputChannels) {
            if (!outputDevice)
                outputDevice = device;
        }
    }

    if (!inputDevice) {
        LOG(ERROR) << "No suitable input device found";
        return -1;
    }
    if (!outputDevice) {
        LOG(ERROR) << "No suitable output device found";
        return -1;
    }

    LOG(INFO) << "Using input device " << inputDevice;
    LOG(INFO) << "Using output device " << outputDevice;

    int midiDeviceCount = MIDIGetNumberOfDevices();
    LOG(INFO) << "Found " << midiDeviceCount << " MIDI devices";
    for (auto midiDevice = 0; midiDevice < midiDeviceCount; ++midiDevice) {
        CFStringRef pname, pmanuf, pmodel;
        char name[64], manuf[64], model[64];

        MIDIDeviceRef dev = MIDIGetDevice(midiDevice);

        MIDIObjectGetStringProperty(dev, kMIDIPropertyName, &pname);
        MIDIObjectGetStringProperty(dev, kMIDIPropertyManufacturer, &pmanuf);
        MIDIObjectGetStringProperty(dev, kMIDIPropertyModel, &pmodel);

        CFStringGetCString(pname, name, sizeof(name), 0);
        CFStringGetCString(pmanuf, manuf, sizeof(manuf), 0);
        CFStringGetCString(pmodel, model, sizeof(model), 0);
        CFRelease(pname);
        CFRelease(pmanuf);
        CFRelease(pmodel);

        LOG(INFO) << "MIDI device " << name << " - " << manuf << " - " << model;
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        LOG(ERROR) << "Pa_Terminate returned " << Pa_GetErrorText(err);
        return -1;
    }
    return 0;
}