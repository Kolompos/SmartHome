using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RGBMonitorInterface {
    public partial class Form : System.Windows.Forms.Form {

        private string EEPROMdata;
        private bool getDataActive, reallyClose;

        byte state, cycleDelay, brightness, sleepbrightness, sleepcycleDelay;
        long sleepTimeout;

        public Form() {
            InitializeComponent();
            GetData();
        }

        private void trackBar_brightness_MouseUp( object sender, MouseEventArgs e ) {
            setBrightness( trackBar_brightness.Value );
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

        private void serialPort_DataReceived( object sender, System.IO.Ports.SerialDataReceivedEventArgs e ) {
            if( !getDataActive )
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
                if( value.Contains( "to sleep" ) && !getDataActive )
                {
                    radioButton_sleep.Checked = true;
                }
            }
            catch { }
        }

        private void notifyIcon1_DoubleClick( object sender, EventArgs e ) {
            this.Show();
            this.WindowState = FormWindowState.Normal;
        }

        private void closeToolStripMenuItem_Click( object sender, EventArgs e ) {
            reallyClose = true;
            this.Close();
        }

        private void Form1_FormClosing( object sender, FormClosingEventArgs e ) {
            if( reallyClose )
            {
                serialPort.Close();
            }
            else
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        private void lightToolStripMenuItem_Click( object sender, EventArgs e ) {
            this.radioButton_lampa.Checked = true;
        }

        private void mixedToolStripMenuItem_Click( object sender, EventArgs e ) {
            this.radioButton_olvasrainbow.Checked = true;
        }

        private void wheelToolStripMenuItem_Click( object sender, EventArgs e ) {
            this.radioButton_wheel.Checked = true;
        }

        private void lowestToolStripMenuItem_Click( object sender, EventArgs e ) {
            setBrightness( 10 );
        }

        private void lowToolStripMenuItem_Click( object sender, EventArgs e ) {
            setBrightness( 70 );
        }

        private void mediumToolStripMenuItem_Click( object sender, EventArgs e ) {
            setBrightness( 150 );
        }

        private void highToolStripMenuItem_Click( object sender, EventArgs e ) {
            setBrightness( 220 );
        }

        private void highestToolStripMenuItem_Click( object sender, EventArgs e ) {
            setBrightness( 255 );
        }

        private void setBrightness( int value ) {
            this.trackBar_brightness.Value = value;
            serialPort.Write( "b" + value );
        }

        private void Form_Shown( object sender, EventArgs e ) {
            this.Close();
        }

        private void Send( string val ) {
            serialPort.Write( val );
            textBox_sent.AppendText( val + Environment.NewLine );
            if( val.Contains( "t" ) )
            {
                keepAwake.Interval = Convert.ToInt32( val.Replace( "t", "" ) );
            }
        }

        private void radioButton_status_CheckedChanged( object sender, EventArgs e ) {
            RadioButton rb = sender as RadioButton;
            if( rb.Checked && !getDataActive )
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

        private void GetData() {
            getDataActive = true;

            string[] ports = SerialPort.GetPortNames();
            foreach( string port in ports )
            {
                serialPort.PortName = port;
                serialPort.Open();
                try
                {
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

                    state = Convert.ToByte( lines[2] );
                    cycleDelay = Convert.ToByte( lines[3] );
                    brightness = Convert.ToByte( lines[4] );
                    sleepTimeout = Convert.ToInt64( lines[5] );
                    sleepbrightness = Convert.ToByte( lines[6] );
                    sleepcycleDelay = Convert.ToByte( lines[7] );
                    break;
                }
                catch
                {
                    serialPort.Close();
                    continue;
                }
            }

            setGUI();
            getDataActive = false;
        }

        private void setGUI() {

            switch( state )
            {
                case 0:
                    radioButton_olvasolampa.Checked = true;
                    break;
                case 1:
                    radioButton_lampa.Checked = true;
                    break;
                case 2:
                    radioButton_olvasrainbow.Checked = true;
                    break;
                case 3:
                    radioButton_wheel.Checked = true;
                    break;
                case 4:
                    radioButton_sleep.Checked = true;
                    break;
            }

            //számolás
            //delay = 30 - trackBar_speed.Value / 3.3333333333;
            //-value / 3,3333 = delay - 30
            // value / 3,3333 = 30 - delay
            // value = ( 30 - delay ) * 3,33333
            trackBar_speed.Value = ( int )( ( 30 - ( double )cycleDelay ) * 3.3333333333 );

            trackBar_brightness.Value = brightness;

            keepAwake.Interval = ( int )sleepTimeout * cycleDelay - 5000;
        }

        private void keepAwake_Tick( object sender, EventArgs e ) {
            serialPort.Write( "k" );
        }
    }
}
