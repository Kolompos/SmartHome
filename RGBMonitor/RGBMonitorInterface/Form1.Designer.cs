namespace RGBMonitorInterface {
    partial class Form {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing ) {
            if( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form));
            this.trackBar_brightness = new System.Windows.Forms.TrackBar();
            this.radioButton_olvasolampa = new System.Windows.Forms.RadioButton();
            this.radioButton_lampa = new System.Windows.Forms.RadioButton();
            this.radioButton_olvasrainbow = new System.Windows.Forms.RadioButton();
            this.radioButton_wheel = new System.Windows.Forms.RadioButton();
            this.radioButton_sleep = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.trackBar_speed = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.textBox_send = new System.Windows.Forms.TextBox();
            this.textBox_sent = new System.Windows.Forms.TextBox();
            this.textBox_received = new System.Windows.Forms.TextBox();
            this.button_send = new System.Windows.Forms.Button();
            this.groupBox_radiobuttons = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.keepAwake = new System.Windows.Forms.Timer(this.components);
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip_notificationIcon = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.brightnessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.highestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.highToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mediumToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lowestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wheelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mixedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_brightness)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_speed)).BeginInit();
            this.groupBox_radiobuttons.SuspendLayout();
            this.contextMenuStrip_notificationIcon.SuspendLayout();
            this.SuspendLayout();
            // 
            // trackBar_brightness
            // 
            this.trackBar_brightness.Location = new System.Drawing.Point(96, 65);
            this.trackBar_brightness.Maximum = 255;
            this.trackBar_brightness.Minimum = 5;
            this.trackBar_brightness.Name = "trackBar_brightness";
            this.trackBar_brightness.Size = new System.Drawing.Size(374, 45);
            this.trackBar_brightness.TabIndex = 0;
            this.trackBar_brightness.Value = 200;
            this.trackBar_brightness.MouseUp += new System.Windows.Forms.MouseEventHandler(this.trackBar_brightness_MouseUp);
            // 
            // radioButton_olvasolampa
            // 
            this.radioButton_olvasolampa.AutoSize = true;
            this.radioButton_olvasolampa.Checked = true;
            this.radioButton_olvasolampa.Location = new System.Drawing.Point(6, 19);
            this.radioButton_olvasolampa.Name = "radioButton_olvasolampa";
            this.radioButton_olvasolampa.Size = new System.Drawing.Size(86, 17);
            this.radioButton_olvasolampa.TabIndex = 1;
            this.radioButton_olvasolampa.TabStop = true;
            this.radioButton_olvasolampa.Text = "Olvasólámpa";
            this.radioButton_olvasolampa.UseVisualStyleBackColor = true;
            this.radioButton_olvasolampa.CheckedChanged += new System.EventHandler(this.radioButton_status_CheckedChanged);
            // 
            // radioButton_lampa
            // 
            this.radioButton_lampa.AutoSize = true;
            this.radioButton_lampa.Location = new System.Drawing.Point(106, 19);
            this.radioButton_lampa.Name = "radioButton_lampa";
            this.radioButton_lampa.Size = new System.Drawing.Size(57, 17);
            this.radioButton_lampa.TabIndex = 2;
            this.radioButton_lampa.Text = "Lámpa";
            this.radioButton_lampa.UseVisualStyleBackColor = true;
            this.radioButton_lampa.CheckedChanged += new System.EventHandler(this.radioButton_status_CheckedChanged);
            // 
            // radioButton_olvasrainbow
            // 
            this.radioButton_olvasrainbow.AutoSize = true;
            this.radioButton_olvasrainbow.Location = new System.Drawing.Point(206, 19);
            this.radioButton_olvasrainbow.Name = "radioButton_olvasrainbow";
            this.radioButton_olvasrainbow.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.radioButton_olvasrainbow.Size = new System.Drawing.Size(87, 17);
            this.radioButton_olvasrainbow.TabIndex = 3;
            this.radioButton_olvasrainbow.Text = "Olvasó+RGB";
            this.radioButton_olvasrainbow.UseVisualStyleBackColor = true;
            this.radioButton_olvasrainbow.CheckedChanged += new System.EventHandler(this.radioButton_status_CheckedChanged);
            // 
            // radioButton_wheel
            // 
            this.radioButton_wheel.AutoSize = true;
            this.radioButton_wheel.Location = new System.Drawing.Point(306, 19);
            this.radioButton_wheel.Name = "radioButton_wheel";
            this.radioButton_wheel.Size = new System.Drawing.Size(82, 17);
            this.radioButton_wheel.TabIndex = 4;
            this.radioButton_wheel.Text = "RGB Wheel";
            this.radioButton_wheel.UseVisualStyleBackColor = true;
            this.radioButton_wheel.CheckedChanged += new System.EventHandler(this.radioButton_status_CheckedChanged);
            // 
            // radioButton_sleep
            // 
            this.radioButton_sleep.AutoSize = true;
            this.radioButton_sleep.Location = new System.Drawing.Point(406, 19);
            this.radioButton_sleep.Name = "radioButton_sleep";
            this.radioButton_sleep.Size = new System.Drawing.Size(52, 17);
            this.radioButton_sleep.TabIndex = 5;
            this.radioButton_sleep.Text = "Sleep";
            this.radioButton_sleep.UseVisualStyleBackColor = true;
            this.radioButton_sleep.CheckedChanged += new System.EventHandler(this.radioButton_status_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Fényerő";
            // 
            // trackBar_speed
            // 
            this.trackBar_speed.Location = new System.Drawing.Point(96, 125);
            this.trackBar_speed.Maximum = 100;
            this.trackBar_speed.Name = "trackBar_speed";
            this.trackBar_speed.Size = new System.Drawing.Size(374, 45);
            this.trackBar_speed.TabIndex = 7;
            this.trackBar_speed.Value = 50;
            this.trackBar_speed.MouseUp += new System.Windows.Forms.MouseEventHandler(this.trackBar_speed_MouseUp);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 125);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(54, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Sebesség";
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.PortName = "COM3";
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            // 
            // textBox_send
            // 
            this.textBox_send.Location = new System.Drawing.Point(18, 197);
            this.textBox_send.Name = "textBox_send";
            this.textBox_send.Size = new System.Drawing.Size(105, 20);
            this.textBox_send.TabIndex = 9;
            // 
            // textBox_sent
            // 
            this.textBox_sent.Location = new System.Drawing.Point(18, 224);
            this.textBox_sent.Multiline = true;
            this.textBox_sent.Name = "textBox_sent";
            this.textBox_sent.ReadOnly = true;
            this.textBox_sent.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_sent.Size = new System.Drawing.Size(187, 228);
            this.textBox_sent.TabIndex = 10;
            // 
            // textBox_received
            // 
            this.textBox_received.Location = new System.Drawing.Point(211, 224);
            this.textBox_received.Multiline = true;
            this.textBox_received.Name = "textBox_received";
            this.textBox_received.ReadOnly = true;
            this.textBox_received.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_received.Size = new System.Drawing.Size(259, 228);
            this.textBox_received.TabIndex = 11;
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(129, 195);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(76, 23);
            this.button_send.TabIndex = 12;
            this.button_send.Text = "SEND";
            this.button_send.UseVisualStyleBackColor = true;
            this.button_send.Click += new System.EventHandler(this.button_send_Click);
            // 
            // groupBox_radiobuttons
            // 
            this.groupBox_radiobuttons.Controls.Add(this.radioButton_olvasolampa);
            this.groupBox_radiobuttons.Controls.Add(this.radioButton_lampa);
            this.groupBox_radiobuttons.Controls.Add(this.radioButton_olvasrainbow);
            this.groupBox_radiobuttons.Controls.Add(this.radioButton_wheel);
            this.groupBox_radiobuttons.Controls.Add(this.radioButton_sleep);
            this.groupBox_radiobuttons.Location = new System.Drawing.Point(12, 12);
            this.groupBox_radiobuttons.Name = "groupBox_radiobuttons";
            this.groupBox_radiobuttons.Size = new System.Drawing.Size(465, 47);
            this.groupBox_radiobuttons.TabIndex = 13;
            this.groupBox_radiobuttons.TabStop = false;
            this.groupBox_radiobuttons.Text = "Állapot";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(211, 165);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(154, 39);
            this.label3.TabIndex = 14;
            this.label3.Text = "t set timeout to sleep pl: t50000\r\np set sleep brightness\r\ne set sleep speed";
            // 
            // keepAwake
            // 
            this.keepAwake.Enabled = true;
            this.keepAwake.Interval = 600000;
            this.keepAwake.Tick += new System.EventHandler(this.keepAwake_Tick);
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.BalloonTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip_notificationIcon;
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "RGB Controller";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.DoubleClick += new System.EventHandler(this.notifyIcon1_DoubleClick);
            // 
            // contextMenuStrip_notificationIcon
            // 
            this.contextMenuStrip_notificationIcon.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.brightnessToolStripMenuItem,
            this.wheelToolStripMenuItem,
            this.mixedToolStripMenuItem,
            this.lightToolStripMenuItem,
            this.closeToolStripMenuItem});
            this.contextMenuStrip_notificationIcon.Name = "contextMenuStrip_notificationIcon";
            this.contextMenuStrip_notificationIcon.Size = new System.Drawing.Size(130, 114);
            // 
            // brightnessToolStripMenuItem
            // 
            this.brightnessToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.highestToolStripMenuItem,
            this.highToolStripMenuItem,
            this.mediumToolStripMenuItem,
            this.lowToolStripMenuItem,
            this.lowestToolStripMenuItem});
            this.brightnessToolStripMenuItem.Name = "brightnessToolStripMenuItem";
            this.brightnessToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.brightnessToolStripMenuItem.Text = "Brightness";
            // 
            // highestToolStripMenuItem
            // 
            this.highestToolStripMenuItem.Name = "highestToolStripMenuItem";
            this.highestToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.highestToolStripMenuItem.Text = "Highest";
            this.highestToolStripMenuItem.Click += new System.EventHandler(this.highestToolStripMenuItem_Click);
            // 
            // highToolStripMenuItem
            // 
            this.highToolStripMenuItem.Name = "highToolStripMenuItem";
            this.highToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.highToolStripMenuItem.Text = "High";
            this.highToolStripMenuItem.Click += new System.EventHandler(this.highToolStripMenuItem_Click);
            // 
            // mediumToolStripMenuItem
            // 
            this.mediumToolStripMenuItem.Name = "mediumToolStripMenuItem";
            this.mediumToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.mediumToolStripMenuItem.Text = "Medium";
            this.mediumToolStripMenuItem.Click += new System.EventHandler(this.mediumToolStripMenuItem_Click);
            // 
            // lowToolStripMenuItem
            // 
            this.lowToolStripMenuItem.Name = "lowToolStripMenuItem";
            this.lowToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.lowToolStripMenuItem.Text = "Low";
            this.lowToolStripMenuItem.Click += new System.EventHandler(this.lowToolStripMenuItem_Click);
            // 
            // lowestToolStripMenuItem
            // 
            this.lowestToolStripMenuItem.Name = "lowestToolStripMenuItem";
            this.lowestToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.lowestToolStripMenuItem.Text = "Lowest";
            this.lowestToolStripMenuItem.Click += new System.EventHandler(this.lowestToolStripMenuItem_Click);
            // 
            // wheelToolStripMenuItem
            // 
            this.wheelToolStripMenuItem.Name = "wheelToolStripMenuItem";
            this.wheelToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.wheelToolStripMenuItem.Text = "Wheel";
            this.wheelToolStripMenuItem.Click += new System.EventHandler(this.wheelToolStripMenuItem_Click);
            // 
            // mixedToolStripMenuItem
            // 
            this.mixedToolStripMenuItem.Name = "mixedToolStripMenuItem";
            this.mixedToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.mixedToolStripMenuItem.Text = "Mixed";
            this.mixedToolStripMenuItem.Click += new System.EventHandler(this.mixedToolStripMenuItem_Click);
            // 
            // lightToolStripMenuItem
            // 
            this.lightToolStripMenuItem.Name = "lightToolStripMenuItem";
            this.lightToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.lightToolStripMenuItem.Text = "Light";
            this.lightToolStripMenuItem.Click += new System.EventHandler(this.lightToolStripMenuItem_Click);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // Form
            // 
            this.AcceptButton = this.button_send;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(501, 464);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.groupBox_radiobuttons);
            this.Controls.Add(this.button_send);
            this.Controls.Add(this.textBox_received);
            this.Controls.Add(this.textBox_sent);
            this.Controls.Add(this.textBox_send);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.trackBar_speed);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.trackBar_brightness);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form";
            this.Text = "RGB Monitor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Shown += new System.EventHandler(this.Form_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_brightness)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_speed)).EndInit();
            this.groupBox_radiobuttons.ResumeLayout(false);
            this.groupBox_radiobuttons.PerformLayout();
            this.contextMenuStrip_notificationIcon.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar trackBar_brightness;
        private System.Windows.Forms.RadioButton radioButton_olvasolampa;
        private System.Windows.Forms.RadioButton radioButton_lampa;
        private System.Windows.Forms.RadioButton radioButton_olvasrainbow;
        private System.Windows.Forms.RadioButton radioButton_wheel;
        private System.Windows.Forms.RadioButton radioButton_sleep;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar trackBar_speed;
        private System.Windows.Forms.Label label2;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.TextBox textBox_send;
        private System.Windows.Forms.TextBox textBox_sent;
        private System.Windows.Forms.TextBox textBox_received;
        private System.Windows.Forms.Button button_send;
        private System.Windows.Forms.GroupBox groupBox_radiobuttons;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Timer keepAwake;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip_notificationIcon;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem wheelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem brightnessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mediumToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem highToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem highestToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mixedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lowestToolStripMenuItem;
    }
}

