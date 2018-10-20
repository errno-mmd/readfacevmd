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
using rfv_gui.Properties;

namespace rfv_gui
{
    public partial class readfacevmd : Form
    {
        public readfacevmd()
        {
            InitializeComponent();
        }

        private void input_file_button_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = Resources.title_select_input_file;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                text_input_file.Text = dialog.FileName;
            }
        }

        [DllImport("readfacevmd.dll", CharSet = CharSet.Ansi)]
        extern static int estimate_face_vmd(string image_file_name, string vmd_file_name);

        private void run_button_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Title = Resources.title_output_file;
            dialog.Filter = Resources.filter_vmd_file;
            dialog.FilterIndex = 1;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                button_input_file.Enabled = false;
                button_run.Enabled = false;
                console_message.Text = "";
                string vmd_file_name = dialog.FileName;
                string image_file_name = text_input_file.Text;
                if (String.IsNullOrWhiteSpace(image_file_name))
                {
                    return;
                }
                //estimate_face_vmd(image_file_name, vmd_file_name);
                string cmd_arg = "";
                if (check_smoothing.Checked)
                {
                    cmd_arg += " --cutoff " + text_cutoff.Text;
                }
                else
                {
                    cmd_arg += " --cutoff -1";
                }
                if (check_reduction.Checked)
                {
                    cmd_arg += " --th_pos " + text_th_pos.Text;
                    cmd_arg += " --th_rot " + text_th_rot.Text;
                    cmd_arg += " --th_morph " + text_th_morph.Text;
                }
                else
                {
                    cmd_arg += " --th_pos -1 --th_rot -1 --th_morph -1";
                }
                cmd_arg += " \"" + image_file_name + "\" \"" + vmd_file_name + "\"";

                var proc = new System.Diagnostics.Process();
                var current_dir = Environment.CurrentDirectory;
                proc.StartInfo.FileName = current_dir + @"\readfacevmd.exe";
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
            MessageBox.Show(Resources.msg_finished);
            this.Invoke(new Action(() =>
            {
                button_input_file.Enabled = true;
                button_run.Enabled = true;
            }));
        }

        private void check_smoothing_CheckedChanged(object sender, EventArgs e)
        {
            if (check_smoothing.Checked)
            {
                text_cutoff.Enabled = true;
            }
            else
            {
                text_cutoff.Enabled = false;
            }
        }

        private void check_reduction_CheckedChanged(object sender, EventArgs e)
        {
            if (check_reduction.Checked)
            {
                text_th_pos.Enabled = true;
                text_th_rot.Enabled = true;
                text_th_morph.Enabled = true;
            }
            else
            {
                text_th_pos.Enabled = false;
                text_th_rot.Enabled = false;
                text_th_morph.Enabled = false;
            }
        }
    }
}
