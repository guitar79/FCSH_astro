﻿namespace ASCOM.GSfocus
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnFastReverse = new System.Windows.Forms.Button();
            this.btnReverse = new System.Windows.Forms.Button();
            this.btnForward = new System.Windows.Forms.Button();
            this.btnFastForward = new System.Windows.Forms.Button();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxCurrentPosition = new System.Windows.Forms.TextBox();
            this.btnMoveTo = new System.Windows.Forms.Button();
            this.textBoxMoveToPosition = new System.Windows.Forms.TextBox();
            this.btnReset = new System.Windows.Forms.Button();
            this.lblAction = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.numericUpDown5 = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.numericUpDownBacklash = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.numericUpDown4 = new System.Windows.Forms.NumericUpDown();
            this.numericUpDown3 = new System.Windows.Forms.NumericUpDown();
            this.numericUpDown2 = new System.Windows.Forms.NumericUpDown();
            this.txtBoxTemperature = new System.Windows.Forms.TextBox();
            this.txtBoxHumidity = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.btnStop = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnShowAdvanced = new System.Windows.Forms.Button();
            this.checkBoxReverse = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBacklash)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnFastReverse
            // 
            this.btnFastReverse.Location = new System.Drawing.Point(20, 71);
            this.btnFastReverse.Name = "btnFastReverse";
            this.btnFastReverse.Size = new System.Drawing.Size(39, 19);
            this.btnFastReverse.TabIndex = 1;
            this.btnFastReverse.Text = "<<";
            this.btnFastReverse.UseVisualStyleBackColor = true;
            this.btnFastReverse.Click += new System.EventHandler(this.btnFastReverse_Click);
            // 
            // btnReverse
            // 
            this.btnReverse.Location = new System.Drawing.Point(65, 71);
            this.btnReverse.Name = "btnReverse";
            this.btnReverse.Size = new System.Drawing.Size(35, 19);
            this.btnReverse.TabIndex = 2;
            this.btnReverse.Text = "<";
            this.btnReverse.UseVisualStyleBackColor = true;
            this.btnReverse.Click += new System.EventHandler(this.btnReverse_Click);
            // 
            // btnForward
            // 
            this.btnForward.Location = new System.Drawing.Point(191, 71);
            this.btnForward.Name = "btnForward";
            this.btnForward.Size = new System.Drawing.Size(39, 19);
            this.btnForward.TabIndex = 3;
            this.btnForward.Text = ">";
            this.btnForward.UseVisualStyleBackColor = true;
            this.btnForward.Click += new System.EventHandler(this.btnForward_Click);
            // 
            // btnFastForward
            // 
            this.btnFastForward.Location = new System.Drawing.Point(236, 71);
            this.btnFastForward.Name = "btnFastForward";
            this.btnFastForward.Size = new System.Drawing.Size(39, 19);
            this.btnFastForward.TabIndex = 4;
            this.btnFastForward.Text = ">>";
            this.btnFastForward.UseVisualStyleBackColor = true;
            this.btnFastForward.Click += new System.EventHandler(this.btnFastForward_Click);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(198, 96);
            this.numericUpDown1.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(77, 21);
            this.numericUpDown1.TabIndex = 5;
            this.numericUpDown1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown1.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            //this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);  ///GSGSGS_edited

            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "Current Position";
            // 
            // textBoxCurrentPosition
            // 
            this.textBoxCurrentPosition.Location = new System.Drawing.Point(119, 11);
            this.textBoxCurrentPosition.Name = "textBoxCurrentPosition";
            this.textBoxCurrentPosition.ReadOnly = true;
            this.textBoxCurrentPosition.Size = new System.Drawing.Size(156, 21);
            this.textBoxCurrentPosition.TabIndex = 7;
            this.textBoxCurrentPosition.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.textBoxCurrentPosition.TextChanged += new System.EventHandler(this.textBoxCurrentPosition_TextChanged);
            // 
            // btnMoveTo
            // 
            this.btnMoveTo.Location = new System.Drawing.Point(21, 120);
            this.btnMoveTo.Name = "btnMoveTo";
            this.btnMoveTo.Size = new System.Drawing.Size(79, 21);
            this.btnMoveTo.TabIndex = 8;
            this.btnMoveTo.Text = "Move To";
            this.btnMoveTo.UseVisualStyleBackColor = true;
            this.btnMoveTo.Click += new System.EventHandler(this.btnMoveTo_Click);
            // 
            // textBoxMoveToPosition
            // 
            this.textBoxMoveToPosition.Location = new System.Drawing.Point(107, 122);
            this.textBoxMoveToPosition.Name = "textBoxMoveToPosition";
            this.textBoxMoveToPosition.Size = new System.Drawing.Size(68, 21);
            this.textBoxMoveToPosition.TabIndex = 9;
            this.textBoxMoveToPosition.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(21, 147);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(152, 21);
            this.btnReset.TabIndex = 10;
            this.btnReset.Text = "Reset to Position to 0";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // lblAction
            // 
            this.lblAction.AutoSize = true;
            this.lblAction.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblAction.Location = new System.Drawing.Point(180, 151);
            this.lblAction.Name = "lblAction";
            this.lblAction.Size = new System.Drawing.Size(65, 13);
            this.lblAction.TabIndex = 11;
            this.lblAction.Text = "READY....";
            this.lblAction.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lblAction_MouseClick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.numericUpDown5);
            this.groupBox1.Controls.Add(this.label10);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.numericUpDownBacklash);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.numericUpDown4);
            this.groupBox1.Controls.Add(this.numericUpDown3);
            this.groupBox1.Controls.Add(this.numericUpDown2);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(259, 170);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Advanced";
            // 
            // numericUpDown5
            // 
            this.numericUpDown5.Increment = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpDown5.Location = new System.Drawing.Point(118, 147);
            this.numericUpDown5.Margin = new System.Windows.Forms.Padding(2);
            this.numericUpDown5.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.numericUpDown5.Minimum = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            this.numericUpDown5.Name = "numericUpDown5";
            this.numericUpDown5.Size = new System.Drawing.Size(142, 21);
            this.numericUpDown5.TabIndex = 11;
            this.numericUpDown5.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown5.Value = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 148);
            this.label10.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(79, 12);
            this.label10.TabIndex = 10;
            this.label10.Text = "Max Position";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 120);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(86, 12);
            this.label9.TabIndex = 7;
            this.label9.Text = "Backlash Step";
            // 
            // numericUpDownBacklash
            // 
            this.numericUpDownBacklash.Location = new System.Drawing.Point(118, 120);
            this.numericUpDownBacklash.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.numericUpDownBacklash.Name = "numericUpDownBacklash";
            this.numericUpDownBacklash.Size = new System.Drawing.Size(140, 21);
            this.numericUpDownBacklash.TabIndex = 6;
            this.numericUpDownBacklash.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDownBacklash.ValueChanged += new System.EventHandler(this.numericUpDownBacklash_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 96);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "Speed";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Max Speed";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(104, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Max Acceleration";
            // 
            // numericUpDown4
            // 
            this.numericUpDown4.Location = new System.Drawing.Point(118, 96);
            this.numericUpDown4.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numericUpDown4.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDown4.Name = "numericUpDown4";
            this.numericUpDown4.Size = new System.Drawing.Size(140, 21);
            this.numericUpDown4.TabIndex = 2;
            this.numericUpDown4.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown4.Value = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.numericUpDown4.ValueChanged += new System.EventHandler(this.numericUpDown4_ValueChanged);
            // 
            // numericUpDown3
            // 
            this.numericUpDown3.Location = new System.Drawing.Point(117, 71);
            this.numericUpDown3.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numericUpDown3.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDown3.Name = "numericUpDown3";
            this.numericUpDown3.Size = new System.Drawing.Size(140, 21);
            this.numericUpDown3.TabIndex = 1;
            this.numericUpDown3.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown3.Value = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.numericUpDown3.ValueChanged += new System.EventHandler(this.numericUpDown3_ValueChanged);
            // 
            // numericUpDown2
            // 
            this.numericUpDown2.Location = new System.Drawing.Point(117, 46);
            this.numericUpDown2.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.numericUpDown2.Minimum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.numericUpDown2.Name = "numericUpDown2";
            this.numericUpDown2.Size = new System.Drawing.Size(140, 21);
            this.numericUpDown2.TabIndex = 0;
            this.numericUpDown2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown2.Value = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.numericUpDown2.ValueChanged += new System.EventHandler(this.numericUpDown2_ValueChanged);
            // 
            // txtBoxTemperature
            // 
            this.txtBoxTemperature.Location = new System.Drawing.Point(159, 47);
            this.txtBoxTemperature.Name = "txtBoxTemperature";
            this.txtBoxTemperature.ReadOnly = true;
            this.txtBoxTemperature.Size = new System.Drawing.Size(116, 21);
            this.txtBoxTemperature.TabIndex = 13;
            this.txtBoxTemperature.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // txtBoxHumidity
            // 
            this.txtBoxHumidity.Location = new System.Drawing.Point(20, 47);
            this.txtBoxHumidity.Name = "txtBoxHumidity";
            this.txtBoxHumidity.ReadOnly = true;
            this.txtBoxHumidity.Size = new System.Drawing.Size(116, 21);
            this.txtBoxHumidity.TabIndex = 14;
            this.txtBoxHumidity.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(56, 32);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 12);
            this.label5.TabIndex = 15;
            this.label5.Text = "Humidity";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(176, 32);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 12);
            this.label6.TabIndex = 16;
            this.label6.Text = "Temperature";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 3000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(109, 71);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(76, 21);
            this.btnStop.TabIndex = 17;
            this.btnStop.Text = "STOP";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 98);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(157, 12);
            this.label7.TabIndex = 18;
            this.label7.Text = "Single Step (x2 for Double)";
            // 
            // label8
            // 
            this.label8.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(132, 360);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(157, 12);
            this.label8.TabIndex = 19;
            this.label8.Text = "EQ ASCOM Focuser Driver";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Location = new System.Drawing.Point(21, 173);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(268, 176);
            this.panel1.TabIndex = 20;
            // 
            // btnShowAdvanced
            // 
            this.btnShowAdvanced.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnShowAdvanced.Location = new System.Drawing.Point(21, 356);
            this.btnShowAdvanced.Name = "btnShowAdvanced";
            this.btnShowAdvanced.Size = new System.Drawing.Size(87, 21);
            this.btnShowAdvanced.TabIndex = 21;
            this.btnShowAdvanced.Text = "Advanced";
            this.btnShowAdvanced.UseVisualStyleBackColor = true;
            this.btnShowAdvanced.Click += new System.EventHandler(this.btnShowAdvanced_Click);
            // 
            // checkBoxReverse
            // 
            this.checkBoxReverse.AutoSize = true;
            this.checkBoxReverse.Location = new System.Drawing.Point(182, 125);
            this.checkBoxReverse.Name = "checkBoxReverse";
            this.checkBoxReverse.Size = new System.Drawing.Size(80, 16);
            this.checkBoxReverse.TabIndex = 22;
            this.checkBoxReverse.Text = "REVERSE";
            this.checkBoxReverse.UseVisualStyleBackColor = true;
            this.checkBoxReverse.CheckedChanged += new System.EventHandler(this.checkBoxReverse_CheckedChanged);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(303, 383);
            this.Controls.Add(this.checkBoxReverse);
            this.Controls.Add(this.btnShowAdvanced);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txtBoxHumidity);
            this.Controls.Add(this.txtBoxTemperature);
            this.Controls.Add(this.lblAction);
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.textBoxMoveToPosition);
            this.Controls.Add(this.btnMoveTo);
            this.Controls.Add(this.textBoxCurrentPosition);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.numericUpDown1);
            this.Controls.Add(this.btnFastForward);
            this.Controls.Add(this.btnForward);
            this.Controls.Add(this.btnReverse);
            this.Controls.Add(this.btnFastReverse);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainWindow";
            this.ShowIcon = false;
            this.Text = "GSfocus controll window";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainWindow_FormClosed);
            this.Load += new System.EventHandler(this.MainWindow_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBacklash)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).EndInit();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnFastReverse;
        private System.Windows.Forms.Button btnReverse;
        private System.Windows.Forms.Button btnForward;
        private System.Windows.Forms.Button btnFastForward;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxCurrentPosition;
        private System.Windows.Forms.Button btnMoveTo;
        private System.Windows.Forms.TextBox textBoxMoveToPosition;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.Label lblAction;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numericUpDown4;
        private System.Windows.Forms.NumericUpDown numericUpDown3;
        private System.Windows.Forms.NumericUpDown numericUpDown2;
        private System.Windows.Forms.TextBox txtBoxTemperature;
        private System.Windows.Forms.TextBox txtBoxHumidity;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown numericUpDownBacklash;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnShowAdvanced;
        private System.Windows.Forms.CheckBox checkBoxReverse;
        private System.Windows.Forms.NumericUpDown numericUpDown5;
        private System.Windows.Forms.Label label10;
    }
}