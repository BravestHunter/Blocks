using ResourceTool.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Service
{
    public class ResourceService : IResourceService
    {
        private readonly MainViewModel _mainVM;

        public ResourceService(MainViewModel mainVM)
        {
            if (mainVM == null)
            {
                throw new ArgumentNullException(nameof(mainVM));
            }

            _mainVM = mainVM;
        }


        public string? GetTexturesPath()
        {
            if (_mainVM.ResourceBaseVM == null)
            {
                return null;
            }

            return _mainVM.ResourceBaseVM.TextureDirectory;
        }

        public string? GetBlockSetPath()
        {
            if (_mainVM.ResourceBaseVM == null)
            {
                return null;
            }

            return Path.Combine(_mainVM.ResourceBaseVM.RootPath, "BlockSets");
        }

        public T? GetResource<T>(Guid id) where T : ResourceViewModel
        {
            if (_mainVM.ResourceBaseVM == null)
            {
                return null;
            }

            return _mainVM.ResourceBaseVM.Resources.OfType<T>().FirstOrDefault(r => r.Id == id);
        }

        public IEnumerable<T> GetAllResources<T>() where T : ResourceViewModel
        {
            if (_mainVM.ResourceBaseVM == null)
            {
                return null;
            }

            return _mainVM.ResourceBaseVM.Resources.OfType<T>();
        }
    }
}
