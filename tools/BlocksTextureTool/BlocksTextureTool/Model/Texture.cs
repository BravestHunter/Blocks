using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlocksTextureTool.Model
{
    public class Texture
    {
        public string Name;
        public string Path;

        public Texture(string name, string path)
        {
            Name = name;
            Path = path;
        }
    }
}
