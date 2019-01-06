#include "rtmidiBackend.h"

namespace fmapp {

	RtMidiBackend::RtMidiBackend()
	{
		auto defaultOutput = std::make_unique<RtMidiOut>();
		auto nOutputs = defaultOutput->getPortCount();
		midiOuts.resize(nOutputs);
		midiOuts[0].swap(defaultOutput);
	}

	RtMidiOut * RtMidiBackend::getRtOutputReadyForSend(int idx)
	{
		if ((size_t)idx > midiOuts.size()) {
			throw std::runtime_error("RtMidiBackend outputs: index out of bounds " + std::to_string(idx));
		}
		if (midiOuts.at(idx).get() == nullptr) {
			auto newOut = std::make_unique<RtMidiOut>();
			newOut->openPort(idx);
			midiOuts.at(idx).swap(newOut);
		}
		if (idx == 0) { // default port exists already but is initial closed
			if (!defaultRtOutput()->isPortOpen()) {
				defaultRtOutput()->openPort(0);
			}
		}
		return midiOuts.at(idx).get();
	}

	RtMidiBackend::~RtMidiBackend() = default;

	RtMidiBackend::Outputs RtMidiBackend::getOutputs() const
	{
		auto nOutputs = defaultRtOutput()->getPortCount();
		Outputs result(nOutputs);
		for (size_t idx = 0; idx < nOutputs; ++idx) {
			auto &output = result[idx];
			output.portid = idx;
			output.id = std::to_string(idx);
			output.name = defaultRtOutput()->getPortName(idx);
		}
		return result;
	}

	bool RtMidiBackend::setOutput(const Output &output)
	{
		this->output_ = output;
		return true;
	}

	void RtMidiBackend::panic() 
	{
		fm::Byte message[3] = {0}; 
		for(auto &rtOut : midiOuts) {
			if (rtOut.get() == nullptr || !rtOut->isPortOpen()) {
				continue;
			}
			for (fm::midi::Channel channel=0; channel <= fm::midi::MaxChannel; ++channel) {
				message[0] = static_cast<fm::Byte>((0x8 << 4) | channel);
				for (fm::midi::Pitch pitch=0; pitch <= fm::midi::MaxPitch; ++pitch)  {
					message[1] = pitch;
					message[2] = pitch;
					rtOut->sendMessage(&message[0], 3);
				}
			}
		}
	}

	void RtMidiBackend::closeAllPorts()
	{
		for(auto &rtOut : midiOuts) {
			if (rtOut.get() == nullptr) {
				continue;
			}
			rtOut->closePort();
		}
	}



	void RtMidiBackend::send(const fm::midi::Event &ev, const Output *output)
	{
		if (output == nullptr) {
			output = &output_;
		}
		if (output->portid == UNKNOWN_PORT) {
			return;
		}
		
		auto port = getRtOutputReadyForSend(output->portid);
		const unsigned int StaticBufferSize = 255;
		fm::Byte buffer[StaticBufferSize];
		std::vector<fm::Byte> fallback;
		auto eventSize = ev.payloadSize();
		fm::Byte *bff = &buffer[0];
		if (eventSize > StaticBufferSize) {
			fallback.resize(eventSize, 0);
			bff = fallback.data();
		}
		
		ev.writePayload(bff, eventSize);
		port->sendMessage(bff, eventSize);
	}
}
