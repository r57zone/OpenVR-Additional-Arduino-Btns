#include <DeviceProvider.h>
#include <ControllerDriver.h>
#include <thread>
#include <atlstr.h> 

bool DebugMode = false;
std::thread *pComReadthread = NULL;
HANDLE hSerial;
bool ArduinoConnected = false;
float ArduinoIMU[5] = { 0, 0, 0, 0, 0 };
float LastArduinoIMU[5] = { 0, 0, 0, 0, 0 };

bool CorrectAxis(float Value)
{
	if (Value < -1.0 || Value > 1.0)
		return false;
	else
		return true;
}

void ArduinoIMURead()
{
	DWORD bytesRead;

	VRDriverLog()->Log("ArduinoButtons: starting com port read thread");

	Sleep(50);

	PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);

	while (ArduinoConnected) {
		ReadFile(hSerial, &ArduinoIMU, sizeof(ArduinoIMU), &bytesRead, 0);

		if (CorrectAxis(ArduinoIMU[0]) == false || CorrectAxis(ArduinoIMU[1]) == false || CorrectAxis(ArduinoIMU[2]) == false || CorrectAxis(ArduinoIMU[3]) == false) //Noise filter
		{
			//Last correct values
			ArduinoIMU[0] = LastArduinoIMU[0];
			ArduinoIMU[1] = LastArduinoIMU[1];
			ArduinoIMU[2] = LastArduinoIMU[2];
			ArduinoIMU[3] = LastArduinoIMU[3];
			ArduinoIMU[4] = LastArduinoIMU[4];


			PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);
		}
		else if (CorrectAxis(ArduinoIMU[0]) && CorrectAxis(ArduinoIMU[1]) && CorrectAxis(ArduinoIMU[2] && CorrectAxis(ArduinoIMU[3]))) //Save last correct values
		{
			LastArduinoIMU[0] = ArduinoIMU[0];
			LastArduinoIMU[1] = ArduinoIMU[1];
			LastArduinoIMU[2] = ArduinoIMU[2];
			LastArduinoIMU[3] = ArduinoIMU[3];
			LastArduinoIMU[4] = ArduinoIMU[4];
		}

		if ((GetAsyncKeyState(VK_F7) & 0x8000) != 0)
			PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
}

EVRInitError DeviceProvider::Init(IVRDriverContext* pDriverContext)
{
	EVRInitError initError = InitServerDriverContext(pDriverContext);
	if (initError != EVRInitError::VRInitError_None)
	{
		return initError;
	}

	VRDriverLog()->Log("ArduinoButtons: Initializing"); //this is how you log out Steam's log file.

	//VR settings
	DebugMode = vr::VRSettings()->GetBool("ArduinoButtons", "DebugMode");

	controllerDriver = new ControllerDriver();
	VRServerDriverHost()->TrackedDeviceAdded("arduinobuttons", TrackedDeviceClass_Controller, controllerDriver); //add all your devices like this.

	//ComPort init
	CString sPortName;
	sPortName.Format(_T("COM%d"), vr::VRSettings()->GetInt32("ArduinoButtons", "COMPort"));

	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial != INVALID_HANDLE_VALUE && GetLastError() != ERROR_FILE_NOT_FOUND) {

		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (GetCommState(hSerial, &dcbSerialParams))
		{
			dcbSerialParams.BaudRate = CBR_115200;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;

			if (SetCommState(hSerial, &dcbSerialParams))
			{
				ArduinoConnected = true;
				pComReadthread = new std::thread(ArduinoIMURead);
			}
			else
				VRDriverLog()->Log("ArduinoButtons: COM port set state error");
		}
		else
			VRDriverLog()->Log("ArduinoButtons: COM port get state error");
	} else
		VRDriverLog()->Log("ArduinoButtons: COM port handle error ");

	return vr::VRInitError_None;
}

void DeviceProvider::Cleanup()
{
	if (ArduinoConnected) {
		ArduinoConnected = false;
		if (pComReadthread) {
			pComReadthread->join();
			delete pComReadthread;
			pComReadthread = nullptr;

		}
		CloseHandle(hSerial);
		VRDriverLog()->Log("ArduinoButtons: COM port cleanup");
	}

	delete controllerDriver;
	controllerDriver = NULL;
}
const char* const* DeviceProvider::GetInterfaceVersions()
{
	return k_InterfaceVersions;
}

void DeviceProvider::RunFrame()
{
	controllerDriver->RunFrame();
}

bool DeviceProvider::ShouldBlockStandbyMode()
{
	return false;
}

void DeviceProvider::EnterStandby() {}

void DeviceProvider::LeaveStandby() {}

EVRInitError ControllerDriver::Activate(uint32_t unObjectId)
{
	driverId = unObjectId; //unique ID for your driver

	PropertyContainerHandle_t props = VRProperties()->TrackedDeviceToPropertyContainer(driverId); //this gets a container object where you store all the information about your driver

	VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "{arduinobuttons}/input/controller_profile.json"); //tell OpenVR where to get your driver's Input Profile
	VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_Treadmill); //tells OpenVR what kind of device this is
	VRDriverInput()->CreateScalarComponent(props, "/input/joystick/y", &joystickYHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided); //sets up handler you'll use to send joystick commands to OpenVR with, in the Y direction (forward/backward)
	VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/y", &trackpadYHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided); //sets up handler you'll use to send trackpad commands to OpenVR with, in the Y direction
	VRDriverInput()->CreateScalarComponent(props, "/input/joystick/x", &joystickXHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided); //Why VRScalarType_Absolute? Take a look at the comments on EVRScalarType.
	VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/x", &trackpadXHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided); //Why VRScalarUnits_NormalizedTwoSided? Take a look at the comments on EVRScalarUnits.

	vr::VRDriverInput()->CreateBooleanComponent(props, "/input/a/click", &aBtnHandle);
	vr::VRDriverInput()->CreateBooleanComponent(props, "/input/b/click", &bBtnHandle);
	
	//The following properites are ones I tried out because I saw them in other samples, but I found they were not needed to get the sample working.
	//There are many samples, take a look at the openvr_header.h file. You can try them out.

	//VRProperties()->SetUint64Property(props, Prop_CurrentUniverseId_Uint64, 2);
	//VRProperties()->SetBoolProperty(props, Prop_HasControllerComponent_Bool, true);
	//VRProperties()->SetBoolProperty(props, Prop_NeverTracked_Bool, true);
	//VRProperties()->SetInt32Property(props, Prop_Axis0Type_Int32, k_eControllerAxis_TrackPad);
	//VRProperties()->SetInt32Property(props, Prop_Axis2Type_Int32, k_eControllerAxis_Joystick);
	//VRProperties()->SetStringProperty(props, Prop_SerialNumber_String, "example_controler_serial");
	//VRProperties()->SetStringProperty(props, Prop_RenderModelName_String, "vr_controller_vive_1_5");
	//uint64_t availableButtons = ButtonMaskFromId(k_EButton_SteamVR_Touchpad) |
	//	ButtonMaskFromId(k_EButton_IndexController_JoyStick);
	//VRProperties()->SetUint64Property(props, Prop_SupportedButtons_Uint64, availableButtons);

	return VRInitError_None;
}

DriverPose_t ControllerDriver::GetPose()
{
	DriverPose_t pose = { 0 }; //This example doesn't use Pose, so this method is just returning a default Pose.
	pose.poseIsValid = false;
	pose.result = TrackingResult_Calibrating_OutOfRange;
	pose.deviceIsConnected = true;

	HmdQuaternion_t quat;
	quat.w = 1;
	quat.x = 0;
	quat.y = 0;
	quat.z = 0;

	pose.qWorldFromDriverRotation = quat;
	pose.qDriverFromHeadRotation = quat;

	return pose;
}

void ControllerDriver::RunFrame()
{
	aAction = false;
	bAction = false;

	if (DebugMode) {
		LastArduinoIMU[0] = 0;
		LastArduinoIMU[1] = 0;
		LastArduinoIMU[2] = 0;
		LastArduinoIMU[3] = 0;

		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0)
			LastArduinoIMU[0] = -1;
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0)
			LastArduinoIMU[0] = 1;
		if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
			LastArduinoIMU[1] = 1;
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
			LastArduinoIMU[1] = -1;

		if ((GetAsyncKeyState(VK_NUMPAD4) & 0x8000) != 0)
			LastArduinoIMU[2] = -1;
		if ((GetAsyncKeyState(VK_NUMPAD6) & 0x8000) != 0)
			LastArduinoIMU[2] = 1;
		if ((GetAsyncKeyState(VK_NUMPAD8) & 0x8000) != 0)
			LastArduinoIMU[3] = 1;
		if ((GetAsyncKeyState(VK_NUMPAD2) & 0x8000) != 0)
			LastArduinoIMU[3] = -1;

		if ((GetAsyncKeyState(VK_NUMPAD7) & 0x8000) != 0)
			aAction = true;

		if ((GetAsyncKeyState(VK_NUMPAD9) & 0x8000) != 0)
			bAction = true;
	} else {
		if (LastArduinoIMU[4] == 1.0f)
			aAction = true;

		if (LastArduinoIMU[4] == 2.0f)
			bAction = true;
	}

	//Since we used VRScalarUnits_NormalizedTwoSided as the unit, the range is -1 to 1.
	VRDriverInput()->UpdateScalarComponent(joystickXHandle, LastArduinoIMU[0], 0);
	VRDriverInput()->UpdateScalarComponent(joystickYHandle, LastArduinoIMU[1], 0);
	VRDriverInput()->UpdateScalarComponent(trackpadXHandle, LastArduinoIMU[2], 0);
	VRDriverInput()->UpdateScalarComponent(trackpadYHandle, LastArduinoIMU[3], 0);

	vr::VRDriverInput()->UpdateBooleanComponent(aBtnHandle, aAction, 0);
	vr::VRDriverInput()->UpdateBooleanComponent(bBtnHandle, bAction, 0);
}

void ControllerDriver::Deactivate()
{
	driverId = k_unTrackedDeviceIndexInvalid;
}

void* ControllerDriver::GetComponent(const char* pchComponentNameAndVersion)
{
	//I found that if this method just returns null always, it works fine. But I'm leaving the if statement in since it doesn't hurt.
	//Check out the IVRDriverInput_Version declaration in openvr_driver.h. You can search that file for other _Version declarations 
	//to see other components that are available. You could also put a log in this class and output the value passed into this 
	//method to see what OpenVR is looking for.
	if (strcmp(IVRDriverInput_Version, pchComponentNameAndVersion) == 0)
	{
		return this;
	}
	return NULL;
}

void ControllerDriver::EnterStandby() {}

void ControllerDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) 
{
	if (unResponseBufferSize >= 1)
	{
		pchResponseBuffer[0] = 0;
	}
}