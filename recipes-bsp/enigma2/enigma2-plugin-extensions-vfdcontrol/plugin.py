from Screens.Screen import Screen
from Plugins.Plugin import PluginDescriptor
from Components.Button import Button
from Components.ActionMap import ActionMap
from Components.config import config, configfile, ConfigSubsection, getConfigListEntry, ConfigSelection, ConfigSlider
from Components.ConfigList import ConfigListScreen
from enigma import iPlayableService, eServiceCenter, eTimer, eActionMap, eDBoxLCD
from Components.ServiceEventTracker import ServiceEventTracker
from Components.SystemInfo import SystemInfo
from Screens.InfoBar import InfoBar
from time import localtime, time
import Screens.Standby
from Tools.HardwareInfo import HardwareInfo

config.plugins.SEG = ConfigSubsection()
# config.plugins.SEG.steps = ConfigSlider(default = 90, increment = 30, limits = (0, 90))
# config.plugins.SEG.steps_standby = ConfigSlider(default = 30, increment = 30, limits = (0, 90))
config.plugins.SEG.showClock = ConfigSelection(default = "True", choices = [("True",_("Channelnumber in Standby Clock"))])
config.plugins.SEG.showCHnumber = ConfigSelection(default = "15", choices = [("15",_("15 sec")),("30",_("30 sec")),("0",_("0 sec")),("50000",_("Allways"))])
config.plugins.SEG.timeMode = ConfigSelection(default = "24h", choices = [("12h",_("12h")),("24h",_("24h"))])
# steps = config.plugins.SEG.steps.value
# steps_standby = config.plugins.SEG.steps.value

def display_write(text):
	if SystemInfo["FrontpanelDisplay"]: 
		try:
			open("/dev/dbox/oled0", "w").write(text)
		except:
			pass
	else:
		try:
			open("/dev/dbox/lcd0", "w").write(text)
		except:
			pass

# def displaybrightness_write(value):
# 	try:
# 		open("/proc/stb/led/oled_brightness", "w").write(value)
# 	except:
# 		pass

class Channelnumber:

	def __init__(self, session):
		self.session = session
		self.sign = 0
		self.updatetime = 15000
		self.channelnrdelay = config.plugins.SEG.showCHnumber.value
		self.dvb_service = ""
		self.begin = int(time())
		self.endkeypress = True
		eActionMap.getInstance().bindAction('', -0x7FFFFFFF, self.keyPressed)
		self.TimerText = eTimer()
		self.TimerText.timeout.get().append(self.showclock)
		self.TimerText.start(1000, True)
		self.onClose = [ ]


		self.__event_tracker = ServiceEventTracker(screen=self,eventmap=
			{
				#iPlayableService.evUpdatedEventInfo: self.__eventInfoChanged,
				iPlayableService.evStart: self.__evStart,
			})

	def __evStart(self):
		self.getCurrentlyPlayingService()

        def getCurrentlyPlayingService(self):
                playref = self.session.nav.getCurrentlyPlayingServiceReference()
                if not playref:
                        self.dvb_service = ""
                else:
                        str_service = playref.toString()
                        if not '%3a//' in str_service and str_service.rsplit(":", 1)[1].startswith("/"):
                                self.dvb_service = "video"
                        else:
                                self.dvb_service = ""

	def __eventInfoChanged(self, manual=False):
		if self.dvb_service == "":
			self.text = "----"
			service = self.session.nav.getCurrentService()
			info = service and service.info()
			if info is None:
				self.text = "----"
			else:
				self.text = self.getchannelnr()
			info = None
			service = None
			if self.text == "----":
				display_write(self.text)
			else:
				Channelnr = "%04d" % (int(self.text))
				display_write(Channelnr)
		else:
			self.text = "----"
			return self.text

	def getchannelnr(self):
		MYCHANSEL = InfoBar.instance.servicelist
		serviceHandler = eServiceCenter.getInstance()
		myRoot = MYCHANSEL.servicelist.getRoot()
		mySSS = serviceHandler.list(myRoot)
		SRVList = mySSS and mySSS.getContent("SN", True)
		markersOffset = 0
		mySrv = MYCHANSEL.servicelist.getCurrent()
		chx = MYCHANSEL.servicelist.l.lookupService(mySrv)
		for i in range(len(SRVList)):
			if chx == i:
				break
			testlinet = SRVList[i]
			testline = testlinet[0].split(":")
			if testline[1] == "64":
				markersOffset = markersOffset + 1
		chx = (chx - markersOffset) + 1
		rx = MYCHANSEL.getBouquetNumOffset(myRoot)
		self.text = str(chx + rx)
		return self.text

	def show(self):
		if config.plugins.SEG.showClock.value == 'True' and self.dvb_service != 'video':
			clock = str(localtime()[3])
			clock1 = str(localtime()[4])
			if config.plugins.SEG.timeMode.value != '24h':
				if int(clock) > 12:
					clock = str(int(clock) - 12)
			if self.sign == 0:
				clock2 = "%02d:%02d" % (int(clock), int(clock1))
				self.sign = 1
			else:
				clock2 = "%02d%02d" % (int(clock), int(clock1))
				self.sign = 0
			display_write(clock2)
		else:
			display_write("....")

	def showclock(self):
		standby_mode = Screens.Standby.inStandby
		if (config.plugins.SEG.showClock.value == 'True' or config.plugins.SEG.showClock.value != 'True') and not standby_mode:
			if config.plugins.SEG.showClock.value == 'True':
				if time() >= self.begin:
					self.endkeypress = False
				if self.endkeypress:
					self.__eventInfoChanged(True)
				else:
					self.show()
			else:
				self.__eventInfoChanged(True)

		if config.plugins.SEG.showClock.value != 'True':
			display_write("....")
			self.TimerText.start(self.updatetime, True)
			return
		else:
			update_time = 1000
			if not standby_mode and self.dvb_service == "video":
				update_time = 15000
			self.TimerText.start(update_time, True)

		if standby_mode:
			self.show()

	def keyPressed(self, key, tag):
		self.begin = time() + int(self.channelnrdelay)
		self.endkeypress = True

ChannelnumberInstance = None

def leaveStandby():
	if config.plugins.SEG.showClock.value != 'True':
		display_write("....")

def standbyCounterChanged(configElement):
	from Screens.Standby import inStandby
	inStandby.onClose.append(leaveStandby)
	if config.plugins.SEG.showClock.value != 'True':
		display_write("....")

def initSEG():
	if config.plugins.SEG.showClock.value != 'True':
		display_write("....")
#	if config.plugins.SEG.showDisplay.value == 'False':
#		Steps_standby = "% d" % (int(steps_standby))
#		displaybrightness_write(Steps_standby)
#	else:
#		Steps = "% d" % (int(steps))
#		displaybrightness_write(Steps)

class SEG_Setup(ConfigListScreen, Screen):
	def __init__(self, session, args = None):

		self.skin = """
			<screen position="center,center" size="500,210" title="7Segment Display Setup" >
				<widget name="config" position="20,15" size="460,150" scrollbarMode="showOnDemand" />
				<ePixmap position="40,165" size="140,40" pixmap="skin_default/buttons/red.png" alphatest="on" />
				<ePixmap position="180,165" size="140,40" pixmap="skin_default/buttons/green.png" alphatest="on" />
				<widget name="key_red" position="40,165" size="140,40" font="Regular;18" backgroundColor="#1f771f" zPosition="2" transparent="1" shadowColor="black" shadowOffset="-1,-1" />
				<widget name="key_green" position="180,165" size="140,40" font="Regular;18" backgroundColor="#1f771f" zPosition="2" transparent="1" shadowColor="black" shadowOffset="-1,-1" />
			</screen>"""

		Screen.__init__(self, session)
		self.setTitle(_("Control 7-Segment display"))
		self.onClose.append(self.abort)

		self.list = []
		ConfigListScreen.__init__(self, self.list, session = self.session, on_change = self.changedEntry)

		self.createSetup()

		self["key_red"] = Button(_("Cancel"))
		self["key_green"] = Button(_("Save"))

		self["setupActions"] = ActionMap(["SetupActions","ColorActions"],
		{
			"save": self.keySave,
			"cancel": self.keyCancel,
			"ok": self.keySave,
		}, -2)

	def createSetup(self):
		self.list = []
#		self.list.append(getConfigListEntry(_("Display brightness"), config.plugins.SEG.steps))
#		self.list.append(getConfigListEntry(_("Standby brightness"), config.plugins.SEG.steps_standby))
		self.list.append(getConfigListEntry(_("Time mode"), config.plugins.SEG.timeMode))
		self.list.append(getConfigListEntry(_("Show Channel in Display"), config.plugins.SEG.showCHnumber))
		self["config"].list = self.list
		self["config"].l.setList(self.list)

	def abort(self):
		pass

	def saveAll(self):
		for x in self["config"].list:
			x[1].save()
		configfile.save()
		initSEG()

	def keySave(self):
		self.saveAll()
		self.close()

	def keyCancel(self):
		for x in self["config"].list:
			x[1].cancel()
		self.close()

class SEG:
	def __init__(self, session):
		self.session = session
		self.onClose = [ ]
		initSEG()

		global ChannelnumberInstance
		if ChannelnumberInstance is None:
			ChannelnumberInstance = Channelnumber(session)

	def shutdown(self):
		self.abort()

	def abort(self):
		config.misc.standbyCounter.addNotifier(standbyCounterChanged, initial_call = False)

def main(menuid):
	if menuid != "system":
		return [ ]
	return [(_("7-Segment Display Setup"), startSEG, "seg", None)]

def startSEG(session, **kwargs):
	session.open(SEG_Setup)

Seg = None
gReason = -1
mySession = None

def controlSeg():
	global Seg
	global gReason
	global mySession

	if gReason == 0 and mySession != None and Seg == None:
		Seg = SEG(mySession)
	elif gReason == 1 and Seg != None:
		Seg = None

def sessionstart(reason, **kwargs):
	global Seg
	global gReason
	global mySession

	if kwargs.has_key("session"):
		mySession = kwargs["session"]
	else:
		gReason = reason
	controlSeg()

def Plugins(**kwargs):
	if SystemInfo["FrontpanelDisplay"]:
		return [ PluginDescriptor(where=[PluginDescriptor.WHERE_AUTOSTART, PluginDescriptor.WHERE_SESSIONSTART], fnc=sessionstart),
			PluginDescriptor(name="7-Segment Display Setup", description=_("Change display settings"),where = PluginDescriptor.WHERE_MENU, fnc = main) ]
	return []