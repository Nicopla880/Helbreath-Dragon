using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using ICSharpCode.SharpZipLib.Zip;
using System.Security.Cryptography;

namespace AutoUpdater
{
    public partial class Form1 : Form
    {
        List<string> toUpdate = new List<string>(); 
        List<string> files = new List<string>();
        List<string> fileHashes = new List<string>();
        float updateCount = 0;

        const string baseUrl = "http://helbreathint.com/Updater/";
        const string updateFileName = "latest.txt";
        const int normalHeight = 149;
        const int logShownHeight = 360;
        string updatesFolder = Application.StartupPath + @"\updates\";

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            lb1.Items.Clear();
            toUpdate.Clear();
            files.Clear();
            fileHashes.Clear();
            if (!GetUpdateFile()) return;
            if (!ReadUpdateFile()) return;
            if (!ValidateFiles())
            {
                downloadFiles();
            }
            else
            {
                MessageBox.Show("Your client is already up to date");
            }
       }

        private void downloadFiles()
        {
            AddMessage("Downloading files...");
            Application.DoEvents();
            updateCount = toUpdate.Count;
            downloadSingleFile(toUpdate[0]);
        }

        private void downloadSingleFile(string name)
        {
            AddMessage("Downloading file: " + name);
            WebClient client = new WebClient();

            client.DownloadFileCompleted += new AsyncCompletedEventHandler(DownloadFileComplete);
            client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(DownloadProgressCallback);

            pb1.Value = 0;
            status.Text = name;
            client.QueryString.Add("filename", name);
            Directory.CreateDirectory(updatesFolder);
            client.DownloadFileAsync(new Uri(baseUrl + name), updatesFolder + name);
        }

        private void DownloadFileComplete(object sender, AsyncCompletedEventArgs e)
        {
           string relatedFile = ((WebClient)sender).QueryString["filename"];
           if(e.Error != null)
           {
               AddMessage("Error: File(" + relatedFile + ") " + e.Error.Message);
               File.Delete(updatesFolder + relatedFile);
           } 
           else if (e.Cancelled)
           {
               AddMessage("Download cancelled");
               File.Delete(updatesFolder + relatedFile);
           }
           else
           {
               TaskSuccess();
               ExtractFile(relatedFile);
           }
           float a = toUpdate.Count;
           double done = Math.Floor(((updateCount - a) / updateCount) * 100);
           pb2.Value = System.Convert.ToInt32(done);

           toUpdate.RemoveAt(0);
           if (toUpdate.Count > 0)
           {
               downloadSingleFile(toUpdate[0]);
           }
           else 
           {
               status.Text = "";
               pb1.Value = 0;
               pb2.Value = 0;
               AddMessage("Update complete");
               MessageBox.Show("Update complete");
           }
        }

        private void ExtractFile(string fileName)
        {
            FastZip fz = new FastZip();
            string fn = "";
            AddMessage("Extracting file: " + fileName);
            Application.DoEvents();
            foreach(string fileN in files)
            {
                if (fileN.Contains(fileName.Substring(0, fileName.LastIndexOf('.')))) 
                {
                    fn = fileN;
                    break;
                }
            }
            try
            {
                string x = Path.GetDirectoryName(fn).Trim('.');
                fz.ExtractZip(updatesFolder + fileName, Application.StartupPath + Path.GetDirectoryName(fn), "");
                TaskSuccess();
            }
            catch (Exception err)
            {
                AddMessage("Error extracting: " + err.Message );
            }
        }

        private void DownloadProgressCallback(object sender, DownloadProgressChangedEventArgs e)
        {
            pb1.Value = e.ProgressPercentage;
            float a = toUpdate.Count;
            double done = Math.Floor((((updateCount - a) / updateCount) * 100) 
                                        + (e.ProgressPercentage / updateCount));

            pb2.Value = System.Convert.ToInt32(done);
        }

        private bool GetUpdateFile()
        {
            AddMessage("Retrieving update file");
            Application.DoEvents();
            Directory.CreateDirectory(updatesFolder);
            WebClient Client = new WebClient();
            try
            {
                Client.DownloadFile(baseUrl + updateFileName, updatesFolder + updateFileName);
                TaskSuccess();
            }
            catch (System.Net.WebException err)
            {
                AddMessage("Error: " + err.Message);
                return false;
            }
            return true;            
        }

        private void AddMessage(string msg)
        {
            lb1.Items.Add(msg);
        }

        private void TaskSuccess()
        {
            lb1.Items[lb1.Items.Count - 1] += "   ...Done";
        }

        private bool ReadUpdateFile()
        {
            AddMessage("Reading update file");
            Application.DoEvents();
            StreamReader SR;
            string S;
            try
            {
                SR = File.OpenText(updatesFolder + updateFileName);
            }
            catch (System.IO.FileNotFoundException err)
            {
                AddMessage("Error: Update file not found");
                return false;
            }

            S = SR.ReadLine();
            while (S != null)
            {
                string[] fileInfo = S.Split('*');
                if (fileInfo.Length != 2)
                {
                    AddMessage("Error: Incorrect update file");
                    files.Clear();
                    fileHashes.Clear();
                    return false;
                }
                files.Add(fileInfo[0]);
                fileHashes.Add(fileInfo[1]);
                S = SR.ReadLine();
            }
            SR.Close();
            TaskSuccess();
            return true;
        }

        private bool ValidateFiles()
        {
            AddMessage("Validating files");
            for (int i = 0; i < files.Count; i++)
            {
                Application.DoEvents();
                bool validFile = true;
                if (Path.GetFileName(files[i]).ToLower() == Path.GetFileName(Application.ExecutablePath).ToLower()) continue;
                try
                {
                    FileStream file = new FileStream(Application.StartupPath + files[i], FileMode.Open);
                    if (getMd5Hash(file) != fileHashes[i])
                    {
                        validFile = false;
                        AddMessage("Wrong file version: " + Path.GetFileName(files[i]));
                    }
                    file.Close();
                }
                catch (System.IO.DirectoryNotFoundException err)
                {
                    AddMessage("Directory missing: " + Path.GetDirectoryName(Application.StartupPath + files[i]));
                    Directory.CreateDirectory(Path.GetDirectoryName(Application.StartupPath + files[i]));
                    AddMessage("Directory created: " + Path.GetDirectoryName(Application.StartupPath + files[i]));
                    validFile = false;
                }
                catch (System.IO.FileNotFoundException err)
                {
                    AddMessage("File missing: " + Path.GetFileName(files[i]));
                    validFile = false;
                }
                catch (System.IO.IOException err)
                {
                    AddMessage("File access error: " + Path.GetFileName(files[i]));
                    validFile = false;
                }
                if (!validFile)
                {
                    toUpdate.Add(Path.GetFileName(files[i]) + ".zip");
                }
            }
            TaskSuccess();
            if (toUpdate.Count > 0){
                return false;
            }
            else
            {
                return true;
            }
            
        }

        static string getMd5Hash(FileStream input)
        {
            MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
            byte[] data = md5.ComputeHash(input);
            
            StringBuilder sBuilder = new StringBuilder();

            for (int i = 0; i < data.Length; i++)
            {
                sBuilder.Append(data[i].ToString("x2"));
            }

            return sBuilder.ToString();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            this.Height = normalHeight;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (this.Height == normalHeight)
            {
                button2.Text = "Hide Log";
                this.Height = logShownHeight;
            }
            else
            {
                button2.Text = "Show Log";
                this.Height = normalHeight;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            TextWriter tw = new StreamWriter("updater log.txt");
            foreach(string logEntry in lb1.Items)
            {
                tw.WriteLine(logEntry);
            }
            tw.Close();
        }

    }
}