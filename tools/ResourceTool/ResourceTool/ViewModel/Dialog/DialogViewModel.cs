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
    public class DialogViewModel : BaseViewModel
    {
        public virtual string Title { get { return "Dialog"; } }

        public ICommand OkCommand { get; private init; }

        public ICommand CancelCommand { get; private init; }

        public event Action OkAction;
        public event Action CancelAction;

        public DialogViewModel()
        {
            OkCommand = new RelayCommand(OkCommandExecute, OkCommandCanExecute);
            CancelCommand = new RelayCommand(CancelCommandExecute);
        }

        private void OkCommandExecute(object parameter)
        {
            OkAction?.Invoke();
        }

        protected virtual bool OkCommandCanExecute(object parameter)
        {
            return true;
        }

        private void CancelCommandExecute(object parameter)
        {
            CancelAction?.Invoke();
        }
    }
}
