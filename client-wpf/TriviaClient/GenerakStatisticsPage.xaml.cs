using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for GenerakStatisticsPage.xaml
    /// </summary>
    public partial class GenerakStatisticsPage : Page
    {
        private Communicator m_communicator;
        public GenerakStatisticsPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void PersonalStatisticsButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new PersonalStatisticsPage(m_communicator));
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new HighScoresPage(m_communicator));
        }

        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new MainMenu(m_communicator));
        }
    }
}
