using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using SimplePaletteQuantizer.Helpers;
using SimplePaletteQuantizer.Quantizers;

namespace Pak_Editor
{
    class Pak
    {
        public List<Sprite> sprites = new List<Sprite>();
        public string fileName;

        #region BitmapFile
        public struct BitmapFile
        {
            public BitmapFileHeader bmpHeader;
            public BitmapInfo bmpInfo;
        }

        public struct BitmapInfoHeader
        {
            public UInt32 biSize;
            public Int32 biWidth;
            public Int32 biHeight;
            public UInt16 biPlanes;
            public UInt16 biBitCount;
            public UInt32 biCompression;
            public UInt32 biSizeImage;
            public Int32 biXPelsPerMeter;
            public Int32 biYPelsPerMeter;
            public UInt32 biClrUsed;
            public UInt32 biClrImportant;
        };

        public struct BitmapInfo
        {
            public BitmapInfoHeader bih;
            public Int32[] bmiColours;
        }

        public struct BitmapFileHeader
        {
            public UInt16 type;
            public UInt32 size;
            public UInt16 reserved1;
            public UInt16 reserved2;
            public UInt32 offBits;
        };
        #endregion

        #region SpriteInfo
        public class FrameInfo
        {
            public Rectangle rect;
            public short fixX;
            public short fixY;
            public int Width { get { return rect.Width; } set { rect.Width = value; } }
            public int Height { get { return rect.Height; } set { rect.Height = value; } }
            public int X { get { return rect.X; } set { rect.X = value; } }
            public int Y { get { return rect.Y; } set { rect.Y = value; } }
            public void setX(short _x) { X = _x; }
            public void setY(short _y) { Y = _y; }
            public void setWidth(short _width) { Width = _width; }
            public void setHeight(short _height) { Height = _height; }
            public void setFixX(short _fixX) { fixX = _fixX; }
            public void setFixY(short _fixY) { fixY = _fixY; }

        };

        public class Sprite 
        {
            public List<FrameInfo> frames;
            public Bitmap image;
            public Sprite() { frames = new List<FrameInfo>(); }
        };
        #endregion

        #region LoadPak
        public bool LoadPak(string file)
        {
            FileStream fs = null;
            try 
            {
                fileName = file;
                fs = new FileStream(file, FileMode.Open, FileAccess.Read);
                BinaryReader br = new BinaryReader(fs);

                br.BaseStream.Seek(20, SeekOrigin.Begin);
                int spriteCount = br.ReadInt32();

                if (file.EndsWith(".spak"))
                    spriteCount = spriteCount ^ 712345;

                if (spriteCount < 1 || spriteCount > 1000) {
                    throw new Exception("Incorrect number of sprites");
                }

                for (int i = 0; i < spriteCount; i++)
                {
                    br.BaseStream.Seek(24 + i * 8, SeekOrigin.Begin);
                    int start = br.ReadInt32();

                    br.BaseStream.Seek(start + 100, SeekOrigin.Begin);

                    int totalFrames = br.ReadInt32();
                    int bitmapStart = start + 108 + (12 * totalFrames);

                    Sprite spr = new Sprite();
                    spr.frames = ReadFrameInfo(br, totalFrames);

                    br.BaseStream.Seek(bitmapStart, SeekOrigin.Begin);
                    spr.image = ReadBitmap2(br);

                    sprites.Add(spr);
                }
                fs.Close();
                return true;
            }
            catch (Exception)
            {
                if (fs != null)
                    fs.Close();

                return sprites.Count != 0;
            }
        }

        private List<FrameInfo> ReadFrameInfo(BinaryReader br, int count)
        {
            List<FrameInfo> frameInfoList = new List<FrameInfo>();
            for (int j = 0; j < count; j++)
            {
                FrameInfo fi = new FrameInfo();
                fi.rect.X = br.ReadInt16();
                fi.rect.Y = br.ReadInt16();
                fi.rect.Width = br.ReadInt16();
                fi.rect.Height = br.ReadInt16();
                fi.fixX = br.ReadInt16();
                fi.fixY = br.ReadInt16();
                frameInfoList.Add(fi);
            }
            return frameInfoList;
        }

        private Bitmap ReadBitmap2(BinaryReader br)
        {
            BitmapFile BF = new BitmapFile();
            BF.bmpHeader = ReadBitmapFileHeader(br);
            br.BaseStream.Seek(-14, SeekOrigin.Current);
            byte[] bitmapData = br.ReadBytes((int)BF.bmpHeader.size);

            return new Bitmap(new MemoryStream(bitmapData));
        }

        #region ParseBitmap
        private Bitmap ReadBitmap(BinaryReader br)
        {
            BitmapFile BF = new BitmapFile();
            BF.bmpHeader = ReadBitmapFileHeader(br);
            BF.bmpInfo = ReadBitmapInfo(br);

            int Height = BF.bmpInfo.bih.biHeight;
            int Width = BF.bmpInfo.bih.biWidth;


            uint bitmapSize;

            if (BF.bmpInfo.bih.biSizeImage == 0)
            {
                bitmapSize = BF.bmpHeader.size - BF.bmpHeader.offBits;
            }
            else
            {
                bitmapSize = BF.bmpInfo.bih.biSizeImage;
            }


            int[] newbmp = new int[bitmapSize];
            byte[] bmpData = new byte[bitmapSize];
            bmpData = br.ReadBytes((int)(bitmapSize));

            int stride = Width * (BF.bmpInfo.bih.biBitCount / 8);
            if (stride % 4 != 0)
                stride += (4 - stride % 4); //Rounds up to nearest 4;

            Bitmap ret = new Bitmap(Width, Height, PixelFormat.Format32bppRgb);
            if (BF.bmpInfo.bmiColours != null)
            {
                Color[] cols = new Color[BF.bmpInfo.bmiColours.Length];
                for (int ind = 0; ind < cols.Length; ind++)
                {
                    cols[ind] = Color.FromArgb(BF.bmpInfo.bmiColours[ind]);
                }


                for (int m = 0; m < Height; m++)//Copy, inverse, translate from 8bpp indexed to 32bpp
                {
                    for (int k = 0; k < Width; k++)
                    {
                        ret.SetPixel(k, Height - m - 1, cols[bmpData[(m * stride) + k]]);
                    }
                }
            }
            else
            {
                BitmapData bmd = ret.LockBits(new Rectangle(0, 0, Width, Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppRgb);
                System.Runtime.InteropServices.Marshal.Copy(bmpData, 0, bmd.Scan0, (int)bitmapSize);
                ret.UnlockBits(bmd);
                ret.RotateFlip(RotateFlipType.RotateNoneFlipY);
            }

            return ret;
        }

        private BitmapFileHeader ReadBitmapFileHeader(BinaryReader br)
        {
            BitmapFileHeader BFH = new BitmapFileHeader();

            BFH.type = br.ReadUInt16();
            BFH.size = br.ReadUInt32();
            BFH.reserved1 = br.ReadUInt16();
            BFH.reserved2 = br.ReadUInt16();
            BFH.offBits = br.ReadUInt32();

            return BFH;
        }

        private BitmapInfo ReadBitmapInfo(BinaryReader br)
        {
            BitmapInfo BI = new BitmapInfo();

            BI.bih.biSize = br.ReadUInt32();
            BI.bih.biWidth = br.ReadInt32();
            BI.bih.biHeight = br.ReadInt32();
            BI.bih.biPlanes = br.ReadUInt16();
            BI.bih.biBitCount = br.ReadUInt16();
            BI.bih.biCompression = br.ReadUInt32();
            BI.bih.biSizeImage = br.ReadUInt32();
            BI.bih.biXPelsPerMeter = br.ReadInt32();
            BI.bih.biYPelsPerMeter = br.ReadInt32();
            BI.bih.biClrUsed = br.ReadUInt32();
            BI.bih.biClrImportant = br.ReadUInt32();

            uint colourCount = 256;
            if (BI.bih.biClrUsed == 0)
            {
                if (BI.bih.biBitCount == 24) colourCount = 0;
                else if (BI.bih.biBitCount == 8) colourCount = 256;
                else if (BI.bih.biBitCount == 1) colourCount = 2;
                else if (BI.bih.biBitCount == 4) colourCount = 16;
                else colourCount = 0;
            }
            else colourCount = BI.bih.biClrUsed;

            if (colourCount > 0)
            {
                BI.bmiColours = new Int32[colourCount];

                for (int i = 0; i < colourCount; i++)
                {
                    BI.bmiColours[i] = br.ReadInt32();
                }
            }

            return BI;
        }
        #endregion
        #endregion

        #region SavePak
        public bool SavePak(string file)
        {
            FileStream fs = new FileStream(file, FileMode.OpenOrCreate, FileAccess.Write);
            BinaryWriter bw = new BinaryWriter(fs);
            int previousEnd = 24 + 8 * sprites.Count;
            bw.Write("<Pak file header>".ToCharArray());
            bw.Seek(20, SeekOrigin.Begin);

            int spriteCount = file.EndsWith(".spak") ? sprites.Count ^ 712345 : sprites.Count;
            bw.Write(spriteCount);
            
            for (int i = 0; i < sprites.Count; i++)
            {
                bw.Seek(24 + i * 8, SeekOrigin.Begin);
                bw.Write(previousEnd);
                bw.Seek(previousEnd, SeekOrigin.Begin);
                WriteSprite(bw, sprites[i]);
                previousEnd = (int)bw.BaseStream.Position;
            }

            bw.Close();
            return true;
        }

        private void WriteSprite(BinaryWriter bw, Sprite spr)
        {
            int startPos = (int)bw.BaseStream.Position;
            bw.Write("<Sprite File Header>".ToCharArray());
            bw.Seek(startPos + 100, SeekOrigin.Begin);
            bw.Write(spr.frames.Count);

            foreach(FrameInfo frmInf in spr.frames)
            {
                bw.Write((Int16)frmInf.X);
                bw.Write((Int16)frmInf.Y);
                bw.Write((Int16)frmInf.Width);
                bw.Write((Int16)frmInf.Height);
                bw.Write(frmInf.fixX);
                bw.Write(frmInf.fixY);
            }

            bw.Seek(4, SeekOrigin.Current);


            Bitmap bmp8bpp;
            //if (spr.image.PixelFormat != PixelFormat.Format8bppIndexed)
            //    bmp8bpp = (Bitmap) GetQuantizedImage(spr.image);
            //else
                bmp8bpp = spr.image;

            //byte[] bytes =System.ComponentModel.TypeDescriptor.GetConverter(bmp).ConvertTo(bmp, typeof(byte[]));

            using(MemoryStream bmpMs = new MemoryStream(spr.image.Width * spr.image.Height * 8 + 1000))
            {
                bmp8bpp.Save(bmpMs, ImageFormat.Bmp);
                bw.Write(bmpMs.ToArray(), 0, (int) bmpMs.Length);
            }
        }


        

        #region 8BppConversion
        private Image GetQuantizedImage(Image image)
        {
            IColorQuantizer activeQuantizer = new SimplePaletteQuantizer.Quantizers.Octree.OctreeQuantizer();
            // checks whether a source image is valid
            if (image == null)
            {
                const String message = "Cannot quantize a null image.";
                throw new ArgumentNullException(message);
            }

            // locks the source image data
            Bitmap bitmap = (Bitmap)image;
            Rectangle bounds = Rectangle.FromLTRB(0, 0, bitmap.Width, bitmap.Height);
            BitmapData sourceData = bitmap.LockBits(bounds, ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

            // prepares time statistics variables
            TimeSpan duration = new TimeSpan(0);
            DateTime before;

            try
            {
                // initalizes the pixel read buffer
                Int32[] sourceBuffer = new Int32[image.Width];

                // sets the offset to the first pixel in the image
                Int64 sourceOffset = sourceData.Scan0.ToInt64();

                for (Int32 row = 0; row < image.Height; row++)
                {
                    // copies the whole row of pixels to the buffer
                    Marshal.Copy(new IntPtr(sourceOffset), sourceBuffer, 0, image.Width);

                    // scans all the colors in the buffer
                    /* foreach (Color color in sourceBuffer.Select(argb => Color.FromArgb(argb)))
                     {
                         before = DateTime.Now;
                         activeQuantizer.AddColor(color);
                         duration += DateTime.Now - before;
                     }*/
                    for (Int32 i = 0; i < sourceBuffer.Length; i++)
                    {
                        activeQuantizer.AddColor(Color.FromArgb(sourceBuffer[i]));
                    }

                    // increases a source offset by a row
                    sourceOffset += sourceData.Stride;
                }
            }
            catch
            {
                bitmap.UnlockBits(sourceData);
                throw;
            }

            Bitmap result = new Bitmap(image.Width, image.Height, PixelFormat.Format8bppIndexed);

            // calculates the palette
            try
            {
                Int32 colorCount = 256;//activeQuantizer.GetColorCount();
                List<Color> palette = activeQuantizer.GetPalette(colorCount);

                // sets our newly calculated palette to the target image
                ColorPalette imagePalette = result.Palette;

                for (Int32 index = 0; index < palette.Count; index++)
                {
                    imagePalette.Entries[index] = palette[index];
                }

                result.Palette = imagePalette;

            }
            catch (Exception)
            {
                bitmap.UnlockBits(sourceData);
                throw;
            }

            // locks the target image data
            BitmapData targetData = result.LockBits(bounds, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

            try
            {
                // initializes read/write buffers
                Byte[] targetBuffer = new Byte[result.Width];
                Int32[] sourceBuffer = new Int32[image.Width];

                // sets the offsets on the beginning of both source and target image
                Int64 sourceOffset = sourceData.Scan0.ToInt64();
                Int64 targetOffset = targetData.Scan0.ToInt64();

                for (Int32 row = 0; row < image.Height; row++)
                {
                    // reads the pixel row from the source image
                    Marshal.Copy(new IntPtr(sourceOffset), sourceBuffer, 0, image.Width);

                    // goes thru all the pixels, reads the color on the source image, and writes calculated palette index on the target
                    for (Int32 index = 0; index < image.Width; index++)
                    {
                        Color color = Color.FromArgb(sourceBuffer[index]);
                        before = DateTime.Now;
                        targetBuffer[index] = (Byte)activeQuantizer.GetPaletteIndex(color);
                        duration += DateTime.Now - before;
                    }

                    // writes the pixel row to the target image
                    Marshal.Copy(targetBuffer, 0, new IntPtr(targetOffset), result.Width);

                    // increases the offsets (on both images) by a row
                    sourceOffset += sourceData.Stride;
                    targetOffset += targetData.Stride;
                }
            }
            finally
            {
                // releases the locks on both images
                bitmap.UnlockBits(sourceData);
                result.UnlockBits(targetData);
            }

            // returns the quantized image
            return result;
        }

        #endregion
        #endregion
    }
}
