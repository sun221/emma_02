#ifndef SUPER_H
#define SUPER_H

#include "elf.h"
#include "tms9918.h"
#include "pixie.h"
#include "mc6847.h"
#include "mc6845.h"
#include "i8275.h"
#include "ledmodule.h"
#include "printer.h"
#include "elfconfiguration.h"

class SuperScreen : public Panel
{
public:
	SuperScreen(wxWindow *parent, const wxSize& size);
	~SuperScreen();

	void init();
	void onPaint(wxPaintEvent&event);
    void onMousePress(wxMouseEvent& event);
    void onMouseRelease(wxMouseEvent& event);
	void releaseButtonOnScreen(HexButton* buttonPointer);

private:
};

class Super : public wxFrame, public MainElf
{
public:
	Super(const wxString& title, const wxPoint& pos, const wxSize& size, double clock, ElfConfiguration conf);
	~Super();

	void onClose(wxCloseEvent&WXUNUSED(event));
	void charEvent(int keycode);
	bool keyDownPressed(int keycode);
	bool keyUpReleased(int keycode);
	void onButtonRelease(wxCommandEvent& event);
	void onButtonPress(wxCommandEvent& event);

	void onRun();
	void onInButtonPress();
	void onInButtonRelease();
	void onHexKeyDown(int keycode);
	void onHexKeyUp(int keycode);

    void configureComputer();
    void setPrinterEf();
    void reLoadKeyDefinition(wxString fileName);
	void reDefineKeysA(int *, int *);
	void initComputer();
	Byte ef(int flag);
	Byte ef4();
	Byte in(Byte port, Word address);
	Byte getData();
	void out(Byte port, Word address, Byte value);
	void showData(Byte value);
	void cycle(int type);
	void cycleA();
	void cycleB();

	void autoBoot();
	void switchQ(int value);
	int getMpButtonState();

	void onRunButton(wxCommandEvent&WXUNUSED(event));
	void onRunButton();
	void onPause(wxCommandEvent&WXUNUSED(event));
	void onPause();
	void onMpButton(wxCommandEvent&WXUNUSED(event));
	void onMpButton();
	void onMonitor(wxCommandEvent&WXUNUSED(event));
	void onMonitor();
	void onLoadButton(wxCommandEvent&WXUNUSED(event));
	void onLoadButton();
	void onSingleStep(wxCommandEvent&WXUNUSED(event));
	void onSingleStep();
	void onResetButton(wxCommandEvent&WXUNUSED(event));
    void onResetButton();
	void onNumberKeyDown(wxCommandEvent& event);
	void onNumberKeyDown(int i);
	void onNumberKeyUp(wxCommandEvent& event);

	void startComputer();
	void writeMemDataType(Word address, Byte type);
	Byte readMemDataType(Word address);
	Byte readMem(Word addr);
	void writeMem(Word addr, Byte value, bool writeRom);
	void cpuInstruction();
	void configureElfExtensions();
	void moveWindows();
	void updateTitle(wxString Title);
	void setForceUpperCase(bool status);
	void onReset();
	void sleepComputer(long ms);

	Byte getTmsMemory(int address) {return tmsPointer->getTmsMemory(address);};
	void setTmsMemory(int address, Byte value) {tmsPointer->setTmsMemory(address, value);};
	Byte read8275CharRom(Word addr);
	void write8275CharRom(Word addr, Byte value);
    Byte read8275VideoRam(Word addr);
    void write8275VideoRam(Word addr, Byte value);
	Byte read6845CharRom(Word addr);
	void write6845CharRom(Word addr, Byte value);
	Byte read6847CharRom(Word addr);
	void write6847CharRom(Word addr, Byte value);
	int readDirect6847(Word addr);
	Word get6847RamMask();
	void writeDirect6847(Word addr, int value);
	void ledTimeout();
	void setLedMs(long ms);
	Byte getKey(Byte vtOut);
	void activateMainWindow();
	void releaseButtonOnScreen(HexButton* buttonPointer, int buttonType);

private:
	class SuperScreen *superScreenPointer;
	Tms9918 *tmsPointer;
	Pixie *pixiePointer;
	MC6845 *mc6845Pointer;
	mc6847 *mc6847Pointer;
	i8275 *i8275Pointer;

	Byte switches_;
	int mpButtonState_;
	Byte lastMode_;
	char state_;
	Byte ef3State_;
	Byte ef4State_;

	char singleStep_;
	bool monitor_;
	Word lastAddress_;

    int keyDefA1_[16];
    int keyDefB1_[16];
    int keyDefA2_[16];
    int keyDefB2_[16];
    
    bool simDefA2_;
    bool simDefB2_;
    
	int keyDefGameHexA_[5];
	int keyDefGameHexB_[5];

	double elfClockSpeed_;

	DECLARE_EVENT_TABLE()
};

#endif  // SUPER_H