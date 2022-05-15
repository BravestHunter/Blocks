using ResourceTool.Model;
using ResourceTool.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel
{
    public class ResourceBaseViewModel : BaseViewModel
    {
        private string _name;
        public string Name { get { return _name; } }

        private string _path;
        public string Path { get { return _path; } }

        public ResourceBaseViewModel(string name, string path)
        {
            if (name == null)
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (path == null)
            {
                throw new ArgumentNullException(nameof(path));
            }

            _name = name;
            _path = path;
        }

        public ResourceBase GetModel()
        {
            ResourceBase resourceBase = new ResourceBase(_name);

            return resourceBase;
        }
    }
}
