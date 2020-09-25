//
//  MTCEmitter.cpp
//  XDAT
//
//  Created by Arvid Rosén on 2012-02-09.
//  Copyright (c) 2012 Arvid Rosén. All rights reserved.
//

#include <iostream>
#include "MTCEmitter.h"
#include "SMPTE.h"

AbstractFifo SMPTE::fifo(1024);
double SMPTE::positionInSeconds(0.0);
MidiMessage::SmpteTimecodeType SMPTE::type(MidiMessage::fps30);
double SMPTE::fps(30);
int SMPTE::buffer[] = {};

MTCEmitter::MTCEmitter()
    :Thread("SMPTE Updater")
{
    smpteTimeCodeType = MidiMessage::fps30;
    framesPerSecond = 30;
    interval = (1000.0/(4.0*framesPerSecond));
    delta = 0.0;
    
    currentQuarterFrame = 0;
    backwards = false;
    
    SMPTE::init(smpteTimeCodeType);
}

MTCEmitter::~MTCEmitter()
{
    stopThread(2000);
}

bool MTCEmitter::open(const int devNum=0)
{
    midiOutput = MidiOutput::openDevice(devNum);
    DBG("Opening MTC device: " << MidiOutput::getDevices()[devNum]);
    
    if(!midiOutput)
        return false;
    
    reset();
    
    return true;
}

bool MTCEmitter::start()
{
    startThread(6);
    
    if(!midiOutput)
        return false;
    
    return true;
}

void MTCEmitter::stop()
{
    if(!midiOutput)
        return;
    
    stopThread(500);
}

void MTCEmitter::run()
{
    while (! threadShouldExit())
    {
        // sleep a bit so the threads don't all grind the CPU to a halt..
        const int timeToWait = roundToInt(interval + delta);
        delta = timeToWait - interval;
        wait (timeToWait);
        
        midiOutput->sendMessageNow(nextMidiMsg);

        postDisplayUpdate();
        prepareNextMessage(false);
    }
}

// Post message to display component, to update its SMPTE display.
void MTCEmitter::postDisplayUpdate()
{
    int hour,min,sec,frame;
    
    SMPTE::getSMPTEValue(hour, min, sec, frame);
    
    SMPTEMessage *msg = new SMPTEMessage();
    msg->intParameter1 = hour;
    msg->intParameter2 = (min<<8) + sec;
    msg->intParameter3 = frame;
    displayOwner->postMessage(msg);
}

// Send full MTC goto message
void MTCEmitter::sendGoto()
{
    prepareNextMessage(true);
    
    midiOutput->sendMessageNow(nextMidiMsg);
        
    postDisplayUpdate();

}

// Re-init SMPTE counter and update possition
void MTCEmitter::reset()
{
    SMPTE::init(smpteTimeCodeType);

    // Send full message
    sendGoto();
}

void MTCEmitter::prepareNextMessage(const bool fullFrame=false)
{
    int val;
    int hour,min,sec,frame;
    
    if(fullFrame) {
        SMPTE::update(samplerate);
        SMPTE::getSMPTEValue(hour, min, sec, frame);
        nextMidiMsg = MidiMessage::fullFrame(hour, min, sec, frame, smpteTimeCodeType);
    } else {
        SMPTE::getSMPTEValue(hour, min, sec, frame);
        
        switch(currentQuarterFrame) {
                case 0: val = frame & 0x0F; break;
                case 1: val = (frame & 0x10) >> 4; break;
                case 2: val = sec & 0x0F; break;
                case 3: val = (sec & 0x30) >> 4; break;
                case 4: val = min & 0x0F; break;
                case 5: val = (min & 0x30) >> 4; break;
                case 6: val = hour & 0x0F; break;
                case 7: val = ((hour & 0x10) >> 4) | ((SMPTE::type & 0x03) << 1); break;
        }
        nextMidiMsg = MidiMessage::quarterFrame(currentQuarterFrame, val);
            
        if(backwards)
            currentQuarterFrame--;
        else
            currentQuarterFrame++;

        if(currentQuarterFrame < 0) {
            currentQuarterFrame = 7;
        }
    
        if(currentQuarterFrame > 7) {
            currentQuarterFrame = 0;
            SMPTE::update(samplerate);
        }
    }
}

