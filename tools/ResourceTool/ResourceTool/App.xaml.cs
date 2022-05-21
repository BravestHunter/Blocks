using Microsoft.Extensions.DependencyInjection;
using ResourceTool.Service;
using ResourceTool.ViewModel;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Navigation;

namespace ResourceTool
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private ServiceProvider _serviceProvider;
        public static IServiceProvider ServiceProvider { get { return ((App)Current)._serviceProvider; } }

        MainViewModel _mainVM = new MainViewModel();

        public App()
        {
            ServiceCollection services = new ServiceCollection();
            ConfigureServices(services);
            _serviceProvider = services.BuildServiceProvider();
        }

        private void ConfigureServices(ServiceCollection services)
        {
            services.AddSingleton<IDialogService, DialogService>();
            services.AddSingleton<IResourceService>(serviceProvider => new ResourceService(_mainVM));
        }

        protected override void OnActivated(EventArgs e)
        {
            base.OnActivated(e);

            if (MainWindow.DataContext != _mainVM)
            {
                MainWindow.DataContext = _mainVM;
            }
        }
    }
}
