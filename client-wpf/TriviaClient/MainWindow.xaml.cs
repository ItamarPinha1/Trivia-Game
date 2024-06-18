using System.Windows;

namespace TriviaClient
{
    public partial class MainWindow : Window
    {
        private int m_port = 23455;
        private Communicator m_communicator;
        public MainWindow()
        {
            InitializeComponent();

            m_communicator = new Communicator(m_port);
            m_communicator.connectToServer();
            
            MainFrame.NavigationService.Navigate(new LoginPage(m_communicator));
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnMinimize_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }
    }
}
