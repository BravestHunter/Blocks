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
        public RawBlock[] Blocks;

        public RawBlockSet(string name, IEnumerable<RawBlock> blocks)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }
            if (blocks == null)
            {
                throw new ArgumentNullException(nameof(blocks));
            }

            Name = name;
            Blocks = blocks.ToArray();
        }
    }
}
