

#include "device.h"

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    FLAGS_stderrthreshold = 0;

    std::shared_ptr<SoundDevice> inputDevice;
    std::shared_ptr<SoundDevice> outputDevice;

    for (auto device : factory.getDevices()) {

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
    LOG(INFO) << "There are " << nPorts << " MIDI input sources available";
    std::string portName;
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiin->getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        LOG(INFO) << "  Input Port #" << i + 1 << ": " << portName;
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
    LOG(INFO) << "There are " << nPorts << " MIDI output ports available.";
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiout->getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }
        LOG(INFO) << "  Output Port #" << i + 1 << ": " << portName;
    }

cleanup:
    delete midiin;
    delete midiout;

    return 0;
}