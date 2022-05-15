using ResourceTool.View.Dialog;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Service
{
    public class DialogService : IDialogService
    {
        public DialogService()
        {

        }

        public bool? ShowDialog(DialogViewModel dialogViewModel)
        {
            DialogWindow dialogWindow = new DialogWindow();
            dialogWindow.DataContext = dialogViewModel;

            Action handleOkAction = () =>
            {
                dialogWindow.DialogResult = true;
                dialogWindow.Close();
            };
            Action handleCancelAction = () =>
            {
                dialogWindow.DialogResult = true;
                dialogWindow.Close();
            };

            dialogViewModel.OkAction += handleOkAction;
            dialogViewModel.CancelAction += handleCancelAction;

            bool? result = dialogWindow.ShowDialog();

            dialogViewModel.OkAction -= handleOkAction;
            dialogViewModel.CancelAction -= handleCancelAction;

            return result;
        }
    }
}
