using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace ResourceTool.ViewModel.Dialog
{
    public class CreateFontDialogViewModel : DialogViewModel
    {
        public override string Title { get { return "Add font"; } }

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

        private string _path = string.Empty;
        public string Path
        {
            get { return _path; }
            set
            {
                if (value == _path)
                {
                    return;
                }

                _path = value;
                OnPropertyChanged(nameof(Path));
            }
        }

        public ICommand SelectFileDirectoryCommand { get; private init; }

        public CreateFontDialogViewModel() : base()
        {
            SelectFileDirectoryCommand = new RelayCommand(SelectFileDirectoryCommandExecute);
        }


        protected override bool OkCommandCanExecute(object parameter)
        {
            return
                base.OkCommandCanExecute(parameter) &&
                !string.IsNullOrEmpty(Name) && Name.Length > 3;
        }

        private void SelectFileDirectoryCommandExecute(object parameter)
        {
            OpenFileDialog dialog = new OpenFileDialog()
            {
                Multiselect = false,
                Filter = "Image Files(*.TTF)|*.TTF|All files (*.*)|*.*"
            };

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                Path = dialog.FileName;
            }
        }
    }
}
