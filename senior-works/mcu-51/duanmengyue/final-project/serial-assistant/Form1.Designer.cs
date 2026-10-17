namespace 串口小助手
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            groupBox1 = new GroupBox();
            button1 = new Button();
            checkBox1 = new CheckBox();
            radioButton2 = new RadioButton();
            radioButton1 = new RadioButton();
            comboBox5 = new ComboBox();
            comboBox4 = new ComboBox();
            label5 = new Label();
            comboBox3 = new ComboBox();
            label4 = new Label();
            comboBox2 = new ComboBox();
            label3 = new Label();
            comboBox1 = new ComboBox();
            label2 = new Label();
            label1 = new Label();
            groupBox2 = new GroupBox();
            textBox1 = new TextBox();
            groupBox3 = new GroupBox();
            button3 = new Button();
            button2 = new Button();
            textBox2 = new TextBox();
            groupBox1.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox3.SuspendLayout();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(button1);
            groupBox1.Controls.Add(checkBox1);
            groupBox1.Controls.Add(radioButton2);
            groupBox1.Controls.Add(radioButton1);
            groupBox1.Controls.Add(comboBox5);
            groupBox1.Controls.Add(comboBox4);
            groupBox1.Controls.Add(label5);
            groupBox1.Controls.Add(comboBox3);
            groupBox1.Controls.Add(label4);
            groupBox1.Controls.Add(comboBox2);
            groupBox1.Controls.Add(label3);
            groupBox1.Controls.Add(comboBox1);
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(label1);
            groupBox1.Location = new Point(12, 613);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(959, 175);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "串口设置";
            // 
            // button1
            // 
            button1.Location = new Point(769, 39);
            button1.Name = "button1";
            button1.Size = new Size(138, 119);
            button1.TabIndex = 3;
            button1.Text = "打开串口";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(571, 130);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(108, 28);
            checkBox1.TabIndex = 2;
            checkBox1.Text = "换行显示";
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            radioButton2.AutoSize = true;
            radioButton2.Location = new Point(571, 84);
            radioButton2.Name = "radioButton2";
            radioButton2.Size = new Size(93, 28);
            radioButton2.TabIndex = 1;
            radioButton2.TabStop = true;
            radioButton2.Text = "16进制";
            radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            radioButton1.AutoSize = true;
            radioButton1.Location = new Point(571, 39);
            radioButton1.Name = "radioButton1";
            radioButton1.Size = new Size(125, 28);
            radioButton1.TabIndex = 1;
            radioButton1.TabStop = true;
            radioButton1.Text = "字符串发送";
            radioButton1.UseVisualStyleBackColor = true;
            // 
            // comboBox5
            // 
            comboBox5.AutoCompleteCustomSource.AddRange(new string[] { "1", "2" });
            comboBox5.FormattingEnabled = true;
            comboBox5.Items.AddRange(new object[] { "1", "2" });
            comboBox5.Location = new Point(355, 126);
            comboBox5.Name = "comboBox5";
            comboBox5.Size = new Size(130, 32);
            comboBox5.TabIndex = 1;
            // 
            // comboBox4
            // 
            comboBox4.AutoCompleteCustomSource.AddRange(new string[] { "None", "Odd", "Even" });
            comboBox4.FormattingEnabled = true;
            comboBox4.Items.AddRange(new object[] { "None", "Odd", "Even" });
            comboBox4.Location = new Point(355, 81);
            comboBox4.Name = "comboBox4";
            comboBox4.Size = new Size(130, 32);
            comboBox4.TabIndex = 1;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(285, 129);
            label5.Name = "label5";
            label5.Size = new Size(64, 24);
            label5.TabIndex = 0;
            label5.Text = "停止位";
            // 
            // comboBox3
            // 
            comboBox3.AutoCompleteCustomSource.AddRange(new string[] { "7", "8" });
            comboBox3.FormattingEnabled = true;
            comboBox3.Items.AddRange(new object[] { "7", "8" });
            comboBox3.Location = new Point(355, 35);
            comboBox3.Name = "comboBox3";
            comboBox3.Size = new Size(130, 32);
            comboBox3.TabIndex = 1;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(285, 84);
            label4.Name = "label4";
            label4.Size = new Size(64, 24);
            label4.TabIndex = 0;
            label4.Text = "校验位";
            // 
            // comboBox2
            // 
            comboBox2.AutoCompleteCustomSource.AddRange(new string[] { "9600", "19200", "38400", "57600", "115200" });
            comboBox2.FormattingEnabled = true;
            comboBox2.Items.AddRange(new object[] { "4800", "9600", "19200", "38400", "57600", "115200" });
            comboBox2.Location = new Point(111, 115);
            comboBox2.Name = "comboBox2";
            comboBox2.Size = new Size(130, 32);
            comboBox2.TabIndex = 1;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(285, 38);
            label3.Name = "label3";
            label3.Size = new Size(64, 24);
            label3.TabIndex = 0;
            label3.Text = "数据位";
            // 
            // comboBox1
            // 
            comboBox1.AutoCompleteCustomSource.AddRange(new string[] { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9" });
            comboBox1.FormattingEnabled = true;
            comboBox1.Items.AddRange(new object[] { "com1", "com2", "com3", "com4", "com5", "com6", "com7", "com8", "com9" });
            comboBox1.Location = new Point(111, 35);
            comboBox1.Name = "comboBox1";
            comboBox1.Size = new Size(130, 32);
            comboBox1.TabIndex = 1;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(41, 118);
            label2.Name = "label2";
            label2.Size = new Size(64, 24);
            label2.TabIndex = 0;
            label2.Text = "波特率";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(41, 38);
            label1.Name = "label1";
            label1.Size = new Size(64, 24);
            label1.TabIndex = 0;
            label1.Text = "串口号";
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(textBox1);
            groupBox2.Font = new Font("Microsoft YaHei UI", 10.5F, FontStyle.Bold, GraphicsUnit.Point, 134);
            groupBox2.Location = new Point(43, 25);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(493, 513);
            groupBox2.TabIndex = 1;
            groupBox2.TabStop = false;
            groupBox2.Text = "数据接收";
            // 
            // textBox1
            // 
            textBox1.Location = new Point(27, 53);
            textBox1.Multiline = true;
            textBox1.Name = "textBox1";
            textBox1.ScrollBars = ScrollBars.Vertical;
            textBox1.Size = new Size(440, 441);
            textBox1.TabIndex = 0;
            // 
            // groupBox3
            // 
            groupBox3.Controls.Add(button3);
            groupBox3.Controls.Add(button2);
            groupBox3.Controls.Add(textBox2);
            groupBox3.Font = new Font("Microsoft YaHei UI", 10.5F, FontStyle.Bold, GraphicsUnit.Point, 134);
            groupBox3.Location = new Point(583, 25);
            groupBox3.Name = "groupBox3";
            groupBox3.Size = new Size(615, 513);
            groupBox3.TabIndex = 2;
            groupBox3.TabStop = false;
            groupBox3.Text = "数据发送";
            // 
            // button3
            // 
            button3.Location = new Point(486, 379);
            button3.Name = "button3";
            button3.Size = new Size(114, 68);
            button3.TabIndex = 4;
            button3.Text = "清空接收";
            button3.UseVisualStyleBackColor = true;
            button3.Click += button3_Click_1;
            // 
            // button2
            // 
            button2.Location = new Point(486, 291);
            button2.Name = "button2";
            button2.Size = new Size(114, 68);
            button2.TabIndex = 4;
            button2.Text = "发送数据";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click_1;
            // 
            // textBox2
            // 
            textBox2.Location = new Point(28, 53);
            textBox2.Multiline = true;
            textBox2.Name = "textBox2";
            textBox2.Size = new Size(440, 419);
            textBox2.TabIndex = 1;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(11F, 24F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1254, 800);
            Controls.Add(groupBox3);
            Controls.Add(groupBox2);
            Controls.Add(groupBox1);
            Name = "Form1";
            Text = "串口小助手";
            Load += Form1_Load;
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox3.ResumeLayout(false);
            groupBox3.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private GroupBox groupBox1;
        private ComboBox comboBox2;
        private ComboBox comboBox1;
        private Label label2;
        private Label label1;
        private ComboBox comboBox5;
        private ComboBox comboBox4;
        private Label label5;
        private ComboBox comboBox3;
        private Label label4;
        private Label label3;
        private RadioButton radioButton1;
        private CheckBox checkBox1;
        private RadioButton radioButton2;
        private Button button1;
        private GroupBox groupBox2;
        private TextBox textBox1;
        private GroupBox groupBox3;
        private TextBox textBox2;
        private Button button3;
        private Button button2;
    }
}
