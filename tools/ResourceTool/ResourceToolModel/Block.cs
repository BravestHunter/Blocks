using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class Block : Resource
    {
        public Guid[] Textures;

        public Block(Guid id, string name, Guid[] textures) : base(id, name)
        {
            if (textures == null)
            {
                throw new ArgumentNullException(nameof(textures));
            }
            if (textures.Length != 6)
            {
                throw new ArgumentOutOfRangeException(nameof(textures));
            }

            Textures = textures;
        }
    }
}
