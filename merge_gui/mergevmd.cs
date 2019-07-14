using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using merge_gui.Properties;
using System.Diagnostics;

namespace merge_gui
{
    public partial class mergevmd : Form
    {
        public mergevmd()
        {
            InitializeComponent();
        }

        private void button_body_file_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = Resources.title_select_body_file;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                text_body_file.Text = dialog.FileName;
            }
        }

        private void button_face_file_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = Resources.title_select_face_file;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                text_face_file.Text = dialog.FileName;
            }
        }
   
//        [DllImport("mergevmd.dll", CharSet = CharSet.Ansi)]
//        extern static int estimate_face_vmd(string image_file_name, string vmd_file_name);

        private void run_button_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Title = Resources.title_output_file;
            dialog.Filter = Resources.filter_vmd_file;
            dialog.FilterIndex = 1;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                button_body_file.Enabled = false;
                button_run.Enabled = false;
                console_message.Text = "";
                string body_file_name = text_body_file.Text;
                string face_file_name = text_face_file.Text;
                string out_file_name = dialog.FileName;
                if (String.IsNullOrWhiteSpace(body_file_name))
                {
                    return;
                }
                //estimate_face_vmd(image_file_name, vmd_file_name);
                string cmd_arg = "";
                cmd_arg += " \"" + body_file_name + "\" \"" + face_file_name + "\" \"" + out_file_name + "\"";

                var proc = new System.Diagnostics.Process();
                var current_dir = Environment.CurrentDirectory;
                proc.StartInfo.FileName = current_dir + @"\mergevmd.exe";
                proc.StartInfo.Arguments = cmd_arg;
                proc.StartInfo.UseShellExecute = false;
                proc.StartInfo.CreateNoWindow = true;
                proc.StartInfo.RedirectStandardOutput = true;
                proc.StartInfo.RedirectStandardError = true;
                proc.OutputDataReceived += output_handler;
                proc.ErrorDataReceived += output_handler;
                proc.EnableRaisingEvents = true;
                proc.SynchronizingObject = this;
                proc.Exited += process_exited;
                proc.Start();
                proc.BeginOutputReadLine();
                proc.BeginErrorReadLine();
            }
        }

        void output_handler(object obj, System.Diagnostics.DataReceivedEventArgs arg)
        {
            if (arg.Data != null)
            {
                this.Invoke(new Action(() =>
                {
                    console_message.AppendText(arg.Data + "\r\n");
                }));
            }
        }

        private void process_exited(object sender, EventArgs e)
        {
            string msg = Resources.msg_finished;
            Process proc = (Process)sender;
            if (proc.ExitCode != 0) {
                msg = Resources.msg_error_exit;
            }
            MessageBox.Show(msg);
            this.Invoke(new Action(() =>
            {
                button_body_file.Enabled = true;
                button_run.Enabled = true;
            }));
        }

        private void label_input_file_Click(object sender, EventArgs e)
        {

        }


    }
}
