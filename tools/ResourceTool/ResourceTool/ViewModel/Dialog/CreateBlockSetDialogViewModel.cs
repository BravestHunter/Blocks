using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ResourceTool.ViewModel.Dialog
{
    public class CreateBlockSetDialogViewModel : DialogViewModel
    {
        public override string Title { get { return "Create block set"; } }

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

        public CreateBlockSetDialogViewModel()
        {

        }

        public BlockSetViewModel GetBlockSetViewModel()
        {
            return new BlockSetViewModel(
                Guid.NewGuid(),
                Name
                );
        }

        protected override bool OkCommandCanExecute(object parameter)
        {
            return
                base.OkCommandCanExecute(parameter) &&
                !string.IsNullOrEmpty(Name) && Name.Length > 3;
        }
    }
}
