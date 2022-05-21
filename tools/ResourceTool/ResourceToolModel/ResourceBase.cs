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
        public Texture[] Textures;
        public Block[] Blocks;
        public BlockSet[] BlockSets;

        public ResourceBase(
            string name, 
            IEnumerable<Texture>? textures = null, 
            IEnumerable<Block>? blocks = null,
            IEnumerable<BlockSet>? blockSets = null
            )
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }

            Name = name;
            Textures = textures?.ToArray() ?? new Texture[0];
            Blocks = blocks?.ToArray() ?? new Block[0];
            BlockSets = blockSets?.ToArray() ?? new BlockSet[0];
        }
    }
}
