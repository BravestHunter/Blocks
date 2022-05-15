using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using ResourceTool.View.Dialog;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace ResourceTool.ViewModel
{
    public class MainViewModel : BaseViewModel
    {
        private ResourceBaseViewModel? _currentResourceBaseVM = null;

        public ResourceBaseViewModel? CurrentResourceBaseVM
        { 
            get { return _currentResourceBaseVM; }
            set
            {
                if (value == _currentResourceBaseVM)
                {
                    return;
                }

                _currentResourceBaseVM = value;
                OnPropertyChanged(nameof(CurrentResourceBaseVM));
            }
        }

        private ICommand _createResourceBaseCommand;
        private ICommand _openResourceBaseCommand;
        private ICommand _saveResourceBaseCommand;

        public ICommand CreateResourceBaseCommand { get { return _createResourceBaseCommand; } }
        public ICommand OpenResourceBaseCommand { get { return _openResourceBaseCommand; } }
        public ICommand SaveResourceBaseCommand { get { return _saveResourceBaseCommand; } }


        public MainViewModel()
        {
            _createResourceBaseCommand = new RelayCommand(CreateResourceBaseCommandExecute);
            _openResourceBaseCommand = new RelayCommand(OpenResourceBaseCommandExecute);
            _saveResourceBaseCommand = new RelayCommand(SaveResourceBaseCommandExecute, SaveResourceBaseCommandCanExecute);
        }


        private void CreateResourceBaseCommandExecute(object parameter)
        {
            var dialogService = App.ServiceProvider.GetService(typeof(IDialogService)) as IDialogService;
            if (dialogService == null)
            {
                throw new NullReferenceException("Dialog service wasn't found");
            }

            if (_currentResourceBaseVM != null)
            {
                // Do some things
            }

            var dialogVM = new CreateResourceBaseDialogViewModel();
            dialogService.ShowDialog(dialogVM);

            _currentResourceBaseVM = new ResourceBaseViewModel(dialogVM.Name, dialogVM.Path);
            OnPropertyChanged(nameof(CurrentResourceBaseVM));
        }

        private void OpenResourceBaseCommandExecute(object parameter)
        {

        }

        private void SaveResourceBaseCommandExecute(object parameter)
        {

        }

        private bool SaveResourceBaseCommandCanExecute(object parameter)
        {
            return _currentResourceBaseVM != null;
        }
    }
}
