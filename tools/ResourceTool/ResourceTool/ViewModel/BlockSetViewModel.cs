using Newtonsoft.Json;
using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ResourceTool.ViewModel
{
    public class BlockSetViewModel : ResourceViewModel
    {
        private readonly Guid[] _initialBlocks;

        private int _resolution;
        public int Resolution
        {
            get { return _resolution; }
            set
            {
                if (value == _resolution)
                {
                    return;
                }

                _resolution = value;
                OnPropertyChanged(nameof(Resolution));
            }
        }

        public ObservableCollection<BlockViewModel> Blocks { get; private init; }

        private BlockViewModel? _selectedBlock;
        public BlockViewModel? SelectedBlock
        {
            get { return _selectedBlock; }
            set
            {
                if (value == _selectedBlock)
                {
                    return;
                }

                _selectedBlock = value;
                OnPropertyChanged();
            }
        }

        public ICommand AddBlockCommand { get; private init; }
        public ICommand RemoveBlockCommand { get; private init; }
        public ICommand UpBlockCommand { get; private init; }
        public ICommand DownBlockCommand { get; private init; }
        public ICommand GenerateBlockSetCommand { get; private init; }


        public BlockSetViewModel(Guid id, string name, int resolution) : base(id, name)
        {
            Resolution = resolution;
            Blocks = new ObservableCollection<BlockViewModel>();

            AddBlockCommand = new RelayCommand(AddBlockCommandExecute);
            RemoveBlockCommand = new RelayCommand(RemoveBlockCommandExecute, RemoveBlockCommandCanExecute);
            UpBlockCommand = new RelayCommand(UpBlockCommandExecute, UpBlockCommandCanExecute);
            DownBlockCommand = new RelayCommand(DownBlockCommandExecute, DownBlockCommandCanExecute);
            GenerateBlockSetCommand = new RelayCommand(GenerateBlockSetCommandExecute, GenerateBlockSetCommandCanExecute);

            _initialBlocks = new Guid[0];
        }

        public BlockSetViewModel(BlockSet blockSet) : this(blockSet.Id, blockSet.Name, blockSet.Resoulution)
        {
            _initialBlocks = blockSet.Blocks;
        }

        public override void RestoreLinks()
        {
            var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
            if (resourceService == null)
            {
                throw new NullReferenceException("Resource service wasn't found");
            }

            Blocks.Clear();
            foreach (Guid blockId in _initialBlocks)
            {
                Blocks.Add(resourceService.GetResource<BlockViewModel>(blockId)!);
            }
        }

        public BlockSet GetModel()
        {
            return new BlockSet(
                Id,
                Name!,
                Resolution,
                Blocks.Select(b => b.Id)
                );
        }

        private void AddBlockCommandExecute(object parameter)
        {
            var dialogService = App.ServiceProvider.GetService(typeof(IDialogService)) as IDialogService;
            if (dialogService == null)
            {
                throw new NullReferenceException("Dialog service wasn't found");
            }

            SelectBlockDialogViewModel dialogVM = new SelectBlockDialogViewModel();
            bool? dialogResult = dialogService.ShowDialog(dialogVM);
            if (dialogResult.HasValue && dialogResult.Value)
            {
                Blocks.Add(dialogVM.SelectedBlock!);
            }
        }

        private void RemoveBlockCommandExecute(object parameter)
        {
            Blocks.Remove(SelectedBlock!);
            SelectedBlock = null;
        }

        private bool RemoveBlockCommandCanExecute(object parameter)
        {
            return SelectedBlock != null;
        }

        private void UpBlockCommandExecute(object parameter)
        {
            int index = Blocks.IndexOf(SelectedBlock!);
            Blocks.Move(index, index - 1);
        }

        private bool UpBlockCommandCanExecute(object parameter)
        {
            return
                Blocks.Count > 1 &&
                SelectedBlock != null &&
                Blocks.IndexOf(SelectedBlock) > 0;
        }

        private void DownBlockCommandExecute(object parameter)
        {
            int index = Blocks.IndexOf(SelectedBlock!);
            Blocks.Move(index, index + 1);
        }

        private bool DownBlockCommandCanExecute(object parameter)
        {
            return 
                Blocks.Count > 1 && 
                SelectedBlock != null &&
                Blocks.IndexOf(SelectedBlock) < Blocks.Count - 1;
        }

        private void GenerateBlockSetCommandExecute(object parameter)
        {
            var resourceService = App.ServiceProvider.GetService(typeof(IResourceService)) as IResourceService;
            if (resourceService == null)
            {
                throw new NullReferenceException("Resource service wasn't found");
            }

            string commonBlockSetsDirectory = resourceService.GetBlockSetPath()!;
            if (!Directory.Exists(commonBlockSetsDirectory))
            {
                Directory.CreateDirectory(commonBlockSetsDirectory);
            }

            string blockSetDirectory = Path.Combine(commonBlockSetsDirectory, Name!);
            if (Directory.Exists(blockSetDirectory))
            {
                Directory.Delete(blockSetDirectory, true);
            }
            Directory.CreateDirectory(blockSetDirectory);

            Dictionary<Guid, int> textureMap = new Dictionary<Guid, int>();
            int counter = 0;
            foreach (TextureViewModel textureVM in Blocks.SelectMany(b => b.Textures.Select(t => resourceService.GetResource<TextureViewModel>(t))))
            {
                if (!textureMap.ContainsKey(textureVM.Id))
                {
                    textureMap.Add(textureVM.Id, counter);

                    string fileName = $"{counter}.png";
                    using (Image image = Image.FromFile(textureVM.Path!))
                    using (Bitmap bm = TextureViewModel.ResizeImage(image, Resolution, Resolution))
                    {
                        bm.Save(Path.Combine(blockSetDirectory, fileName), ImageFormat.Png);
                    }

                    counter++;
                }
            }

            List<RawBlock> rawBlocks = new List<RawBlock>(Blocks.Count);
            foreach (BlockViewModel blockVM in Blocks)
            {
                RawBlock rawBlock = new RawBlock(
                    blockVM.Name!,
                    blockVM.Textures.Select(t => textureMap[t]).ToArray()
                    );
                rawBlocks.Add(rawBlock);
            }

            RawBlockSet rawBlockSet = new RawBlockSet(Name!, Resolution, textureMap.Count, rawBlocks);
            string rawBlockSetStr = JsonConvert.SerializeObject(rawBlockSet, Formatting.Indented);
            File.WriteAllText(Path.Combine(blockSetDirectory, $"{Name}.bs"), rawBlockSetStr);
        }

        private bool GenerateBlockSetCommandCanExecute(object parameter)
        {
            return Blocks.Count > 0;
        }
    }
}
