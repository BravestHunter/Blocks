using ResourceTool.Model;
using ResourceTool.Service;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.ViewModel
{
    public class BlockSetViewModel : ResourceViewModel
    {
        public ObservableCollection<BlockViewModel> Blocks { get; private init; }

        public BlockSetViewModel(Guid id, string name) : base(id, name)
        {
            Blocks = new ObservableCollection<BlockViewModel>();
        }

        public BlockSetViewModel(BlockSet blockSet) : base(blockSet.Id, blockSet.Name)
        {
            var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
            if (resourceService == null)
            {
                throw new NullReferenceException("Resource service wasn't found");
            }

            Blocks = new ObservableCollection<BlockViewModel>(
                blockSet.Blocks.Select(id => resourceService.GetResource<BlockViewModel>(id)!)
                );
        }

        public BlockSet GetModel()
        {
            return new BlockSet(
                Id,
                Name!,
                Blocks.Select(b => b.Id)
                );
        }
    }
}
