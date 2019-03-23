using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RGBMonitorInterface {
    public partial class Form1 : Form {

        private string EEPROMdata;
        private bool setGUIActive;

        public Form1() {
            InitializeComponent();
            serialPort.Open();
            setGUI();
        }

        private void trackBar_brightness_MouseUp( object sender, MouseEventArgs e ) {
            serialPort.Write( "b" + trackBar_brightness.Value );
            textBox_sent.AppendText( "b" + trackBar_brightness.Value + Environment.NewLine );
        }

        private void trackBar_speed_MouseUp( object sender, MouseEventArgs e ) {
            double delay = ( double )30 - ( double )trackBar_speed.Value / 3.3333333333;
            if( delay < 1 )
                delay = 1;
            Send( "d" + ( int )delay );
        }

        private void button_send_Click( object sender, EventArgs e ) {
            Send( textBox_send.Text );
            textBox_send.Clear();
        }

        private void Form1_FormClosing( object sender, FormClosingEventArgs e ) {
            serialPort.Close();
        }

        private void serialPort_DataReceived( object sender, System.IO.Ports.SerialDataReceivedEventArgs e ) {
            if( !setGUIActive )
            {
                Thread.Sleep( 100 );
                AppendTextBox( serialPort.ReadExisting() );
            }
        }

        private void AppendTextBox( string value ) {
            try
            {
                if( InvokeRequired )
                {
                    this.Invoke( new Action<string>( AppendTextBox ), new object[] { value } );
                    return;
                }

                textBox_received.AppendText( value + Environment.NewLine );
                if( value.Contains( "to sleep" ) && !setGUIActive )
                {
                    radioButton_sleep.Checked = true;
                }
            }
            catch { }
        }

        private void Send( string val ) {
            serialPort.Write( val );
            textBox_sent.AppendText( val + Environment.NewLine );
        }

        private void radioButton_status_CheckedChanged( object sender, EventArgs e ) {
            RadioButton rb = sender as RadioButton;
            if( rb.Checked && !setGUIActive )
            {

                if( radioButton_olvasolampa.Checked )
                {
                    Send( "s0" );
                }
                else if( radioButton_lampa.Checked )
                {
                    Send( "s1" );
                }
                else if( radioButton_olvasrainbow.Checked )
                {
                    Send( "s2" );
                }
                else if( radioButton_wheel.Checked )
                {
                    Send( "s3" );
                }
                else if( radioButton_sleep.Checked )
                {
                    Send( "s4" );
                }
            }
        }

        private void setGUI() {
            setGUIActive = true;

            serialPort.Write( "w" );
            Thread.Sleep( 200 );
            EEPROMdata = serialPort.ReadExisting();
            EEPROMdata = EEPROMdata.Replace( "\r", "" );
            string[] lines = EEPROMdata.Split( '\n' );
            lines[2] = lines[2].Replace( "State: ", "" );
            lines[3] = lines[3].Replace( "Delay: ", "" );
            lines[4] = lines[4].Replace( "Bright: ", "" );
            lines[5] = lines[5].Replace( "Timeout: ", "" );
            lines[6] = lines[6].Replace( "SlpBright: ", "" );
            lines[7] = lines[7].Replace( "SlpDelay: ", "" );

            switch( lines[2] )
            {
                case "0":
                    radioButton_olvasolampa.Checked = true;
                    break;
                case "1":
                    radioButton_lampa.Checked = true;
                    break;
                case "2":
                    radioButton_olvasrainbow.Checked = true;
                    break;
                case "3":
                    radioButton_wheel.Checked = true;
                    break;
                case "4":
                    radioButton_sleep.Checked = true;
                    break;
            }
            int seged = Convert.ToInt32( lines[3] );
            trackBar_speed.Value = ( int )( ( 30 - ( double )seged ) * 3.3333333333 );

            //delay = 30 - trackBar_speed.Value / 3.3333333333;
            //-value / 3,3333 = delay - 30
            // value / 3,3333 = 30 - delay
            // value = ( 30 - delay ) * 3,33333

            seged = Convert.ToInt32( lines[4] );
            trackBar_brightness.Value = seged;


            /*
            lines[5];
            lines[6];
            lines[7];
            */
            setGUIActive = false;
        }
    }
}
