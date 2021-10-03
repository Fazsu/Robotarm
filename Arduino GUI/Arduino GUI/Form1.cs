using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Arduino_GUI
{
    public partial class Form1 : Form
    {
        private string angle;           //String to hold the angle value
        public Form1()
        {
            InitializeComponent();
            serialPort1.Open();

        }

        private void Servo1Button_Click(object sender, EventArgs e)
        {
            //Move Servo with button
            string m1 = "S" + angle;

            if (string.IsNullOrEmpty(angle))
            {
                m1 = "S0";
            }

            serialPort1.Write(m1);


        }

        private void LEDButtonOn_Click(object sender, EventArgs e)
        {
            //Turn on LED with button
            serialPort1.Write("L");
        }

        private void LEDButtonOff_Click(object sender, EventArgs e)
        {
            //Turn off LED with button
            serialPort1.Write("l");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        int val;
            if (!int.TryParse(textBox1.Text, out val)) {
                MessageBox.Show("This is number only field!");
            } else if (textBox1.Text == null)
            {
                Angleval.Value = 0;
                
            }
            else
            {
                if (val > 270)
                {
                    angle = "270";
                    Angleval.Value = 0;
                }
                else
                {
                    Angleval.Value = val;
                    angle = val.ToString();
                }
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Input desired angle for Servo 1 (degrees).");
        }

        private void AngleVal_Scroll(object sender, EventArgs e)
        {
            angle = Angleval.Value.ToString();
            textBox1.Text = angle;
        }
    }
}
