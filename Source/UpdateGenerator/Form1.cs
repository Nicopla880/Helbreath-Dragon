using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security.Cryptography;

namespace UpdateGenerator
{
    public partial class Form1 : Form
    {
        string updateFile = Application.StartupPath + "\\latest.txt";
        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            FileStream c = File.Create(updateFile);
            c.Close();
           
            foreach (string f in Directory.GetFiles(Application.StartupPath, "*"))
            {
                try
                {
                    if (f.ToLower() == Application.ExecutablePath.ToLower()) continue;
                    if (f == updateFile) continue;
                    FileStream file = new FileStream(f, FileMode.Open);
                    StreamWriter SW;
                    SW = File.AppendText(updateFile);
                    SW.WriteLine(f.Replace(Application.StartupPath, "") + "*" + getMd5Hash(file));
                    SW.Close();
                    file.Close();
                }
                catch (System.Exception excpt)
                {
                    Console.WriteLine(excpt.Message);
                }
            }
           

            DirSearch(Application.StartupPath);
            MessageBox.Show("Done");
        }


        void DirSearch(string sDir)
        {
            try
            {
                foreach (string d in Directory.GetDirectories(sDir))
                {
                    string dir = d.Replace(Application.StartupPath, "").ToLower();
                    if (dir.Contains("save")) continue;
                    if (dir.Contains("contents")) continue;
                    if (dir.Contains("updates")) continue;
                    if (dir.Contains("conduct")) continue;
                    if (dir.Contains("svn")) continue;
                    foreach (string f in Directory.GetFiles(d, "*"))
                    {
                        try
                        {
                            if (f == Path.GetFileName(Application.ExecutablePath)) continue;
                            FileStream file = new FileStream(f, FileMode.Open);
                            StreamWriter SW;
                            SW = File.AppendText(updateFile);
                            SW.WriteLine(f.Replace(Application.StartupPath, "") + "*" + getMd5Hash(file));
                            SW.Close();
                            file.Close();
                        }
                        catch(System.Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }
                    DirSearch(d);
                }
            }
            catch (System.Exception excpt)
            {
                Console.WriteLine(excpt.Message);
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
    }
}