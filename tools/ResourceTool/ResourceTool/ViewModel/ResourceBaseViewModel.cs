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


        public ResourceBaseViewModel([NotNull] string path, [NotNull] string name)
        {
            _path = path;
            _name = name;
        }

        public ResourceBaseViewModel(ResourceBase resourceBase)
        {
            _path = resourceBase.Path;
        }

        public ResourceBase GetModel()
        {
            ResourceBase resourceBase = new ResourceBase(_path, _name);

            return resourceBase;
        }
    }
}
