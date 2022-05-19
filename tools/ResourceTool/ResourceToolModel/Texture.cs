using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class Texture : Resource
    {
        public Texture(Guid id, string path, string name) : base(id, path, name)
        {
        }
    }
}
