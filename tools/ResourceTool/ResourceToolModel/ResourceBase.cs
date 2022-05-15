using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class ResourceBase
    {
        public string Path;
        public string Name;

        public ResourceBase([NotNull] string path, [NotNull] string name)
        {
            Path = path;
            Name = name;
        }
    }
}
