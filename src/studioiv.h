#ifndef STUDIOIV_H
#define STUDIOIV_H

#include "cdp1802.h"
#include "pixie.h"

class StudioIV : public Cdp1802, public Pixie
{
public:
	StudioIV(const wxString& title, const wxPoint& pos, const wxSize& size, double zoom, double zoomfactor, int computerType);
	~StudioIV();

	void configureComputer();
	void reDefineKeysA(int *, int *);
	void reDefineKeysB(int *, int *);
	void keyDown(int keycode);
	void keyUp(int keycode);

	Byte ef(int flag);
	Byte ef3();
	Byte ef4();
	Byte in(Byte port, Word address);
	void out(Byte port, Word address, Byte value);
	void outStudioIV(Byte value);
	void cycle(int type);

	void startComputer();
	void writeMemDataType(Word address, Byte type);
	Byte readMemDataType(Word address);
	Byte readMem(Word address);
	void writeMem(Word address, Byte value, bool writeRom);
    Byte readMemDebug(Word address);
    void writeMemDebug(Word address, Byte value, bool writeRom);
    Byte read1864ColorDirect(Word address);
    void write1864ColorDirect(Word address, Byte value);
	void cpuInstruction();
    void resetPressed();
	void onReset();

private:
	int victoryKeyPort_;
	Byte victoryKeyState_[2][10];

	int keyDefA1_[16];
	int keyDefB1_[16];
	int keyDefA2_[16];
	int keyDefB2_[16];
    int inKey1_;
    int inKey2_;

	bool simDefA2_;
	bool simDefB2_;

	int keyDefGameHexA_[5];
	int keyDefGameHexB_[5];

	KeyDef keyDefinition[512];
    
    int buildInGame_;
    bool gameAuto_;
};

#endif  // STUDIOIV_H
