using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;

namespace ResourceTool.ViewModel
{
    public class TextureViewModel : ResourceViewModel
    {
        private string? _path = null;
        public string? Path
        {
            get { return _path; }
            set
            {
                if (value == _path)
                {
                    return;
                }

                _path = value;
                OnPropertyChanged();
            }
        }

        private Image? _image;
        public BitmapSource? BitmapSource 
        { 
            get 
            { 
                if (_image == null)
                {
                    return null;
                }

                return GetImageStream(_image); 
            } 
        }

        private string? ActualPath
        {
            get
            {
                if (string.IsNullOrEmpty(Path))
                {
                    return null;
                }

                var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
                if (resourceService == null)
                {
                    throw new NullReferenceException("Resorce service wasn't found");
                }

                string? texturesPath = resourceService.GetTexturesPath();
                if (string.IsNullOrEmpty(texturesPath))
                {
                    return null;
                }

                return System.IO.Path.Combine(texturesPath, Path);
            }
        }


        public TextureViewModel(Guid id, string path, string name) : base(id, name)
        {
            Path = path;
        }

        public TextureViewModel(Texture texture) : base(texture.Id, texture.Name)
        {
            Path = texture.Path;
        }

        public override void RestoreLinks()
        {
            UpdateImage();
        }


        public Texture GetModel()
        {
            return new Texture(
                Id,
                Path!,
                Name!
                );
        }


        private void UpdateImage()
        {
            string actualPath = ActualPath;
            if (!File.Exists(actualPath))
            {
                _image = null;
                return;
            }

            using (FileStream fs = new FileStream(actualPath, FileMode.Open))
            {
                _image = Image.FromStream(fs);
            }
        }

        [DllImport("gdi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool DeleteObject(IntPtr value);

        private static BitmapSource GetImageStream(Image myImage)
        {
            var bitmap = new Bitmap(myImage);
            IntPtr bmpPt = bitmap.GetHbitmap();

            BitmapSource bitmapSource =
             System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                   bmpPt,
                   IntPtr.Zero,
                   Int32Rect.Empty,
                   BitmapSizeOptions.FromEmptyOptions()
                   );

            // Freeze bitmapSource and clear memory to avoid memory leaks
            bitmapSource.Freeze();

            DeleteObject(bmpPt);
            bitmap.Dispose();

            return bitmapSource;
        }

        /// <summary>
        /// Resize the image to the specified width and height.
        /// </summary>
        /// <param name="image">The image to resize.</param>
        /// <param name="width">The width to resize to.</param>
        /// <param name="height">The height to resize to.</param>
        /// <returns>The resized image.</returns>
        public static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.HighQuality;
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }

            return destImage;
        }
    }
}
