using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Collections;
using System.Text;
using System.Data;

namespace Pak_Editor
{
    public partial class Form1 : Form
    {
        const int minFixX = -240, minFixY = -240;
        const int maxFixX = 240, maxFixY = 240;

        List<Pak> Paks = new List<Pak>();
        Timer statusClearTimer;
        Timer aniTimer;
        Form aniForm;

        int curSprite;
        int curPak;
        int curFrame;

        public Form1()
        {
            InitializeComponent();
        }



        private void openFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Multiselect = true;
            fileDialog.Filter = "Pak file (*.pak)|*.pak|Spak file (*.spak)|*.spak|All files (*.*)|*.*";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (string fileName in fileDialog.FileNames)
                {
                    Pak pk = new Pak();
                    if (pk.LoadPak(fileName))
                    {
                        AddPak(pk);
                    }
                }

                if (Paks.Count > 0)
                {
                    ChangePak(Paks.Count - 1);
                    UpdateControls();
                }

                SetGeneralStatus((Paks.Count > 0 ? Paks.Count.ToString() : "No") + " pak file" + (Paks.Count == 1 ? " was" : "s were") + " loaded");
            }
        }

        private void pic_Paint(object sender, PaintEventArgs e)
        {
            if (displayFrames.Checked && picBox.Image != null)
            {
                if (curPak != -1 && curSprite != -1)
                {
                    List<Rectangle> adjustedRects = GetAdjustedImageRects();
                    for (int frameNo = 0; frameNo < adjustedRects.Count; frameNo++)
                    {
                        if (frameNo == curFrame) continue;
                        e.Graphics.DrawRectangle(Pens.Red, adjustedRects[frameNo]);
                    }

                    if (curFrame != -1)
                        e.Graphics.DrawRectangle(Pens.Yellow, adjustedRects[curFrame]);
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            TxtTBarLink.SetupLink(txtX, tbarX, "setX");
            TxtTBarLink.SetupLink(txtY, tbarY, "setY");
            TxtTBarLink.SetupLink(txtWidth, tbarWidth, "setWidth");
            TxtTBarLink.SetupLink(txtHeight, tbarHeight, "setHeight");
            TxtTBarLink.SetupLink(txtFixX, tbarFixX, "setFixX");
            TxtTBarLink.SetupLink(txtFixY, tbarFixY, "setFixY");

            for (int i = 1; i < 31; i++)
            {
                fpsSelection.Items.Add(i + " fps");
            }
            fpsSelection.SelectedIndex = 11;

            Reset();
        }

        private void displayFrames_Click(object sender, EventArgs e)
        {
            displayFrames.Checked = !displayFrames.Checked;
            picBox.Refresh();
        }

        private void picBox_Click(object sender, System.EventArgs e)
        {
            if (picBox.Image == null) return;
            int selectedFrame = 0;
            foreach (Rectangle rect in GetAdjustedImageRects())
            {
                if (rect.Contains(((MouseEventArgs)e).Location))
                {
                    ChangeFrame(selectedFrame);
                    picBox.Refresh();
                    break;
                }
                selectedFrame++;
            }
        }

        #region ChangeView

        private void UpdateTitle()
        {
            string title = "Pak Editor";
            if (curPak != -1)
                title += " - " + fileList.Items[curPak];

            if (curSprite != -1)
                title += " : Sprite " + curSprite;

            if (curFrame != -1)
                title += " : Frame " + curFrame;

            this.Text = title;
        }

        private void ChangePak(int newPak)
        {
            if (Paks.Count == 0)
                curPak = -1;
            else
                curPak = newPak;

            ChangeSprite(0);
            RefreshSpriteTree();
            UpdateTitle();
        }

        private void ChangeSprite(int newSprite)
        {
            if (curPak == -1 || Paks[curPak].sprites.Count == 0)
            {
                UpdateControls();
                curSprite = -1;
                picBox.Image = null;
                return;
            }

            picBox.Image = Paks[curPak].sprites[newSprite].image;

            if (picBox.Image.Height > picBox.ClientSize.Height)
            {
                this.Height += picBox.Image.Height - picBox.ClientSize.Height;
            }
            if (picBox.Image.Width > picBox.ClientSize.Width)
            {
                this.Width += picBox.Image.Width - picBox.ClientSize.Width;
            }

            SetPakStatus(Paks[curPak].sprites.Count, Paks[curPak].sprites[newSprite].frames.Count, picBox.Image.Width, picBox.Image.Height);

            curSprite = newSprite;

            ChangeFrame(0);
            UpdateTitle();

            if (fixedScaling.Checked)
                UseScaledBitmap();

        }

        private void ChangeFrame(int newFrame)
        {
            if (curPak == -1 || curSprite == -1 || Paks[curPak].sprites[curSprite].frames.Count == 0)
            {
                SetFrameControls(false);
                curFrame = -1;
                return;
            }
            SetFrameControls(true);

            Pak.FrameInfo frameInf = Paks[curPak].sprites[curSprite].frames[newFrame];
            int width = Paks[curPak].sprites[curSprite].image.Width;
            int height = Paks[curPak].sprites[curSprite].image.Height;

            TxtTBarLink.SetValue(tbarX, frameInf.X, 0, width);
            TxtTBarLink.SetValue(tbarY, frameInf.Y, 0, height);

            TxtTBarLink.SetValue(tbarWidth, frameInf.Width, 0, width);
            TxtTBarLink.SetValue(tbarHeight, frameInf.Height, 0, height);

            TxtTBarLink.SetValue(tbarFixX, frameInf.fixX, minFixX, maxFixX);
            TxtTBarLink.SetValue(tbarFixY, frameInf.fixY, minFixY, maxFixY);

            curFrame = newFrame;

        }

        private void RefreshSpriteTree()
        {
            spriteTree.BeginUpdate();
            spriteTree.Nodes.Clear();
            for (int sprCount = 0; sprCount < Paks[curPak].sprites.Count; sprCount++)
            {
                TreeNode treeN = new TreeNode("Sprite " + sprCount);
                Pak.Sprite spr = Paks[curPak].sprites[sprCount];
                for (int frameCount = 0; frameCount < spr.frames.Count; frameCount++)
                {
                    treeN.Nodes.Add("Frame " + frameCount);
                }
                spriteTree.Nodes.Add(treeN);
            }

            spriteTree.EndUpdate();
            spriteTree.Refresh();

            if (curSprite != -1)
                spriteTree.Nodes[curSprite].Expand();
        }

        private void SetPakStatus(int spriteCount, int frameCount, int width, int height)
        {
            imgInfoLabel.Text = String.Format("Sprites: {0}  Frames: {1}  Width: {2}  Height: {3} ",
                    spriteCount, frameCount, width, height);

        }

        private void SetGeneralStatus(string text)
        {
            otherStatus.Text = text;
            if (statusClearTimer != null) statusClearTimer.Stop();
            statusClearTimer = new Timer();
            statusClearTimer.Interval = 20000;
            statusClearTimer.Tick += delegate(object sender, EventArgs e)
            {
                otherStatus.Text = "";
                statusClearTimer.Stop();
            };
            statusClearTimer.Start();
        }


        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ClosePak(curPak);
        }


        private void spriteTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent != null)
            {
                if (curSprite != e.Node.Parent.Index)
                    ChangeSprite(e.Node.Parent.Index);

                if (curFrame != e.Node.Index)
                    ChangeFrame(e.Node.Index);
            }
            else
            {
                if (curSprite != e.Node.Index)
                    ChangeSprite(e.Node.Index);
            }
            picBox.Refresh();
            //spriteTree.Nodes[curSprite].Expand();
        }



        private void Reset()
        {
            UpdateControls();
            spriteTree.Nodes.Clear();
            picBox.Image = null;
            SetPakStatus(0, 0, 0, 0);
            curFrame = -1;
            curPak = -1;
            curSprite = -1;
        }

        private void UpdateControls()
        {
            bool pakLoaded = Paks.Count > 0;
            bool hasSprite = Paks.Count > 0 && Paks[curPak].sprites.Count > 0;

            generateSpriteSheet.Enabled = pakLoaded;
            saveAs.Enabled = pakLoaded;
            savePak.Enabled = pakLoaded;
            saveImage.Enabled = pakLoaded && hasSprite;
            close.Enabled = pakLoaded;
            replaceImage.Enabled = pakLoaded && hasSprite;
            addFrame.Enabled = pakLoaded && hasSprite;
            deleteCurrentFrame.Enabled = pakLoaded && hasSprite;
            addSprite.Enabled = pakLoaded;
            deleteSprite.Enabled = pakLoaded && hasSprite;
            startAnimation.Enabled = pakLoaded && hasSprite;

            SetFrameControls(pakLoaded && hasSprite);
        }

        private void SetFrameControls(bool enable)
        {
            if (enable == false)
            {
                TxtTBarLink.SetValue(tbarX, 0, 0, 0);
                TxtTBarLink.SetValue(tbarY, 0, 0, 0);
                TxtTBarLink.SetValue(tbarHeight, 0, 0, 0);
                TxtTBarLink.SetValue(tbarWidth, 0, 0, 0);
                TxtTBarLink.SetValue(tbarFixX, 0, 0, 0);
                TxtTBarLink.SetValue(tbarFixY, 0, 0, 0);
            }

            foreach (Control ctrl in frameCtrls.Controls)
                ctrl.Enabled = enable;
        }
        #endregion


        #region FrameAdjustments
        private List<Rectangle> GetAdjustedImageRects()
        {
            List<Rectangle> rects = new List<Rectangle>();
            Rectangle imgRect = picBox.ClientRectangle;
            float ratioX, ratioY;
            int adjX, adjY;

            switch (picBox.SizeMode)
            {
                case PictureBoxSizeMode.CenterImage:
                    int factor = 1;
                    if (fixedScaling.Checked)
                    {
                        factor = GetScaleFactor();
                    }
                    adjX = (imgRect.Right - picBox.Image.Width) / 2;
                    adjY = (imgRect.Bottom - picBox.Image.Height) / 2;
                    foreach (Pak.FrameInfo fi in Paks[curPak].sprites[curSprite].frames)
                    {
                        Rectangle rect = fi.rect;
                        if (factor > 1)
                        {
                            rect.Y *= factor;
                            rect.X *= factor;
                            rect.Width *= factor;
                            rect.Height *= factor;
                        }
                        rect.Offset(adjX, adjY);
                        rects.Add(rect);
                    }
                    break;
                case PictureBoxSizeMode.StretchImage:
                    ratioX = imgRect.Right / (float)picBox.Image.Width;
                    ratioY = imgRect.Bottom / (float)picBox.Image.Height;
                    foreach (Pak.FrameInfo fi in Paks[curPak].sprites[curSprite].frames)
                    {
                        Rectangle rect = fi.rect;
                        rect.X = (int)Math.Round(rect.X * ratioX);
                        rect.Y = (int)Math.Round(rect.Y * ratioY);
                        ScaleRect(ref rect, ratioX, ratioY);

                        rects.Add(rect);
                    }
                    break;
                case PictureBoxSizeMode.Zoom:
                    adjX = (int)Math.Round((float)imgRect.Width / 2);
                    adjY = (int)Math.Round((float)imgRect.Height / 2);
                    if ((picBox.Image.Width / (double)picBox.Image.Height) > (imgRect.Right / (float)imgRect.Bottom)) //image is wider than picbox
                    {
                        ratioY = ratioX = (imgRect.Right / (float)Paks[curPak].sprites[curSprite].image.Width);
                        foreach (Pak.FrameInfo fi in Paks[curPak].sprites[curSprite].frames)
                        {
                            Rectangle rect = fi.rect;
                            rect.X = (int)Math.Round(rect.X * ratioX);
                            rect.Y = (int)Math.Round(adjY - (Paks[curPak].sprites[curSprite].image.Height * ratioY) / 2 + (rect.Y * ratioY));
                            ScaleRect(ref rect, ratioX, ratioY);

                            rects.Add(rect);
                        }
                    }
                    else
                    {
                        ratioX = ratioY = (imgRect.Bottom / (float)Paks[curPak].sprites[curSprite].image.Height);
                        foreach (Pak.FrameInfo fi in Paks[curPak].sprites[curSprite].frames)
                        {
                            Rectangle rect = fi.rect;
                            rect.X = (int)Math.Round(adjX - (Paks[curPak].sprites[curSprite].image.Width * ratioX) / 2 + (rect.X * ratioX));
                            rect.Y = (int)Math.Round(rect.Y * ratioY);
                            ScaleRect(ref rect, ratioX, ratioY);

                            rects.Add(rect);
                        }
                    }
                    break;
            }
            return rects;
        }

        private void ScaleRect(ref Rectangle rect, float ratioX, float ratioY)
        {
            rect.Width = (int)Math.Round(rect.Width * ratioX);
            rect.Height = (int)Math.Round(rect.Height * ratioY);
        }

        private void txt_Validating(object sender, System.ComponentModel.CancelEventArgs e)
        {
            int lowerbound, upperbound, textValue;

            TxtTBarLink.GetBounds(sender, out lowerbound, out upperbound);

            if (!int.TryParse(((TextBox)sender).Text, out textValue))
            {
                e.Cancel = true;
                return;
            }
            if (textValue < lowerbound)
            {
                textValue = lowerbound;
            }
            else if (textValue > upperbound)
            {
                textValue = upperbound;
            }
            SetFrameAttrib(sender, textValue);
        }

        private void txt_ValidateAdjust(object sender, System.ComponentModel.CancelEventArgs e)
        {
            int lowerbound, upperbound, textValue;

            TxtTBarLink.GetBounds(sender, out lowerbound, out upperbound);

            if (!int.TryParse(((TextBox)sender).Text, out textValue))
            {
                e.Cancel = true;
                return;
            }
            if (textValue < lowerbound)
            {
                TxtTBarLink.SetValue(sender, textValue, textValue);
            }
            else if (textValue > upperbound)
            {
                TxtTBarLink.SetValue(sender, textValue, lowerbound, textValue);
            }
            SetFrameAttrib(sender, textValue);
        }

        private void tbar_Scroll(object sender, EventArgs e)
        {
            SetFrameAttrib(sender, ((TrackBar)sender).Value);
        }

        private void SetFrameAttrib(object sender, int value)
        {
            TxtTBarLink.SetValue(sender, value);
            System.Reflection.MethodInfo MI = Type.GetType("Pak_Editor.Pak+FrameInfo").GetMethod(TxtTBarLink.GetSetMethod(sender));
            MI.Invoke(Paks[curPak].sprites[curSprite].frames[curFrame], new object[1] { (object)((Int16)value) });
            picBox.Refresh();
        }

        private void addFrameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Paks[curPak].sprites[curSprite].frames.Add(new Pak.FrameInfo());
            ChangeFrame(Paks[curPak].sprites[curSprite].frames.Count - 1);
            SetGeneralStatus("Added new frame (Frame " + (Paks[curPak].sprites[curSprite].frames.Count - 1) + ")");
            RefreshSpriteTree();
        }

        private void deleteCurrentFrameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (curFrame == -1) return;
            Paks[curPak].sprites[curSprite].frames.RemoveAt(curFrame);
            SetGeneralStatus("Removed frame " + curFrame);
            ChangeFrame(0);
            RefreshSpriteTree();
            picBox.Refresh();
        }

        #endregion

        private void save_Click(object sender, EventArgs e)
        {
            int invalidFrame;
            for (int i = 0; i < Paks[curPak].sprites.Count; i++)
            {
                invalidFrame = GetInvalidFrame(Paks[curPak].sprites[i]);
                if (invalidFrame != -1)
                {
                    SetGeneralStatus("Warning: Frame " + invalidFrame + " in Sprite " + i + " is outside the image boundaries. Cannot save");
                    return;
                }
            }

            Paks[curPak].SavePak( fileList.Items[curPak].ToString() );
            SetGeneralStatus("Saved pak as " + fileList.Items[curPak]);
        }

        private void saveAs_Click(object sender, EventArgs e)
        {
            int invalidFrame;
            for (int i = 0; i < Paks[curPak].sprites.Count; i++)
            {
                invalidFrame = GetInvalidFrame(Paks[curPak].sprites[i]);
                if (invalidFrame != -1)
                {
                    SetGeneralStatus("Warning: Frame " + invalidFrame + " in Sprite " + i + " is outside the image boundaries. Cannot save");
                    return;
                }
            }
            SaveFileDialog saveDialog = new SaveFileDialog();
            saveDialog.Filter = "Pak file (*.pak)|*.pak|Spak file (*.spak)|*.spak|All files (*.*)|*.*";
            saveDialog.FileName = fileList.Items[curPak].ToString();
            if (saveDialog.ShowDialog() == DialogResult.OK)
            {
                Paks[curPak].SavePak(saveDialog.FileName);
                SetGeneralStatus("Saved pak " + fileList.Items[curPak] + " as " + Path.GetFileName(saveDialog.FileName));
            }
        }

        private void saveImage_Click(object sender, EventArgs e)
        {
            if (Paks.Count > 0)
            {
                const string filterStr = "Bitmap (*.bmp)|*.bmp|Gif (*.gif)|*.gif|Jpeg (*.jpg)|*.jpg|PNG (*.png)|*.png|All files (*.*)|*.*";
                SaveFileDialog saveDialog = new SaveFileDialog();
                saveDialog.Filter = filterStr;
                if (saveDialog.ShowDialog() == DialogResult.OK)
                {
                    ImageFormat imageFrmt;
                    switch (saveDialog.FilterIndex)
                    {
                        case 1: imageFrmt = ImageFormat.Bmp; break;
                        case 2: imageFrmt = ImageFormat.Gif; break;
                        case 3: imageFrmt = ImageFormat.Jpeg; break;
                        case 4: imageFrmt = ImageFormat.Png; break;
                        case 5: imageFrmt = ImageFormat.Bmp; break;
                        default: imageFrmt = ImageFormat.Bmp; break;
                    }
                    Paks[curPak].sprites[curSprite].image.Save(saveDialog.FileName, imageFrmt);
                    SetGeneralStatus("Saved sprite " + curSprite + " to " + Path.GetFileName(saveDialog.FileName));
                }
            }
        }

        private void replaceImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                var stream = File.OpenRead(fileDialog.FileName);
                Paks[curPak].sprites[curSprite].image = (Bitmap)Bitmap.FromStream(stream);
                stream.Close();
                SetGeneralStatus("Replaced image on sprite " + curSprite + " with " + Path.GetFileName(fileDialog.FileName));
                ChangeSprite(curSprite);
            }
        }

        private void fileList_Click(object sender, EventArgs e)
        {
            if (curPak != fileList.SelectedIndex && fileList.SelectedIndex != -1)
                ChangePak(fileList.SelectedIndex);
        }

        private void addSprite_Click(object sender, EventArgs e)
        {
            if (AddSprite())
            {
                SetGeneralStatus("Added new sprite (Sprite " + (Paks[curPak].sprites.Count - 1) + ")");
            }
        }

        private bool AddSprite()
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                Pak.Sprite newSprite = new Pak.Sprite();
                var stream = File.OpenRead(fileDialog.FileName);
                newSprite.image = (Bitmap)Bitmap.FromStream(stream);
                stream.Close();
                Paks[curPak].sprites.Add(newSprite);
                ChangeSprite(Paks[curPak].sprites.Count - 1);
                RefreshSpriteTree();
                UpdateControls();
                return true;
            }
            return false;
        }

        private void deleteSprite_Click(object sender, EventArgs e)
        {
            Paks[curPak].sprites.RemoveAt(curSprite);
            SetGeneralStatus("Removed Sprite " + curSprite);
            ChangeSprite(0);
            RefreshSpriteTree();
        }

        #region Animation
        private void startAnimation_Click(object sender, EventArgs e)
        {
            if (aniForm != null)
            {
                aniForm.Close();
            }
            if (curPak == -1 || Paks[curPak].sprites.Count == 0) return;
            if (curSprite == -1 || Paks[curPak].sprites[curSprite].frames.Count == 0) return;

            int invalidFrame = GetInvalidFrame(Paks[curPak].sprites[curSprite]);
            if (invalidFrame != -1)
            {
                SetGeneralStatus("Warning: Frame " + invalidFrame + " in Sprite " + curPak + " is outside the image boundaries or 0 width/height. Cannot animate!");
                return;
            }


            aniForm = new Form();
            aniForm.Text = "Animation - Pak: " + fileList.Items[curPak] + "   Sprite: " + curSprite;
            aniForm.TopMost = true;

            PictureBox newPicBox = new PictureBox();
            newPicBox.Name = "animation";
            newPicBox.SizeMode = PictureBoxSizeMode.AutoSize;
            aniForm.Controls.Add(newPicBox);


            List<Bitmap> aniFrames = CutToFrames(Paks[curPak].sprites[curSprite].image, Paks[curPak].sprites[curSprite].frames);
            List<Pak.FrameInfo> frames = new List<Pak.FrameInfo>();

            int lowestFixX = int.MaxValue;
            int lowestFixY = int.MaxValue;
            int topWidth = 0;
            int topHeight = 0;


            for (int i = 0; i < Paks[curPak].sprites[curSprite].frames.Count; i++)
            {
                Pak.FrameInfo frame = Paks[curPak].sprites[curSprite].frames[i];

                if (frame.Width > topWidth) topWidth = frame.Width;
                if (frame.Height > topHeight) topHeight = frame.Height;
                if (frame.fixX < lowestFixX) lowestFixX = frame.fixX;
                if (frame.fixY < lowestFixY) lowestFixY = frame.fixY;
            }

            int maxDimension = topHeight > topWidth ? topHeight : topWidth;
            int scaleFactor = 1;

            if (scaleAnimation.Checked)
            {
                scaleFactor = (int)Math.Round((float)350 / maxDimension);
                if (scaleFactor < 1) scaleFactor = 1;

                topWidth *= scaleFactor;
                topHeight *= scaleFactor;
                lowestFixX *= scaleFactor;
                lowestFixY *= scaleFactor;
            }

            for (int i = 0; i < Paks[curPak].sprites[curSprite].frames.Count; i++)
            {
                Pak.FrameInfo frame = Paks[curPak].sprites[curSprite].frames[i];
                Pak.FrameInfo newFrame = new Pak.FrameInfo();
                newFrame.Y = frame.Y * scaleFactor;
                newFrame.X = frame.X * scaleFactor;
                newFrame.Width = frame.Width * scaleFactor;
                newFrame.Height = frame.Height * scaleFactor;
                newFrame.fixX = (Int16)(frame.fixX * scaleFactor);
                newFrame.fixY = (Int16)(frame.fixY * scaleFactor);
                frames.Add(newFrame);

                aniFrames[i] = ResizeImage(aniFrames[i], scaleFactor);

                frames[i].fixX -= (Int16)lowestFixX;
                frames[i].fixY -= (Int16)lowestFixY;
                if (aniForm.Width < (frames[i].Width + frames[i].fixX + 50))
                {
                    aniForm.Width = (frames[i].Width + frames[i].fixX + 50);
                }
                if (aniForm.Height < (frames[i].Height + frames[i].fixY + 50))
                {
                    aniForm.Height = (frames[i].Height + frames[i].fixY + 50);
                }
            }

            aniForm.BackColor = Paks[curPak].sprites[curSprite].image.GetPixel(0, 0);

            aniForm.FormClosed += delegate(Object sender2, FormClosedEventArgs e2)
            {
                this.aniTimer.Stop();
                this.aniTimer.Dispose();
            };

            aniForm.Show();

            aniForm.Tag = 0;
            if (aniTimer != null) aniTimer.Dispose();
            aniTimer = new Timer();
            if (fpsSelection.SelectedIndex == -1) fpsSelection.SelectedIndex = 11;
            newPicBox.Left = newPicBox.Top = 10;

            aniTimer.Interval = (int)Math.Round(1000 / (float)(fpsSelection.SelectedIndex + 1));
            aniTimer.Tick += delegate(object send, EventArgs ee)
            {
                newPicBox.Image = aniFrames[(int)aniForm.Tag];
                newPicBox.Left = (aniForm.Width - topWidth) / 2 + frames[(int)aniForm.Tag].fixX;
                newPicBox.Top = (aniForm.Height - topHeight) / 2 + frames[(int)aniForm.Tag].fixY;
                aniForm.Tag = ((int)aniForm.Tag) + 1;
                if ((int)aniForm.Tag >= aniFrames.Count)
                    aniForm.Tag = 0;
            };

            aniTimer.Start();
        }

        private void fpsSelection_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (fpsSelection.SelectedIndex != -1 && aniTimer != null)
            {
                aniTimer.Stop();
                aniForm.Tag = 0;
                aniTimer.Interval = (int)Math.Round(1000 / (float)(fpsSelection.SelectedIndex + 1));
                aniTimer.Start();
            }
        }

        private void scaleAnimation_Click(object sender, EventArgs e)
        {
            scaleAnimation.Checked = !scaleAnimation.Checked;
        }

        private List<Bitmap> CutToFrames(Bitmap img, List<Pak.FrameInfo> frms)
        {
            List<Bitmap> cutFrames = new List<Bitmap>();
            foreach (Pak.FrameInfo frm in frms)
            {
                cutFrames.Add(img.Clone(frm.rect, img.PixelFormat));
            }
            return cutFrames;
        }
        #endregion

        #region Scaling

        private void normalView_Click(object sender, EventArgs e)
        {
            normalView.Checked = true;
            scaledView.Checked = false;
            stretchedView.Checked = false;
            fixedScaling.Checked = false;
            picBox.SizeMode = PictureBoxSizeMode.CenterImage;
            if (curPak == -1 || curSprite == -1) return;
            picBox.Image = Paks[curPak].sprites[curSprite].image;
        }

        private void scaledView_Click(object sender, EventArgs e)
        {
            normalView.Checked = false;
            scaledView.Checked = true;
            stretchedView.Checked = false;
            fixedScaling.Checked = false;
            picBox.SizeMode = PictureBoxSizeMode.Zoom;
            if (curPak == -1 || curSprite == -1) return;
            picBox.Image = Paks[curPak].sprites[curSprite].image;
        }

        private void stretchedView_Click(object sender, EventArgs e)
        {
            normalView.Checked = false;
            scaledView.Checked = false;
            stretchedView.Checked = true;
            fixedScaling.Checked = false;
            picBox.SizeMode = PictureBoxSizeMode.StretchImage;
            if (curPak == -1 || curSprite == -1) return;
            picBox.Image = Paks[curPak].sprites[curSprite].image;

        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (picBox.Image == null) return;
            if (fixedScaling.Checked)
            {
                UseScaledBitmap();
            }
        }

        private void UseScaledBitmap()
        {
            if (curPak == -1 || curSprite == -1) return;
            picBox.Image = ResizeImage(Paks[curPak].sprites[curSprite].image, GetScaleFactor());
        }

        private int GetScaleFactor()
        {
            int factor;
            Bitmap currentImage = (Bitmap)Paks[curPak].sprites[curSprite].image;
            if (picBox.ClientRectangle.Width / currentImage.Width <
                picBox.ClientRectangle.Height / currentImage.Height)
            {
                factor = (int)Math.Floor(picBox.ClientRectangle.Width / (float)currentImage.Width);
            }
            else
            {
                factor = (int)Math.Floor(picBox.ClientRectangle.Height / (float)currentImage.Height);
            }
            if (factor < 1) factor = 1;

            return factor;
        }

        private void fixedScaling_Click(object sender, EventArgs e)
        {
            normalView.Checked = false;
            scaledView.Checked = false;
            stretchedView.Checked = false;
            fixedScaling.Checked = true;
            picBox.SizeMode = PictureBoxSizeMode.CenterImage;
            UseScaledBitmap();
        }

        private Bitmap ResizeImage(Bitmap bmp, int factor)
        {
            if (factor == 1) return bmp;

            Bitmap newBmp = new Bitmap(bmp.Width * factor, bmp.Height * factor, bmp.PixelFormat);

            if (bmp.PixelFormat == PixelFormat.Format8bppIndexed)
            {
                newBmp.Palette = bmp.Palette;
                BitmapData oldBmd = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, bmp.PixelFormat);
                BitmapData newBmd = newBmp.LockBits(new Rectangle(0, 0, newBmp.Width, newBmp.Height), ImageLockMode.ReadOnly, newBmp.PixelFormat);

                unsafe
                {
                    for (int y = 0; y < bmp.Height; y++)
                    {
                        for (int x = 0; x < bmp.Width; x++)
                        {
                            byte colour = *((byte*)oldBmd.Scan0.ToPointer() + x + (y * oldBmd.Stride));
                            for (int newY = 0; newY < factor; newY++)
                            {
                                for (int newX = 0; newX < factor; newX++)
                                {
                                    *((byte*)newBmd.Scan0.ToPointer() + (x * factor) + newX + ((y * factor + newY) * newBmd.Stride)) = colour;
                                }
                            }
                        }
                    }
                }

                newBmp.UnlockBits(newBmd);
                bmp.UnlockBits(oldBmd);
            }
            else
            {
                for (int y = 0; y < bmp.Height; y++)
                {
                    for (int x = 0; x < bmp.Width; x++)
                    {
                        Color oldColor = bmp.GetPixel(x, y);
                        for (int newY = 0; newY < factor; newY++)
                        {
                            for (int newX = 0; newX < factor; newX++)
                            {
                                newBmp.SetPixel((x * factor) + newX, (y * factor) + newY, oldColor);
                            }
                        }
                    }
                }
            }
            return newBmp;
        }
        #endregion

        private int GetInvalidFrame(Pak.Sprite sprite)
        {
            GraphicsUnit graphicsUnit = GraphicsUnit.Pixel;
            for (int i = 0; i < sprite.frames.Count; i++)
            {
                if (!sprite.image.GetBounds(ref graphicsUnit).Contains(sprite.frames[i].rect) ||
                    sprite.frames[i].rect.Width == 0 ||
                    sprite.frames[i].rect.Height == 0)
                {
                    return i;
                }
            }
            return -1;
        }

        private void newFile_Click(object sender, EventArgs e)
        {
            AddPak(new Pak(), "New Pak" + Paks.Count + ".pak");
            ChangePak(Paks.Count - 1);

            UpdateControls();
        }

        private void AddPak(Pak toAdd, string filename = null)
        {
            if (filename != null)
            {
                toAdd.fileName = filename;
            }

            fileList.Items.Add(Path.GetFileName(toAdd.fileName));
            Paks.Add(toAdd);

        }

        private void ClosePak(int pakIndex)
        {
            Paks.RemoveAt(pakIndex);
            fileList.Items.RemoveAt(pakIndex);

            if (Paks.Count > 0)
            {
                ChangePak(0);
            }
            else
            {
                Reset();
            }
        }

        private void generateSpriteSheet_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "Bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
            fileDialog.Multiselect = true;
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {     
                List<Image> loadedImages = new List<Image>();
                List<Rectangle> frameBounds = new List<Rectangle>();

                String []splitFile = fileDialog.FileName.Split('.');
                int start;
                if (int.TryParse(splitFile[splitFile.Length - 2], out start) == false)
                {
                    MessageBox.Show("Unable to parse file name: " + Path.GetFileName(fileDialog.FileName), "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }
                splitFile[splitFile.Length - 2] = "{0:D2}";

                String pathFormat = String.Join(".", splitFile);

                for (int i = start; i < 100; i++)
                {
                    String fileName = String.Format(pathFormat, i);
                    try
                    {
                        var stream = File.OpenRead(fileName);
                        loadedImages.Add((Bitmap)Bitmap.FromStream(stream));
                        stream.Close();
                    }
                    catch (FileNotFoundException)
                    {
                        break;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(String.Format("Error occoured while loading image ({0}) : {1}", Path.GetFileName(fileName), ex.Message,
                            "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation));
                        if (Paks[curPak].sprites.Count == 0)
                            ClosePak(curPak);
                        return;
                    }
                }

                int totalWidth = 0, maxHeight = 0;

                foreach (Image frame in loadedImages)
                {
                    Rectangle bounds = getFrameBounds((Bitmap)frame);
                    if(bounds.Width < 1 || bounds.Height < 1)
                    {
                        MessageBox.Show("Could not find frame within image", "Error", MessageBoxButtons.OK,MessageBoxIcon.Exclamation);
                        return;
                    }
                    frameBounds.Add(bounds);
                    totalWidth += bounds.Width;
                    if (bounds.Height > maxHeight)
                        maxHeight = bounds.Height;
                }

                Bitmap generatedImage = new Bitmap(totalWidth, maxHeight, loadedImages[0].PixelFormat);
                generatedImage.SetResolution(loadedImages[0].HorizontalResolution, loadedImages[0].VerticalResolution);
                
                Pak.Sprite newSprite = new Pak.Sprite();

                using (Graphics g = Graphics.FromImage((Image)generatedImage))
                {
                    GraphicsUnit gUnit = GraphicsUnit.Pixel;
                    Brush keyBrush = new SolidBrush(((Bitmap)loadedImages[0]).GetPixel(0,0));
                    g.FillRectangle(keyBrush, generatedImage.GetBounds(ref gUnit));

                    for (int i = 0, x = 0; i < loadedImages.Count; i++)
                    {
                        g.DrawImage(loadedImages[i], x, 0, frameBounds[i], GraphicsUnit.Pixel);
                       
                        Pak.FrameInfo frmInfo = new Pak.FrameInfo();
                        Rectangle frameRect = frameBounds[i];
                        frameRect.Y = 0;
                        frameRect.X = x;
                        frmInfo.rect = frameRect;
                        frmInfo.fixX = (short)(loadedImages[i].Width /2 + frameBounds[i].Left);
                        frmInfo.fixY = (short)(loadedImages[i].Height /2 + frameBounds[i].Top);
                        newSprite.frames.Add(frmInfo);

                        x += frameBounds[i].Width;
                    }
                }

                newSprite.image = (Bitmap)generatedImage;
                
                Paks[curPak].sprites.Add(newSprite);
                ChangeSprite(Paks[curPak].sprites.Count - 1);
                RefreshSpriteTree();
                UpdateControls();
            }
        }

        private Rectangle getFrameBounds(Bitmap bmp)
        {
            int right = 0;
            int bot = 0;
            int left = int.MaxValue;
            int top = int.MaxValue;
            Color keyColor = bmp.GetPixel(0, 0);
            for (int x = 0; x < bmp.Width; x++)
            {
                for (int y = 0; y < bmp.Height; y++)
                {
                    if (bmp.GetPixel(x, y) == keyColor) continue;
                    if (x < left) left = x;
                    if (x > right) right = x;
                    if (y < top) top = y;
                    if (y > bot) bot = y;
                }
            }

            return new Rectangle(left, top, right - left, bot - top);
        }
    }
}
