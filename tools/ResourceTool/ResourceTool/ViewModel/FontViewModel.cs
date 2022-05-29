using ResourceTool.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel
{
    public class FontViewModel : ResourceViewModel
    {
        private string? _path = null;
        public string? Path
        {
            get { return _path; }
            set
            {
                if (value == _path)
                {
                    return;
                }

                _path = value;
                OnPropertyChanged();
            }
        }


        public FontViewModel(Guid id, string path, string name) : base(id, name)
        {
            Path = path;
        }

        public FontViewModel(Font texture) : base(texture.Id, texture.Name)
        {
            Path = texture.Path;
        }

        public override void RestoreLinks()
        {
            // Nothing to do here
        }


        public Font GetModel()
        {
            return new Font(
                Id,
                Name!,
                Path!
                );
        }
    }
}
