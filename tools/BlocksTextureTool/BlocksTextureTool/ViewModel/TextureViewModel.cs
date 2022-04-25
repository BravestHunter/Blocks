using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;

namespace BlocksTextureTool.ViewModel
{
    public class TextureViewModel : BaseViewModel
    {
        private readonly string _path;
        public string Path { get { return _path; } }

        private string _name;
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                if (string.IsNullOrEmpty(_name))
                {
                    throw new ArgumentException(nameof(Name));
                }

                _name = value;
                OnPropertyChanged();
            }
        }

        private readonly Image _image;
        public Image Image { get { return _image; } }
        public BitmapSource BitmapSource { get { return GetImageStream(_image); } }

        public TextureViewModel(string path, string name, Image image)
        {
            if (string.IsNullOrEmpty(path))
            {
                throw new ArgumentNullException(nameof(path));
            }
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (image == null)
            {
                throw new ArgumentNullException(nameof(image));
            }

            _path = path;
            _name = name;
            _image = image;
        }

        [DllImport("gdi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool DeleteObject(IntPtr value);

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
