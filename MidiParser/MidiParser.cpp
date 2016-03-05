// MidiParser.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

//#include "stdafx.h"
#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

void printAllMessagesOfEachTrack(MidiFile& midifile);
void printAllMessagesOfEachTrack(MidiFile& midifile, Options options);
void pritConcurrentMessageOfAllTrack(MidiFile& midifile);
string hexify(int i);


int main(int argc, char* argv[])
{
	Options options;
	options.process(argc, argv);
	MidiFile midifile;
	
	if (options.getArgCount() > 0) 
	{ 
		midifile.read(options.getArg(1)); 
	} 
	else 
	{ 
		midifile.read(cin);
	}

	//printAllMessagesOfEachTrack(midifile);
	//pritConcurrentMessageOfAllTrack(midifile);
	printAllMessagesOfEachTrack(midifile, options);

	return 0;
}

void printAllMessagesOfEachTrack(MidiFile& midifile)
{
	int tempo = midifile.getTempo();
	int tpqs = midifile.getTicksPerQuarterNote();
	int usecPerTick = 60 * 1000000 / tpqs / tempo;

	int tracks = midifile.getTrackCount();
	cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
	cout << "Tempo: " << tempo << " BPM";

	if (tracks > 1) 
	{ 
		cout << "TRACKS: " << tracks << endl; 
	} 

	for (int track=0; track < tracks; track++)
	{ 
		if (tracks > 1) 
		{ 
			cout << "\nTrack " << track << endl;
		} 
		for (int event=0; event < midifile[track].size(); event++)
		{ 
			cout << dec << midifile[track][event].tick; 
			cout << '\t' << hex; 
			for (int i=0; i<midifile[track][event].size(); i++) 
			{ 
				cout << (int)midifile[track][event][i] << ' '; 
			} 
			cout << endl; 
		} 
	}
}

string hexify(int i)
{
	std::stringbuf buf;
	std::ostream os(&buf);

	os << std::setfill('0') << std::setw(2) << std::hex << i;

	return buf.str().c_str();
}

void printAllMessagesOfEachTrack(MidiFile& midifile, Options options)
{
	string fn = options.getArg(1) + "_EachTrack.txt";
	ofstream ostr(fn);		
	
	int tempo = midifile.getTempo();
	int tpqs = midifile.getTicksPerQuarterNote();
	int usecPerTick = 60 * 1000000 / tpqs / tempo;

	string tmpStr = "TPQ: " + to_string((long long)tpqs) + ", usecPerTick = " + to_string((long long)usecPerTick) + "\n";
	ostr << tmpStr;

	tmpStr = "Tempo: " + to_string((long long)midifile.getTempo()) + "\n";
	ostr << tmpStr;

	int tracks = midifile.getTrackCount();
	if (tracks > 1) 
	{ 
		tmpStr = "TRACKS: " + to_string((long long) tracks)  + "\n";
		ostr << tmpStr;
	} 

	for (int track=0; track < tracks; track++)
	{
		if (tracks > 1) 
		{ 
			tmpStr = "\nTrack " + to_string((long long) track)  + "\n";
			ostr << tmpStr;
		} 
		for (int event=0; event < midifile[track].size(); event++)
		{ 
			tmpStr = to_string((long long)midifile[track][event].tick) + "\t"; 
			for (int i=0; i<midifile[track][event].size(); i++) 
			{ 
				tmpStr += hexify(midifile[track][event][i]) + ' '; 
			} 
			tmpStr += "\n";
			ostr << tmpStr;
		} 
	}

	ostr.close();
}

void pritConcurrentMessageOfAllTrack(MidiFile& midifile)
{
	cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
	cout << "TRACKS: " << midifile.getTrackCount() << endl;
	midifile.joinTracks(); // midifile.getTrackCount() will now return "1", but original // track assignments can be seen in .track field of MidiEvent.

	cout << "TICK DELTA TRACK MIDI MESSAGE\n";
	cout << "____________________________________\n";
	MidiEvent* mev;
	int deltatick;
	for (int event=0; event < midifile[0].size(); event++)
	{ 
		mev = &midifile[0][event]; 
		if (event == 0)
		{ 
			deltatick = mev->tick;
		}
		else
		{
			deltatick = mev->tick - midifile[0][event-1].tick;
		} 
		cout << dec << mev->tick;
		cout << '\t' << deltatick;
		cout << '\t' << mev->track;
		cout << '\t' << hex;
		for (int i=0; i < mev->size(); i++)
		{ 
			cout << (int)(*mev)[i] << ' ';
		} 
		cout << endl; 
	}
}