using ResourceTool.Model;
using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Drawing;
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


        public TextureViewModel(Guid id, string path, string name) : base(id, name)
        {
            Path = path;

            UpdateImage();
        }

        public TextureViewModel(Texture texture) : base(texture.Id, texture.Name)
        {
            Path = texture.Path;

            UpdateImage();
        }

        public override void RestoreLinks()
        {
            // Nothing to do here
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
            if (!File.Exists(Path))
            {
                _image = null;
                return;
            }

            using (FileStream fs = new FileStream(Path, FileMode.Open))
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
    }
}
