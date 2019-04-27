//tabs=4
// --------------------------------------------------------------------------------
// TODO fill in this information for your driver, then remove this line!
//
// ASCOM Focuser driver for GSfocus
//
// Description:	Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam 
//				nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam 
//				erat, sed diam voluptua. At vero eos et accusam et justo duo 
//				dolores et ea rebum. Stet clita kasd gubergren, no sea takimata 
//				sanctus est Lorem ipsum dolor sit amet.
//
// Implements:	ASCOM Focuser interface version: <To be completed by driver developer>
// Author:		(XXX) Your N. Here <your@email.here>
//
// Edit Log:
//
// Date			Who	Vers	Description
// -----------	---	-----	-------------------------------------------------------
// dd-mmm-yyyy	XXX	6.0.0	Initial edit, created from ASCOM driver template
// --------------------------------------------------------------------------------
// 17-apr-2017  Kiehyun Park 6.0.1  studying compatibility with absolute focusers


// This is used to define code in the template that is specific to one class implementation
// unused code canbe deleted and this definition removed.
#define Focuser

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Runtime.InteropServices;

using ASCOM;
using ASCOM.Astrometry;
using ASCOM.Astrometry.AstroUtils;
using ASCOM.Utilities;
using ASCOM.DeviceInterface;
using System.Globalization;
using System.Collections;
using System.IO.Ports;

namespace ASCOM.GSfocus
{
    //
    // Your driver's DeviceID is ASCOM.GSfocus.Focuser
    //
    // The Guid attribute sets the CLSID for ASCOM.GSfocus.Focuser
    // The ClassInterface/None addribute prevents an empty interface called
    // _GSfocus from being created and used as the [default] interface
    //
    // TODO Replace the not implemented exceptions with code to implement the function or
    // throw the appropriate ASCOM exception.
    //

    /// <summary>
    /// ASCOM Focuser Driver for GSfocus.
    /// </summary>
    [Guid("7731f491-e846-4050-8b86-2ff82c4265e1")]
    [ClassInterface(ClassInterfaceType.None)]
    public class Focuser : IFocuserV3
    {
        /// <summary>
        /// ASCOM DeviceID (COM ProgID) for this driver.
        /// The DeviceID is used by ASCOM applications to load the driver at runtime.
        /// </summary>
        internal static string driverID = "ASCOM.GSfocus.Focuser";
        // TODO Change the descriptive string for your driver then remove this line
        /// <summary>
        /// Driver description that displays in the ASCOM Chooser.
        /// </summary>
        private static string driverDescription = "ASCOM Focuser Driver for GSfocus.";

        internal static string comPortProfileName = "COM Port"; // Constants used for Profile persistence
        internal static string showUIProfileName = "Show Controller"; // GSGSGS_insert
       
        internal static string comPortDefault = "COM1";
        internal static string traceStateProfileName = "Trace Level";
        internal static string traceStateDefault = "true";
        internal static string showUIDefault = "true";// GSGSGS_insert


        internal static string comPort; // Variables to hold the currrent device configuration
        internal static bool traceState;  // GSGSGS_insert
        internal static bool showUI;  // GSGSGS_insert
        /// <summary>
        /// Private variable to hold the connected state
        /// </summary>
        private bool connectedState;

        /// <summary>
        /// Private variable to hold an ASCOM Utilities object
        /// </summary>
        private Util utilities;

        /// <summary>
        /// Private variable to hold an ASCOM AstroUtilities object to provide the Range method
        /// </summary>
        private AstroUtils astroUtilities;

        /// <summary>
        /// Variable to hold the trace logger object (creates a diagnostic log file with information that you specify)
        /// </summary>
        internal static TraceLogger tl;

        ///GSGSGS_start
        /// <summary>
        /// Private variable to hold the serial port object
        /// </summary>
        private SerialPort serialPort;

        /// <summary>
        /// Private variable that hold the value whether the focuser is moving or not
        /// </summary>
        private bool isMoving = false;

        private double temperature;

        private double humidity;

        private int motor;

        /// <summary>
        /// Private variable that hold the reference to the Main Window
        /// </summary>

        private MainWindow mainWindow;

        public event EventHandler<FocuserValueChangedEventArgs> FocuserValueChanged;

        public event EventHandler<FocuserStateChangedEventArgs> FocuserStateChanged;

        public event EventHandler<FocuserTemperatureChangedEventArgs> FocuserTemperatureChanged;

        public event EventHandler<FocuserHumidityChangedEventArgs> FocuserHumidityChanged;

        public event EventHandler<FocuserMotorChangedEventArgs> FocuserMotorChanged;

        ///GSGSGS_end


        /// <summary>
        /// Initializes a new instance of the <see cref="GSfocus"/> class.
        /// Must be public for COM registration.
        /// </summary>
        public Focuser()
        {
            tl = new TraceLogger("", "GSfocus");
            ReadProfile(); // Read device configuration from the ASCOM Profile store

            tl.LogMessage("Focuser", "Starting initialisation");

            connectedState = false; // Initialise connected to false
            utilities = new Util(); //Initialise util object
            astroUtilities = new AstroUtils(); // Initialise astro utilities object
            //TODO: Implement your additional construction here

            tl.LogMessage("Focuser", "Completed initialisation");
        }


        ///GSGSGS_start
        private string message;
        private string existingMessage;
        ///GSGSGS_end

        //
        // PUBLIC COM INTERFACE IFocuserV3 IMPLEMENTATION
        //


        ///GSGSGS_start
        #region Event Handling
        public virtual void OnFocuserValueChanged(FocuserValueChangedEventArgs e)
        {
            if (FocuserValueChanged != null)
            {
                FocuserValueChanged(this, e);
            }
        }

        public virtual void OnFocuserStateChanged(FocuserStateChangedEventArgs e)
        {
            if (FocuserStateChanged != null)
            {
                FocuserStateChanged(this, e);
            }
        }

        public virtual void OnFocuserTemperatureChanged(FocuserTemperatureChangedEventArgs e)
        {
            if (FocuserTemperatureChanged != null)
            {
                FocuserTemperatureChanged(this, e);
            }
        }

        public virtual void OnFocuserHumidityChanged(FocuserHumidityChangedEventArgs e)
        {
            if (FocuserHumidityChanged != null)
            {
                FocuserHumidityChanged(this, e);
            }
        }

        public virtual void OnFocuserMotorChanged(FocuserMotorChangedEventArgs e)
        {
            if (FocuserMotorChanged != null)
            {
                FocuserMotorChanged(this, e);
            }
        }
        #endregion
        
        ///GSGSGS_end


        #region Common properties and methods.

        /// <summary>
        /// Displays the Setup Dialog form.
        /// If the user clicks the OK button to dismiss the form, then
        /// the new settings are saved, otherwise the old values are reloaded.
        /// THIS IS THE ONLY PLACE WHERE SHOWING USER INTERFACE IS ALLOWED!
        /// </summary>
        public void SetupDialog()
        {
            // consider only showing the setup dialog if not connected
            // or call a different dialog if connected
            if (IsConnected)
                System.Windows.Forms.MessageBox.Show("Already connected, just press OK");

            using (SetupDialogForm F = new SetupDialogForm())
            {
                var result = F.ShowDialog();
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    WriteProfile(); // Persist device configuration values to the ASCOM Profile store
                }
            }
        }

        public ArrayList SupportedActions
        {
            get
            {
                tl.LogMessage("SupportedActions Get", "Returning empty arraylist");
                return new ArrayList();
            }
        }

        public string Action(string actionName, string actionParameters)
        {
            LogMessage("", "Action {0}, parameters {1} not implemented", actionName, actionParameters);
            throw new ASCOM.ActionNotImplementedException("Action " + actionName + " is not implemented by this driver");
        }

        public void CommandBlind(string command, bool raw)
        {
            CheckConnected("CommandBlind");
            // Call CommandString and return as soon as it finishes
            this.CommandString(command, raw);
            // or
            throw new ASCOM.MethodNotImplementedException("CommandBlind");
            // DO NOT have both these sections!  One or the other
        }

        public bool CommandBool(string command, bool raw)
        {
            CheckConnected("CommandBool");
            string ret = CommandString(command, raw);
            // TODO decode the return string and return true or false
            // or
            throw new ASCOM.MethodNotImplementedException("CommandBool");
            // DO NOT have both these sections!  One or the other
        }

        public string CommandString(string command, bool raw)
        {
            CheckConnected("CommandString");
            // it's a good idea to put all the low level communication with the device here,
            // then all communication calls this function
            // you need something to ensure that only one command is in progress at a time

            throw new ASCOM.MethodNotImplementedException("CommandString");
        }

        public void Dispose()
        {
            // Clean up the tracelogger and util objects
            tl.Enabled = false;
            tl.Dispose();
            tl = null;
            utilities.Dispose();
            utilities = null;
            astroUtilities.Dispose();
            astroUtilities = null;
        }

        ///GSGSGS_start
        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                message = SerialPort.ReadTo("#");
                System.Diagnostics.Debug.WriteLine(message);
                existingMessage = SerialPort.ReadExisting();
                System.Diagnostics.Debug.WriteLine("Existing" + existingMessage);

                if (message.Contains("POSITION"))
                {
                    focuserPosition = Convert.ToInt16(message.Split(':')[1]);
                    OnFocuserValueChanged(new FocuserValueChangedEventArgs(focuserPosition, focuserPosition));
                    OnFocuserStateChanged(new FocuserStateChangedEventArgs(false));
                    this.isMoving = false;
                }

                if (message.Contains("MOVING"))
                {
                    OnFocuserStateChanged(new FocuserStateChangedEventArgs(true));
                    this.isMoving = true;
                }

                if (message.Contains("TEMPERATURE"))
                {
                    this.temperature = Convert.ToDouble(message.Split(':')[1]);
                    OnFocuserTemperatureChanged(
                        new FocuserTemperatureChangedEventArgs(message.Split(':')[1] + " °C"));
                }

                if (message.Contains("HUMIDITY"))
                {
                    this.humidity = Convert.ToDouble(message.Split(':')[1]);
                    OnFocuserHumidityChanged(
                        new FocuserHumidityChangedEventArgs(message.Split(':')[1] + "%"));
                }

                ///if (message.Contains("SELECTEDMOTOR"))
                ///{
                    ///this.motor = Convert.ToInt32(message.Split(':')[1]);
                    ///OnFocuserMotorChanged(
                        ///new FocuserMotorChangedEventArgs(Convert.ToInt32(message.Split(':')[1])));
                ///}
            }
            catch (Exception ex)
            {
                tl.LogMessage("Encountered an Exeption", ex.Message);
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }

        }

        ///GSGSGS_end






        public bool Connected
        {
            get
            {
                LogMessage("Connected", "Get {0}", IsConnected);
                return IsConnected;
            }
            set
            {
                tl.LogMessage("Connected", "Set {0}", value);
                if (value == IsConnected)
                    return;

                if (value)
                {
                    connectedState = true;
                    LogMessage("Connected Set", "Connecting to port {0}", comPort);
                    // TODO connect to the device

                    ///GSGSGS_start
                    try
                    {
                        serialPort = new SerialPort(comPort, 115200);
                        serialPort.DataReceived += new SerialDataReceivedEventHandler(this.serialPort_DataReceived);
                        if (!serialPort.IsOpen)
                        {
                            serialPort.Open();
                        }
                        Action("F", "");    // GET POSITION
                        Action("k", "");    // GET TEMPERATURE AND HUMIDITY

                        // when we establish connection, set up the increment, step and speed
                        Action("I", "300"); // SET SPEED
                        Action("J", "300"); // SET MAXSPEED
                        Action("H", "300"); // SET ACCELERATION

                        // Show the Window for the EQFocuser here
                        if (showUI)
                        {
                        mainWindow = new MainWindow(this);
                        mainWindow.Show();
                        System.Diagnostics.Debug.WriteLine("Connected " + IsConnected.ToString());
                        }
                    }
                    catch (Exception e)
                    {
                        System.Diagnostics.Debug.WriteLine(e.Message);
                        connectedState = false;
                        tl.LogMessage("Cannot Open Serial Port", comPort);
                        if (serialPort.IsOpen)
                        {
                            serialPort.Close();
                        }
                    }

                    ///GSGSGS_end

                }
                else
                {
                    connectedState = false;
                    LogMessage("Connected Set", "Disconnecting from port {0}", comPort);
                    // TODO disconnect from the device
                    
                    ///GSGSGS_start
                    if (showUI)
                    {
                        mainWindow.Close();
                    }
                    if (serialPort.IsOpen)
                    {
                        serialPort.Close();
                    }
                    ///GSGSGS_end

                }
            }
        }


        ///GSGSGS_end
        public SerialPort SerialPort
        {
            get
            {
                return serialPort;
            }
        }
        ///GSGSGS_end
        

        public string Description
        {
            // TODO customise this device description
            get
            {
                tl.LogMessage("Description Get", driverDescription);
                return driverDescription;
            }
        }

        public string DriverInfo
        {
            get
            {
                Version version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
                // TODO customise this driver description
                string driverInfo = "Information about the driver itself. Version: " + String.Format(CultureInfo.InvariantCulture, "{0}.{1}", version.Major, version.Minor);
                tl.LogMessage("DriverInfo Get", driverInfo);
                return driverInfo;
            }
        }

        public string DriverVersion
        {
            get
            {
                Version version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
                string driverVersion = String.Format(CultureInfo.InvariantCulture, "{0}.{1}", version.Major, version.Minor);
                tl.LogMessage("DriverVersion Get", driverVersion);
                return driverVersion;
            }
        }

        public short InterfaceVersion
        {
            // set by the driver wizard
            get
            {
                LogMessage("InterfaceVersion Get", "3");
                return Convert.ToInt16("3");
            }
        }

        public string Name
        {
            get
            {
                string name = "Short driver name - please customise";
                tl.LogMessage("Name Get", name);
                return name;
            }
        }

        #endregion

        #region IFocuser Implementation

        private int focuserPosition = 0; // Class level variable to hold the current focuser position
        private const int focuserSteps = 10000;
        ///GSGSGS_start
        private double stepSize = 500;
        private int maxIncrement = 1000;
        ///GSGSGS_end


        public bool Absolute
        {
            get
            {
                tl.LogMessage("Absolute Get", true.ToString());
                return true; // This is an absolute focuser
            }
        }

        public void Halt()
        {
            tl.LogMessage("Halt", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("Halt");
        }

        public bool IsMoving
        {
            get
            {
                tl.LogMessage("IsMoving Get", false.ToString());
                return false; // This focuser always moves instantaneously so no need for IsMoving ever to be True
            }
        }

        public bool Link
        {
            get
            {
                tl.LogMessage("Link Get", this.Connected.ToString());
                return this.Connected; // Direct function to the connected method, the Link method is just here for backwards compatibility
            }
            set
            {
                tl.LogMessage("Link Set", value.ToString());
                this.Connected = value; // Direct function to the connected method, the Link method is just here for backwards compatibility
            }
        }

        public int MaxIncrement
        {
            get
            {
                tl.LogMessage("MaxIncrement Get", focuserSteps.ToString());
                return focuserSteps; // Maximum change in one move
            }

        }

        public int MaxStep
        {
            get
            {
                tl.LogMessage("MaxStep Get", focuserSteps.ToString());
                return focuserSteps; // Maximum extent of the focuser, so position range is 0 to 10,000
            }
        }

        public void Move(int Position)
        {
            tl.LogMessage("Move", Position.ToString());
            focuserPosition = Position; // Set the focuser position
        }

        public int Position
        {
            get
            {
                return focuserPosition; // Return the focuser position
            }
        }

        public double StepSize
        {
            get
            {
                tl.LogMessage("StepSize Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("StepSize", false);
            }
        }

        public bool TempComp
        {
            get
            {
                tl.LogMessage("TempComp Get", false.ToString());
                return false;
            }
            set
            {
                tl.LogMessage("TempComp Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("TempComp", false);
            }
        }

        public bool TempCompAvailable
        {
            get
            {
                tl.LogMessage("TempCompAvailable Get", false.ToString());
                return false; // Temperature compensation is not available in this driver
            }
        }

        public double Temperature
        {
            get
            {
                tl.LogMessage("Temperature Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("Temperature", false);
            }
        }

        #endregion

        #region Private properties and methods
        // here are some useful properties and methods that can be used as required
        // to help with driver development

        #region ASCOM Registration

        // Register or unregister driver for ASCOM. This is harmless if already
        // registered or unregistered. 
        //
        /// <summary>
        /// Register or unregister the driver with the ASCOM Platform.
        /// This is harmless if the driver is already registered/unregistered.
        /// </summary>
        /// <param name="bRegister">If <c>true</c>, registers the driver, otherwise unregisters it.</param>
        private static void RegUnregASCOM(bool bRegister)
        {
            using (var P = new ASCOM.Utilities.Profile())
            {
                P.DeviceType = "Focuser";
                if (bRegister)
                {
                    P.Register(driverID, driverDescription);
                }
                else
                {
                    P.Unregister(driverID);
                }
            }
        }

        /// <summary>
        /// This function registers the driver with the ASCOM Chooser and
        /// is called automatically whenever this class is registered for COM Interop.
        /// </summary>
        /// <param name="t">Type of the class being registered, not used.</param>
        /// <remarks>
        /// This method typically runs in two distinct situations:
        /// <list type="numbered">
        /// <item>
        /// In Visual Studio, when the project is successfully built.
        /// For this to work correctly, the option <c>Register for COM Interop</c>
        /// must be enabled in the project settings.
        /// </item>
        /// <item>During setup, when the installer registers the assembly for COM Interop.</item>
        /// </list>
        /// This technique should mean that it is never necessary to manually register a driver with ASCOM.
        /// </remarks>
        [ComRegisterFunction]
        public static void RegisterASCOM(Type t)
        {
            RegUnregASCOM(true);
        }

        /// <summary>
        /// This function unregisters the driver from the ASCOM Chooser and
        /// is called automatically whenever this class is unregistered from COM Interop.
        /// </summary>
        /// <param name="t">Type of the class being registered, not used.</param>
        /// <remarks>
        /// This method typically runs in two distinct situations:
        /// <list type="numbered">
        /// <item>
        /// In Visual Studio, when the project is cleaned or prior to rebuilding.
        /// For this to work correctly, the option <c>Register for COM Interop</c>
        /// must be enabled in the project settings.
        /// </item>
        /// <item>During uninstall, when the installer unregisters the assembly from COM Interop.</item>
        /// </list>
        /// This technique should mean that it is never necessary to manually unregister a driver from ASCOM.
        /// </remarks>
        [ComUnregisterFunction]
        public static void UnregisterASCOM(Type t)
        {
            RegUnregASCOM(false);
        }

        #endregion

        /// <summary>
        /// Returns true if there is a valid connection to the driver hardware
        /// </summary>
        private bool IsConnected
        {
            get
            {
                // TODO check that the driver hardware connection exists and is connected to the hardware
                return connectedState;
            }
        }

        /// <summary>
        /// Use this function to throw an exception if we aren't connected to the hardware
        /// </summary>
        /// <param name="message"></param>
        private void CheckConnected(string message)
        {
            if (!IsConnected)
            {
                throw new ASCOM.NotConnectedException(message);
            }
        }

        /// <summary>
        /// Read the device configuration from the ASCOM Profile store
        /// </summary>
        internal void ReadProfile()
        {
            using (Profile driverProfile = new Profile())
            {
                driverProfile.DeviceType = "Focuser";
                tl.Enabled = Convert.ToBoolean(driverProfile.GetValue(driverID, traceStateProfileName, string.Empty, traceStateDefault));
                comPort = driverProfile.GetValue(driverID, comPortProfileName, string.Empty, comPortDefault);
            }
        }

        /// <summary>
        /// Write the device configuration to the  ASCOM  Profile store
        /// </summary>
        internal void WriteProfile()
        {
            using (Profile driverProfile = new Profile())
            {
                driverProfile.DeviceType = "Focuser";
                driverProfile.WriteValue(driverID, traceStateProfileName, tl.Enabled.ToString());
                driverProfile.WriteValue(driverID, comPortProfileName, comPort.ToString());
            }
        }

        /// <summary>
        /// Log helper function that takes formatted strings and arguments
        /// </summary>
        /// <param name="identifier"></param>
        /// <param name="message"></param>
        /// <param name="args"></param>
        internal static void LogMessage(string identifier, string message, params object[] args)
        {
            var msg = string.Format(message, args);
            tl.LogMessage(identifier, msg);
        }
        #endregion
    }
}
