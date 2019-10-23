using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using ASCOM.Utilities;
using ASCOM.test0816;

using System.IO.Ports;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Linq;

namespace ASCOM.test0816
{
    [ComVisible(false)]					// Form not registered for COM!
    public partial class SetupDialogForm : Form
    {
        public SetupDialogForm()
        {
            InitializeComponent();
            // Initialise current values of user settings from the ASCOM Profile
            InitUI();
        }

        private void cmdOK_Click(object sender, EventArgs e) // OK button event handler
        {
            // Place any validation constraint checks here
            // Update the state variables with results from the dialogue
            Focuser.comPort = (string)comboBoxComPort.SelectedItem;
            Focuser.showUI = showUI.Checked;

            //String modeString = comboBoxMS.SelectedIndex.ToString().Substring(0,0);
            //int mode = Convert.ToInt32(modeString);
            //Focuser.MicroSteppingMode = mode;
        }

        private void cmdCancel_Click(object sender, EventArgs e) // Cancel button event handler
        {
            Close();
        }

        private void BrowseToAscom(object sender, EventArgs e) // Click on ASCOM logo event handler
        {
            try
            {
                System.Diagnostics.Process.Start("http://ascom-standards.org/");
            }
            catch (System.ComponentModel.Win32Exception noBrowser)
            {
                if (noBrowser.ErrorCode == -2147467259)
                    MessageBox.Show(noBrowser.Message);
            }
            catch (System.Exception other)
            {
                MessageBox.Show(other.Message);
            }
        }

        private void InitUI()
        {
            chkTrace.Checked = Focuser.traceState;
            showUI.Checked = Focuser.showUI;

            // set the list of com ports to those that are currently available
            comboBoxComPort.Items.Clear();

            String[] ports = SerialPort.GetPortNames();
            foreach(string port in ports)
            {
                Debug.WriteLine("Port here " + port);
                if(Detect_TFocuser(port))
                {
                    comboBoxComPort.Items.Add(port);
                    Detect.Visible = true;
                }
            }

            comboBoxComPort.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());      // use System.IO because it's static
            // select the current port if possible
            if (comboBoxComPort.Items.Contains(Focuser.comPort))
            {
                comboBoxComPort.SelectedItem = Focuser.comPort;
            }

            if (comboBoxMS.DropDownWidth == 0)
            {
                comboBoxMS.Items.Add("1 (full step)");
                comboBoxMS.Items.Add("2 (1/2 step)");
                comboBoxMS.Items.Add("3 (1/4 step)");
                comboBoxMS.Items.Add("4 (1/8 step)");
                comboBoxMS.SelectedIndex = 0;
            }
        }

        private bool Detect_TFocuser(string portName)
        {
            SerialPort testPort = new SerialPort(portName, 115200);
            try
            {
                testPort.Open();
                testPort.WriteLine("Z");

                Thread.Sleep(100);
                string returnMessage = testPort.ReadExisting().ToString();

                testPort.Close();
                Debug.WriteLine(returnMessage);

                if (returnMessage.Contains("EQEQFOCUSER_STEPPER") || returnMessage.Contains("POSITION"))
                {
                    Focuser.motorDriver = Focuser.stepperMotor;
                    Detect.Visible = true;
                    return true;
                }
                else return false;

            }
            catch(Exception e)
            {
                Debug.WriteLine(e.Message);
                return false;
            }
        }

        private void chkTrace_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void showUI_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void Reconnect_Click(object sender, EventArgs e)
        {
            this.InitUI();
        }

        private void MS_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}