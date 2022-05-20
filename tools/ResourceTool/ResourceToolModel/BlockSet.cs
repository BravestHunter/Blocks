using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Model
{
    public class BlockSet : Resource
    {
        public Guid[] Blocks;

        public BlockSet(Guid id, string name, IEnumerable<Guid> blocks) : base(id, name)
        {
            if (blocks == null)
            {
                throw new ArgumentNullException(nameof(blocks));
            }

            Blocks = blocks.ToArray();
        }
    }
}
