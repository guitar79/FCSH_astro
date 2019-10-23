namespace ASCOM.test0816
{
    partial class ResetForm
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
            this.label_ResetPosition = new System.Windows.Forms.Label();
            this.label_ResetMS = new System.Windows.Forms.Label();
            this.textBox_ResetPosition = new System.Windows.Forms.TextBox();
            this.textBox_ResetMS = new System.Windows.Forms.TextBox();
            this.button_Reset = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label_ResetPosition
            // 
            this.label_ResetPosition.AutoSize = true;
            this.label_ResetPosition.Location = new System.Drawing.Point(32, 34);
            this.label_ResetPosition.Name = "label_ResetPosition";
            this.label_ResetPosition.Size = new System.Drawing.Size(81, 12);
            this.label_ResetPosition.TabIndex = 0;
            this.label_ResetPosition.Text = "Position want";
            // 
            // label_ResetMS
            // 
            this.label_ResetMS.AutoSize = true;
            this.label_ResetMS.Location = new System.Drawing.Point(27, 62);
            this.label_ResetMS.Name = "label_ResetMS";
            this.label_ResetMS.Size = new System.Drawing.Size(86, 12);
            this.label_ResetMS.TabIndex = 1;
            this.label_ResetMS.Text = "MicroStepping";
            // 
            // textBox_ResetPosition
            // 
            this.textBox_ResetPosition.Location = new System.Drawing.Point(134, 28);
            this.textBox_ResetPosition.Name = "textBox_ResetPosition";
            this.textBox_ResetPosition.Size = new System.Drawing.Size(100, 21);
            this.textBox_ResetPosition.TabIndex = 2;
            // 
            // textBox_ResetMS
            // 
            this.textBox_ResetMS.Location = new System.Drawing.Point(134, 56);
            this.textBox_ResetMS.Name = "textBox_ResetMS";
            this.textBox_ResetMS.Size = new System.Drawing.Size(100, 21);
            this.textBox_ResetMS.TabIndex = 3;            
            // 
            // button_Reset
            // 
            this.button_Reset.Location = new System.Drawing.Point(134, 96);
            this.button_Reset.Name = "button_Reset";
            this.button_Reset.Size = new System.Drawing.Size(100, 23);
            this.button_Reset.TabIndex = 4;
            this.button_Reset.Text = "Apply";
            this.button_Reset.UseVisualStyleBackColor = true;
            this.button_Reset.Click += new System.EventHandler(this.button_Reset_Click);
            // 
            // ResetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(264, 131);
            this.Controls.Add(this.button_Reset);
            this.Controls.Add(this.textBox_ResetMS);
            this.Controls.Add(this.textBox_ResetPosition);
            this.Controls.Add(this.label_ResetMS);
            this.Controls.Add(this.label_ResetPosition);
            this.Name = "ResetForm";
            this.Text = "Reset Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_ResetPosition;
        private System.Windows.Forms.Label label_ResetMS;
        private System.Windows.Forms.TextBox textBox_ResetPosition;
        private System.Windows.Forms.TextBox textBox_ResetMS;
        private System.Windows.Forms.Button button_Reset;
    }
}