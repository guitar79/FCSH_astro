using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ASCOM.test0816
{
    public partial class ResetForm : Form
    {
        ASCOM.test0816.Focuser focuser;
        private ASCOM.test0816.MainWindow mainWindow;
        public ResetForm(MainWindow mainWindow)
        {
            InitializeComponent();
            textBox_ResetPosition.Text = focuser.Position.ToString();
            textBox_ResetMS.Text = focuser.MicroStepMode.ToString();
        }

        internal const int MaxStep = 500000;

        private void button_Reset_Click(object sender, EventArgs e)
        {
            if(Convert.ToInt32(textBox_ResetPosition.Text) > MaxStep || Convert.ToInt32(textBox_ResetPosition.Text)<0)
            {
                System.Windows.Forms.MessageBox.Show("Reset Position Out of Range");
            }
            else if(Convert.ToInt32(textBox_ResetMS.Text)>4 || Convert.ToInt32(textBox_ResetMS.Text)<0)
            {
                System.Windows.Forms.MessageBox.Show("Reset MicroStepping Mode Out of Range");
            }
            else
            {
                focuser.CommandString("G", true);
                mainWindow.SetCurrentPosition(Convert.ToInt32(textBox_ResetPosition));
                Focuser.MicroSteppingMode = Convert.ToInt32(textBox_ResetMS.Text);
            }
        }

    }
}
