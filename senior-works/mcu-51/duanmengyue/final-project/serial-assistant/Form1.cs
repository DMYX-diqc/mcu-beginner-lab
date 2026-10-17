using System.IO.Ports;
using System.Text;

namespace 串口小助手
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        #region 变量定义
        bool m_p = true;//判断串口是否打开
        SerialPort serialport = new SerialPort();//创建串口类的对象
        public static string comData = null;//接收串口数据并解析
        #endregion

        /// <summary>
        /// 初始化窗体
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_Load(object sender, EventArgs e)
        {
            //设置串口号com1
            this.comboBox1.SelectedIndex = 0;
            //设置波特率9600
            this.comboBox2.SelectedIndex = 0;
            //设置数据位8
            this.comboBox3.SelectedIndex = 1;
            //设置校验位None
            this.comboBox4.SelectedIndex = 0;
            //设置停止位1
            this.comboBox5.SelectedIndex = 0;

        }
        /// <summary>
        /// 打开串口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            OpenPort();
        }

        private void OpenPort()
        {
            if (m_p == true) //判断串口是否打开
            {
                this.button1.Text = "关闭串口";
                serialport.PortName = this.comboBox1.Text;//端口名称//设置串口号
                serialport.BaudRate = int.Parse(this.comboBox2.Text);//设置波特率
                serialport.DataBits = int.Parse(this.comboBox3.Text);//设置数据位
                serialport.StopBits = (StopBits)int.Parse(this.comboBox5.Text);//设置停止位

                serialport.Open();//打开串口
                serialport.DataReceived += new SerialDataReceivedEventHandler(Serialport_Datareceived);

                m_p = false;
            }
            else
            {
                this.button1.Text = "打开串口";
                serialport.Close();//关闭串口
                m_p = true;
            }
        }

        /// <summary>
        /// 接收数据
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Serialport_Datareceived(object sender, SerialDataReceivedEventArgs e)
        {
            //将byte数组转换成字符显示出来
            int count = serialport.BytesToRead;
            byte[] recive = new byte[count];//创建一个字节数组

            //读取数据
            serialport.Read(recive, 0, count);//将缓冲区的数据从0开始读取count个到byte字节数组中

            //判断转换成字符串还是十六进制
            if (this.radioButton1.Checked) //字符串
            {
                //将接收到的Byte数据转换成字符串
                string strRecive = Encoding.Default.GetString(recive);

                Control.CheckForIllegalCrossThreadCalls = false;//避免线程占用
                if (checkBox1.Checked)//换行显示
                {
                    this.textBox1.Text += strRecive + "\n";//显示内容回车换行
                }
                else
                {
                    this.textBox1.Text += strRecive;
                }
            }
            else if (this.radioButton2.Checked) //十六进制
            {
                if (checkBox1.Checked)//换行显示
                {
                    this.Invoke(new MethodInvoker(() =>
                    {
                        textBox1.Text += byteTohexstr(recive) + "\n";
                        comData = byteTohexstr(recive);//存入变量
                        ExtractData();//提取串口数据
                    }));
                }
                else
                {
                    this.Invoke(new MethodInvoker(() =>
                    {
                        textBox1.Text += byteTohexstr(recive);

                    }));
                }
                //ExtractData();//提取串口数据
            }
        }

        public static string byteTohexstr(byte[] bytes)//输入byte数组，转换成16进制字符串
        {
            string str = "";//构建返回的字符串
            if (bytes != null)
            {
                for (int i = 0; i < bytes.Length; i++)//循环转换成字符串 
                {
                    str += bytes[i].ToString("x2") + "";
                }
            }
            return str;
        }
        public void ExtractData()
        {
            int[] intArray = { };
            string[] strArray = { };
            strArray = comData.Split(); //comData里面是16进制字符串，将其以空白符号分开
            strArray = strArray.Take(strArray.Count() - 1).ToArray();//从数组中删除最后一项
            for (int i = 0; i < strArray.Length; i++)
            {
                try
                {
                    intArray[i] = Convert.ToInt32(strArray[i], 16);//把16进制字符串转换成数字
                }
                catch (Exception)
                {

                }
            }
        }

        /// <summary>
        /// 清空数据
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click_1(object sender, EventArgs e)
        {
            this.textBox1.Text = "";
        }


        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click_1(object sender, EventArgs e)
        {

            try
            {
                if (radioButton2.Checked)//16进制 
                {
                    //将字符串转换为16进制发送
                    byte[] d = strTohexByte(this.textBox2.Text);

                    //发送数据
                    serialport.Write(d, 0, d.Length);
                }
                if (radioButton1.Checked) //字符串
                {
                    byte[] SendBuf = new byte[1024 * 1024];//形成数组的形式
                    //将字符串转换成Byte数组
                    SendBuf = Encoding.Default.GetBytes(this.textBox2.Text);
                    //发送数据
                    serialport.Write(SendBuf, 0, SendBuf.Length);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("发送异常：" + ex.Message);
            }
        }

        private byte[] strTohexByte(string HEXstring) //字符窜转换16进制
        {
            //去空格
            HEXstring = HEXstring.Replace(" ", "");
            //如果16进制的情况下应当能被2整除，如果不能就加上空字符
            if ((HEXstring.Length % 2) != 0)
            {
                HEXstring += "";
            }
            byte[] returnbytes = new byte[HEXstring.Length / 2];//准备缓存数据集

            //转换16进制
            for (int i = 0; i < returnbytes.Length; i++)
            {
                //转换成16进制的Byte
                returnbytes[i] = Convert.ToByte(HEXstring.Substring(i * 2, 2).Replace(" ", ""), 16);
            }
            return returnbytes;//返回对应的byte数据
        }
    }
}

