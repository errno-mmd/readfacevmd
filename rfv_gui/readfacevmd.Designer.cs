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
            this.label_input_file = new System.Windows.Forms.Label();
            this.text_input_file = new System.Windows.Forms.TextBox();
            this.button_input_file = new System.Windows.Forms.Button();
            this.button_run = new System.Windows.Forms.Button();
            this.console_message = new System.Windows.Forms.TextBox();
            this.group_smoothing = new System.Windows.Forms.GroupBox();
            this.check_smoothing = new System.Windows.Forms.CheckBox();
            this.text_cutoff = new System.Windows.Forms.TextBox();
            this.label_cutoff = new System.Windows.Forms.Label();
            this.group_reduction = new System.Windows.Forms.GroupBox();
            this.label_th_pos = new System.Windows.Forms.Label();
            this.text_th_pos = new System.Windows.Forms.TextBox();
            this.check_reduction = new System.Windows.Forms.CheckBox();
            this.label_th_rot = new System.Windows.Forms.Label();
            this.text_th_rot = new System.Windows.Forms.TextBox();
            this.label_th_morph = new System.Windows.Forms.Label();
            this.text_th_morph = new System.Windows.Forms.TextBox();
            this.group_smoothing.SuspendLayout();
            this.group_reduction.SuspendLayout();
            this.SuspendLayout();
            // 
            // label_input_file
            // 
            this.label_input_file.AutoSize = true;
            this.label_input_file.Location = new System.Drawing.Point(13, 19);
            this.label_input_file.Name = "label_input_file";
            this.label_input_file.Size = new System.Drawing.Size(63, 12);
            this.label_input_file.TabIndex = 0;
            this.label_input_file.Text = "入力ファイル";
            // 
            // text_input_file
            // 
            this.text_input_file.Location = new System.Drawing.Point(82, 16);
            this.text_input_file.Name = "text_input_file";
            this.text_input_file.Size = new System.Drawing.Size(404, 19);
            this.text_input_file.TabIndex = 1;
            // 
            // button_input_file
            // 
            this.button_input_file.Location = new System.Drawing.Point(492, 11);
            this.button_input_file.Name = "button_input_file";
            this.button_input_file.Size = new System.Drawing.Size(62, 29);
            this.button_input_file.TabIndex = 2;
            this.button_input_file.Text = "参照";
            this.button_input_file.UseVisualStyleBackColor = true;
            this.button_input_file.Click += new System.EventHandler(this.input_file_button_Click);
            // 
            // button_run
            // 
            this.button_run.Location = new System.Drawing.Point(445, 60);
            this.button_run.Name = "button_run";
            this.button_run.Size = new System.Drawing.Size(109, 45);
            this.button_run.TabIndex = 3;
            this.button_run.Text = "VMDファイル生成";
            this.button_run.UseVisualStyleBackColor = true;
            this.button_run.Click += new System.EventHandler(this.run_button_Click);
            // 
            // console_message
            // 
            this.console_message.Location = new System.Drawing.Point(6, 179);
            this.console_message.Multiline = true;
            this.console_message.Name = "console_message";
            this.console_message.ReadOnly = true;
            this.console_message.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.console_message.Size = new System.Drawing.Size(548, 199);
            this.console_message.TabIndex = 4;
            // 
            // group_smoothing
            // 
            this.group_smoothing.Controls.Add(this.label_cutoff);
            this.group_smoothing.Controls.Add(this.text_cutoff);
            this.group_smoothing.Controls.Add(this.check_smoothing);
            this.group_smoothing.Location = new System.Drawing.Point(6, 54);
            this.group_smoothing.Name = "group_smoothing";
            this.group_smoothing.Size = new System.Drawing.Size(182, 119);
            this.group_smoothing.TabIndex = 5;
            this.group_smoothing.TabStop = false;
            this.group_smoothing.Text = "平滑化(smoothing)";
            // 
            // check_smoothing
            // 
            this.check_smoothing.AutoSize = true;
            this.check_smoothing.Checked = true;
            this.check_smoothing.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_smoothing.Location = new System.Drawing.Point(9, 21);
            this.check_smoothing.Name = "check_smoothing";
            this.check_smoothing.Size = new System.Drawing.Size(79, 16);
            this.check_smoothing.TabIndex = 0;
            this.check_smoothing.Text = "平滑化する";
            this.check_smoothing.UseVisualStyleBackColor = true;
            this.check_smoothing.CheckedChanged += new System.EventHandler(this.check_smoothing_CheckedChanged);
            // 
            // text_cutoff
            // 
            this.text_cutoff.Location = new System.Drawing.Point(132, 44);
            this.text_cutoff.Name = "text_cutoff";
            this.text_cutoff.Size = new System.Drawing.Size(38, 19);
            this.text_cutoff.TabIndex = 1;
            this.text_cutoff.Text = "5.0";
            // 
            // label_cutoff
            // 
            this.label_cutoff.AutoSize = true;
            this.label_cutoff.Location = new System.Drawing.Point(7, 47);
            this.label_cutoff.Name = "label_cutoff";
            this.label_cutoff.Size = new System.Drawing.Size(103, 12);
            this.label_cutoff.TabIndex = 2;
            this.label_cutoff.Text = "カットオフ周波数[Hz]";
            // 
            // group_reduction
            // 
            this.group_reduction.Controls.Add(this.label_th_morph);
            this.group_reduction.Controls.Add(this.label_th_rot);
            this.group_reduction.Controls.Add(this.label_th_pos);
            this.group_reduction.Controls.Add(this.text_th_morph);
            this.group_reduction.Controls.Add(this.text_th_rot);
            this.group_reduction.Controls.Add(this.text_th_pos);
            this.group_reduction.Controls.Add(this.check_reduction);
            this.group_reduction.Location = new System.Drawing.Point(217, 54);
            this.group_reduction.Name = "group_reduction";
            this.group_reduction.Size = new System.Drawing.Size(179, 119);
            this.group_reduction.TabIndex = 6;
            this.group_reduction.TabStop = false;
            this.group_reduction.Text = "間引き(reduction)";
            // 
            // label_th_pos
            // 
            this.label_th_pos.AutoSize = true;
            this.label_th_pos.Location = new System.Drawing.Point(7, 47);
            this.label_th_pos.Name = "label_th_pos";
            this.label_th_pos.Size = new System.Drawing.Size(87, 12);
            this.label_th_pos.TabIndex = 2;
            this.label_th_pos.Text = "位置の許容誤差";
            // 
            // text_th_pos
            // 
            this.text_th_pos.Location = new System.Drawing.Point(132, 44);
            this.text_th_pos.Name = "text_th_pos";
            this.text_th_pos.Size = new System.Drawing.Size(38, 19);
            this.text_th_pos.TabIndex = 1;
            this.text_th_pos.Text = "0.2";
            // 
            // check_reduction
            // 
            this.check_reduction.AutoSize = true;
            this.check_reduction.Checked = true;
            this.check_reduction.CheckState = System.Windows.Forms.CheckState.Checked;
            this.check_reduction.Location = new System.Drawing.Point(9, 21);
            this.check_reduction.Name = "check_reduction";
            this.check_reduction.Size = new System.Drawing.Size(120, 16);
            this.check_reduction.TabIndex = 0;
            this.check_reduction.Text = "キーフレームを間引く";
            this.check_reduction.UseVisualStyleBackColor = true;
            this.check_reduction.CheckedChanged += new System.EventHandler(this.check_reduction_CheckedChanged);
            // 
            // label_th_rot
            // 
            this.label_th_rot.AutoSize = true;
            this.label_th_rot.Location = new System.Drawing.Point(7, 70);
            this.label_th_rot.Name = "label_th_rot";
            this.label_th_rot.Size = new System.Drawing.Size(87, 12);
            this.label_th_rot.TabIndex = 2;
            this.label_th_rot.Text = "角度の許容誤差";
            // 
            // text_th_rot
            // 
            this.text_th_rot.Location = new System.Drawing.Point(132, 67);
            this.text_th_rot.Name = "text_th_rot";
            this.text_th_rot.Size = new System.Drawing.Size(38, 19);
            this.text_th_rot.TabIndex = 1;
            this.text_th_rot.Text = "3.0";
            // 
            // label_th_morph
            // 
            this.label_th_morph.AutoSize = true;
            this.label_th_morph.Location = new System.Drawing.Point(7, 93);
            this.label_th_morph.Name = "label_th_morph";
            this.label_th_morph.Size = new System.Drawing.Size(90, 12);
            this.label_th_morph.TabIndex = 2;
            this.label_th_morph.Text = "モーフの許容誤差";
            // 
            // text_th_morph
            // 
            this.text_th_morph.Location = new System.Drawing.Point(132, 90);
            this.text_th_morph.Name = "text_th_morph";
            this.text_th_morph.Size = new System.Drawing.Size(38, 19);
            this.text_th_morph.TabIndex = 1;
            this.text_th_morph.Text = "0.1";
            // 
            // readfacevmd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(563, 384);
            this.Controls.Add(this.group_reduction);
            this.Controls.Add(this.group_smoothing);
            this.Controls.Add(this.console_message);
            this.Controls.Add(this.button_run);
            this.Controls.Add(this.button_input_file);
            this.Controls.Add(this.text_input_file);
            this.Controls.Add(this.label_input_file);
            this.Name = "readfacevmd";
            this.Text = "readfacevmd";
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

