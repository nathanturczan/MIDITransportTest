//
//  MTCEmitter.h
//  XDAT
//
//  Created by Arvid Rosén on 2012-02-09.
//  Copyright (c) 2012 Arvid Rosén. All rights reserved.
//

#ifndef XDAT_MTCEmitter_h
#define XDAT_MTCEmitter_h

#include "../JuceLibraryCode/JuceHeader.h"

class MTCEmitter: public Thread
{
public:
    MTCEmitter();
    ~MTCEmitter();
    
    bool start();
    void stop();
    bool open(const int devNum);
    void reset();
    void sendGoto();
    
    void setSamplerate(const double samplerate_) { samplerate = samplerate_; }
    
    void prepareNextMessage(const bool fullFrame);
    void run();

    void postDisplayUpdate();
    MessageListener *displayOwner;
    
private:
    MidiMessage nextMidiMsg;
    std::unique_ptr<MidiOutput> midiOutput;

    int currentQuarterFrame;
    float interval;
    float delta;
    bool backwards;
    double samplerate;
    double framesPerSecond;
    MidiMessage::SmpteTimecodeType smpteTimeCodeType;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MTCEmitter);
    
};


#endif
