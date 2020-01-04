using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ASCOM.GS_touch
{
    public partial class FCSH : Form
    {
        private ASCOM.GS_touch.MainWindow mainWindow;
        private int FCSH_Value;
        public FCSH(MainWindow mainWindow)
        {
            this.mainWindow = mainWindow;
            InitializeComponent();
            InitStatue();
            FCSH_Value = new int();
            FCSH_Value = mainWindow.askFCSH();
        }
        private void InitStatue()
        {
            if (FCSH_Value / 2 % 2 == 1) label_C.Text = "Cover Statue : Opened";
            else label_C.Text = "Cover Statue : Closed";

            if (FCSH_Value / 4 % 2 == 1) label_M.Text = "Mask Statue : Opened";
            else label_M.Text = "Mask Statue : Closed";

            if (FCSH_Value / 8 % 2 == 1) label_M.Text = "CCD Power : ON";
            else label_CC.Text = "CCD Power : OFF";

            if (FCSH_Value / 16 % 2 == 1) label_M.Text = "Mount Power : ON";
            else label_Mo.Text = "Mount : OFF";

        }

        private void FCSH_Load(object sender, EventArgs e)
        {

        }

        private void button_Cover_Click(object sender, EventArgs e)
        {
            mainWindow.applyFCSH(1);
            if (label_C.Text == "Cover Statue : Closed") label_C.Text = "Cover Statue : Opened";
            else label_C.Text = "Cover Statue : Closed";
        }

        private void button_Mask_Click(object sender, EventArgs e)
        {
            mainWindow.applyFCSH(2);
            if (label_M.Text == "Mask Statue : Closed") label_M.Text = "Mask Statue : Opened";
            else label_M.Text = "Mask Statue : Closed";
        }

        private void button_CCD_Click(object sender, EventArgs e)
        {
            mainWindow.applyFCSH(3);
            if (label_CC.Text == "CCD Power : OFF") label_CC.Text = "CCD Power : ON";
            else label_CC.Text = "CCD Power : OFF";
        }

        private void button_Mount_Click(object sender, EventArgs e)
        {
            mainWindow.applyFCSH(4);
            if (label_Mo.Text == "Mount Power : OFF") label_Mo.Text = "Mount Power : ON";
            else label_Mo.Text = "Mount Power : OFF";
        }
    }
}
