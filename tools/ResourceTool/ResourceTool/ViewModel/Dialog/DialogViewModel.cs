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

        private readonly ICommand _okCommand;
        public ICommand OkCommand { get { return _okCommand; } }

        private readonly ICommand _cancelCommand;
        public ICommand CancelCommand { get { return _cancelCommand; } }

        public event Action OkAction;
        public event Action CancelAction;

        public DialogViewModel()
        {
            _okCommand = new RelayCommand(OkCommandExecute, OkCommandCanExecute);
            _cancelCommand = new RelayCommand(CancelCommandExecute);
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
