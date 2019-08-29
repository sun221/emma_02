/*
 *******************************************************************
 *** This software is copyright 2008 by Marcel van Tongeren      ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
 */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

#if !wxUSE_COMBOCTRL
#error "Please set wxUSE_COMBOCTRL to 1 and rebuild the library."
#endif

#if defined(__linux__)
#include "app_icon.xpm"
#endif

#include "main.h"

#include "cdp18s600.h"
#include "cdp18s640.h"
#include "pio.h"
#include "upd765.h"

Word romSize[]=
{
    0x7ff, 0xfff, 0x1fff
};

BEGIN_EVENT_TABLE(Cdp18s600, wxFrame)
    EVT_CLOSE (Cdp18s600::onClose)
    EVT_BUTTON(1, Cdp18s600::onRunButton)
    EVT_BUTTON(2, Cdp18s600::onRunPButton)
    EVT_BUTTON(3, Cdp18s600::onResetButton)
END_EVENT_TABLE()

Cdp18s600::Cdp18s600(const wxString& title, const wxPoint& pos, const wxSize& size, double clock, ElfConfiguration conf)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    Cdp18s600Configuration = conf;
    
    Cdp18s600ClockSpeed_ = clock;
    lastAddress_ = 0;
    
#ifndef __WXMAC__
    SetIcon(wxICON(app_icon));
#endif
    
    this->SetClientSize(size);
    
    cycleSize_ = (int)(1000 / ((1/1.75) * 8)); // ~1000 Hz on 1.75 CPU
    cycleValue_ = cycleSize_;

    cdp18s640ScreenPointer = new Cdp18s640Screen(this, size);
    cdp18s640ScreenPointer->init();
    
#if defined(__WXMAC__)
    pioFramePointer = new PioFrame("PIO", p_Main->getSecondFramePos(CDP18S600), wxSize(310, 202));
#else
    pioFramePointer = new PioFrame("PIO", p_Main->getSecondFramePos(CDP18S600), wxSize(320, 210));
#endif
}

Cdp18s600::~Cdp18s600()
{
    if (Cdp18s600Configuration.vtType != VTNONE)
    {
        p_Main->setVtPos(CDP18S600, vtPointer->GetPosition());
        vtPointer->Destroy();
    }
    if (Cdp18s600Configuration.vtExternal)
        delete p_Serial;
    p_Main->setMainPos(CDP18S600, GetPosition());
    delete cdp18s640ScreenPointer;

	if (Cdp18s600Configuration.usePio)
		p_Main->setSecondFramePos(CDP18S600, pioFramePointer->GetPosition());
    pioFramePointer->Destroy();
}

void Cdp18s600::onClose(wxCloseEvent&WXUNUSED(event) )
{
    p_Main->stopComputer();
}

void Cdp18s600::configureComputer()
{
    setCycleType(COMPUTERCYCLE, LEDCYCLE);

    inType_[1] = MS2000IOGROUP;
    inType_[2] = MS2000IO2;
    inType_[3] = MS2000IO3;
    inType_[4] = MS2000IO4;
    inType_[5] = MS2000IO5;
    inType_[6] = MS2000IO6;
    inType_[7] = MS2000IO7;
    outType_[1] = MS2000IOGROUP;
    outType_[2] = MS2000IO2;
    outType_[3] = MS2000IO3;
    outType_[4] = MS2000IO4;
    outType_[5] = MS2000IO5;
    outType_[6] = MS2000IO6;
    outType_[7] = MS2000IO7;
    
    efType_[1] = CDP18SEF1;
    efType_[2] = CDP18SEF2;

    efState_[1] = 1;
    efState_[2] = 2;

    p_Main->message("Configuring CDP18S600");
    p_Main->message("	Output 1: set I/O group, input 1: read I/O group");
    p_Main->message("	I/O group 1: video terminal");
    
    p_Main->message("");
    
    if (Cdp18s600Configuration.vtType != VTNONE)
    {
        double zoom = p_Main->getZoomVt();
        if (Cdp18s600Configuration.vtType == VT52)
            vtPointer = new Vt100("CDP18S600 - VT 52", p_Main->getVtPos(CDP18S600), wxSize(640*zoom, 400*zoom), zoom, CDP18S600, Cdp18s600ClockSpeed_, Cdp18s600Configuration);
        else
            vtPointer = new Vt100("CDP18S600 - VT 100", p_Main->getVtPos(CDP18S600), wxSize(640*zoom, 400*zoom), zoom, CDP18S600, Cdp18s600ClockSpeed_, Cdp18s600Configuration);
        p_Vt100 = vtPointer;
        vtPointer->configureMs2000(Cdp18s600Configuration.baudR, Cdp18s600Configuration.baudT);
    }
    
    if (Cdp18s600Configuration.vtExternal)
    {
        p_Serial = new Serial(CDP18S600, Cdp18s600ClockSpeed_, Cdp18s600Configuration);
        p_Serial->configureMs2000(Cdp18s600Configuration.baudR, Cdp18s600Configuration.baudT);
    }
    
    p_Main->message("Configuring PIO on group 8");
    p_Main->message("	Output 2: write to port B, output 3: write to port A");
    p_Main->message("	Input 2: read port B, input 3: read port A");
    p_Main->message("	Output 6: write control register, input 6: read status");
    p_Main->message("   EF 1: ARDY, EF 2: BRDY");

    resetCpu();
}

void Cdp18s600::initComputer()
{
    Show(p_Main->getUseCdp18s600ControlWindows());
    addressLatch_ = p_Main->getBootAddress("CDP18S600", computerType_);
    setCpuMode(RESET); // CLEAR = 0, WAIT = 1, CLEAR LED ON, WAIT LED OFF, RUN LED OFF

    ioGroup_ = 0;
    
    mcdsRunState_ = RESETSTATE;
}

void Cdp18s600::onRunButton(wxCommandEvent&WXUNUSED(event))
{
    onRunButton();
}

void Cdp18s600::onRunPButton(wxCommandEvent&WXUNUSED(event))
{
    onRunPButton();
}

void Cdp18s600::onResetButton(wxCommandEvent&WXUNUSED(event))
{
    onReset();
}

void Cdp18s600::onRunButton()
{
    if (!singleStateStep_)
        addressLatch_ = 0x8000;
    if (idle_)
        dmaOut();
    onRun();
}

void Cdp18s600::onRunPButton()
{
    addressLatch_ = 0x0000;
    if (idle_)
        dmaOut();
    onRun();
}

void Cdp18s600::onRun()
{
//    if (!singleStateStep_)
//        resetCpu();
    
    if (cpuMode_ != RUN)
        resetEffectiveClock();
    
    setCpuMode(RUN); // CLEAR = 1, WAIT = 1, CLEAR LED OFF, WAIT LED OFF, RUN LED ON

    p_Main->eventUpdateTitle();
}

void Cdp18s600::autoBoot()
{
    setCpuMode(RUN); // CLEAR = 1, WAIT = 1, CLEAR LED OFF, WAIT LED OFF, RUN LED ON
}

void Cdp18s600::setAddressLatch(Word bootAddress)
{
    addressLatch_ = bootAddress;
}

void Cdp18s600::onSingleStep()
{
    singleStateStep_ = !singleStateStep_;
    if (singleStateStep_)
    {
        setMsValue_ = (int) p_Main->getLedTimeMs(CDP18S600);
        setLedMs(0);
    }
    else
        setLedMs(setMsValue_);
}

Byte Cdp18s600::ef(int flag)
{
    switch(efType_[flag])
    {
        case 0:
            return 1;
        break;
            
        case VT100EF:       // EF4
            return vtPointer->ef();
        break;
            
        case VTSERIALEF:
            return p_Serial->ef();
        break;
            
        case CDP18SEF1:
            switch (ioGroup_)
            {
                case IO_GRP_PIO:
                    return efState_[1];
                break;
                    
                default:
                    return 1;
                break;
            }
        break;
            
        case CDP18SEF2:
            switch (ioGroup_)
            {
                case IO_GRP_PIO:
                    return efState_[2];
                break;

                    
                default:
                    return 1;
                break;
            }
        break;

        default:
            return 1;
    }
}

void Cdp18s600::setEfState(int number, Byte value)
{
    efState_[number] = value;
}

Byte Cdp18s600::in(Byte port, Word WXUNUSED(address))
{
    Byte ret;
    ret = 0;
    
    switch(inType_[port])
    {
        case 0:
            ret = 255;
        break;
            
        case MS2000IOGROUP:
        break;
            
        case MS2000IO2:
            switch (ioGroup_)
            {
                case IO_GRP1_UART:
                    if (Cdp18s600Configuration.uartGroup == 0)
                    {
                        if (p_Vt100 != NULL)
                            return p_Vt100->uartIn();
                        if (p_Serial != NULL)
                            return p_Serial->uartIn();
                    }
                break;
                
                case IO_GRP2_UART:
                    if (Cdp18s600Configuration.uartGroup == 1)
                    {
                        if (p_Vt100 != NULL)
                            return p_Vt100->uartIn();
                        if (p_Serial != NULL)
                            return p_Serial->uartIn();
                    }
                break;
                    
                case IO_GRP_PIO:
                    return pioFramePointer->readPortB();
                break;
                    
                default:
                    ret = 255;
            }
        break;
            
        case MS2000IO3:
            switch (ioGroup_)
            {
                case IO_GRP1_UART:
                    if (Cdp18s600Configuration.uartGroup == 0)
                    {
                        if (p_Vt100 != NULL)
                            return p_Vt100->uartStatus();
                        if (p_Serial != NULL)
                            return p_Serial->uartStatus();
                    }
                break;
                
                case IO_GRP2_UART:
                    if (Cdp18s600Configuration.uartGroup == 1)
                    {
                        if (p_Vt100 != NULL)
                            return p_Vt100->uartStatus();
                        if (p_Serial != NULL)
                            return p_Serial->uartStatus();
                    }
                break;
                    
                case IO_GRP_PIO:
                    return pioFramePointer->readPortA();
                    break;

                default:
                    ret = 255;
            }
        break;
            
        case MS2000IO4:
        break;
            
        case MS2000IO5:
        break;
            
        case MS2000IO6:
            switch (ioGroup_)
            {
                case IO_GRP_PIO:
                    return pioFramePointer->readStatusRegister();
                break;
            }
        break;
            
        case MS2000IO7:
        break;
            
        default:
            ret = 255;
    }
    inValues_[port] = ret;
    return ret;
}

void Cdp18s600::out(Byte port, Word WXUNUSED(address), Byte value)
{
    outValues_[port] = value;
    
    switch(outType_[port])
    {
        case 0:
            return;
        break;
            
        case MS2000IOGROUP:
            ioGroup_ = value;
        break;
            
        case MS2000IO2:
            switch (ioGroup_)
            {
                case IO_GRP1_UART:
                    if (Cdp18s600Configuration.uartGroup == 0)
                    {
                        if (p_Vt100 != NULL)
                            p_Vt100->uartOut(value);
                        if (p_Serial != NULL)
                            p_Serial->uartOut(value);
                    }
                break;

                case IO_GRP2_UART:
                    if (Cdp18s600Configuration.uartGroup == 1)
                    {
                        if (p_Vt100 != NULL)
                            p_Vt100->uartOut(value);
                        if (p_Serial != NULL)
                            p_Serial->uartOut(value);
                    }
                break;
                    
                case IO_GRP_PIO:
                    pioFramePointer->writePortB(value);
                break;
            }
        break;
            
        case MS2000IO3:
            switch (ioGroup_)
            {
                case IO_GRP1_UART:
                    if (Cdp18s600Configuration.uartGroup == 0)
                    {
                        if (p_Vt100 != NULL)
                            p_Vt100->uartControl(value);
                        if (p_Serial != NULL)
                            p_Serial->uartControl(value);
                    }
                break;
                    
                case IO_GRP2_UART:
                    if (Cdp18s600Configuration.uartGroup == 1)
                    {
                        if (p_Vt100 != NULL)
                            p_Vt100->uartControl(value);
                        if (p_Serial != NULL)
                            p_Serial->uartControl(value);
                    }
                break;

                case IO_GRP_PIO:
                    pioFramePointer->writePortA(value);
                break;
            }
        break;
            
        case MS2000IO4:
        break;
            
        case MS2000IO5:
        break;
            
        case MS2000IO6:
            switch (ioGroup_)
            {
                case IO_GRP_PIO:
                    pioFramePointer->writeControlRegister(value);
                break;
            }
        break;
            
        case MS2000IO7:
        break;
    }
}

void Cdp18s600::switchQ(int value)
{
    cdp18s640ScreenPointer->setQLed(value);
    
    if (Cdp18s600Configuration.vtType != VTNONE)
        vtPointer->switchQ(value);
    
    if (Cdp18s600Configuration.vtExternal)
        p_Serial->switchQ(value);
}

void Cdp18s600::showCycleData(Byte val)
{
    cdp18s640ScreenPointer->showDataTil313Italic(val);
}

void Cdp18s600::showAddress(Word val)
{
    cdp18s640ScreenPointer->showAddressTil313Italic(val);
}

void Cdp18s600::showState(int state)
{
    cdp18s640ScreenPointer->setStateLed(SC0LED, state&1);
    cdp18s640ScreenPointer->setStateLed(SC1LED, (state&2)>>1);
}

void Cdp18s600::setCpuMode(int mode)
{
    int clear = (mode>>1)&1;
    int wait = mode&1;
    
    setWait(wait);
    setClear(clear);

    if (clear == 0)
	{
        pioFramePointer->reset();
	}

    cdp18s640ScreenPointer->setRunLed(clear & wait);

    wait ^= 1;
    clear ^= 1;

    cdp18s640ScreenPointer->setStateLed(CLEARLED, clear);
    cdp18s640ScreenPointer->setStateLed(WAITLED, wait);
}

void Cdp18s600::cycle(int type)
{
    switch(cycleType_[type])
    {
        case 0:
            return;
        break;
            
        case VT100CYCLE:
            vtPointer->cycleVt();
        break;
            
        case VTSERIALCYCLE:
            p_Serial->cycleVt();
        break;

        case LEDCYCLE:
            cycleLed();
            pioFramePointer->interruptCycle();
        break;
    }
}

void Cdp18s600::cycleLed()
{
    if (ledCycleValue_ > 0)
    {
        ledCycleValue_ --;
        if (ledCycleValue_ <= 0)
        {
            ledCycleValue_ = ledCycleSize_;
            cdp18s640ScreenPointer->ledTimeout();
			if (Cdp18s600Configuration.usePio)
				pioFramePointer->ledTimeout();
        }
    }
}

void Cdp18s600::startComputer()
{
    resetPressed_ = false;
        
//    p_Main->checkAndReInstallFile(CDP18S600, "ROM U21", U21ROM);
    Word currentRomSize = romSize[p_Main->getMicroChipType(CDP18S600, ONE_SOCKET)];
    int divider = (2 << (p_Main->getMicroChipType(CDP18S600, ONE_SOCKET)))/2;
    int location = p_Main->getMicroChipLocation(CDP18S600, ONE_SOCKET) / divider;
    Word startAddress = location * (currentRomSize+1);
    Word lastAddress = startAddress + currentRomSize;
    
    readMicro(U21ROM, startAddress, lastAddress);

//    p_Main->checkAndReInstallFile(CDP18S600, "ROM U20", U20ROM);
    currentRomSize = romSize[p_Main->getMicroChipType(CDP18S600, FOUR_SOCKET)];
    divider = (2 << (p_Main->getMicroChipType(CDP18S600, FOUR_SOCKET)))/2;
    location = p_Main->getMicroChipLocation(CDP18S600, FOUR_SOCKET) / divider;
    startAddress = location * (4*(currentRomSize+1));
    lastAddress = startAddress + currentRomSize;
    
    readMicro(U20ROM, startAddress, lastAddress);

//    p_Main->checkAndReInstallFile(CDP18S600, "ROM U19", U19ROM);
    startAddress += (currentRomSize+1);
    lastAddress = startAddress + currentRomSize;
    
    readMicro(U19ROM, startAddress, lastAddress);

//    p_Main->checkAndReInstallFile(CDP18S600, "ROM U18", U18ROM);
    startAddress += (currentRomSize+1);
    lastAddress = startAddress + currentRomSize;
    
    readMicro(U18ROM, startAddress, lastAddress);

//    p_Main->checkAndReInstallFile(CDP18S600, "ROM U17", U17ROM);
    startAddress += (currentRomSize+1);
    lastAddress = startAddress + currentRomSize;
    
    readMicro(U17ROM, startAddress, lastAddress);

    if (p_Vt100 != NULL)
        p_Vt100->Show(true);
    
    if (Cdp18s600Configuration.autoBoot)
        autoBoot();

    p_Main->setSwName("");
    p_Main->updateTitle();
    
    cpuCycles_ = 0;
    p_Main->startTime();
    
    int ms = (int) p_Main->getLedTimeMs(CDP18S600);
    cdp18s640ScreenPointer->setLedMs(ms);
    pioFramePointer->setLedMs(ms);
    if (ms == 0)
        ledCycleSize_ = -1;
    else
        ledCycleSize_ = (((Cdp18s600ClockSpeed_ * 1000000) / 8) / 1000) * ms;
    ledCycleValue_ = ledCycleSize_;

	if (Cdp18s600Configuration.usePio)
	    pioFramePointer->Show(true);

    threadPointer->Run();
}

void Cdp18s600::readMicro(int romNumber, Word startAddress, Word lastAddress )
{
    if (p_Main->getMicroChipDisable(CDP18S600, romNumber))
        return;
    
    switch (p_Main->getMicroChipMemory(CDP18S600, romNumber))
    {
        case MICRO_ROM:
            readProgramMicro(p_Main->getRomDir(CDP18S600, romNumber), p_Main->getRomFile(CDP18S600, romNumber), ROM, startAddress, lastAddress+1, NONAME);
            defineMemoryType(startAddress, lastAddress, ROM);
        break;

        case MICRO_RAM:
            readProgramMicro(p_Main->getRomDir(CDP18S600, romNumber), p_Main->getRomFile(CDP18S600, romNumber), RAM, startAddress, lastAddress+1, NONAME);
            defineMemoryType(startAddress, lastAddress, RAM);
            p_Main->assDefault("mycode", startAddress, lastAddress);
        break;

        case MICRO_CPURAM:
            readProgramMicro(p_Main->getRomDir(CDP18S600, romNumber), p_Main->getRomFile(CDP18S600, romNumber), CPURAM, startAddress, lastAddress+1, NONAME);
            defineMemoryType(startAddress, lastAddress, CPURAM);
            p_Main->assDefault("mycode", startAddress, lastAddress);
        break;
    }
}

void Cdp18s600::writeMemDataType(Word address, Byte type)
{
    if (address < 0x8000)
        address = (address | addressLatch_);

    switch (memoryType_[address/256])
    {
        case ROM:
            if (mainMemoryDataType_[address] != type)
            {
                p_Main->updateAssTabCheck(scratchpadRegister_[programCounter_]);
                mainMemoryDataType_[address] = type;
            }
        break;
            
        case RAM:
            if (mainMemoryDataType_[address] != type)
            {
                p_Main->updateAssTabCheck(scratchpadRegister_[programCounter_]);
                mainMemoryDataType_[address] = type;
            }
        break;

        case CPURAM:
            if (cpuRamDataType_[address] != type)
            {
                p_Main->updateAssTabCheck(scratchpadRegister_[programCounter_]);
                cpuRamDataType_[address] = type;
            }
        break;
    }
}

Byte Cdp18s600::readMemDataType(Word address)
{
    if (address < 0x8000)
        address = (address | addressLatch_);

    switch (memoryType_[address/256])
    {
        case RAM:
        case ROM:
            return mainMemoryDataType_[address];
        break;

        case CPURAM:
            return cpuRamDataType_[address];
        break;
    }
    return MEM_TYPE_UNDEFINED;
}

Byte Cdp18s600::readMem(Word address)
{
    address_ = address;
    return readMemDebug(address);
}

Byte Cdp18s600::readMemDebug(Word address)
{
    if ((address & 0x8000) == 0x8000)
        addressLatch_ = 0;
    
    if (address < 0x8000)
        address = (address | addressLatch_);
    
    switch (memoryType_[address / 256])
    {
        case UNDEFINED:
            return 255;
        break;
            
        case ROM:
        case RAM:
            return mainMemory_[address];
        break;
            
        case CPURAM:
            return cpuRam_[address&0xff];
        break;
            
        default:
            return 255;
        break;
    }
}

void Cdp18s600::writeMem(Word address, Byte value, bool writeRom)
{
    address_ = address;
    writeMemDebug(address, value, writeRom);
}

void Cdp18s600::writeMemDebug(Word address, Byte value, bool writeRom)
{
    switch (memoryType_[address/256])
    {
        case UNDEFINED:
        case ROM:
            if (writeRom)
                mainMemory_[address]=value;
        break;
            
        case RAM:
            if (mainMemory_[address]==value)
                return;
            mainMemory_[address]=value;
            if (address >= (memoryStart_) && address<((memoryStart_) +256))
                p_Main->updateDebugMemory(address);
            p_Main->updateAssTabCheck(address);
        break;

        case CPURAM:
            if (cpuRam_[address&0xff]==value)
                return;
            cpuRam_[address&0xff]=value;
            p_Main->updateDebugMemory(address);
            p_Main->updateAssTabCheck(address);
        break;
    }
}

void Cdp18s600::cpuInstruction()
{
    if (cpuMode_ == RUN)
    {
        cpuCycleStep();
        if (idle_ || cpuMode_ != RUN)
            cdp18s640ScreenPointer->setRunLed(0);
        else
            cdp18s640ScreenPointer->setRunLed(1);
    }
    else
    {
        machineCycle();
        machineCycle();
        if (resetPressed_)
            resetPressed();
    }
    if (interruptRequested_ == true && cpuState_ == STATE_FETCH_1)
        p_Computer->interrupt();
}

void Cdp18s600::resetPressed()
{
    resetCpu();
    initComputer();
    
    p_Main->setSwName("");
    p_Main->eventUpdateTitle();

    setCpuMode(RESET); // CLEAR = 0, WAIT = 1, CLEAR LED ON, WAIT LED OFF, RUN LED OFF
    
    showCycleData(0);
    if (Cdp18s600Configuration.autoBoot)
        autoBoot();

    resetPressed_ = false;
}

void Cdp18s600::moveWindows()
{
    if (Cdp18s600Configuration.vtType != VTNONE)
        vtPointer->Move(p_Main->getVtPos(CDP18S600));
}

void Cdp18s600::setForceUpperCase(bool status)
{
    if (Cdp18s600Configuration.vtType != VTNONE)
        vtPointer->setForceUCVt(status);
}

void Cdp18s600::setBootRam(bool status)
{
    Cdp18s600Configuration.bootRam = status;
}

void Cdp18s600::updateTitle(wxString Title)
{
    if (Cdp18s600Configuration.vtType == VT52)
        vtPointer->SetTitle("CDP18S600 - VT 52"+Title);
    if (Cdp18s600Configuration.vtType == VT100)
        vtPointer->SetTitle("CDP18S600 - VT 100"+Title);
}

void Cdp18s600::checkComputerFunction()
{
    wxString buffer;
    
    switch (scratchpadRegister_[programCounter_])
    {
        case 0xb011:
            mcdsRunState_ = RESETSTATECW;
        break;
            
        case 0xb053:
            mcdsRunState_ = BASICSTATE;
        break;
            
        case 0xC076:    // RUN
            mcdsRunState_ = RUNSTATE;
        break;
            
        case 0xc79f:    // CALL
            mcdsRunState_ = RUNSTATE;
        break;
            
        case 0xB225:    // BYE
            mcdsRunState_ = RESETSTATE;
        break;
            
            /*        case 0x9f:
             case 0xa3:
             buffer.Printf("%04X", mainMemory_[scratchpadRegister_[programCounter_]+1]*256+mainMemory_[scratchpadRegister_[programCounter_]+2]);
             p_Main->eventShowTextMessage(buffer);
             buffer.Printf("%04X", mainMemory_[scratchpadRegister_[programCounter_]+5]*256+mainMemory_[scratchpadRegister_[programCounter_]+6]);
             p_Main->eventShowTextMessage(buffer);
             break;*/
    }
}

void Cdp18s600::onReset()
{
    resetPressed_ = true;
}

void Cdp18s600::sleepComputer(long ms)
{
    threadPointer->Sleep(ms);
}

void Cdp18s600::setLedMs(long ms)
{
    cdp18s640ScreenPointer->setLedMs(ms);
    pioFramePointer->setLedMs(ms);
    if (ms == 0)
        ledCycleSize_ = -1;
    else
        ledCycleSize_ = (((Cdp18s600ClockSpeed_ * 1000000) / 8) / 1000) * ms;
    ledCycleValue_ = ledCycleSize_;
}

void Cdp18s600::startComputerRun(bool load)
{
    if (p_Vt100 != NULL)
        vtPointer->startMcdsRun(load);
}

bool Cdp18s600::isComputerRunning()
{
    if (mcdsRunState_ == RUNSTATE)
        return true;
    else
        return false;
}

void Cdp18s600::releaseButtonOnScreen(HexButton* buttonPointer, int WXUNUSED(buttonType))
{
    cdp18s640ScreenPointer->releaseButtonOnScreen(buttonPointer);
}

void Cdp18s600::releaseButtonOnScreen2(HexButton* buttonPointer, int WXUNUSED(buttonType))
{
    pioFramePointer->releaseButtonOnScreen(buttonPointer);
}

void Cdp18s600::activateMainWindow()
{
    bool maximize = IsMaximized();
    Iconize(false);
    Raise();
    Show(true);
    Maximize(maximize);
}

void Cdp18s600::showPio(bool state)
{
	pioFramePointer->Show(state);
    if (state)
        pioFramePointer->refreshLeds();
}

void Cdp18s600::removePio()
{
    Cdp18s600Configuration.usePio = false;
    p_Main->pioWindows(false);
    showPio(false);
}
