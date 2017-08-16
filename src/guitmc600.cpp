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

#include "wx/xrc/xmlres.h"
#include "wx/spinctrl.h"

#include "main.h"
#include "guitmc600.h"
#include "psave.h"
#include "keymaptmc.h"

BEGIN_EVENT_TABLE(GuiTelmac, GuiPecom)

	EVT_TEXT(XRCID("MainRomTmc600"), GuiMain::onMainRom1Text)
	EVT_COMBOBOX(XRCID("MainRomTmc600"), GuiMain::onMainRom1Text)
	EVT_BUTTON(XRCID("RomButtonTmc600"), GuiMain::onMainRom1)

	EVT_TEXT(XRCID("ExpRomTmc600"), GuiTelmac::onTelmacExpRomText)
	EVT_COMBOBOX(XRCID("ExpRomTmc600"), GuiTelmac::onTelmacExpRomText)
	EVT_BUTTON(XRCID("ExpRomButtonTmc600"), GuiTelmac::onTelmacExpRom)

	EVT_TEXT(XRCID("CharRomTmc600"), GuiMain::onCharRomText)
	EVT_COMBOBOX(XRCID("CharRomTmc600"), GuiMain::onCharRomText)
	EVT_BUTTON(XRCID("CharRomButtonTmc600"), GuiMain::onCharRom)

	EVT_BUTTON(XRCID("ScreenDumpFileButtonTmc600"), GuiMain::onScreenDumpFile)
	EVT_TEXT(XRCID("ScreenDumpFileTmc600"), GuiMain::onScreenDumpFileText)
	EVT_COMBOBOX(XRCID("ScreenDumpFileTmc600"), GuiMain::onScreenDumpFileText)
	EVT_BUTTON(XRCID("ScreenDumpF5Tmc600"), GuiMain::onScreenDump)

	EVT_SPIN_UP(XRCID("ZoomSpinTmc600"), GuiMain::onZoomUp)
	EVT_SPIN_DOWN(XRCID("ZoomSpinTmc600"), GuiMain::onZoomDown)
	EVT_TEXT(XRCID("ZoomValueTmc600"), GuiMain::onZoomValue)
	EVT_BUTTON(XRCID("FullScreenF3Tmc600"), GuiMain::onFullScreen)
	EVT_COMMAND_SCROLL_THUMBTRACK(XRCID("VolumeTmc600"), GuiMain::onVolume) 
	EVT_COMMAND_SCROLL_CHANGED(XRCID("VolumeTmc600"), GuiMain::onVolume) 
	EVT_BUTTON(XRCID("CasButtonTmc600"), GuiMain::onCassette)
	EVT_BUTTON(XRCID("EjectCasTmc600"), GuiMain::onCassetteEject)
	EVT_TEXT(XRCID("WavFileTmc600"), GuiMain::onCassetteText)
	EVT_BUTTON(XRCID("RealCasLoadTmc600"), GuiMain::onRealCas)
	EVT_BUTTON(XRCID("CasLoadTmc600"), GuiMain::onCassetteLoad)
	EVT_BUTTON(XRCID("CasSaveTmc600"), GuiMain::onCassetteSave)
	EVT_BUTTON(XRCID("CasStopTmc600"), GuiMain::onCassetteStop)
	EVT_CHECKBOX(XRCID("TurboTmc600"), GuiMain::onTurbo)
	EVT_TEXT(XRCID("TurboClockTmc600"), GuiMain::onTurboClock)
	EVT_CHECKBOX(XRCID("AutoCasLoadTmc600"), GuiMain::onAutoLoad)

	EVT_BUTTON(XRCID("PrintFileButtonTmc600"), GuiMain::onPrintFile)
	EVT_TEXT(XRCID("PrintFileTmc600"), GuiMain::onPrintFileText)
	EVT_BUTTON(XRCID("PrintButtonTmc600"), GuiMain::onPrintButton)
	EVT_CHOICE(XRCID("PrintModeTmc600"), GuiMain::onPrintMode)

	EVT_TEXT(XRCID("KeyFileTmc600"), GuiMain::onKeyFileText)
	EVT_BUTTON(XRCID("KeyFileButtonTmc600"), GuiMain::onKeyFile)
	EVT_BUTTON(XRCID("EjectKeyFileTmc600"), GuiMain::onKeyFileEject)

	EVT_CHOICE(XRCID("RamTmc600"), GuiMain::onChoiceRam)

	EVT_TEXT(XRCID("SaveStartTmc600"), GuiMain::onSaveStart)
	EVT_TEXT(XRCID("SaveEndTmc600"), GuiMain::onSaveEnd)
	EVT_TEXT(XRCID("SaveExecTmc600"), GuiMain::onSaveExec)

	EVT_CHECKBOX(XRCID("RealTimeClockTmc600"), GuiTelmac::onRealTimeClockTmc600)
	EVT_SPINCTRL(XRCID("AdsChannel"), GuiTelmac::onTelmacAdsChannel)
	EVT_TEXT(XRCID("AdsChannel"), GuiTelmac::onTelmacAdsChannelText)
	EVT_SPINCTRL(XRCID("AdsVolt"), GuiTelmac::onTelmacAdsVolt)
	EVT_TEXT(XRCID("AdsVolt"), GuiTelmac::onTelmacAdsVoltText)
	EVT_SPINCTRL(XRCID("AdiChannel"), GuiTelmac::onTelmacAdiChannel)
	EVT_TEXT(XRCID("AdiChannel"), GuiTelmac::onTelmacAdiChannelText)
	EVT_SPINCTRL(XRCID("AdiVolt"), GuiTelmac::onTelmacAdiVolt)
	EVT_TEXT(XRCID("AdiVolt"), GuiTelmac::onTelmacAdiVoltText)
	EVT_CHECKBOX(XRCID("UseLocationTmc600"), GuiMain::onUseLocation)
	EVT_BUTTON(XRCID("SaveButtonTmc600"), GuiMain::onSaveButton) 
	EVT_BUTTON(XRCID("LoadButtonTmc600"), GuiMain::onLoadButton)
	EVT_BUTTON(XRCID("RunButtonTmc600"), GuiMain::onLoadRunButton)
	EVT_BUTTON(XRCID("DsaveButtonTmc600"), GuiMain::onDataSaveButton)
	EVT_COMMAND(wxID_ANY, OPEN_PRINTER_WINDOW, GuiMain::openPrinterFrame) 
	EVT_BUTTON(XRCID("ColoursTmc600"), Main::onColoursDef)

    EVT_BUTTON(XRCID("KeyMapTmc600"), GuiTelmac::onKeyDef)

END_EVENT_TABLE()

GuiTelmac::GuiTelmac(const wxString& title, const wxPoint& pos, const wxSize& size, Mode mode, wxString dataDir, wxString iniDir)
: GuiPecom(title, pos, size, mode, dataDir, iniDir)
{
	conf[TMC600].loadFileNameFull_ = "";
	conf[TMC600].loadFileName_ = "";

	conf[TMC600].pLoadSaveName_[0] = 'T';
	conf[TMC600].pLoadSaveName_[1] = 'M';
	conf[TMC600].pLoadSaveName_[2] = 'C';
	conf[TMC600].pLoadSaveName_[3] = '6';

	conf[TMC600].defus_ = 0x6181;
	conf[TMC600].eop_ = 0x6183;
	conf[TMC600].string_ = 0x6192;
	conf[TMC600].arrayValue_ = 0x6194;
	conf[TMC600].eod_ = 0x6199;
	conf[TMC600].basicRamAddress_ = 0x6300;

	conf[TMC600].saveStartString_ = "";
	conf[TMC600].saveEndString_ = "";
	conf[TMC600].saveExecString_ = "";
}

void GuiTelmac::readTelmacConfig()
{
	selectedComputer_ = TMC600;

    conf[TMC600].configurationDir_ = iniDir_ + "Configurations" + pathSeparator_ + "TMC600" + pathSeparator_;
    conf[TMC600].mainDir_ = readConfigDir("/Dir/TMC600/Main", dataDir_ + "TMC600" + pathSeparator_);
    
    conf[TMC600].romDir_[MAINROM1] = readConfigDir("/Dir/TMC600/Main_Rom_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].romDir_[EXPROM] = readConfigDir("/Dir/TMC600/Expansion_Rom_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].charRomDir_ = readConfigDir("/Dir/TMC600/Font_Rom_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].ramDir_ = readConfigDir("/Dir/TMC600/Software_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].keyFileDir_ = readConfigDir("/Dir/TMC600/Key_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].printFileDir_ = readConfigDir("/Dir/TMC600/Print_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].screenDumpFileDir_ = readConfigDir("/Dir/TMC600/Video_Dump_File", dataDir_ + "TMC600" + pathSeparator_);
	conf[TMC600].wavFileDir_ = readConfigDir("/Dir/TMC600/Wav_File", dataDir_ + "TMC600" + pathSeparator_); 

	conf[TMC600].rom_[MAINROM1] = configPointer->Read("/TMC600/Main_Rom_File", "tmc.24.3.bin");
	conf[TMC600].rom_[EXPROM] = configPointer->Read("/TMC600/Expansion_Rom_File", "151182.bin");
	conf[TMC600].charRom_ = configPointer->Read("/TMC600/Font_Rom_File", "character.bin");
	conf[TMC600].keyFile_ = configPointer->Read("/TMC600/Key_File", "");
	conf[TMC600].printFile_ = configPointer->Read("/TMC600/Print_File", "printerout.txt");
	conf[TMC600].screenDumpFile_ = configPointer->Read("/TMC600/Video_Dump_File", "screendump.png");
	conf[TMC600].wavFile_ = configPointer->Read("/TMC600/Wav_File", "");

	wxString defaultZoom;
	defaultZoom.Printf("%2.2f", 2.0);
	conf[TMC600].zoom_ = configPointer->Read("/TMC600/Zoom", defaultZoom);
	wxString defaultClock;
	defaultClock.Printf("%1.3f", 3.579);
	conf[TMC600].clock_ = configPointer->Read("/TMC600/Clock_Speed", defaultClock);

	conf[TMC600].volume_ = (int)configPointer->Read("/TMC600/Volume", 25l);
	conf[TMC600].turboClock_ = configPointer->Read("/TMC600/Turbo_Clock_Speed", "15");
	conf[TMC600].printMode_ = (int)configPointer->Read("/TMC600/Print_Mode", 1l);
	conf[TMC600].useLoadLocation_ = false;
	conf[TMC600].ramType_ = (int)configPointer->Read("/TMC600/Ram_Type", 02);

	configPointer->Read("/TMC600/Enable_Turbo_Cassette", &conf[TMC600].turbo_, true);
	configPointer->Read("/TMC600/Enable_Auto_Cassette", &conf[TMC600].autoCassetteLoad_, true);
	configPointer->Read("/TMC600/Enable_Real_Cassette", &conf[TMC600].realCassetteLoad_, false);
	configPointer->Read("/TMC600/Real_Time_Clock", &realTimeClock_, false);

	setRealCas(TMC600);

	if (mode_.gui)
	{
		XRCCTRL(*this, "MainRomTmc600", wxComboBox)->SetValue(conf[TMC600].rom_[MAINROM1]);
		XRCCTRL(*this, "ExpRomTmc600", wxComboBox)->SetValue(conf[TMC600].rom_[EXPROM]);
		XRCCTRL(*this, "CharRomTmc600", wxComboBox)->SetValue(conf[TMC600].charRom_);
		XRCCTRL(*this, "KeyFileTmc600", wxTextCtrl)->SetValue(conf[TMC600].keyFile_);
		XRCCTRL(*this, "PrintFileTmc600", wxTextCtrl)->SetValue(conf[TMC600].printFile_);
		XRCCTRL(*this, "ScreenDumpFileTmc600", wxComboBox)->SetValue(conf[TMC600].screenDumpFile_);
		XRCCTRL(*this, "WavFileTmc600", wxTextCtrl)->SetValue(conf[TMC600].wavFile_);

		XRCCTRL(*this, "ZoomValueTmc600", wxTextCtrl)->ChangeValue(conf[TMC600].zoom_);
		XRCCTRL(*this, "VolumeTmc600", wxSlider)->SetValue(conf[TMC600].volume_);
		clockTextCtrl[TMC600]->ChangeValue(conf[TMC600].clock_);
		XRCCTRL(*this, "TurboTmc600", wxCheckBox)->SetValue(conf[TMC600].turbo_);
		turboGui("Tmc600");
		XRCCTRL(*this, "TurboClockTmc600", wxTextCtrl)->SetValue(conf[TMC600].turboClock_);
		XRCCTRL(*this, "AutoCasLoadTmc600", wxCheckBox)->SetValue(conf[TMC600].autoCassetteLoad_);
		XRCCTRL(*this, "RealTimeClockTmc600", wxCheckBox)->SetValue(realTimeClock_);

		XRCCTRL(*this, "RamTmc600", wxChoice)->SetSelection(conf[TMC600].ramType_);

		XRCCTRL(*this, "PrintModeTmc600", wxChoice)->SetSelection(conf[TMC600].printMode_);
		setPrintMode();
		XRCCTRL(*this, "UseLocationTmc600", wxCheckBox)->SetValue(false);
	}
}

void GuiTelmac::writeTelmacDirConfig()
{
    writeConfigDir("/Dir/TMC600/Main", conf[TMC600].mainDir_);
    writeConfigDir("/Dir/TMC600/Main_Rom_File", conf[TMC600].romDir_[MAINROM1]);
	writeConfigDir("/Dir/TMC600/Expansion_Rom_File", conf[TMC600].romDir_[EXPROM]);
	writeConfigDir("/Dir/TMC600/Font_Rom_File", conf[TMC600].charRomDir_);
	writeConfigDir("/Dir/TMC600/Software_File", conf[TMC600].ramDir_);
	writeConfigDir("/Dir/TMC600/Key_File", conf[TMC600].keyFileDir_);
	writeConfigDir("/Dir/TMC600/Print_File", conf[TMC600].printFileDir_);
	writeConfigDir("/Dir/TMC600/Video_Dump_File", conf[TMC600].screenDumpFileDir_);
	writeConfigDir("/Dir/TMC600/Wav_File", conf[TMC600].wavFileDir_);
}

void GuiTelmac::writeTelmacConfig()
{
	configPointer->Write("/TMC600/Main_Rom_File", conf[TMC600].rom_[MAINROM1]);
	configPointer->Write("/TMC600/Expansion_Rom_File", conf[TMC600].rom_[EXPROM]);
	configPointer->Write("/TMC600/Font_Rom_File", conf[TMC600].charRom_);
	configPointer->Write("/TMC600/Key_File", conf[TMC600].keyFile_);
	configPointer->Write("/TMC600/Print_File", conf[TMC600].printFile_);
	configPointer->Write("/TMC600/Video_Dump_File", conf[TMC600].screenDumpFile_);
	configPointer->Write("/TMC600/Wav_File", conf[TMC600].wavFile_);

	configPointer->Write("/TMC600/Zoom", conf[TMC600].zoom_);
	configPointer->Write("/TMC600/Volume", conf[TMC600].volume_);

	configPointer->Write("/TMC600/Clock_Speed", conf[TMC600].clock_);
	configPointer->Write("/TMC600/Enable_Turbo_Cassette", conf[TMC600].turbo_);
	configPointer->Write("/TMC600/Turbo_Clock_Speed", conf[TMC600].turboClock_); 
	configPointer->Write("/TMC600/Enable_Auto_Cassette", conf[TMC600].autoCassetteLoad_);
	configPointer->Write("/TMC600/Enable_Real_Cassette", conf[TMC600].realCassetteLoad_);
	configPointer->Write("/TMC600/Real_Time_Clock", realTimeClock_);
	configPointer->Write("/TMC600/Ram_Type", conf[TMC600].ramType_);
	configPointer->Write("/TMC600/Print_Mode", conf[TMC600].printMode_);
}

void GuiTelmac::readTelmacWindowConfig()
{
    conf[TMC600].mainX_ = (int)configPointer->Read("/TMC600/Window_Position_X", mainWindowX_+windowInfo.mainwX+windowInfo.xBorder);
    conf[TMC600].mainY_ = (int)configPointer->Read("/TMC600/Window_Position_Y", mainWindowY_);
}

void GuiTelmac::writeTelmacWindowConfig()
{
    if (conf[TMC600].mainX_ > 0)
        configPointer->Write("/TMC600/Window_Position_X", conf[TMC600].mainX_);
    if (conf[TMC600].mainY_ > 0)
        configPointer->Write("/TMC600/Window_Position_Y", conf[TMC600].mainY_);
}

void GuiTelmac::onTelmacExpRom(wxCommandEvent& WXUNUSED(event) )
{
	wxString fileName;

	fileName = wxFileSelector( "Select the Expansion ROM file to load",
                               conf[TMC600].romDir_[EXPROM], XRCCTRL(*this, "ExpRomTmc600", wxComboBox)->GetValue(),
                               "",
                               wxString::Format
                              (
                                   "Binary File|*.bin;*.rom;*.ram;*.cos|Intel Hex File|*.hex|All files (%s)|%s",
                                   wxFileSelectorDefaultWildcardStr,
                                   wxFileSelectorDefaultWildcardStr
                               ),
                               wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_PREVIEW,
                               this
                              );
	if (!fileName)
		return;

	wxFileName FullPath = wxFileName(fileName, wxPATH_NATIVE);
	conf[TMC600].romDir_[EXPROM] = FullPath.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR, wxPATH_NATIVE);
	conf[TMC600].rom_[EXPROM] = FullPath.GetFullName();

	XRCCTRL(*this, "ExpRomTmc600", wxComboBox)->SetValue(conf[TMC600].rom_[EXPROM]);
}

void GuiTelmac::onTelmacExpRomText(wxCommandEvent& WXUNUSED(event))
{
	conf[selectedComputer_].rom_[EXPROM] = XRCCTRL(*this, "ExpRomTmc600", wxComboBox)->GetValue();
}

void GuiTelmac::onRealTimeClockTmc600(wxCommandEvent&event)
{
	realTimeClock_ = event.IsChecked();
}

void GuiTelmac::onTelmacAdsChannel(wxSpinEvent&event)
{
	if (runningComputer_ == TMC600)
	{
		XRCCTRL(*this, "AdsVolt", wxSpinCtrl)->SetValue(p_Tmc600->getAds(event.GetPosition())); 
	}
}

void GuiTelmac::onTelmacAdsVolt(wxSpinEvent&event)
{
	if (runningComputer_ == TMC600)
	{
		p_Tmc600->setAds(XRCCTRL(*this, "AdsChannel", wxSpinCtrl)->GetValue(), event.GetPosition());
	}
}

void GuiTelmac::onTelmacAdsChannelText(wxCommandEvent&WXUNUSED(event))
{
	if (runningComputer_ == TMC600)
	{
		XRCCTRL(*this, "AdsVolt", wxSpinCtrl)->SetValue(p_Tmc600->getAds(XRCCTRL(*this, "AdsChannel", wxSpinCtrl)->GetValue())); 
	}
}

void GuiTelmac::onTelmacAdsVoltText(wxCommandEvent&WXUNUSED(event))
{
	if (runningComputer_ == TMC600)
	{
		p_Tmc600->setAds(XRCCTRL(*this, "AdsChannel", wxSpinCtrl)->GetValue(), XRCCTRL(*this, "AdsVolt", wxSpinCtrl)->GetValue());
	}
}

void GuiTelmac::onTelmacAdiChannel(wxSpinEvent&event)
{
	if (runningComputer_ == TMC600)
	{
		XRCCTRL(*this, "AdiVolt", wxSpinCtrl)->SetValue(p_Tmc600->getAdi(event.GetPosition())); 
	}
}

void GuiTelmac::onTelmacAdiVolt(wxSpinEvent&event)
{
	if (runningComputer_ == TMC600)
	{
		p_Tmc600->setAdi(XRCCTRL(*this, "AdiChannel", wxSpinCtrl)->GetValue(), event.GetPosition());
	}
}

void GuiTelmac::onTelmacAdiChannelText(wxCommandEvent&WXUNUSED(event))
{
	if (runningComputer_ == TMC600)
	{
		XRCCTRL(*this, "AdiVolt", wxSpinCtrl)->SetValue(p_Tmc600->getAdi(XRCCTRL(*this, "AdiChannel", wxSpinCtrl)->GetValue())); 
	}
}

void GuiTelmac::onTelmacAdiVoltText(wxCommandEvent&WXUNUSED(event))
{
	if (runningComputer_ == TMC600)
	{
		p_Tmc600->setAdi(XRCCTRL(*this, "AdiChannel", wxSpinCtrl)->GetValue(), XRCCTRL(*this, "AdiVolt", wxSpinCtrl)->GetValue());
	}
}

void GuiTelmac::enableIoGui()
{
	if (!mode_.gui)
		return;
	XRCCTRL(*this,"AdiInputText", wxStaticText)->Enable(true);
	XRCCTRL(*this,"AdiChannel", wxSpinCtrl)->Enable(true);
	XRCCTRL(*this,"AdiVolt", wxSpinCtrl)->Enable(true);
	XRCCTRL(*this,"AdiVoltText", wxStaticText)->Enable(true);
	XRCCTRL(*this,"AdsInputText", wxStaticText)->Enable(true);
	XRCCTRL(*this,"AdsChannel", wxSpinCtrl)->Enable(true);
	XRCCTRL(*this,"AdsVolt", wxSpinCtrl)->Enable(true);
	XRCCTRL(*this,"AdsVoltText", wxStaticText)->Enable(true);
}

void GuiTelmac::onKeyDef(wxCommandEvent&WXUNUSED(event))
{
    KeyMapTmcDialog KeyMapTmcDialog(this);
    KeyMapTmcDialog.ShowModal();
    
    if (computerRunning_)
        p_Computer->reDefineKeys();
}

