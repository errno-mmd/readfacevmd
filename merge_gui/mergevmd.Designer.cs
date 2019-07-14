namespace merge_gui
{
    partial class mergevmd
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mergevmd));
            this.label_body_file = new System.Windows.Forms.Label();
            this.text_body_file = new System.Windows.Forms.TextBox();
            this.button_body_file = new System.Windows.Forms.Button();
            this.button_run = new System.Windows.Forms.Button();
            this.console_message = new System.Windows.Forms.TextBox();
            this.label_face_file = new System.Windows.Forms.Label();
            this.text_face_file = new System.Windows.Forms.TextBox();
            this.group_input = new System.Windows.Forms.GroupBox();
            this.button_face_file = new System.Windows.Forms.Button();
            this.group_input.SuspendLayout();
            this.SuspendLayout();
            // 
            // label_body_file
            // 
            resources.ApplyResources(this.label_body_file, "label_body_file");
            this.label_body_file.Name = "label_body_file";
            this.label_body_file.Click += new System.EventHandler(this.label_input_file_Click);
            // 
            // text_body_file
            // 
            resources.ApplyResources(this.text_body_file, "text_body_file");
            this.text_body_file.Name = "text_body_file";
            // 
            // button_body_file
            // 
            resources.ApplyResources(this.button_body_file, "button_body_file");
            this.button_body_file.Name = "button_body_file";
            this.button_body_file.UseVisualStyleBackColor = true;
            this.button_body_file.Click += new System.EventHandler(this.button_body_file_Click);
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
            // label_face_file
            // 
            resources.ApplyResources(this.label_face_file, "label_face_file");
            this.label_face_file.Name = "label_face_file";
            // 
            // text_face_file
            // 
            resources.ApplyResources(this.text_face_file, "text_face_file");
            this.text_face_file.Name = "text_face_file";
            // 
            // group_input
            // 
            this.group_input.Controls.Add(this.button_face_file);
            this.group_input.Controls.Add(this.text_body_file);
            this.group_input.Controls.Add(this.label_body_file);
            this.group_input.Controls.Add(this.label_face_file);
            this.group_input.Controls.Add(this.text_face_file);
            this.group_input.Controls.Add(this.button_body_file);
            resources.ApplyResources(this.group_input, "group_input");
            this.group_input.Name = "group_input";
            this.group_input.TabStop = false;
            // 
            // button_face_file
            // 
            resources.ApplyResources(this.button_face_file, "button_face_file");
            this.button_face_file.Name = "button_face_file";
            this.button_face_file.UseVisualStyleBackColor = true;
            this.button_face_file.Click += new System.EventHandler(this.button_face_file_Click);
            // 
            // mergevmd
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.group_input);
            this.Controls.Add(this.console_message);
            this.Controls.Add(this.button_run);
            this.Name = "mergevmd";
            this.group_input.ResumeLayout(false);
            this.group_input.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_body_file;
        private System.Windows.Forms.TextBox text_body_file;
        private System.Windows.Forms.Button button_body_file;
        private System.Windows.Forms.Button button_run;
        private System.Windows.Forms.TextBox console_message;
        private System.Windows.Forms.Label label_face_file;
        private System.Windows.Forms.TextBox text_face_file;
        private System.Windows.Forms.GroupBox group_input;
        private System.Windows.Forms.Button button_face_file;
    }
}

