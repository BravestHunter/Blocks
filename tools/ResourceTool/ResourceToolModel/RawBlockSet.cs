using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class RawBlockSet
    {
        public string Name;
        public int Resolution;
        public int TexturesNumber;
        public RawBlock[] Blocks;

        public RawBlockSet(string name, int resolution, int texturesNumber, IEnumerable<RawBlock> blocks)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (resolution <= 0)
            {
                throw new ArgumentOutOfRangeException(nameof(resolution));
            }
            if (texturesNumber < 0)
            {
                throw new ArgumentOutOfRangeException(nameof(texturesNumber));
            }
            if (blocks == null)
            {
                throw new ArgumentNullException(nameof(blocks));
            }

            Name = name;
            Resolution = resolution;
            TexturesNumber = texturesNumber;
            Blocks = blocks.ToArray();
        }
    }
}
