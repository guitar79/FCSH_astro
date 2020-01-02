namespace ASCOM.GS_touch
{
    partial class FCSH
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
            this.groupBox_Statue = new System.Windows.Forms.GroupBox();
            this.label_Mo = new System.Windows.Forms.Label();
            this.label_CC = new System.Windows.Forms.Label();
            this.label_M = new System.Windows.Forms.Label();
            this.label_C = new System.Windows.Forms.Label();
            this.button_Cover = new System.Windows.Forms.Button();
            this.button_Mask = new System.Windows.Forms.Button();
            this.button_CCD = new System.Windows.Forms.Button();
            this.button_Mount = new System.Windows.Forms.Button();
            this.groupBox_Control = new System.Windows.Forms.GroupBox();
            this.groupBox_Statue.SuspendLayout();
            this.groupBox_Control.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox_Statue
            // 
            this.groupBox_Statue.Controls.Add(this.label_Mo);
            this.groupBox_Statue.Controls.Add(this.label_CC);
            this.groupBox_Statue.Controls.Add(this.label_M);
            this.groupBox_Statue.Controls.Add(this.label_C);
            this.groupBox_Statue.Location = new System.Drawing.Point(12, 12);
            this.groupBox_Statue.Name = "groupBox_Statue";
            this.groupBox_Statue.Size = new System.Drawing.Size(175, 146);
            this.groupBox_Statue.TabIndex = 0;
            this.groupBox_Statue.TabStop = false;
            this.groupBox_Statue.Text = "Current Statue";
            // 
            // label_Mo
            // 
            this.label_Mo.AutoSize = true;
            this.label_Mo.Location = new System.Drawing.Point(7, 120);
            this.label_Mo.Name = "label_Mo";
            this.label_Mo.Size = new System.Drawing.Size(92, 12);
            this.label_Mo.TabIndex = 3;
            this.label_Mo.Text = "Mount Power : ";
            // 
            // label_CC
            // 
            this.label_CC.AutoSize = true;
            this.label_CC.Location = new System.Drawing.Point(16, 90);
            this.label_CC.Name = "label_CC";
            this.label_CC.Size = new System.Drawing.Size(79, 12);
            this.label_CC.TabIndex = 1;
            this.label_CC.Text = "CCD Power :";
            // 
            // label_M
            // 
            this.label_M.AutoSize = true;
            this.label_M.Location = new System.Drawing.Point(13, 60);
            this.label_M.Name = "label_M";
            this.label_M.Size = new System.Drawing.Size(87, 12);
            this.label_M.TabIndex = 2;
            this.label_M.Text = "Mask Statue : ";
            // 
            // label_C
            // 
            this.label_C.AutoSize = true;
            this.label_C.Location = new System.Drawing.Point(11, 30);
            this.label_C.Name = "label_C";
            this.label_C.Size = new System.Drawing.Size(89, 12);
            this.label_C.TabIndex = 1;
            this.label_C.Text = "Cover Statue : ";
            // 
            // button_Cover
            // 
            this.button_Cover.Location = new System.Drawing.Point(6, 21);
            this.button_Cover.Name = "button_Cover";
            this.button_Cover.Size = new System.Drawing.Size(75, 26);
            this.button_Cover.TabIndex = 2;
            this.button_Cover.Text = "Cover";
            this.button_Cover.UseVisualStyleBackColor = true;
            this.button_Cover.Click += new System.EventHandler(this.button_Cover_Click);
            // 
            // button_Mask
            // 
            this.button_Mask.Location = new System.Drawing.Point(6, 51);
            this.button_Mask.Name = "button_Mask";
            this.button_Mask.Size = new System.Drawing.Size(75, 26);
            this.button_Mask.TabIndex = 3;
            this.button_Mask.Text = "Mask";
            this.button_Mask.UseVisualStyleBackColor = true;
            this.button_Mask.Click += new System.EventHandler(this.button_Mask_Click);
            // 
            // button_CCD
            // 
            this.button_CCD.Location = new System.Drawing.Point(6, 81);
            this.button_CCD.Name = "button_CCD";
            this.button_CCD.Size = new System.Drawing.Size(75, 26);
            this.button_CCD.TabIndex = 4;
            this.button_CCD.Text = "CCD";
            this.button_CCD.UseVisualStyleBackColor = true;
            this.button_CCD.Click += new System.EventHandler(this.button_CCD_Click);
            // 
            // button_Mount
            // 
            this.button_Mount.Location = new System.Drawing.Point(6, 111);
            this.button_Mount.Name = "button_Mount";
            this.button_Mount.Size = new System.Drawing.Size(75, 26);
            this.button_Mount.TabIndex = 5;
            this.button_Mount.Text = "Mount";
            this.button_Mount.UseVisualStyleBackColor = true;
            this.button_Mount.Click += new System.EventHandler(this.button_Mount_Click);
            // 
            // groupBox_Control
            // 
            this.groupBox_Control.Controls.Add(this.button_Cover);
            this.groupBox_Control.Controls.Add(this.button_Mount);
            this.groupBox_Control.Controls.Add(this.button_Mask);
            this.groupBox_Control.Controls.Add(this.button_CCD);
            this.groupBox_Control.Location = new System.Drawing.Point(193, 12);
            this.groupBox_Control.Name = "groupBox_Control";
            this.groupBox_Control.Size = new System.Drawing.Size(87, 146);
            this.groupBox_Control.TabIndex = 6;
            this.groupBox_Control.TabStop = false;
            this.groupBox_Control.Text = "Control";
            // 
            // FCSH
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 165);
            this.Controls.Add(this.groupBox_Control);
            this.Controls.Add(this.groupBox_Statue);
            this.Name = "FCSH";
            this.Text = "FCSH Statue";
            this.Load += new System.EventHandler(this.FCSH_Load);
            this.groupBox_Statue.ResumeLayout(false);
            this.groupBox_Statue.PerformLayout();
            this.groupBox_Control.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox_Statue;
        private System.Windows.Forms.Label label_Mo;
        private System.Windows.Forms.Label label_CC;
        private System.Windows.Forms.Label label_M;
        private System.Windows.Forms.Label label_C;
        private System.Windows.Forms.Button button_Cover;
        private System.Windows.Forms.Button button_Mask;
        private System.Windows.Forms.Button button_CCD;
        private System.Windows.Forms.Button button_Mount;
        private System.Windows.Forms.GroupBox groupBox_Control;
    }
}