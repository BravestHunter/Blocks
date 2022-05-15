using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Service
{
    public interface IDialogService
    {
        bool? ShowDialog(DialogViewModel dialogViewModel);
    }
}
