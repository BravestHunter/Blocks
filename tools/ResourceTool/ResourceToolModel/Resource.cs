using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class Resource
    {
        public Guid Id;
        public string Path = string.Empty;
        public string Name = string.Empty;

        public Resource(Guid id, string path, string name)
        {
            if (string.IsNullOrEmpty(path))
            {
                throw new ArgumentNullException(nameof(path));
            }
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }

            Id = id;
            Path = path;
            Name = name;
        }
    }
}
