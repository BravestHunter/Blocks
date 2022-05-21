using ResourceTool.Service;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel.Dialog
{
    public class SelectBlockDialogViewModel : DialogViewModel
    {
        public ObservableCollection<BlockViewModel> Blocks { get; private init; }

        private BlockViewModel? _selectedBlock;
        public BlockViewModel? SelectedBlock
        {
            get { return _selectedBlock; }
            set
            {
                if (value == _selectedBlock)
                {
                    return;
                }

                _selectedBlock = value;
                OnPropertyChanged();
            }
        }

        public SelectBlockDialogViewModel()
        {
            var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
            if (resourceService == null)
            {
                throw new NullReferenceException("Resource service wasn't found");
            }

            Blocks = new ObservableCollection<BlockViewModel>(resourceService.GetAllResources<BlockViewModel>());
        }

        protected override bool OkCommandCanExecute(object parameter)
        {
            return
                base.OkCommandCanExecute(parameter) &&
                SelectedBlock != null;
        }
    }
}
