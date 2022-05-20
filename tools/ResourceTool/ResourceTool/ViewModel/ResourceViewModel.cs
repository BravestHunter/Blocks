using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel
{
    public class ResourceViewModel : BaseViewModel
    {
        private Guid _id;
        public Guid Id
        {
            get { return _id; }
            set
            {
                if (value == _id)
                {
                    return;
                }

                _id = value;
                OnPropertyChanged();
            }
        }

        private string? _name = null;
        public string? Name
        {
            get { return _name; }
            set
            {
                if (value == _name)
                {
                    return;
                }

                _name = value;
                OnPropertyChanged();
            }
        }

        public ResourceViewModel()
        {
            Id = Guid.NewGuid();
        }

        public ResourceViewModel(Guid id, string name)
        {
            Id = id;
            Name = name;
        }
    }
}
