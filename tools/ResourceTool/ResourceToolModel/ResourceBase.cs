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
        public List<Block> Blocks;

        public ResourceBase(
            string name, 
            IEnumerable<Texture>? textures = null, 
            IEnumerable<Block>? blocks = null
            )
        {
            if (name == null)
            {
                throw new ArgumentNullException(nameof(name));
            }

            Name = name;
            Textures = textures?.ToList() ?? new List<Texture>();
            Blocks = blocks?.ToList() ?? new List<Block>();
        }
    }
}
