namespace rfv_gui
{
    partial class readfacevmd
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(readfacevmd));
            this.label_input_file = new System.Windows.Forms.Label();
            this.text_input_file = new System.Windows.Forms.TextBox();
            this.button_input_file = new System.Windows.Forms.Button();
            this.button_run = new System.Windows.Forms.Button();
            this.console_message = new System.Windows.Forms.TextBox();
            this.group_smoothing = new System.Windows.Forms.GroupBox();
            this.label_cutoff = new System.Windows.Forms.Label();
            this.text_cutoff = new System.Windows.Forms.TextBox();
            this.check_smoothing = new System.Windows.Forms.CheckBox();
            this.group_reduction = new System.Windows.Forms.GroupBox();
            this.label_th_morph = new System.Windows.Forms.Label();
            this.label_th_rot = new System.Windows.Forms.Label();
            this.label_th_pos = new System.Windows.Forms.Label();
            this.text_th_morph = new System.Windows.Forms.TextBox();
            this.text_th_rot = new System.Windows.Forms.TextBox();
            this.text_th_pos = new System.Windows.Forms.TextBox();
            this.check_reduction = new System.Windows.Forms.CheckBox();
            this.group_smoothing.SuspendLayout();
            this.group_reduction.SuspendLayout();
            this.SuspendLayout();
            // 
            // label_input_file
            // 
            resources.ApplyResources(this.label_input_file, "label_input_file");
            this.label_input_file.Name = "label_input_file";
            // 
            // text_input_file
            // 
            resources.ApplyResources(this.text_input_file, "text_input_file");
            this.text_input_file.Name = "text_input_file";
            // 
            // button_input_file
            // 
            resources.ApplyResources(this.button_input_file, "button_input_file");
            this.button_input_file.Name = "button_input_file";
            this.button_input_file.UseVisualStyleBackColor = true;
            this.button_input_file.Click += new System.EventHandler(this.input_file_button_Click);
            // 
            // button_run
            // 
            resources.ApplyResources(this.button_run, "button_run");
            this.button_run.Name = "button_run";
            this.button_run.UseVisualStyleBackColor = true;
            this.button_run.Click += new System.EventHandler(this.run_button_Click);
            // 
            // console_message
            // 
            resources.ApplyResources(this.console_message, "console_message");
            this.console_message.Name = "console_message";
            this.console_message.ReadOnly = true;
            // 
            // group_smoothing
            // 
            resources.ApplyResources(this.group_smoothing, "group_smoothing");
            this.group_smoothing.Controls.Add(this.label_cutoff);
            this.group_smoothing.Controls.Add(this.text_cutoff);
            this.group_smoothing.Controls.Add(this.check_smoothing);
            this.group_smoothing.Name = "group_smoothing";
            this.group_smoothing.TabStop = false;
            // 
            // label_cutoff
            // 
            resources.ApplyResources(this.label_cutoff, "label_cutoff");
            this.label_cutoff.Name = "label_cutoff";
            // 
            // text_cutoff
            // 
            resources.ApplyResources(this.text_cutoff, "text_cutoff");
            this.text_cutoff.Name = "text_cutoff";
            // 
            // check_smoothing
            // 
            resources.ApplyResources(this.check_smoothing, "check_smoothing");
            this.check_smoothing.Checked = true;
            this.check_smoothing.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_smoothing.Name = "check_smoothing";
            this.check_smoothing.UseVisualStyleBackColor = true;
            this.check_smoothing.CheckedChanged += new System.EventHandler(this.check_smoothing_CheckedChanged);
            // 
            // group_reduction
            // 
            resources.ApplyResources(this.group_reduction, "group_reduction");
            this.group_reduction.Controls.Add(this.label_th_morph);
            this.group_reduction.Controls.Add(this.label_th_rot);
            this.group_reduction.Controls.Add(this.label_th_pos);
            this.group_reduction.Controls.Add(this.text_th_morph);
            this.group_reduction.Controls.Add(this.text_th_rot);
            this.group_reduction.Controls.Add(this.text_th_pos);
            this.group_reduction.Controls.Add(this.check_reduction);
            this.group_reduction.Name = "group_reduction";
            this.group_reduction.TabStop = false;
            // 
            // label_th_morph
            // 
            resources.ApplyResources(this.label_th_morph, "label_th_morph");
            this.label_th_morph.Name = "label_th_morph";
            // 
            // label_th_rot
            // 
            resources.ApplyResources(this.label_th_rot, "label_th_rot");
            this.label_th_rot.Name = "label_th_rot";
            // 
            // label_th_pos
            // 
            resources.ApplyResources(this.label_th_pos, "label_th_pos");
            this.label_th_pos.Name = "label_th_pos";
            // 
            // text_th_morph
            // 
            resources.ApplyResources(this.text_th_morph, "text_th_morph");
            this.text_th_morph.Name = "text_th_morph";
            // 
            // text_th_rot
            // 
            resources.ApplyResources(this.text_th_rot, "text_th_rot");
            this.text_th_rot.Name = "text_th_rot";
            // 
            // text_th_pos
            // 
            resources.ApplyResources(this.text_th_pos, "text_th_pos");
            this.text_th_pos.Name = "text_th_pos";
            // 
            // check_reduction
            // 
            resources.ApplyResources(this.check_reduction, "check_reduction");
            this.check_reduction.Checked = true;
            this.check_reduction.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_reduction.Name = "check_reduction";
            this.check_reduction.UseVisualStyleBackColor = true;
            this.check_reduction.CheckedChanged += new System.EventHandler(this.check_reduction_CheckedChanged);
            // 
            // readfacevmd
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.group_reduction);
            this.Controls.Add(this.group_smoothing);
            this.Controls.Add(this.console_message);
            this.Controls.Add(this.button_run);
            this.Controls.Add(this.button_input_file);
            this.Controls.Add(this.text_input_file);
            this.Controls.Add(this.label_input_file);
            this.Name = "readfacevmd";
            this.group_smoothing.ResumeLayout(false);
            this.group_smoothing.PerformLayout();
            this.group_reduction.ResumeLayout(false);
            this.group_reduction.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_input_file;
        private System.Windows.Forms.TextBox text_input_file;
        private System.Windows.Forms.Button button_input_file;
        private System.Windows.Forms.Button button_run;
        private System.Windows.Forms.TextBox console_message;
        private System.Windows.Forms.GroupBox group_smoothing;
        private System.Windows.Forms.Label label_cutoff;
        private System.Windows.Forms.TextBox text_cutoff;
        private System.Windows.Forms.CheckBox check_smoothing;
        private System.Windows.Forms.GroupBox group_reduction;
        private System.Windows.Forms.Label label_th_morph;
        private System.Windows.Forms.Label label_th_rot;
        private System.Windows.Forms.Label label_th_pos;
        private System.Windows.Forms.TextBox text_th_morph;
        private System.Windows.Forms.TextBox text_th_rot;
        private System.Windows.Forms.TextBox text_th_pos;
        private System.Windows.Forms.CheckBox check_reduction;
    }
}

