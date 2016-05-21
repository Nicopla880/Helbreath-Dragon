namespace Pak_Editor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newFile = new System.Windows.Forms.ToolStripMenuItem();
            this.openFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.saveImage = new System.Windows.Forms.ToolStripMenuItem();
            this.savePak = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.close = new System.Windows.Forms.ToolStripMenuItem();
            this.exitMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addSprite = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteSprite = new System.Windows.Forms.ToolStripMenuItem();
            this.replaceImage = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.addFrame = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteCurrentFrame = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.startAnimation = new System.Windows.Forms.ToolStripMenuItem();
            this.fpsSelection = new System.Windows.Forms.ToolStripComboBox();
            this.scaleAnimation = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.pictureModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.normalView = new System.Windows.Forms.ToolStripMenuItem();
            this.fixedScaling = new System.Windows.Forms.ToolStripMenuItem();
            this.scaledView = new System.Windows.Forms.ToolStripMenuItem();
            this.stretchedView = new System.Windows.Forms.ToolStripMenuItem();
            this.displayFrames = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateSpriteSheet = new System.Windows.Forms.ToolStripMenuItem();
            this.fileList = new System.Windows.Forms.ListBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.imgInfoLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.otherStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.spriteTree = new System.Windows.Forms.TreeView();
            this.picBox = new System.Windows.Forms.PictureBox();
            this.frameCtrls = new System.Windows.Forms.GroupBox();
            this.frameCtrlsTable = new System.Windows.Forms.TableLayoutPanel();
            this.tbarX = new System.Windows.Forms.TrackBar();
            this.lblFixY = new System.Windows.Forms.Label();
            this.tbarY = new System.Windows.Forms.TrackBar();
            this.lblFixX = new System.Windows.Forms.Label();
            this.tbarWidth = new System.Windows.Forms.TrackBar();
            this.lblHeight = new System.Windows.Forms.Label();
            this.tbarHeight = new System.Windows.Forms.TrackBar();
            this.lblWidth = new System.Windows.Forms.Label();
            this.tbarFixX = new System.Windows.Forms.TrackBar();
            this.lblY = new System.Windows.Forms.Label();
            this.tbarFixY = new System.Windows.Forms.TrackBar();
            this.lblX = new System.Windows.Forms.Label();
            this.txtFixY = new System.Windows.Forms.TextBox();
            this.txtY = new System.Windows.Forms.TextBox();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.txtFixX = new System.Windows.Forms.TextBox();
            this.txtX = new System.Windows.Forms.TextBox();
            this.mainLayoutTable = new System.Windows.Forms.TableLayoutPanel();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picBox)).BeginInit();
            this.frameCtrls.SuspendLayout();
            this.frameCtrlsTable.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbarX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarFixX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarFixY)).BeginInit();
            this.mainLayoutTable.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.ImageScalingSize = new System.Drawing.Size(15, 15);
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Padding = new System.Windows.Forms.Padding(1, 1, 0, 1);
            this.menuStrip.Size = new System.Drawing.Size(928, 24);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newFile,
            this.openFile,
            this.toolStripSeparator3,
            this.saveImage,
            this.savePak,
            this.saveAs,
            this.toolStripSeparator1,
            this.close,
            this.exitMenu});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 22);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newFile
            // 
            this.newFile.Name = "newFile";
            this.newFile.Size = new System.Drawing.Size(155, 22);
            this.newFile.Text = "New";
            this.newFile.Click += new System.EventHandler(this.newFile_Click);
            // 
            // openFile
            // 
            this.openFile.Name = "openFile";
            this.openFile.Size = new System.Drawing.Size(155, 22);
            this.openFile.Text = "Open";
            this.openFile.Click += new System.EventHandler(this.openFile_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(152, 6);
            // 
            // saveImage
            // 
            this.saveImage.Enabled = false;
            this.saveImage.Name = "saveImage";
            this.saveImage.Size = new System.Drawing.Size(155, 22);
            this.saveImage.Text = "Save image";
            this.saveImage.Click += new System.EventHandler(this.saveImage_Click);
            // 
            // savePak
            // 
            this.savePak.Enabled = false;
            this.savePak.Name = "savePak";
            this.savePak.Size = new System.Drawing.Size(155, 22);
            this.savePak.Text = "Save pak";
            this.savePak.Click += new System.EventHandler(this.save_Click);
            // 
            // saveAs
            // 
            this.saveAs.Enabled = false;
            this.saveAs.Name = "saveAs";
            this.saveAs.Size = new System.Drawing.Size(155, 22);
            this.saveAs.Text = "Save pak as...";
            this.saveAs.Click += new System.EventHandler(this.saveAs_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(152, 6);
            // 
            // close
            // 
            this.close.Enabled = false;
            this.close.Name = "close";
            this.close.Size = new System.Drawing.Size(155, 22);
            this.close.Text = "Close";
            this.close.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // exitMenu
            // 
            this.exitMenu.Name = "exitMenu";
            this.exitMenu.Size = new System.Drawing.Size(155, 22);
            this.exitMenu.Text = "Exit";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addSprite,
            this.deleteSprite,
            this.replaceImage,
            this.toolStripSeparator2,
            this.addFrame,
            this.deleteCurrentFrame});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 22);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // addSprite
            // 
            this.addSprite.Enabled = false;
            this.addSprite.Name = "addSprite";
            this.addSprite.Size = new System.Drawing.Size(190, 22);
            this.addSprite.Text = "Add sprite";
            this.addSprite.Click += new System.EventHandler(this.addSprite_Click);
            // 
            // deleteSprite
            // 
            this.deleteSprite.Enabled = false;
            this.deleteSprite.Name = "deleteSprite";
            this.deleteSprite.Size = new System.Drawing.Size(190, 22);
            this.deleteSprite.Text = "Delete sprite";
            this.deleteSprite.Click += new System.EventHandler(this.deleteSprite_Click);
            // 
            // replaceImage
            // 
            this.replaceImage.Enabled = false;
            this.replaceImage.Name = "replaceImage";
            this.replaceImage.Size = new System.Drawing.Size(190, 22);
            this.replaceImage.Text = "Replace image";
            this.replaceImage.Click += new System.EventHandler(this.replaceImage_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(187, 6);
            // 
            // addFrame
            // 
            this.addFrame.Enabled = false;
            this.addFrame.Name = "addFrame";
            this.addFrame.Size = new System.Drawing.Size(190, 22);
            this.addFrame.Text = "Add frame";
            this.addFrame.Click += new System.EventHandler(this.addFrameToolStripMenuItem_Click);
            // 
            // deleteCurrentFrame
            // 
            this.deleteCurrentFrame.Enabled = false;
            this.deleteCurrentFrame.Name = "deleteCurrentFrame";
            this.deleteCurrentFrame.Size = new System.Drawing.Size(190, 22);
            this.deleteCurrentFrame.Text = "Delete selected frame";
            this.deleteCurrentFrame.Click += new System.EventHandler(this.deleteCurrentFrameToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.startAnimation,
            this.fpsSelection,
            this.scaleAnimation,
            this.toolStripSeparator4,
            this.pictureModeToolStripMenuItem,
            this.displayFrames});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Padding = new System.Windows.Forms.Padding(4, 0, 4, 1);
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 22);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // startAnimation
            // 
            this.startAnimation.Enabled = false;
            this.startAnimation.Name = "startAnimation";
            this.startAnimation.Size = new System.Drawing.Size(159, 22);
            this.startAnimation.Text = "Start animation";
            this.startAnimation.Click += new System.EventHandler(this.startAnimation_Click);
            // 
            // fpsSelection
            // 
            this.fpsSelection.Margin = new System.Windows.Forms.Padding(0);
            this.fpsSelection.MaxDropDownItems = 10;
            this.fpsSelection.Name = "fpsSelection";
            this.fpsSelection.Size = new System.Drawing.Size(90, 21);
            this.fpsSelection.SelectedIndexChanged += new System.EventHandler(this.fpsSelection_SelectedIndexChanged);
            // 
            // scaleAnimation
            // 
            this.scaleAnimation.Name = "scaleAnimation";
            this.scaleAnimation.Size = new System.Drawing.Size(159, 22);
            this.scaleAnimation.Text = "Scale animation";
            this.scaleAnimation.Click += new System.EventHandler(this.scaleAnimation_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(156, 6);
            // 
            // pictureModeToolStripMenuItem
            // 
            this.pictureModeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.normalView,
            this.fixedScaling,
            this.scaledView,
            this.stretchedView});
            this.pictureModeToolStripMenuItem.Name = "pictureModeToolStripMenuItem";
            this.pictureModeToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.pictureModeToolStripMenuItem.Text = "Picture mode";
            // 
            // normalView
            // 
            this.normalView.Checked = true;
            this.normalView.CheckState = System.Windows.Forms.CheckState.Checked;
            this.normalView.Name = "normalView";
            this.normalView.Size = new System.Drawing.Size(176, 22);
            this.normalView.Text = "Normal";
            this.normalView.Click += new System.EventHandler(this.normalView_Click);
            // 
            // fixedScaling
            // 
            this.fixedScaling.Name = "fixedScaling";
            this.fixedScaling.Size = new System.Drawing.Size(176, 22);
            this.fixedScaling.Text = "High quality scaling";
            this.fixedScaling.Click += new System.EventHandler(this.fixedScaling_Click);
            // 
            // scaledView
            // 
            this.scaledView.Name = "scaledView";
            this.scaledView.Size = new System.Drawing.Size(176, 22);
            this.scaledView.Text = "Scaled";
            this.scaledView.Click += new System.EventHandler(this.scaledView_Click);
            // 
            // stretchedView
            // 
            this.stretchedView.Name = "stretchedView";
            this.stretchedView.Size = new System.Drawing.Size(176, 22);
            this.stretchedView.Text = "Stretched";
            this.stretchedView.Click += new System.EventHandler(this.stretchedView_Click);
            // 
            // displayFrames
            // 
            this.displayFrames.Checked = true;
            this.displayFrames.CheckState = System.Windows.Forms.CheckState.Checked;
            this.displayFrames.Name = "displayFrames";
            this.displayFrames.Size = new System.Drawing.Size(159, 22);
            this.displayFrames.Text = "Display frames";
            this.displayFrames.Click += new System.EventHandler(this.displayFrames_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateSpriteSheet});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 22);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // generateSpriteSheet
            // 
            this.generateSpriteSheet.Enabled = false;
            this.generateSpriteSheet.Name = "generateSpriteSheet";
            this.generateSpriteSheet.Size = new System.Drawing.Size(190, 22);
            this.generateSpriteSheet.Text = "Generate sprite sheet";
            this.generateSpriteSheet.Click += new System.EventHandler(this.generateSpriteSheet_Click);
            // 
            // fileList
            // 
            this.fileList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fileList.Location = new System.Drawing.Point(811, 3);
            this.fileList.Name = "fileList";
            this.mainLayoutTable.SetRowSpan(this.fileList, 2);
            this.fileList.Size = new System.Drawing.Size(114, 470);
            this.fileList.TabIndex = 2;
            this.fileList.Click += new System.EventHandler(this.fileList_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.imgInfoLabel,
            this.otherStatus});
            this.statusStrip.Location = new System.Drawing.Point(0, 500);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(928, 22);
            this.statusStrip.TabIndex = 3;
            // 
            // imgInfoLabel
            // 
            this.imgInfoLabel.Name = "imgInfoLabel";
            this.imgInfoLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // otherStatus
            // 
            this.otherStatus.Name = "otherStatus";
            this.otherStatus.Size = new System.Drawing.Size(913, 17);
            this.otherStatus.Spring = true;
            this.otherStatus.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // spriteTree
            // 
            this.spriteTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.spriteTree.Location = new System.Drawing.Point(3, 3);
            this.spriteTree.Name = "spriteTree";
            this.mainLayoutTable.SetRowSpan(this.spriteTree, 2);
            this.spriteTree.Size = new System.Drawing.Size(136, 470);
            this.spriteTree.TabIndex = 4;
            this.spriteTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.spriteTree_AfterSelect);
            // 
            // picBox
            // 
            this.picBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.picBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.picBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.picBox.Location = new System.Drawing.Point(145, 3);
            this.picBox.Name = "picBox";
            this.picBox.Size = new System.Drawing.Size(660, 386);
            this.picBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.picBox.TabIndex = 5;
            this.picBox.TabStop = false;
            this.picBox.Click += new System.EventHandler(this.picBox_Click);
            this.picBox.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_Paint);
            // 
            // frameCtrls
            // 
            this.frameCtrls.Controls.Add(this.frameCtrlsTable);
            this.frameCtrls.Dock = System.Windows.Forms.DockStyle.Fill;
            this.frameCtrls.Location = new System.Drawing.Point(145, 392);
            this.frameCtrls.Margin = new System.Windows.Forms.Padding(3, 0, 3, 3);
            this.frameCtrls.Name = "frameCtrls";
            this.frameCtrls.Padding = new System.Windows.Forms.Padding(0, 0, 0, 4);
            this.frameCtrls.Size = new System.Drawing.Size(660, 81);
            this.frameCtrls.TabIndex = 7;
            this.frameCtrls.TabStop = false;
            // 
            // frameCtrlsTable
            // 
            this.frameCtrlsTable.BackColor = System.Drawing.Color.Transparent;
            this.frameCtrlsTable.ColumnCount = 6;
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.66667F));
            this.frameCtrlsTable.Controls.Add(this.tbarX, 0, 0);
            this.frameCtrlsTable.Controls.Add(this.lblFixY, 5, 2);
            this.frameCtrlsTable.Controls.Add(this.tbarY, 1, 0);
            this.frameCtrlsTable.Controls.Add(this.lblFixX, 4, 2);
            this.frameCtrlsTable.Controls.Add(this.tbarWidth, 2, 0);
            this.frameCtrlsTable.Controls.Add(this.lblHeight, 3, 2);
            this.frameCtrlsTable.Controls.Add(this.tbarHeight, 3, 0);
            this.frameCtrlsTable.Controls.Add(this.lblWidth, 2, 2);
            this.frameCtrlsTable.Controls.Add(this.tbarFixX, 4, 0);
            this.frameCtrlsTable.Controls.Add(this.lblY, 1, 2);
            this.frameCtrlsTable.Controls.Add(this.tbarFixY, 5, 0);
            this.frameCtrlsTable.Controls.Add(this.lblX, 0, 2);
            this.frameCtrlsTable.Controls.Add(this.txtFixY, 5, 1);
            this.frameCtrlsTable.Controls.Add(this.txtY, 1, 1);
            this.frameCtrlsTable.Controls.Add(this.txtWidth, 2, 1);
            this.frameCtrlsTable.Controls.Add(this.txtHeight, 3, 1);
            this.frameCtrlsTable.Controls.Add(this.txtFixX, 4, 1);
            this.frameCtrlsTable.Controls.Add(this.txtX, 0, 1);
            this.frameCtrlsTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.frameCtrlsTable.Location = new System.Drawing.Point(0, 13);
            this.frameCtrlsTable.Name = "frameCtrlsTable";
            this.frameCtrlsTable.RowCount = 3;
            this.frameCtrlsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.frameCtrlsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 23F));
            this.frameCtrlsTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.frameCtrlsTable.Size = new System.Drawing.Size(660, 64);
            this.frameCtrlsTable.TabIndex = 19;
            // 
            // tbarX
            // 
            this.tbarX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarX.Location = new System.Drawing.Point(3, 3);
            this.tbarX.Name = "tbarX";
            this.tbarX.Size = new System.Drawing.Size(103, 22);
            this.tbarX.TabIndex = 0;
            this.tbarX.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarX.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblFixY
            // 
            this.lblFixY.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblFixY.AutoSize = true;
            this.lblFixY.Location = new System.Drawing.Point(587, 51);
            this.lblFixY.Name = "lblFixY";
            this.lblFixY.Size = new System.Drawing.Size(30, 13);
            this.lblFixY.TabIndex = 17;
            this.lblFixY.Text = "Fix Y";
            // 
            // tbarY
            // 
            this.tbarY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarY.Location = new System.Drawing.Point(112, 3);
            this.tbarY.Name = "tbarY";
            this.tbarY.Size = new System.Drawing.Size(103, 22);
            this.tbarY.TabIndex = 1;
            this.tbarY.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarY.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblFixX
            // 
            this.lblFixX.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblFixX.AutoSize = true;
            this.lblFixX.Location = new System.Drawing.Point(475, 51);
            this.lblFixX.Name = "lblFixX";
            this.lblFixX.Size = new System.Drawing.Size(30, 13);
            this.lblFixX.TabIndex = 16;
            this.lblFixX.Text = "Fix X";
            // 
            // tbarWidth
            // 
            this.tbarWidth.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarWidth.Location = new System.Drawing.Point(221, 3);
            this.tbarWidth.Name = "tbarWidth";
            this.tbarWidth.Size = new System.Drawing.Size(103, 22);
            this.tbarWidth.TabIndex = 2;
            this.tbarWidth.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarWidth.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblHeight
            // 
            this.lblHeight.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblHeight.AutoSize = true;
            this.lblHeight.Location = new System.Drawing.Point(362, 51);
            this.lblHeight.Name = "lblHeight";
            this.lblHeight.Size = new System.Drawing.Size(38, 13);
            this.lblHeight.TabIndex = 15;
            this.lblHeight.Text = "Height";
            // 
            // tbarHeight
            // 
            this.tbarHeight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarHeight.Location = new System.Drawing.Point(330, 3);
            this.tbarHeight.Name = "tbarHeight";
            this.tbarHeight.Size = new System.Drawing.Size(103, 22);
            this.tbarHeight.TabIndex = 3;
            this.tbarHeight.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarHeight.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblWidth
            // 
            this.lblWidth.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblWidth.AutoSize = true;
            this.lblWidth.Location = new System.Drawing.Point(255, 51);
            this.lblWidth.Name = "lblWidth";
            this.lblWidth.Size = new System.Drawing.Size(35, 13);
            this.lblWidth.TabIndex = 14;
            this.lblWidth.Text = "Width";
            // 
            // tbarFixX
            // 
            this.tbarFixX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarFixX.Location = new System.Drawing.Point(439, 3);
            this.tbarFixX.Name = "tbarFixX";
            this.tbarFixX.Size = new System.Drawing.Size(103, 22);
            this.tbarFixX.TabIndex = 4;
            this.tbarFixX.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarFixX.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblY
            // 
            this.lblY.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblY.AutoSize = true;
            this.lblY.Location = new System.Drawing.Point(156, 51);
            this.lblY.Name = "lblY";
            this.lblY.Size = new System.Drawing.Size(14, 13);
            this.lblY.TabIndex = 13;
            this.lblY.Text = "Y";
            // 
            // tbarFixY
            // 
            this.tbarFixY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbarFixY.Location = new System.Drawing.Point(548, 3);
            this.tbarFixY.Name = "tbarFixY";
            this.tbarFixY.Size = new System.Drawing.Size(109, 22);
            this.tbarFixY.TabIndex = 5;
            this.tbarFixY.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tbarFixY.Scroll += new System.EventHandler(this.tbar_Scroll);
            // 
            // lblX
            // 
            this.lblX.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lblX.AutoSize = true;
            this.lblX.Location = new System.Drawing.Point(47, 51);
            this.lblX.Name = "lblX";
            this.lblX.Size = new System.Drawing.Size(14, 13);
            this.lblX.TabIndex = 12;
            this.lblX.Text = "X";
            // 
            // txtFixY
            // 
            this.txtFixY.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtFixY.Location = new System.Drawing.Point(579, 31);
            this.txtFixY.MaxLength = 6;
            this.txtFixY.Name = "txtFixY";
            this.txtFixY.Size = new System.Drawing.Size(46, 20);
            this.txtFixY.TabIndex = 10;
            this.txtFixY.Validating += new System.ComponentModel.CancelEventHandler(this.txt_ValidateAdjust);
            // 
            // txtY
            // 
            this.txtY.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtY.Location = new System.Drawing.Point(140, 31);
            this.txtY.MaxLength = 6;
            this.txtY.Name = "txtY";
            this.txtY.Size = new System.Drawing.Size(46, 20);
            this.txtY.TabIndex = 7;
            this.txtY.Validating += new System.ComponentModel.CancelEventHandler(this.txt_Validating);
            // 
            // txtWidth
            // 
            this.txtWidth.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtWidth.Location = new System.Drawing.Point(249, 31);
            this.txtWidth.MaxLength = 6;
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new System.Drawing.Size(46, 20);
            this.txtWidth.TabIndex = 8;
            this.txtWidth.Validating += new System.ComponentModel.CancelEventHandler(this.txt_Validating);
            // 
            // txtHeight
            // 
            this.txtHeight.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtHeight.Location = new System.Drawing.Point(358, 31);
            this.txtHeight.MaxLength = 6;
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(46, 20);
            this.txtHeight.TabIndex = 9;
            this.txtHeight.Validating += new System.ComponentModel.CancelEventHandler(this.txt_Validating);
            // 
            // txtFixX
            // 
            this.txtFixX.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtFixX.Location = new System.Drawing.Point(467, 31);
            this.txtFixX.MaxLength = 6;
            this.txtFixX.Name = "txtFixX";
            this.txtFixX.Size = new System.Drawing.Size(46, 20);
            this.txtFixX.TabIndex = 11;
            this.txtFixX.Validating += new System.ComponentModel.CancelEventHandler(this.txt_ValidateAdjust);
            // 
            // txtX
            // 
            this.txtX.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.txtX.Location = new System.Drawing.Point(31, 31);
            this.txtX.MaxLength = 6;
            this.txtX.Name = "txtX";
            this.txtX.Size = new System.Drawing.Size(46, 20);
            this.txtX.TabIndex = 6;
            this.txtX.Validating += new System.ComponentModel.CancelEventHandler(this.txt_Validating);
            // 
            // mainLayoutTable
            // 
            this.mainLayoutTable.ColumnCount = 3;
            this.mainLayoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 142F));
            this.mainLayoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainLayoutTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.mainLayoutTable.Controls.Add(this.spriteTree, 0, 0);
            this.mainLayoutTable.Controls.Add(this.fileList, 2, 0);
            this.mainLayoutTable.Controls.Add(this.picBox, 1, 0);
            this.mainLayoutTable.Controls.Add(this.frameCtrls, 1, 1);
            this.mainLayoutTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainLayoutTable.Location = new System.Drawing.Point(0, 24);
            this.mainLayoutTable.Name = "mainLayoutTable";
            this.mainLayoutTable.RowCount = 2;
            this.mainLayoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainLayoutTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 84F));
            this.mainLayoutTable.Size = new System.Drawing.Size(928, 476);
            this.mainLayoutTable.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(928, 522);
            this.Controls.Add(this.mainLayoutTable);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.MinimumSize = new System.Drawing.Size(850, 400);
            this.Name = "Form1";
            this.Text = "Pak Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picBox)).EndInit();
            this.frameCtrls.ResumeLayout(false);
            this.frameCtrlsTable.ResumeLayout(false);
            this.frameCtrlsTable.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbarX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarFixX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbarFixY)).EndInit();
            this.mainLayoutTable.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newFile;
        private System.Windows.Forms.ToolStripMenuItem openFile;
        private System.Windows.Forms.ToolStripMenuItem savePak;
        private System.Windows.Forms.ToolStripMenuItem saveAs;
        private System.Windows.Forms.ToolStripMenuItem close;
        private System.Windows.Forms.ToolStripMenuItem exitMenu;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem displayFrames;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem saveImage;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ListBox fileList;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel imgInfoLabel;
        private System.Windows.Forms.TreeView spriteTree;
        private System.Windows.Forms.PictureBox picBox;
        private System.Windows.Forms.GroupBox frameCtrls;
        private System.Windows.Forms.ToolStripMenuItem addFrame;
        private System.Windows.Forms.ToolStripMenuItem deleteCurrentFrame;
        private System.Windows.Forms.ToolStripMenuItem replaceImage;
        private System.Windows.Forms.ToolStripStatusLabel otherStatus;
        private System.Windows.Forms.ToolStripMenuItem addSprite;
        private System.Windows.Forms.ToolStripMenuItem deleteSprite;
        private System.Windows.Forms.ToolStripMenuItem startAnimation;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem pictureModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem normalView;
        private System.Windows.Forms.ToolStripMenuItem scaledView;
        private System.Windows.Forms.ToolStripMenuItem stretchedView;
        private System.Windows.Forms.TableLayoutPanel frameCtrlsTable;
        private System.Windows.Forms.TrackBar tbarX;
        private System.Windows.Forms.Label lblFixY;
        private System.Windows.Forms.TrackBar tbarY;
        private System.Windows.Forms.Label lblFixX;
        private System.Windows.Forms.TrackBar tbarWidth;
        private System.Windows.Forms.Label lblHeight;
        private System.Windows.Forms.TrackBar tbarHeight;
        private System.Windows.Forms.Label lblWidth;
        private System.Windows.Forms.TrackBar tbarFixX;
        private System.Windows.Forms.Label lblY;
        private System.Windows.Forms.TrackBar tbarFixY;
        private System.Windows.Forms.Label lblX;
        private System.Windows.Forms.TextBox txtFixY;
        private System.Windows.Forms.TextBox txtX;
        private System.Windows.Forms.TextBox txtY;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.TextBox txtFixX;
        private System.Windows.Forms.TableLayoutPanel mainLayoutTable;
        private System.Windows.Forms.ToolStripMenuItem fixedScaling;
        private System.Windows.Forms.ToolStripComboBox fpsSelection;
        private System.Windows.Forms.ToolStripMenuItem scaleAnimation;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateSpriteSheet;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    }
}

