using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ResourceTool.ViewModel
{
    public class BlockViewModel : ResourceViewModel
    {
        private Guid[] _textures;
        public Guid[] Textures { get { return _textures; } }

        public Guid FrontTextureId
        {
            get { return _textures[(int)BlockSide.Front]; }
            set
            {
                _textures[(int)BlockSide.Front] = value;
                OnPropertyChanged();
            }
        }
        public Guid BackTextureId
        {
            get { return _textures[(int)BlockSide.Back]; }
            set
            {
                _textures[(int)BlockSide.Back] = value;
                OnPropertyChanged();
            }
        }
        public Guid RightTextureId
        {
            get { return _textures[(int)BlockSide.Right]; }
            set
            {
                _textures[(int)BlockSide.Right] = value;
                OnPropertyChanged();
            }
        }
        public Guid LeftTextureId
        {
            get { return _textures[(int)BlockSide.Left]; }
            set
            {
                _textures[(int)BlockSide.Left] = value;
                OnPropertyChanged();
            }
        }
        public Guid TopTextureId
        {
            get { return _textures[(int)BlockSide.Top]; }
            set
            {
                _textures[(int)BlockSide.Top] = value;
                OnPropertyChanged();
            }
        }
        public Guid BottomTextureId
        {
            get { return _textures[(int)BlockSide.Bottom]; }
            set
            {
                _textures[(int)BlockSide.Bottom] = value;
                OnPropertyChanged();
            }
        }

        public ICommand SelectTextureCommand { get; private init; }


        public BlockViewModel(Guid id, string name) : base(id, name)
        {
            SelectTextureCommand = new RelayCommand(SelectTextureCommandExecute);
        }

        public BlockViewModel(Guid id, string name, Guid[] textures) : this(id, name)
        {
            if (textures == null)
            {
                throw new ArgumentNullException(nameof(textures));
            }
            if (textures.Length != 6)
            {
                throw new ArgumentOutOfRangeException(nameof(textures));
            }

            _textures = textures;
        }

        public BlockViewModel(Block block) : this(block.Id, block.Name)
        {
            _textures = block.Textures;
        }

        public override void RestoreLinks()
        {
            // Nothing to do here
        }

        public Block GetModel()
        {
            return new Block(
                Id,
                Name!,
                _textures
                );
        }

        private void SelectTextureCommandExecute(object parameter)
        {
            var dialogService = App.ServiceProvider.GetService(typeof(IDialogService)) as IDialogService;
            if (dialogService == null)
            {
                throw new NullReferenceException("Dialog service wasn't found");
            }

            SelectTextureDialogViewModel dialogVM = new SelectTextureDialogViewModel();
            bool? dialogResult = dialogService.ShowDialog(dialogVM);
            if (dialogResult.HasValue && dialogResult.Value)
            {
                Guid textureId = dialogVM.SelectedTexture!.Id;
                if (parameter is BlockSide blockSide)
                {
                    switch (blockSide)
                    {
                        case BlockSide.Front:
                            FrontTextureId = textureId;
                            break;
                        case BlockSide.Back:
                            BackTextureId = textureId;
                            break;
                        case BlockSide.Right:
                            RightTextureId = textureId;
                            break;
                        case BlockSide.Left:
                            LeftTextureId = textureId;
                            break;
                        case BlockSide.Top:
                            TopTextureId = textureId;
                            break;
                        case BlockSide.Bottom:
                            BottomTextureId = textureId;
                            break;
                    }
                }
                else
                {
                    Array.Fill(_textures, textureId);
                    OnPropertyChanged(nameof(FrontTextureId));
                    OnPropertyChanged(nameof(BackTextureId));
                    OnPropertyChanged(nameof(RightTextureId));
                    OnPropertyChanged(nameof(LeftTextureId));
                    OnPropertyChanged(nameof(TopTextureId));
                    OnPropertyChanged(nameof(BottomTextureId));
                }
            }
        }
    }
}
