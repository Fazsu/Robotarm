
namespace Arduino_GUI
{
    partial class Form1
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
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.LEDButtonOn = new System.Windows.Forms.Button();
            this.LEDButtonOff = new System.Windows.Forms.Button();
            this.Servo1Button = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.Angleval = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.Angleval)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.PortName = "COM6";
            // 
            // LEDButtonOn
            // 
            this.LEDButtonOn.Location = new System.Drawing.Point(701, 386);
            this.LEDButtonOn.Name = "LEDButtonOn";
            this.LEDButtonOn.Size = new System.Drawing.Size(87, 23);
            this.LEDButtonOn.TabIndex = 1;
            this.LEDButtonOn.Text = "Turn on LED";
            this.LEDButtonOn.UseVisualStyleBackColor = true;
            this.LEDButtonOn.Click += new System.EventHandler(this.LEDButtonOn_Click);
            // 
            // LEDButtonOff
            // 
            this.LEDButtonOff.Location = new System.Drawing.Point(701, 415);
            this.LEDButtonOff.Name = "LEDButtonOff";
            this.LEDButtonOff.Size = new System.Drawing.Size(87, 23);
            this.LEDButtonOff.TabIndex = 2;
            this.LEDButtonOff.Text = "Turn off LED";
            this.LEDButtonOff.UseVisualStyleBackColor = true;
            this.LEDButtonOff.Click += new System.EventHandler(this.LEDButtonOff_Click);
            // 
            // Servo1Button
            // 
            this.Servo1Button.Location = new System.Drawing.Point(118, 56);
            this.Servo1Button.Name = "Servo1Button";
            this.Servo1Button.Size = new System.Drawing.Size(87, 23);
            this.Servo1Button.TabIndex = 0;
            this.Servo1Button.Text = "Send Angle";
            this.Servo1Button.UseVisualStyleBackColor = true;
            this.Servo1Button.Click += new System.EventHandler(this.Servo1Button_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 58);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 3;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(118, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Input value for Servo 1:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // Angleval
            // 
            this.Angleval.Location = new System.Drawing.Point(8, 85);
            this.Angleval.Maximum = 270;
            this.Angleval.Name = "Angleval";
            this.Angleval.Size = new System.Drawing.Size(197, 45);
            this.Angleval.TabIndex = 5;
            this.Angleval.Scroll += new System.EventHandler(this.AngleVal_Scroll);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Angleval);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.LEDButtonOff);
            this.Controls.Add(this.LEDButtonOn);
            this.Controls.Add(this.Servo1Button);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.Angleval)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button LEDButtonOn;
        private System.Windows.Forms.Button LEDButtonOff;
        private System.Windows.Forms.Button Servo1Button;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar Angleval;
    }
}

