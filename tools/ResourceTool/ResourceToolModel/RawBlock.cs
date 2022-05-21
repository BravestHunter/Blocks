using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class RawBlock
    {
        public string Name;
        public int[] Textures;

        public RawBlock(string name, int[] textures)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (textures == null)
            {
                throw new ArgumentNullException(nameof(textures));
            }
            if (textures.Length != 6)
            {
                throw new ArgumentOutOfRangeException(nameof(textures));
            }

            Name = name;
            Textures = textures;
        }
    }
}
