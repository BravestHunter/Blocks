using ResourceTool.Service;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel.Dialog
{
    public class SelectTextureDialogViewModel : DialogViewModel
    {
        public ObservableCollection<TextureViewModel> Textures { get; private init; }

        private TextureViewModel? _selectedTexture;
        public TextureViewModel? SelectedTexture
        {
            get { return _selectedTexture; }
            set
            {
                if (value == _selectedTexture)
                {
                    return;
                }

                _selectedTexture = value;
                OnPropertyChanged();
            }
        }

        public SelectTextureDialogViewModel()
        {
            var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
            if (resourceService == null)
            {
                throw new NullReferenceException("Resource service wasn't found");
            }

            Textures = new ObservableCollection<TextureViewModel>(resourceService.GetAllResources<TextureViewModel>());
        }

        protected override bool OkCommandCanExecute(object parameter)
        {
            return
                base.OkCommandCanExecute(parameter) &&
                SelectedTexture != null;
        }
    }
}
