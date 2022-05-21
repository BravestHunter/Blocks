using ResourceTool.Model;
using ResourceTool.Service;
using ResourceTool.Utils;
using ResourceTool.ViewModel.Dialog;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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


        public BlockSetViewModel(Guid id, string name, int resolution) : base(id, name)
        {
            Resolution = resolution;
            Blocks = new ObservableCollection<BlockViewModel>();

            AddBlockCommand = new RelayCommand(AddBlockCommandExecute);
            RemoveBlockCommand = new RelayCommand(RemoveBlockCommandExecute, RemoveBlockCommandCanExecute);
            UpBlockCommand = new RelayCommand(UpBlockCommandExecute, UpBlockCommandCanExecute);
            DownBlockCommand = new RelayCommand(DownBlockCommandExecute, DownBlockCommandCanExecute);

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
    }
}
