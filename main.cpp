

#include <memory>

#include <cstdlib>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <portaudio.h>
#include <rtmidi/RtMidi.h>
#include <yaml-cpp/yaml.h>

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

    YAML::Node node = YAML::Load("[1, 2, 3]");
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

    RtMidiIn *midiin = 0;
    RtMidiOut *midiout = 0;
    // RtMidiIn constructor
    try {
        midiin = new RtMidiIn();
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
    // Check inputs.
    unsigned int nPorts = midiin->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiin->getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        std::cout << "  Input Port #" << i + 1 << ": " << portName << '\n';
    }
    // RtMidiOut constructor
    try {
        midiout = new RtMidiOut();
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
    // Check outputs.
    nPorts = midiout->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiout->getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        std::cout << "  Output Port #" << i + 1 << ": " << portName << '\n';
    }
    std::cout << '\n';
    // Clean up

cleanup:
    delete midiin;
    delete midiout;

    err = Pa_Terminate();
    if (err != paNoError) {
        LOG(ERROR) << "Pa_Terminate returned " << Pa_GetErrorText(err);
        return -1;
    }
    return 0;
}