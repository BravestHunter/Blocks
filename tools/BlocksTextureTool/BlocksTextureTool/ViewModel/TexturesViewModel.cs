using BlocksTextureTool.Model;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace BlocksTextureTool.ViewModel
{
    public class TexturesViewModel : BaseViewModel
    {
        private ObservableCollection<TextureViewModel> _textures = new ObservableCollection<TextureViewModel>();
        public ObservableCollection<TextureViewModel> Textures { get { return _textures; } }

        private TextureViewModel _selectedTexture = null;
        public TextureViewModel SelectedTexture
        {
            get
            {
                return _selectedTexture;
            }
            set
            {
                _selectedTexture = value;
                OnPropertyChanged();
            }
        }

        private ICommand _loadTextureCommand = null;
        public ICommand LoadTextureCommand
        {
            get
            {
                if (_loadTextureCommand == null)
                {
                    _loadTextureCommand = new BaseCommand(LoadTextureCommandExecute);
                }

                return _loadTextureCommand;
            }
        }

        private ICommand _buildTexturesAtlasCommand = null;
        public ICommand BuildTexturesAtlasCommand
        {
            get
            {
                if (_buildTexturesAtlasCommand == null)
                {
                    _buildTexturesAtlasCommand = new BaseCommand(BuildTexturesAtlasCommandExecute);
                }

                return _buildTexturesAtlasCommand;
            }
        }

        public TexturesViewModel()
        {

        }


        private void LoadTextureCommandExecute(object parameter)
        {
            OpenFileDialog dialog = new OpenFileDialog()
            {
                Multiselect = false,
                Filter = "Image Files(*.BMP;*.JPG;*.PNG)|*.BMP;*.JPG;*.PNG|All files (*.*)|*.*"
            };

            bool? result = dialog.ShowDialog();
            if (result.HasValue && result.Value)
            {
                using (FileStream fs = new FileStream(dialog.FileName, FileMode.Open))
                {
                    Image image = Image.FromStream(fs);
                    Textures.Add(new TextureViewModel(dialog.FileName, Path.GetFileNameWithoutExtension(dialog.FileName), image));
                }
            }
        }

        private void BuildTexturesAtlasCommandExecute(object parameter)
        {
            int size = 1;
            while (size * size < Textures.Count)
            {
                size *= 2;
            }

            int blockSize = 64;
            int fullSize = size * blockSize;

            using (Bitmap bitmap = new Bitmap(fullSize, fullSize))
            {
                using (Graphics g = Graphics.FromImage(bitmap))
                {
                    for (int i = 0; i < Textures.Count; i++)
                    {
                        using (Bitmap resized = ResizeImage(Textures[i].Image, blockSize, blockSize))
                        {
                            int y = i / size;
                            int x = i - (y * size);

                            y = size - y - 1;

                            g.DrawImage(resized, x * blockSize, y * blockSize, blockSize, blockSize);
                        }
                    }
                }

                SaveFileDialog dialog = new SaveFileDialog();

                bool? result = dialog.ShowDialog();
                if (result.HasValue && result.Value)
                {
                    bitmap.Save(dialog.FileName, ImageFormat.Png);
                }
            }
        }

        /// <summary>
        /// Resize the image to the specified width and height.
        /// </summary>
        /// <param name="image">The image to resize.</param>
        /// <param name="width">The width to resize to.</param>
        /// <param name="height">The height to resize to.</param>
        /// <returns>The resized image.</returns>
        private static Bitmap ResizeImage(Image image, int width, int height)
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
