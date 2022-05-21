using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ResourceTool.ViewModel.Dialog
{
    public class CreateBlockDialogViewModel : DialogViewModel
    {
        public override string Title { get { return "Add block"; } }

        private string _name = string.Empty;
        public string Name
        {
            get { return _name; }
            set
            {
                if (value == _name)
                {
                    return;
                }

                _name = value;
                OnPropertyChanged(nameof(Name));
                CommandManager.InvalidateRequerySuggested();
            }
        }

        private Guid?[] _textures;
        public Guid? FrontTextureId 
        { 
            get { return _textures[(int)BlockSide.Front]; }
            set
            {
                _textures[(int)BlockSide.Front] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }
        public Guid? BackTextureId
        {
            get { return _textures[(int)BlockSide.Back]; }
            set
            {
                _textures[(int)BlockSide.Back] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }
        public Guid? RightTextureId
        {
            get { return _textures[(int)BlockSide.Right]; }
            set
            {
                _textures[(int)BlockSide.Right] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }
        public Guid? LeftTextureId
        {
            get { return _textures[(int)BlockSide.Left]; }
            set
            {
                _textures[(int)BlockSide.Left] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }
        public Guid? TopTextureId
        {
            get { return _textures[(int)BlockSide.Top]; }
            set
            {
                _textures[(int)BlockSide.Top] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }
        public Guid? BottomTextureId
        {
            get { return _textures[(int)BlockSide.Bottom]; }
            set
            {
                _textures[(int)BlockSide.Bottom] = value;
                OnPropertyChanged();
                CommandManager.InvalidateRequerySuggested();
            }
        }

        public ICommand SelectTextureCommand { get; private init; }

        public CreateBlockDialogViewModel()
        {
            _textures = new Guid?[6];

            SelectTextureCommand = new RelayCommand(SelectTextureCommandExecute);
        }

        public BlockViewModel GetBlockViewModel()
        {
            return new BlockViewModel(
                Guid.NewGuid(),
                Name,
                _textures.Select(t => t!.Value).ToArray()
                );
        }

        protected override bool OkCommandCanExecute(object parameter)
        {
            return
                base.OkCommandCanExecute(parameter) &&
                !string.IsNullOrEmpty(Name) && Name.Length > 3 &&
                _textures.All(t => t != null);
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
