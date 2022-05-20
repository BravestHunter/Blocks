using ResourceTool.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ResourceTool.Service
{
    public interface IResourceService
    {
        T? GetResource<T>(Guid id) where T : ResourceViewModel;
        IEnumerable<T> GetAllResources<T>() where T : ResourceViewModel;
    }
}
