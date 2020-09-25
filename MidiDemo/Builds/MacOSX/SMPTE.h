//
//  SMPTE.h
//  XDAT
//
//  Created by Arvid Rosén on 2012-02-28.
//  Copyright (c) 2012 Arvid Rosén. All rights reserved.
//

#ifndef XDAT_SMPTE_h
#define XDAT_SMPTE_h

#include "../JuceLibraryCode/JuceHeader.h"

class SMPTEMessage: public Message
{
public:
    int intParameter1;
    int intParameter2;
    int intParameter3;
};

class SMPTE
{
public:
    static MidiMessage::SmpteTimecodeType type;
    
    void static init(const MidiMessage::SmpteTimecodeType type_)
    {
        positionInSeconds = 0.0;
        fifo.reset();
        
        if(type==MidiMessage::fps30)
            fps = 30;
        if(type==MidiMessage::fps25)
            fps = 25;
        if(type==MidiMessage::fps24)
            fps = 24;
    }
    
    // Add samples that are not yet used to update the current time
    static void addSamples (const int samples)
    {
        int start1, size1, start2, size2;
        fifo.prepareToWrite (1, start1, size1, start2, size2);
        
        if (size1 > 0)
            buffer[start1]=samples;
        
        if (size2 > 0)
            buffer[start2]=samples;
        
        fifo.finishedWrite (size1 + size2);
    }
    
    // Update time using the added samples
    static void update (const double samplerate)
    {
        if(samplerate <= 0)
            return;
        
        int start1, size1, start2, size2;
        int newSamples=0;
        
        fifo.prepareToRead (fifo.getNumReady(), start1, size1, start2, size2);
        
        for(int i=0; i<size1; i++)
            newSamples += buffer[start1+i];
        
        for(int i=0; i<size2; i++)
            newSamples += buffer[start2+i];
        
        fifo.finishedRead (size1 + size2);
        
        positionInSeconds += double(newSamples)/samplerate;
    }
    
    static void getSMPTEValue(int &hour, int &min, int &sec, int &frames)
    {
        double tmp = positionInSeconds;
        int q;
        
        q = floor(tmp/(3600.0));
        tmp -= q*3600.0;
        hour = q;
        
        q = floor(tmp/(60.0));
        tmp -= q*60.0;
        min  = q;
        
        q = floor(tmp);
        tmp -= q;
        sec  = q;
        
        frames = floor(tmp*fps);
    }
    
    static void setSMPTEValue(const int hour, const int min, const int sec, const int frames)
    {
        positionInSeconds = hour*3600 + min*60 + sec + double(frames)/fps;
    }
    
    static int getNumReady()
    {
        return fifo.getNumReady();
    }
    
    static double getPosition()
    {
        return positionInSeconds;
    }
private:
    static double positionInSeconds;
    static double fps;
    static AbstractFifo fifo;
    static int buffer[1024];
};


#endif
