using Newtonsoft.Json;
using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using ResourceTool.View.Dialog;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace ResourceTool.ViewModel
{
    public class MainViewModel : BaseViewModel
    {
        private ResourceBaseViewModel? _resourceBaseVM = null;
        public ResourceBaseViewModel? ResourceBaseVM
        { 
            get { return _resourceBaseVM; }
            set
            {
                if (value == _resourceBaseVM)
                {
                    return;
                }

                _resourceBaseVM = value;
                OnPropertyChanged();
            }
        }

        public ICommand CreateResourceBaseCommand { get; private init; }
        public ICommand OpenResourceBaseCommand { get; private init; }
        public ICommand SaveResourceBaseCommand { get; private init; }


        public MainViewModel()
        {
            CreateResourceBaseCommand = new RelayCommand(CreateResourceBaseCommandExecute);
            OpenResourceBaseCommand = new RelayCommand(OpenResourceBaseCommandExecute);
            SaveResourceBaseCommand = new RelayCommand(SaveResourceBaseCommandExecute, SaveResourceBaseCommandCanExecute);
        }


        private void CreateResourceBaseCommandExecute(object parameter)
        {
            var dialogService = App.ServiceProvider.GetService(typeof(IDialogService)) as IDialogService;
            if (dialogService == null)
            {
                throw new NullReferenceException("Dialog service wasn't found");
            }

            if (_resourceBaseVM != null)
            {
                // Do some things
            }

            var dialogVM = new CreateResourceBaseDialogViewModel();

            bool? dialogResult = dialogService.ShowDialog(dialogVM);
            if (dialogResult.HasValue && dialogResult.Value)
            {
                ResourceBaseVM = new ResourceBaseViewModel(dialogVM.Path, dialogVM.Name);

                Directory.CreateDirectory(Path.Combine(dialogVM.Path, "Textures"));
            }
        }

        private void OpenResourceBaseCommandExecute(object parameter)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Resource base files (*.rb)|*.rb|All files (*.*)|*.*";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string resourceBaseStr = File.ReadAllText(dialog.FileName);
                ResourceBase resourceBase = JsonConvert.DeserializeObject<ResourceBase>(resourceBaseStr)!;
                string rootPath = Path.GetDirectoryName(dialog.FileName)!;
                ResourceBaseVM = new ResourceBaseViewModel(rootPath, resourceBase);
            }
        }

        private void SaveResourceBaseCommandExecute(object parameter)
        {
            ResourceBase resourceBase = ResourceBaseVM.GetModel();
            string resourceBaseStr = JsonConvert.SerializeObject(resourceBase, Formatting.Indented);

            File.WriteAllText($"{ResourceBaseVM.RootPath}/{ResourceBaseVM.Name}.rb", resourceBaseStr);
        }

        private bool SaveResourceBaseCommandCanExecute(object parameter)
        {
            return ResourceBaseVM != null;
        }
    }
}
