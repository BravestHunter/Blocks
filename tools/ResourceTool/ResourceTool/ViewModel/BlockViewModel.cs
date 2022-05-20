using ResourceTool.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel
{
    public class BlockViewModel : ResourceViewModel
    {
        private Guid[] _textures;

        public BlockViewModel(Guid id, string name, Guid[] textures) : base(id, name)
        {
            if (textures == null)
            {
                throw new ArgumentNullException(nameof(textures));
            }
            if (textures.Length != 6)
            {
                throw new ArgumentOutOfRangeException(nameof(textures));
            }

            _textures = textures;
        }

        public BlockViewModel(Block block) : base(block.Id, block.Name)
        {
            _textures = block.Textures;
        }

        public Block GetModel()
        {
            return new Block(
                Id,
                Name,
                _textures
                );
        }
    }
}
