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
        public string Name;
        public List<Texture> Textures;

        public ResourceBase(string name, IEnumerable<Texture> textures)
        {
            if (name == null)
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (textures == null)
            {
                throw new ArgumentNullException();
            }

            Name = name;
            Textures = textures.ToList();
        }
    }
}
