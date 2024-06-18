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
    /// Interaction logic for PersonalStatisticsPage.xaml
    /// </summary>
    public partial class PersonalStatisticsPage : Page
    {
        private Communicator m_communicator;
        public PersonalStatisticsPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            DisplayStatistics();
        }

        private void DisplayStatistics() 
        {
            var statistics = getStatistics();

            if (statistics != null)
            {
                averageAnswerTime.Text = $"Average Answer Time: {statistics.avarageAnswerTime}";
                numberOfCorrectAnswers.Text = $"Number Of Correct Answers: {statistics.numOfCorrectAnswers}";
                numberOfTotalAnswers.Text = $"Number Of Total Answers: {statistics.numOfTotalAnswers}";
                numberOfPlayerGames.Text = $"Number Of Player Games: {statistics.numOfPlayerGames}";
                playerScore.Text = $"Player Score: {statistics.playerScore}";
            }
            else
            {
                MessageBox.Show("Failed to retrieve statistics from the server.");
            }

        }

        private PersonalStatistics getStatistics()
        {
            GetPersonalStatisticsRequest highScoresRequest = new GetPersonalStatisticsRequest();

            byte[] message = Serializer.Serialize<GetPersonalStatisticsRequest>(MessageType.GET_STATISTICS, highScoresRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.GET_STATISTICS:
                    GetPersonalStatisticsResponse response = Desrializer.Deserialize<GetPersonalStatisticsResponse>(responseBytes);
                    return response.statistics;
                default:
                    return null;
            }
        }


        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new GenerakStatisticsPage(m_communicator));
        }

    }
}
